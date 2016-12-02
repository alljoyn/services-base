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

#ifndef MYDEVICECURRENTHUMIDITYSTRINGPROPERTY_H_
#define MYDEVICECURRENTHUMIDITYSTRINGPROPERTY_H_

#include <alljoyn/controlpanel/Property.h>

namespace ajn {
namespace services {

/**
 * Generated class
 */
class MyDeviceCurrentHumidityStringProperty : public ajn::services::Property {
  public:
    MyDeviceCurrentHumidityStringProperty(qcc::String name, Widget* rootWidget, PropertyType propertyType);
    virtual ~MyDeviceCurrentHumidityStringProperty();

    QStatus setValue(const char* value);
};
} //namespace services
} //namespace ajn

#endif /* MYDEVICECURRENTHUMIDITYSTRINGPROPERTY_H_ */