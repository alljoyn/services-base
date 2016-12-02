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

#include "NotificationProducerListener.h"
#include <iostream>
#include <algorithm>
#include <alljoyn/notification/NotificationService.h>
#include "NotificationConstants.h"
#include <alljoyn/notification/LogModule.h>

using namespace ajn;
using namespace services;
using namespace qcc;
using namespace nsConsts;

NotificationProducerListener::NotificationProducerListener() :
    SessionPortListener(), m_SessionPort(0)
{
}

NotificationProducerListener::~NotificationProducerListener()
{
}

void NotificationProducerListener::setSessionPort(ajn::SessionPort sessionPort)
{
    m_SessionPort = sessionPort;
}

SessionPort NotificationProducerListener::getSessionPort()
{
    return m_SessionPort;
}

bool NotificationProducerListener::AcceptSessionJoiner(ajn::SessionPort sessionPort, const char* joiner, const ajn::SessionOpts& opts)
{
    if (sessionPort != m_SessionPort) {
        return false;
    }

    QCC_DbgPrintf(("NotificationProducerListener::AcceptSessionJoiner() sessionPort=%hu joiner:%s", sessionPort, joiner));
    return true;
}

void NotificationProducerListener::SessionJoined(SessionPort sessionPort, SessionId sessionId, const char* joiner)
{

}