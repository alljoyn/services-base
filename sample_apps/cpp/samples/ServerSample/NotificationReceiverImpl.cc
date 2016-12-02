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

#include <time.h>
#include <stdio.h>

#include <alljoyn/notification/NotificationService.h>
#include <alljoyn/notification/NotificationText.h>
#include <alljoyn/notification/Notification.h>
#include "NotificationReceiverImpl.h"
#include <alljoyn/Status.h>
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace ajn;
using namespace services;
using namespace qcc;

static char const* const QCC_MODULE = "ServerSample";

NotificationReceiverImpl::NotificationReceiverImpl() {
}

NotificationReceiverImpl::~NotificationReceiverImpl() {
}

void NotificationReceiverImpl::Receive(Notification const& notification) {

    QCC_DbgHLPrintf(("Received message with msgId: %d; msgType: %d",
                     notification.getMessageId(), notification.getMessageType()));

    // output the message text(s)
    std::vector<NotificationText> vecMessages = notification.getText();
    for (std::vector<NotificationText>::const_iterator vecMessage_it = vecMessages.begin(); vecMessage_it != vecMessages.end(); ++vecMessage_it) {
        QCC_SyncPrintf("Message[%s]: %s;\n", vecMessage_it->getLanguage().c_str(),
                       vecMessage_it->getText().c_str());
    }

    QCC_SyncPrintf("DeviceId: %s; DeviceName: %s; AppId: %s; AppName: %s; SenderBusName: %s;\n",
                   notification.getDeviceId(), notification.getDeviceName(), notification.getAppId(),
                   notification.getAppName(), notification.getSenderBusName());
}

void NotificationReceiverImpl::Dismiss(const int32_t msgId, const qcc::String appId)
{
    QCC_DbgHLPrintf(("NotificationReceiverImpl::Dismiss with msgId=%d appId=%s", msgId, appId.c_str()));
}