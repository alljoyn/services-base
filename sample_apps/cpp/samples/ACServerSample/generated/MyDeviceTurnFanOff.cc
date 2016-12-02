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

#include "MyDeviceTurnFanOff.h"
#include "ControlPanelProvided.h"


namespace ajn {
namespace services {

MyDeviceTurnFanOff::MyDeviceTurnFanOff(qcc::String name, Widget* rootWidget) : Dialog(name, rootWidget)
{

}

MyDeviceTurnFanOff::~MyDeviceTurnFanOff()
{

}

bool MyDeviceTurnFanOff::executeAction1CallBack()
{
    OnTurnFanOffButton(true);

    //by default return success
    return true;
}

bool MyDeviceTurnFanOff::executeAction2CallBack()
{
    OnTurnFanOffButton(false);

    //by default return success
    return true;
}

bool MyDeviceTurnFanOff::executeAction3CallBack()
{
    return executeActionNotDefined();

    //by default return success
    return true;
}

} /* namespace services */
} /* namespace ajn */