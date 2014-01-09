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

#include <alljoyn/Message.h>
#include <alljoyn/notification/NotificationService.h>

#include "NotificationTransportProducer.h"
#include "NotificationConstants.h"
#include "Transport.h"

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

NotificationTransportProducer::NotificationTransportProducer(BusAttachment* bus,
                                                             String const& servicePath, QStatus& status,
                                                             qcc::String const& interfaceName, uint32_t serialNumber) :
    NotificationTransport(bus, servicePath, status, interfaceName, TAG_TRANSPORT_PRODUCER),
    m_SerialNumber(serialNumber), m_MsgId(0)
{
}

QStatus NotificationTransportProducer::sendSignal(ajn::MsgArg const notificationArgs[AJ_NOTIFY_NUM_PARAMS],
                                                  uint16_t ttl)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if (m_SignalMethod == 0) {
        if (logger) {
            logger->warn(TAG, "signalMethod not set. Can't send signal");
        }
        return ER_BUS_INTERFACE_NO_SUCH_MEMBER;
    }

    uint8_t flags =  ALLJOYN_FLAG_SESSIONLESS;

    Message msg(*Transport::getInstance()->getBusAttachment());
    m_MsgId = notificationArgs[1].v_int32; //grab message id from payload.

    QStatus status = Signal(NULL, 0, *m_SignalMethod, notificationArgs, AJ_NOTIFY_NUM_PARAMS, ttl, flags, &msg);

    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not send signal. Status: " + String(QCC_StatusText(status)));
        }
        return status;
    }

    m_SerialNumber = msg->GetCallSerial();


    if (logger) {
        logger->debug(TAG, "Sent signal successfully");
    }
    return status;
}

QStatus NotificationTransportProducer::deleteLastMsg(NotificationMessageType messageType)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if (m_SerialNumber == 0) {
        if (logger) {
            logger->debug(TAG, "Unable to delete the last message.  No message on this object.");
        }
        return ER_BUS_INVALID_HEADER_SERIAL;
    }

    QStatus status = CancelSessionlessMessage(m_SerialNumber);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not delete last message. Status: " + String(QCC_StatusText(status)));
        }
        return status;
    }

    m_SerialNumber = 0;
    m_MsgId = 0;

    if (logger) {
        logger->debug(TAG, "Deleted last message successfully");
    }
    return status;
}

QStatus NotificationTransportProducer::deleteMsg(int32_t msgId)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "NotificationTransportProducer::deleteMsg()");
    }

    if (m_SerialNumber == 0) {
        return ER_BUS_INVALID_HEADER_SERIAL;
    }

    if (m_MsgId != msgId) {
        if (logger) {
            logger->debug(TAG, "Unable to delete the message. No such message id on this object.");
        }
        return ER_BUS_INVALID_HEADER_SERIAL;
    }

    QStatus status = CancelSessionlessMessage(m_SerialNumber);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not delete last message. Status: " + String(QCC_StatusText(status)));
        }
        return status;
    }

    m_SerialNumber = 0;
    m_MsgId = 0;

    if (logger) {
        logger->debug(TAG, "Deleted last message successfully");
    }
    return status;
}
