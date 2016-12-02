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

#include "MyDeviceOvenAction.h"
#include "ControlPanelProvided.h"


namespace ajn {
namespace services {

MyDeviceOvenAction::MyDeviceOvenAction(qcc::String name, Widget* rootWidget) : Action(name, rootWidget)
{

}

MyDeviceOvenAction::~MyDeviceOvenAction()
{

}

bool MyDeviceOvenAction::executeCallBack()
{
    AJ_Printf("Starting the Oven. Execute was called\n");

    //by default return success
    return true;
}

} /* namespace services */
} /* namespace ajn */