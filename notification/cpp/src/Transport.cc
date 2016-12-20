/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include <alljoyn/about/AboutServiceApi.h>
#include <alljoyn/about/AnnouncementRegistrar.h>
#include <alljoyn/notification/NotificationService.h>
#include <alljoyn/notification/NotificationReceiver.h>

#include "NotificationConstants.h"
#include "PayloadAdapter.h"
#include "Transport.h"
#include "NotificationProducerReceiver.h"
#include "NotificationProducerSender.h"
#include "NotificationProducerListener.h"
#include "NotificationDismisserSender.h"
#include "NotificationDismisserReceiver.h"
#include "NotificationConstants.h"
#include "NotificationTransportConsumer.h"
#include "NotificationTransportProducer.h"
#include <alljoyn/notification/LogModule.h>

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

Transport* Transport::s_Instance(NULL);

Transport::Transport() : m_Bus(0), m_Receiver(0), m_Consumer(0),
    m_IsSendingDisabled(false), m_IsReceivingDisabled(false),
    m_NotificationProducerSender(0), m_NotificationProducerReceiver(0),
    m_NotificationProducerListener(0), m_NotificationDismisserSender(0), m_NotificationDismisserReceiver(0)
{
    Notification::m_AsyncTaskQueue.Start();

    for (int32_t indx = 0; indx < MESSAGE_TYPE_CNT; indx++)
        m_Producers[indx] = 0;
}

Transport::~Transport()
{
    if (m_Bus) {
        cleanup();
    }
}

Transport* Transport::getInstance()
{
    if (!s_Instance) {
        s_Instance = new Transport();
    }
    return s_Instance;
}

QStatus Transport::setBusAttachment(ajn::BusAttachment* bus)
{
    if (m_Bus && bus->GetUniqueName().compare(m_Bus->GetUniqueName()) != 0) {
        QCC_LogError(ER_BUS_LISTENER_ALREADY_SET, ("Could not accept this BusAttachment, a different bus attachment already exists"));
        return ER_BUS_LISTENER_ALREADY_SET;
    }

    if (!bus->IsStarted() || !bus->IsConnected()) {
        QCC_LogError(ER_BAD_ARG_1, ("Could not acccept this BusAttachment, bus attachment not started or not connected"));
        return ER_BAD_ARG_1;
    }

    m_Bus = bus;
    return ER_OK;
}

void Transport::setNotificationReceiver(NotificationReceiver* notificationReceiver)
{
    m_Receiver = notificationReceiver;
}

void Transport::onReceivedNotification(Notification const& notification)
{
    if (m_Receiver == 0) {
        QCC_DbgHLPrintf(("Could not receive message, Receiver is not initialized"));
        return;
    }

    if (m_IsReceivingDisabled) {
        QCC_DbgHLPrintf(("Could not receive message, Receiving is Disabled"));
        return;
    }

    m_Receiver->Receive(notification);
}

QStatus Transport::sendNotification(NotificationMessageType messageType,
                                    MsgArg const notificationArgs[AJ_NOTIFY_NUM_PARAMS], uint16_t ttl)
{
    if (m_Producers[messageType] == 0) {
        QCC_LogError(ER_BUS_OBJECT_NOT_REGISTERED, ("Could not send message, Sender is not initialized"));
        return ER_BUS_OBJECT_NOT_REGISTERED;
    }

    if (m_IsSendingDisabled) {
        QCC_LogError(ER_BUS_NOT_ALLOWED, ("Could not send message, Sending is Disabled"));
        return ER_BUS_NOT_ALLOWED;
    }

    return m_Producers[messageType]->sendSignal(notificationArgs, ttl);
}

QStatus Transport::deleteLastMsg(NotificationMessageType messageType)
{
    if (m_Producers[messageType] == 0) {
        QCC_LogError(ER_BUS_OBJECT_NOT_REGISTERED, ("Could not delete message, Sender is not initialized"));
        return ER_BUS_OBJECT_NOT_REGISTERED;
    }

    return m_Producers[messageType]->deleteLastMsg(messageType);
}

QStatus Transport::getLastMsgId(NotificationMessageType messageType, int32_t* messageId)
{
    if (m_Producers[messageType] == 0) {
        QCC_LogError(ER_BUS_OBJECT_NOT_REGISTERED, ("Sender is not initialized"));
        return ER_BUS_OBJECT_NOT_REGISTERED;
    }

    return m_Producers[messageType]->getLastMsgId(messageType, messageId);
}


