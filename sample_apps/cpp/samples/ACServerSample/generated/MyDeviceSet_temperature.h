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

#ifndef MYDEVICESET_TEMPERATURE_H_
#define MYDEVICESET_TEMPERATURE_H_

#include <alljoyn/controlpanel/Property.h>

namespace ajn {
namespace services {

/**
 * Generated class
 */
class MyDeviceSet_temperature : public ajn::services::Property {
  public:
    MyDeviceSet_temperature(qcc::String name, Widget* rootWidget, PropertyType propertyType);
    virtual ~MyDeviceSet_temperature();

    QStatus setValue(uint16_t value);
};
} //namespace services
} //namespace ajn

#endif /* MYDEVICESET_TEMPERATURE_H_ */