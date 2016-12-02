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

#include "MyDeviceAreYouSure.h"
#include "ControlPanelProvided.h"


namespace ajn {
namespace services {

MyDeviceAreYouSure::MyDeviceAreYouSure(qcc::String name, Widget* rootWidget) : Dialog(name, rootWidget)
{

}

MyDeviceAreYouSure::~MyDeviceAreYouSure()
{

}

bool MyDeviceAreYouSure::executeAction1CallBack()
{
    AJ_Printf("Execute Action1 was called\n");

    //by default return success
    return true;
}

bool MyDeviceAreYouSure::executeAction2CallBack()
{
    return executeActionNotDefined();

    //by default return success
    return true;
}

bool MyDeviceAreYouSure::executeAction3CallBack()
{
    return executeActionNotDefined();

    //by default return success
    return true;
}

} /* namespace services */
} /* namespace ajn */