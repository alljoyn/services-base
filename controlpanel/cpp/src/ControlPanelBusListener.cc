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

#include <alljoyn/controlpanel/ControlPanelBusListener.h>
#include <iostream>
#include <algorithm>
#include <alljoyn/controlpanel/LogModule.h>

namespace ajn {
namespace services {

ControlPanelBusListener::ControlPanelBusListener() :
    BusListener(), SessionPortListener(), SessionListener(), m_SessionPort(0)
{
}

ControlPanelBusListener::~ControlPanelBusListener()
{
}

void ControlPanelBusListener::setSessionPort(ajn::SessionPort sessionPort)
{
    m_SessionPort = sessionPort;
}

SessionPort ControlPanelBusListener::getSessionPort()
{
    return m_SessionPort;
}

bool ControlPanelBusListener::AcceptSessionJoiner(ajn::SessionPort sessionPort, const char* joiner, const ajn::SessionOpts& opts)
{
    if (sessionPort != m_SessionPort) {
        return false;
    }

    QCC_DbgPrintf(("Accepting JoinSessionRequest"));
    return true;
}

void ControlPanelBusListener::SessionJoined(SessionPort sessionPort, SessionId sessionId, const char* joiner)
{
    if (std::find(m_SessionIds.begin(), m_SessionIds.end(), sessionId) != m_SessionIds.end()) {
        return;
    }
    m_SessionIds.push_back(sessionId);
}

void ControlPanelBusListener::SessionMemberAdded(SessionId sessionId, const char* uniqueName)
{
    if (std::find(m_SessionIds.begin(), m_SessionIds.end(), sessionId) != m_SessionIds.end()) {
        return;
    }
    m_SessionIds.push_back(sessionId);
}

void ControlPanelBusListener::SessionMemberRemoved(SessionId sessionId, const char* uniqueName)
{
    std::vector<SessionId>::iterator it = std::find(m_SessionIds.begin(), m_SessionIds.end(), sessionId);
    if (it != m_SessionIds.end()) {
        m_SessionIds.erase(it);
    }
}

void ControlPanelBusListener::SessionLost(SessionId sessionId, SessionLostReason reason)
{
    std::vector<SessionId>::iterator it = std::find(m_SessionIds.begin(), m_SessionIds.end(), sessionId);
    if (it != m_SessionIds.end()) {
        m_SessionIds.erase(it);
    }
}

const std::vector<SessionId>& ControlPanelBusListener::getSessionIds() const
{
    return m_SessionIds;
}

} /* namespace services */
} /* namespace ajn */