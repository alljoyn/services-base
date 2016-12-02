/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#ifdef _WIN32
/* Disable deprecation warnings */
#pragma warning(disable: 4996)
#endif

#include <alljoyn/notification/NotificationSender.h>
#include <alljoyn/notification/NotificationService.h>
#include <alljoyn/notification/Notification.h>

#include "PayloadAdapter.h"
#include "Transport.h"
#include "NotificationConstants.h"
#include <string.h>
#include <alljoyn/notification/LogModule.h>

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

NotificationSender::NotificationSender(AboutData* aboutdata) :
    m_aboutdata(aboutdata), m_PropertyStore(NULL)
{

}

NotificationSender::NotificationSender(PropertyStore* propertyStore) :
    m_aboutdata(NULL), m_PropertyStore(propertyStore)
{

}

QStatus NotificationSender::send(Notification const& notification, uint16_t ttl)
{
    QCC_DbgTrace(("Send Message called"));

    //Validations
    if (notification.getMessageType() < 0 || notification.getMessageType() >= MESSAGE_TYPE_CNT) {
        QCC_LogError(ER_BAD_ARG_1, ("MessageType sent is not a valid MessageType"));
        return ER_BAD_ARG_1;
    }

    if (notification.getText().size() == 0) {
        QCC_LogError(ER_BAD_ARG_1, ("There must be at least one notification defined"));
        return ER_BAD_ARG_1;
    } else if ((TTL_MIN > ttl) || (ttl > TTL_MAX)) {   // ttl value is not in range
        QCC_LogError(ER_BAD_ARG_2, ("TTL sent is not a valid TTL value"));
        return ER_BAD_ARG_2;
    }

    ajn::BusAttachment* pBus = Transport::getInstance()->getBusAttachment();
    String originalSender = pBus ? pBus->GetUniqueName() : "";
    if (m_aboutdata) {
        return PayloadAdapter::sendPayload(m_aboutdata,
                                           notification.getMessageType(), notification.getText(),
                                           notification.getCustomAttributes(), ttl, notification.getRichIconUrl(),
                                           notification.getRichAudioUrl(), notification.getRichIconObjectPath(),
                                           notification.getRichAudioObjectPath(), notification.getControlPanelServiceObjectPath(),
                                           originalSender.c_str());
    }

    if (m_PropertyStore) {
        return PayloadAdapter::sendPayload(m_PropertyStore,
                                           notification.getMessageType(), notification.getText(),
                                           notification.getCustomAttributes(), ttl, notification.getRichIconUrl(),
                                           notification.getRichAudioUrl(), notification.getRichIconObjectPath(),
                                           notification.getRichAudioObjectPath(), notification.getControlPanelServiceObjectPath(),
                                           originalSender.c_str());
    }
    return ER_FAIL;
}

QStatus NotificationSender::deleteLastMsg(NotificationMessageType messageType)
{
    QCC_DbgTrace(("Delete Last Message called"));

    //Validation
    if (messageType < 0 || messageType >= MESSAGE_TYPE_CNT) {
        QCC_LogError(ER_BAD_ARG_1, ("MessageType sent is not a valid MessageType"));
        return ER_BAD_ARG_1;
    }

    Transport* transport = Transport::getInstance();
    return transport->deleteLastMsg(messageType);
}

QStatus NotificationSender::getLastMsgId(NotificationMessageType messageType, int32_t* messageId)
{
    QCC_DbgTrace(("Get Last Message Id called"));

    if (messageType < 0 || messageType >= MESSAGE_TYPE_CNT) {
        QCC_LogError(ER_BAD_ARG_1, ("MessageType sent is not a valid MessageType"));
        return ER_BAD_ARG_1;
    }

    Transport* transport = Transport::getInstance();
    return transport->getLastMsgId(messageType, messageId);
}