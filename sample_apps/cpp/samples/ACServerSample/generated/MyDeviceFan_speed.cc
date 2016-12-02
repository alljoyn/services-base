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

#include "MyDeviceFan_speed.h"
#include "ControlPanelProvided.h"


namespace ajn {
namespace services {

MyDeviceFan_speed::MyDeviceFan_speed(qcc::String name, Widget* rootWidget, PropertyType propertyType) :
    Property(name, rootWidget, propertyType)
{

}

MyDeviceFan_speed::~MyDeviceFan_speed()
{

}

QStatus MyDeviceFan_speed::setValue(uint16_t value)
{
    setFanSpeed(value);
    return ER_OK;
}

} /* namespace services */
} /* namespace ajn */