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

#include <alljoyn/controlpanel/ControlPanelControllee.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelConstants.h"
#include <alljoyn/controlpanel/LogModule.h>

namespace ajn {
namespace services {
using namespace cpsConsts;

ControlPanelControllee::ControlPanelControllee()
{
}

ControlPanelControllee::~ControlPanelControllee()
{

}

QStatus ControlPanelControllee::registerObjects(BusAttachment* bus)
{
    QStatus status = ER_OK;

    for (size_t indx = 0; indx < m_Units.size(); indx++) {
        status = m_Units[indx]->registerObjects(bus);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not register Objects for the Units"));
            return status;
        }
    }
    return status;
}

QStatus ControlPanelControllee::unregisterObjects(BusAttachment* bus)
{
    QStatus returnStatus = ER_OK;

    for (size_t indx = 0; indx < m_Units.size(); indx++) {
        QStatus status = m_Units[indx]->unregisterObjects(bus);
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not register Objects for the Units"));
            returnStatus = status;
        }
    }

    return returnStatus;
}

QStatus ControlPanelControllee::addControlPanelUnit(ControlPanelControlleeUnit* unit)
{
    if (!unit) {
        QCC_DbgHLPrintf(("Could not add a NULL unit"));
        return ER_BAD_ARG_1;
    }

    m_Units.push_back(unit);
    return ER_OK;
}

const std::vector<ControlPanelControlleeUnit*>& ControlPanelControllee::getControlleeUnits() const
{
    return m_Units;
}

} /* namespace services */
} /* namespace ajn */