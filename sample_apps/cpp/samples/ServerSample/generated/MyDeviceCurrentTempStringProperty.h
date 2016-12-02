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

#ifndef MYDEVICECURRENTTEMPSTRINGPROPERTY_H_
#define MYDEVICECURRENTTEMPSTRINGPROPERTY_H_

#include <alljoyn/controlpanel/Property.h>

namespace ajn {
namespace services {

/**
 * Generated class
 */
class MyDeviceCurrentTempStringProperty : public ajn::services::Property {
  public:
    MyDeviceCurrentTempStringProperty(qcc::String name, Widget* rootWidget, PropertyType propertyType);
    virtual ~MyDeviceCurrentTempStringProperty();

    QStatus setValue(const char* value);
};
} //namespace services
} //namespace ajn

#endif /* MYDEVICECURRENTTEMPSTRINGPROPERTY_H_ */