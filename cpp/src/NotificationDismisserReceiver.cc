/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include <alljoyn/notification/NotificationService.h>
#include <alljoyn/notification/NotificationReceiver.h>
#include "NotificationDismisserReceiver.h"
#include "NotificationConstants.h"
#include "Transport.h"
#include <sstream>
#include <alljoyn/MsgArg.h>
#include <alljoyn/services_common/Conversions.h>

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

NotificationDismisserReceiver::NotificationDismisserReceiver(BusAttachment* bus, QStatus& status) :
    NotificationDismisser(bus, AJ_NOTIFICATION_DISMISSER_PATH, status), m_IsStopping(false)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "NotificationDismisserReceiver::NotificationDismisserReceiver() - called()");
    }

    /**
     * Do not add code until the status that returned from the base class is verified.
     */
    if (status != ER_OK) {
        return;
    }

    pthread_mutex_init(&m_Lock, NULL);
    pthread_cond_init(&m_QueueChanged, NULL);

    status =  bus->RegisterSignalHandler(this,
                                         static_cast<MessageReceiver::SignalHandler>(&NotificationDismisserReceiver::handleSignal),
                                         m_SignalMethod,
                                         NULL);
    if (logger) {
        if (status != ER_OK) {
            logger->warn(TAG, "Could not register the SignalHandler");
        } else {
            logger->debug(TAG, "Registered the SignalHandler successfully");
        }
    }
    pthread_create(&m_ReceiverThread, NULL, ReceiverThreadWrapper, this);
}


void NotificationDismisserReceiver::handleSignal(const InterfaceDescription::Member* member, const char* srcPath, Message& msg)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "Received dismisser signal.");
    }
    pthread_mutex_lock(&m_Lock);
    m_MessageQueue.push(msg);
    pthread_cond_signal(&m_QueueChanged);
    pthread_mutex_unlock(&m_Lock);
}

void NotificationDismisserReceiver::unregisterHandler(BusAttachment* bus)
{
    pthread_mutex_lock(&m_Lock);
    while (!m_MessageQueue.empty()) {
        m_MessageQueue.pop();
    }
    m_IsStopping = true;
    pthread_cond_signal(&m_QueueChanged);
    pthread_mutex_unlock(&m_Lock);
    pthread_join(m_ReceiverThread, NULL);

    bus->UnregisterSignalHandler(this,
                                 static_cast<MessageReceiver::SignalHandler>(&NotificationDismisserReceiver::handleSignal),
                                 m_SignalMethod,
                                 NULL);

    pthread_cond_destroy(&m_QueueChanged);
    pthread_mutex_destroy(&m_Lock);
}

void* NotificationDismisserReceiver::ReceiverThreadWrapper(void* context)
{
    NotificationDismisserReceiver* consumer = reinterpret_cast<NotificationDismisserReceiver*>(context);
    if (consumer == NULL) { // should not happen
        return NULL;
    }
    consumer->ReceiverThread();
    return NULL;
}

void NotificationDismisserReceiver::ReceiverThread()
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    pthread_mutex_lock(&m_Lock);
    while (!m_IsStopping) {
        while (!m_MessageQueue.empty()) {
            Message message = m_MessageQueue.front();
            m_MessageQueue.pop();
            pthread_mutex_unlock(&m_Lock);
            if (logger) {
                logger->debug(TAG, "ReceiverThread() - got a dismiss message.");
            }
            int32_t msgId;
            qcc::String appId;
            QStatus status = UnmarshalMessage(message, msgId, appId);
            if (status == ER_OK) {
                Transport::getInstance()->getNotificationReceiver()->Dismiss(msgId, appId);
            }
            pthread_mutex_lock(&m_Lock);
        }
        pthread_cond_wait(&m_QueueChanged, &m_Lock);
    }
    pthread_mutex_unlock(&m_Lock);
}

QStatus NotificationDismisserReceiver::UnmarshalMessage(Message& in_message, int32_t& msgId, qcc::String& appId)
{
    QStatus status = ER_OK;
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    const MsgArg* messageIdArg = in_message.unwrap()->GetArg(0);
    const MsgArg* appIdArg = in_message.unwrap()->GetArg(1);

    if ((messageIdArg == NULL) || (appIdArg == NULL)) {
        status = ER_BAD_ARG_COUNT;
        return status;
    }

    //unmarshal messageid
    if (messageIdArg->typeId != ALLJOYN_INT32) {
        status = ER_BUS_BAD_VALUE_TYPE;
        if (logger) {
            logger->debug(TAG, "UnmarshalMessage() - bad type to unmarshal.");
        }
        return status;
    }
    status = messageIdArg->Get(AJPARAM_INT.c_str(), &msgId);
    if (status != ER_OK) {
        if (logger) {
            logger->debug(TAG, "UnmarshalMessage() - failed to get parameter.");
        }
        return status;
    }

    //Unmarshal appId
    uint8_t* appIdBin = NULL;
    size_t len;
    status = Conversions::MsgArgToArrayOfBytes(appIdArg, &appIdBin, &len, logger);
    if (status != ER_OK) {
        if (logger) {
            logger->debug(TAG, "UnmarshalMessage() - failed to get array of bytes.");
        }
        return status;
    }

    //convert bytes to qcc::String
    Conversions::ArrayOfBytesToString(&appIdBin, len, &appId);
    return status;
}
