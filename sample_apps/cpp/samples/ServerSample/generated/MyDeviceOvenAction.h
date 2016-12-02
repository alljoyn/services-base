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

#ifndef MYDEVICEOVENACTION_H_
#define MYDEVICEOVENACTION_H_

#include <alljoyn/controlpanel/Action.h>

namespace ajn {
namespace services {

/**
 * Generated class - implements Action Widget
 */
class MyDeviceOvenAction : public ajn::services::Action {
  public:
    MyDeviceOvenAction(qcc::String name, Widget* rootWidget);
    virtual ~MyDeviceOvenAction();

    bool executeCallBack();
};
} //namespace services
} //namespace ajn

#endif /* MYDEVICEOVENACTION_H_ */