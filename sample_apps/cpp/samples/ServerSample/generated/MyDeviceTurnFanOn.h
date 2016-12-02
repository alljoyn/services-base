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

#ifndef MYDEVICETURNFANON_H_
#define MYDEVICETURNFANON_H_

#include <alljoyn/controlpanel/Dialog.h>

namespace ajn {
namespace services {

/**
 * Generated class
 */
class MyDeviceTurnFanOn : public ajn::services::Dialog {
  public:
    MyDeviceTurnFanOn(qcc::String name, Widget* rootWidget);
    virtual ~MyDeviceTurnFanOn();

    bool executeAction1CallBack();
    bool executeAction2CallBack();
    bool executeAction3CallBack();
};
} //namespace services
} //namespace ajn

#endif /* MYDEVICETURNFANON_H_ */