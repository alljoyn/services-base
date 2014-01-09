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

#include <alljoyn/notification/NotificationSender.h>
#include <alljoyn/notification/NotificationService.h>
#include <alljoyn/notification/Notification.h>
#include <alljoyn/services_common/GenericLogger.h>

#include "PayloadAdapter.h"
#include "Transport.h"
#include "NotificationConstants.h"
#include <string.h>

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

NotificationSender::NotificationSender(PropertyStore* propertyStore) :
    m_PropertyStore(propertyStore), TAG(TAG_NOTSENDER)
{

}

QStatus NotificationSender::send(Notification const& notification, uint16_t ttl)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "Send Message called");
    }

    //Validations
    if (notification.getMessageType() < 0 || notification.getMessageType() >= MESSAGE_TYPE_CNT) {
        if (logger) {
            logger->warn(TAG, "MessageType sent is not a valid MessageType");
        }
        return ER_BAD_ARG_1;
    }

    if (notification.getText().size() == 0) {
        if (logger) {
            logger->warn(TAG, "There must be at least one notification defined");
        }
        return ER_BAD_ARG_1;
    } else if ((TTL_MIN > ttl) || (ttl  > TTL_MAX)) {   // ttl value is not in range
        if (logger) {
            logger->warn(TAG, "TTL sent is not a valid TTL value");
        }
        return ER_BAD_ARG_2;
    }

    ajn::BusAttachment* pBus = Transport::getInstance()->getBusAttachment();
    String originalSender = pBus ? pBus->GetUniqueName() : "";
    return PayloadAdapter::sendPayload(m_PropertyStore,
                                       notification.getMessageType(), notification.getText(),
                                       notification.getCustomAttributes(), ttl, notification.getRichIconUrl(),
                                       notification.getRichAudioUrl(), notification.getRichIconObjectPath(),
                                       notification.getRichAudioObjectPath(), notification.getControlPanelServiceObjectPath(),
                                       originalSender.c_str());
}

QStatus NotificationSender::deleteLastMsg(NotificationMessageType messageType)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "Delete Last Message called");
    }

    //Validation
    if (messageType < 0 || messageType >= MESSAGE_TYPE_CNT) {
        if (logger) {
            logger->warn(TAG, "MessageType sent is not a valid MessageType");
        }
        return ER_BAD_ARG_1;
    }

    Transport* transport = Transport::getInstance();
    return transport->deleteLastMsg(messageType);
}