QStatus Transport::deleteMsg(int32_t msgId)
{
    QCC_DbgTrace(("Transport::deleteMsg() msgId=%d", msgId));
    QStatus ret = ER_OK;

    for (size_t i  = 0; i < ajn::services::MESSAGE_TYPE_CNT; i++) {
        if (m_Producers[i] == 0) {
            QCC_DbgHLPrintf(("Could not delete message, Sender is not initialized"));
            continue;
        } else {
            QCC_DbgHLPrintf(("deleting message"));
        }

        ret = m_Producers[i]->deleteMsg(msgId);
        if (ret == ER_OK) {
            break;
        }
    }

    if (ret == ER_OK) {
        QCC_DbgPrintf(("Transport::deleteMsg() - message successfully deleted. msgId=%d", msgId));
    } else {
        QCC_DbgPrintf(("Transport::deleteMsg() - didn't find message to delete. msgId=%d", msgId));
    }
    return ret;
}

QStatus Transport::startSenderTransport(BusAttachment* bus)
{
    QCC_DbgTrace(("Transport::startSenderTransport"));
    QStatus status;

    if ((status = setBusAttachment(bus)) != ER_OK) {
        QCC_LogError(status, ("Could not start Sender Transport. BusAttachment is not valid"));
        goto exit;
    }

    if (m_Producers[0]) {
        QCC_DbgPrintf(("Sender Transport already started. Ignoring request"));
        goto exit;
    }

    {

        for (int32_t messageTypeIndx = 0; messageTypeIndx < MESSAGE_TYPE_CNT; messageTypeIndx++) {
            status = ER_OK;
            
                m_Producers[messageTypeIndx] = new NotificationTransportProducer(m_Bus,
                                                                                 AJ_PRODUCER_SERVICE_PATH_PREFIX + MessageTypeUtil::getMessageTypeString(messageTypeIndx), status);
           

            if (status != ER_OK) {
                QCC_LogError(status, ("Could not create BusObject."));
                goto exit;
            }

            status = m_Bus->RegisterBusObject(*m_Producers[messageTypeIndx]);
            if (status != ER_OK) {
                QCC_LogError(status, ("Could not register BusObject."));
                goto exit;
            }
        }
    }

    //Code handles NotificationProducerReceiver - Start
    cleanupNotificationProducerReceiverInternal();
    cleanupNotificationProducerSenderInternal();
    m_NotificationProducerReceiver = new NotificationProducerReceiver(m_Bus, status);
    if (status != ER_OK) {
        goto exit;
    }
    status = m_Bus->RegisterBusObject(*m_NotificationProducerReceiver);
    if (status != ER_OK) {
        QCC_LogError(status, ("Could not register BusObject."));
        goto exit;
    } else {
        QCC_DbgPrintf(("Transport::startSenderTransport - registered NotificationProducerReceiver successfully."));
    }

    m_NotificationProducerListener = new NotificationProducerListener();
    m_NotificationProducerListener->setSessionPort(AJ_NOTIFICATION_PRODUCER_SERVICE_PORT);

    { //Handling Bind session
        SessionPort servicePort = AJ_NOTIFICATION_PRODUCER_SERVICE_PORT;
        SessionOpts sessionOpts(SessionOpts::TRAFFIC_MESSAGES, true, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);

        status = m_Bus->BindSessionPort(servicePort, sessionOpts, *m_NotificationProducerListener);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not bind Session Port successfully"));
            goto exit;
        } else {
            QCC_DbgPrintf(("bind Session Port successfully for notification producer service"));
        }
    }
    //Code handles NotificationProducerReceiver - End

    //Handling NotificationDismisserSender - start here
    cleanupNotificationDismisserSenderInternal();
    cleanupNotificationDismisserReceiverInternal();
    m_NotificationDismisserSender = new NotificationDismisserSender(m_Bus, AJ_NOTIFICATION_DISMISSER_PATH, status);
    if (status != ER_OK) {
        QCC_LogError(status, ("Could not create NotificationDismisserSender."));
        goto exit;
    }

    status = m_Bus->RegisterBusObject(*m_NotificationDismisserSender);
    if (status != ER_OK) {
        QCC_LogError(status, ("Could not register NotificationDismisserSender."));
        goto exit;
    }
    //Handling NotificationDismisserSender - end here

exit:    //label exit
    if (status == ER_OK) {
        QCC_DbgTrace(("Started Sender successfully"));
    } else {   //if (status != ER_OK)
        QCC_LogError(status, ("Started Sender with error."));
        cleanupSenderTransport();
    }

    return status;
}

