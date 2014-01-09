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

#include "NotificationDismisserSender.h"
#include "NotificationConstants.h"
#include "Transport.h"

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

NotificationDismisserSender::NotificationDismisserSender(BusAttachment* bus, String const& objectPath, QStatus& status) :
    NotificationDismisser(bus, objectPath, status)

{
    /**
     * Do not add code until the status that returned from the base class is verified.
     */
    if (status != ER_OK)
        return;

    //Add code here
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if (logger) {
        String log("NotificationDismisserSender()  - Got objectpath=");
        log.append(objectPath);
        logger->debug(TAG, log);
    }
}

QStatus NotificationDismisserSender::sendSignal(ajn::MsgArg const dismisserArgs[AJ_DISMISSER_NUM_PARAMS],
                                                uint16_t ttl)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if (logger) {
        logger->debug(TAG, "Notification::sendSignal() called");
    }

    if (m_SignalMethod == 0) {
        if (logger) {
            logger->warn(TAG, "signalMethod not set. Can't send signal");
        }
        return ER_BUS_INTERFACE_NO_SUCH_MEMBER;
    }

    uint8_t flags =  ALLJOYN_FLAG_SESSIONLESS;

    Message msg(*Transport::getInstance()->getBusAttachment());

    QStatus status = Signal(NULL, 0, *m_SignalMethod, dismisserArgs, AJ_DISMISSER_NUM_PARAMS, ttl, flags, &msg);

    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not send signal. Status: " + String(QCC_StatusText(status)));
        }
        return status;
    }

    if (logger) {
        logger->debug(TAG, "Sent signal successfully");
    }
    return status;
}
