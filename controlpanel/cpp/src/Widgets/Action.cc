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

#include <alljoyn/controlpanel/Action.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "../BusObjects/ActionBusObject.h"
#include "../ControlPanelConstants.h"
#include <alljoyn/controlpanel/LogModule.h>

namespace ajn {
namespace services {
using namespace cpsConsts;

Action::Action(qcc::String const& name, Widget* rootWidget) : Widget(name, rootWidget, WIDGET_TYPE_ACTION)
{
}

Action::Action(qcc::String const& name, Widget* rootWidget, ControlPanelDevice* device) :
    Widget(name, rootWidget, device, WIDGET_TYPE_ACTION)
{

}

Action::~Action()
{
}

WidgetBusObject* Action::createWidgetBusObject(BusAttachment* bus, qcc::String const& objectPath,
                                               uint16_t langIndx, QStatus& status)
{
    return new ActionBusObject(bus, objectPath, langIndx, status, this);
}

QStatus Action::executeAction()
{
    if (m_ControlPanelMode == CONTROLLEE_MODE) {
        QCC_DbgHLPrintf(("Cannot execute Action. Widget is a Controllee widget"));
        return ER_NOT_IMPLEMENTED;
    }

    if (!m_BusObjects.size()) {
        QCC_DbgHLPrintf(("Cannot execute Action. BusObject is not set"));
        return ER_BUS_BUS_NOT_STARTED;
    }

    return ((ActionBusObject*)m_BusObjects[0])->ExecuteAction();
}

bool Action::executeCallBack()
{
    return false;
}

} /* namespace services */
} /* namespace ajn */