QStatus Transport::startReceiverTransport(BusAttachment* bus)
{
    QStatus status = ER_OK;

    if ((status = setBusAttachment(bus)) != ER_OK) {
        QCC_LogError(status, ("Could not start Receiver Transport. BusAttachment is not valid"));
        goto exit;
    }

    if (m_Consumer == NULL) {
        m_Consumer = new NotificationTransportConsumer(m_Bus, AJ_CONSUMER_SERVICE_PATH, status);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not create Consumer BusObject."));
            goto exit;
        }

        status = m_Bus->RegisterBusObject(*m_Consumer);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not register Consumer BusObject."));
            goto exit;
        }

        String AJ_NOTIFICATION_INTERFACE_MATCH = "type='signal',sessionless='t',interface='" + AJ_NOTIFICATION_INTERFACE_NAME + "'";
        QCC_DbgPrintf(("Match String is: %s", AJ_NOTIFICATION_INTERFACE_MATCH.c_str()));

        status = m_Bus->AddMatch(AJ_NOTIFICATION_INTERFACE_MATCH.c_str());

        if (status != ER_OK) {
            QCC_LogError(status, ("Could not add filter match for notification service."));
            goto exit;
        }
    }

    //Handling NotificationProducerSender - Start
    if (m_NotificationProducerSender == NULL) {
        cleanupNotificationProducerReceiverInternal();
        m_NotificationProducerSender = new NotificationProducerSender(m_Bus, status);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not create NotificationProducerSender BusObject."));
            goto exit;
        }
        status = m_Bus->RegisterBusObject(*m_NotificationProducerSender);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not register NotificationProducerSender BusObject."));
            goto exit;
        }
    }
    //Handling NotificationProducerSender - End

    //Handling NotificationDismisserReceiver - Start
    if (m_NotificationDismisserReceiver == NULL) {
        cleanupNotificationDismisserSenderInternal();
        m_NotificationDismisserReceiver = new NotificationDismisserReceiver(m_Bus, status);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not create NotificationDismisserReceiver BusObject."));
            goto exit;
        }
        status = m_Bus->RegisterBusObject(*m_NotificationDismisserReceiver);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not register m_NotificationDismisserReceiver BusObject."));
            goto exit;
        }

        String AJ_DISMISSER_INTERFACE_MATCH = "type='signal',sessionless='t',interface='" + AJ_NOTIFICATION_DISMISSER_INTERFACE + "'";
        QCC_DbgPrintf(("NotificationDismisserReceiver Match String is: %s", AJ_DISMISSER_INTERFACE_MATCH.c_str()));

        status = m_Bus->AddMatch(AJ_DISMISSER_INTERFACE_MATCH.c_str());
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not add filter match."));
            goto exit;
        }
    }
    //Handling NotificationDismisserReceiver - End

    //Handling NotificationDismisserSender - Start
    if (m_NotificationDismisserSender == NULL) {
        cleanupNotificationDismisserReceiverInternal();
        m_NotificationDismisserSender = new NotificationDismisserSender(m_Bus, AJ_NOTIFICATION_DISMISSER_PATH, status);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not create NotificationDismisserSender."));
            goto exit;
        }

        status = m_Bus->RegisterBusObject(*m_NotificationDismisserSender);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not register BusObject."));
            goto exit;
        }
    }

exit:
    if (status != ER_OK) {
        QCC_LogError(status, ("Not listening to any producers"));
        cleanupReceiverTransport();
    } else {
        QCC_DbgPrintf(("Started Receiver successfully"));
    }
    return status;
}

void Transport::cleanupTransportProducer(int32_t messageTypeIndx, bool unregister)
{
    QCC_DbgTrace(("Transport::cleanupTransportProducer - Start"));
    for  (; messageTypeIndx >= 0; messageTypeIndx--) {
        if (!m_Producers[messageTypeIndx]) {
            continue;
        }
        if (unregister) {
            m_Bus->UnregisterBusObject(*m_Producers[messageTypeIndx]);
        }
        delete m_Producers[messageTypeIndx];
        m_Producers[messageTypeIndx] = 0;
    }
    QCC_DbgTrace(("Transport::cleanupTransportProducer - End"));

}

void Transport::cleanupNotificationProducerReceiver()
{
    QCC_DbgTrace(("Transport::cleanupNotificationProducerReceiver start"));
    SessionPort sp = AJ_NOTIFICATION_PRODUCER_SERVICE_PORT;
    QStatus status = m_Bus->UnbindSessionPort(sp);
    if (status != ER_OK) {
        QCC_LogError(status, ("Could not unbind the SessionPort "));
    }

    QCC_DbgPrintf(("cleaning NotificationProducerListener"));

    if (m_NotificationProducerListener) {
        delete m_NotificationProducerListener;
        m_NotificationProducerListener = 0;
    }

    if (m_NotificationProducerReceiver) {
        m_NotificationProducerReceiver->unregisterHandler(m_Bus);
        cleanupNotificationProducerReceiverInternal();
    }
    delete m_NotificationProducerReceiver;
    m_NotificationProducerReceiver = NULL;

    QCC_DbgTrace(("Transport::cleanupNotificationProducerReceiver end"));
}

