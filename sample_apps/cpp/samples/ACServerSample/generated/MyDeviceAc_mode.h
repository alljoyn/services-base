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

#ifndef MYDEVICEAC_MODE_H_
#define MYDEVICEAC_MODE_H_

#include <alljoyn/controlpanel/Property.h>

namespace ajn {
namespace services {

/**
 * Generated class
 */
class MyDeviceAc_mode : public ajn::services::Property {
  public:
    MyDeviceAc_mode(qcc::String name, Widget* rootWidget, PropertyType propertyType);
    virtual ~MyDeviceAc_mode();

    QStatus setValue(uint16_t value);
};
} //namespace services
} //namespace ajn

#endif /* MYDEVICEAC_MODE_H_ */