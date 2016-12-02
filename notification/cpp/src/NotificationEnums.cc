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

#include <alljoyn/notification/NotificationEnums.h>

using namespace ajn;
using namespace services;

const qcc::String MessageTypeUtil::MESSAGE_TYPE_STRINGS[3] = { "emergency", "warning", "info" };

qcc::String const& MessageTypeUtil::getMessageTypeString(int32_t messageType)
{
    return MESSAGE_TYPE_STRINGS[messageType];
}

NotificationMessageType MessageTypeUtil::getMessageType(int32_t messageType)
{
    return (NotificationMessageType) messageType;
}

int32_t MessageTypeUtil::getNumMessageTypes()
{
    return (int32_t) MESSAGE_TYPE_CNT;
}
