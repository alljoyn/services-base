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

#ifndef MYDEVICEAREYOUSURE_H_
#define MYDEVICEAREYOUSURE_H_

#include <alljoyn/controlpanel/Dialog.h>

#if defined(QCC_OS_GROUP_WINDOWS)
/* Disabling warning C 4702. unreachable code */
#pragma warning(push)
#pragma warning(disable: 4702)
#endif

namespace ajn {
namespace services {

/**
 * Generated class
 */
class MyDeviceAreYouSure : public ajn::services::Dialog {
  public:
    MyDeviceAreYouSure(qcc::String name, Widget* rootWidget);
    virtual ~MyDeviceAreYouSure();

    bool executeAction1CallBack();
    bool executeAction2CallBack();
    bool executeAction3CallBack();
};
} //namespace services
} //namespace ajn

#endif /* MYDEVICEAREYOUSURE_H_ */