void Transport::cleanupNotificationProducerReceiverInternal()
{
    QCC_DbgTrace(("Transport::cleanupNotificationProducerReceiverInternal start"));

    if (m_NotificationProducerReceiver) {
        QCC_DbgPrintf(("cleaning NotificationProducerReceiver"));
        m_Bus->UnregisterBusObject(*m_NotificationProducerReceiver);
    }
    QCC_DbgTrace(("Transport::cleanupNotificationProducerReceiverInternal end"));
}

void Transport::cleanupNotificationDismisserSender()
{
    if (!m_NotificationDismisserSender) {
        return;
    }

    cleanupNotificationDismisserSenderInternal();

    delete m_NotificationDismisserSender;
    m_NotificationDismisserSender = 0;
}

void Transport::cleanupNotificationDismisserSenderInternal()
{
    if (!m_NotificationDismisserSender) {
        return;
    }

    m_Bus->UnregisterBusObject(*m_NotificationDismisserSender);
}

void Transport::cleanupNotificationDismisserReceiver()
{
    QCC_DbgTrace(("Transport::cleanupNotificationDismisserReceiver start"));
    if (!m_NotificationDismisserReceiver) {
        return;
    }
    m_NotificationDismisserReceiver->unregisterHandler(m_Bus);
    cleanupNotificationDismisserReceiverInternal();
    delete m_NotificationDismisserReceiver;
    m_NotificationDismisserReceiver = 0;
    QCC_DbgTrace(("Transport::cleanupNotificationDismisserReceiver end"));
}

void Transport::cleanupNotificationDismisserReceiverInternal()
{
    QCC_DbgTrace(("Transport::cleanupNotificationDismisserReceiverInternal start"));

    if (m_NotificationDismisserReceiver) {
        m_Bus->UnregisterBusObject(*m_NotificationDismisserReceiver);
    }

    QCC_DbgTrace(("Transport::cleanupNotificationDismisserReceiverInternal end"));
}

void Transport::cleanupSenderTransport()
{
    cleanupNotificationDismisserSender();
    cleanupNotificationProducerSender();
    cleanupNotificationProducerReceiver();
    cleanupTransportProducer(MESSAGE_TYPE_CNT - 1, true);
}

void Transport::cleanupReceiverTransport()
{
    QCC_DbgTrace(("Transport::cleanupReceiverTransport start"));
    cleanupNotificationDismisserReceiver();
    cleanupNotificationProducerSender();
    cleanupTransportConsumer(true);
    QCC_DbgTrace(("Transport::cleanupReceiverTransport end"));
}

void Transport::cleanupTransportConsumer(bool unregister)
{
    QCC_DbgTrace(("Transport::cleanupTransportConsumer start"));
    if (!m_Consumer) {
        return;
    }

    if (unregister) {
        m_Consumer->unregisterHandler(m_Bus);
        cleanupTransportConsumerInternal();
    }
    delete m_Consumer;
    m_Consumer = 0;
    QCC_DbgTrace(("Transport::cleanupTransportConsumer end"));
}

void Transport::cleanupTransportConsumerInternal(void)
{
    QCC_DbgTrace(("Transport::cleanupTransportConsumerInternal start"));
    if (!m_Consumer) {
        return;
    }
    m_Bus->UnregisterBusObject(*m_Consumer);
    QCC_DbgTrace(("Transport::cleanupTransportConsumerInternal end"));
}

void Transport::cleanupNotificationProducerSender()
{
    QCC_DbgTrace(("Transport::cleanupNotificationProducerSender start"));
    if (!m_NotificationProducerSender) {
        return;
    }

    cleanupNotificationProducerSenderInternal();

    delete m_NotificationProducerSender;
    m_NotificationProducerSender = 0;
    QCC_DbgTrace(("Transport::cleanupNotificationProducerSender end"));
}

void Transport::cleanupNotificationProducerSenderInternal()
{
    QCC_DbgTrace(("Transport::cleanupNotificationProducerSender start"));
    if (!m_NotificationProducerSender) {
        return;
    }

    m_Bus->UnregisterBusObject(*m_NotificationProducerSender);

    QCC_DbgTrace(("Transport::cleanupNotificationProducerSender end"));
}

void Transport::cleanup()
{
    if (m_Bus == 0) {
        return;
    }
    Notification::m_AsyncTaskQueue.Stop();
    cleanupSenderTransport();
    cleanupReceiverTransport();

    m_Bus = 0;
    s_Instance = 0;

    delete this;
}

ajn::BusAttachment* Transport::getBusAttachment()
{
    return m_Bus;
}

NotificationProducerSender* Transport::getNotificationProducerSender()
{
    return m_NotificationProducerSender;
}

NotificationProducerReceiver* Transport::getNotificationProducerReceiver()
{
    return m_NotificationProducerReceiver;
}

NotificationDismisserSender* Transport::getNotificationDismisserSender()
{
    return m_NotificationDismisserSender;
}

NotificationReceiver* Transport::getNotificationReceiver()
{
    return m_Receiver;
}