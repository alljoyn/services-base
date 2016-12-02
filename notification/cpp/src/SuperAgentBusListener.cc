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

#include "SuperAgentBusListener.h"
#include "Transport.h"
#include <alljoyn/notification/NotificationService.h>
#include <qcc/String.h>
#include <alljoyn/notification/LogModule.h>

namespace ajn {
namespace services {

SuperAgentBusListener::SuperAgentBusListener(ajn::BusAttachment* bus)
    : BusListener(), m_Bus(bus), m_SuperAgentBusUniqueName("")
{

}

SuperAgentBusListener::~SuperAgentBusListener()
{

}

void SuperAgentBusListener::FoundAdvertisedName(const char* name, TransportMask transport, const char* namePrefix)
{
    QCC_DbgPrintf(("FoundAdvertisedName name:%s", name));
    // We must enable concurrent callbacks since some of the calls below are blocking
    m_Bus->EnableConcurrentCallbacks();

    if (0 == strcmp(name, m_SuperAgentBusUniqueName.c_str())) {
        Transport::getInstance()->listenToSuperAgent(m_SuperAgentBusUniqueName.c_str());
    }
}

void SuperAgentBusListener::LostAdvertisedName(const char* name, TransportMask transport, const char* prefix)
{
    QCC_DbgTrace(("LostAdvertisedName"));
    // We must enable concurrent callbacks since some of the calls below are blocking
    m_Bus->EnableConcurrentCallbacks();

    if (0 == ::strcmp(name, m_SuperAgentBusUniqueName.c_str())) {
        Transport::getInstance()->cancelListenToSuperAgent(m_SuperAgentBusUniqueName.c_str());
    }
}

} //services
} //ajn