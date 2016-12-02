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

#include "MyDeviceLightConfirm.h"
#include "ControlPanelProvided.h"


namespace ajn {
namespace services {

MyDeviceLightConfirm::MyDeviceLightConfirm(qcc::String name, Widget* rootWidget) : Dialog(name, rootWidget)
{

}

MyDeviceLightConfirm::~MyDeviceLightConfirm()
{

}

bool MyDeviceLightConfirm::executeAction1CallBack()
{
    AJ_Printf("Execute Action1 was called\n");

    //by default return success
    return true;
}

bool MyDeviceLightConfirm::executeAction2CallBack()
{
    AJ_Printf("Execute Action2 was called\n");

    //by default return success
    return true;
}

bool MyDeviceLightConfirm::executeAction3CallBack()
{
    AJ_Printf("Execute Action3 was called\n");

    //by default return success
    return true;
}

} /* namespace services */
} /* namespace ajn */