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

#include "MyDeviceTurnFanOn.h"
#include "ControlPanelProvided.h"


namespace ajn {
namespace services {

MyDeviceTurnFanOn::MyDeviceTurnFanOn(qcc::String name, Widget* rootWidget) : Dialog(name, rootWidget)
{

}

MyDeviceTurnFanOn::~MyDeviceTurnFanOn()
{

}

bool MyDeviceTurnFanOn::executeAction1CallBack()
{
    OnTurnFanOnButton(true);

    //by default return success
    return true;
}

bool MyDeviceTurnFanOn::executeAction2CallBack()
{
    OnTurnFanOnButton(false);

    //by default return success
    return true;
}

bool MyDeviceTurnFanOn::executeAction3CallBack()
{
    return executeActionNotDefined();

    //by default return success
    return true;
}

} /* namespace services */
} /* namespace ajn */