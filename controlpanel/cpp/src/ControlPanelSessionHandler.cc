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

#include <alljoyn/controlpanel/ControlPanelSessionHandler.h>
#include <alljoyn/controlpanel/ControlPanelDevice.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelConstants.h"
#include <sstream>
#include <alljoyn/controlpanel/LogModule.h>

namespace ajn {
namespace services {
using namespace cpsConsts;


ControlPanelSessionHandler::ControlPanelSessionHandler(ControlPanelDevice* device) : m_SessionId(0), m_Device(device)
{

}

ControlPanelSessionHandler::~ControlPanelSessionHandler()
{
}


void ControlPanelSessionHandler::SessionLost(ajn::SessionId sessionId)
{
    QCC_DbgPrintf(("Session lost for sessionId: %u", sessionId));
    m_SessionId = 0;

    ControlPanelListener* listener = m_Device->getListener();
    if (listener) {
        listener->sessionLost(m_Device);
    }
}

void ControlPanelSessionHandler::JoinSessionCB(QStatus status, ajn::SessionId id, const ajn::SessionOpts& opts, void* context)
{
    if (status != ER_OK) {
        QCC_LogError(status, ("Joining session failed."));
        ControlPanelListener* listener = m_Device->getListener();
        if (listener) {
            listener->errorOccured(m_Device, status, SESSION_JOIN, "Could not join session");
        }
        return;
    }

    QCC_DbgPrintf(("Joining session succeeded. SessionId: %u", id));

    m_SessionId = id;
    m_Device->handleSessionJoined();
}

ajn::SessionId ControlPanelSessionHandler::getSessionId() const
{
    return m_SessionId;
}

} /* namespace services */
} /* namespace ajn */