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

#ifndef CAPITAL_NAME_HERE_H_
#define CAPITAL_NAME_HERE_H_

#include <alljoyn/controlpanel/Dialog.h>

namespace ajn {
namespace services {

/**
 * Generated class
 */
class REGULAR_NAME_HERE : public ajn::services::Dialog {
  public:
    REGULAR_NAME_HERE(qcc::String name, Widget* rootWidget);
    virtual ~REGULAR_NAME_HERE();

    bool executeAction1CallBack();
    bool executeAction2CallBack();
    bool executeAction3CallBack();
};
} //namespace services
} //namespace ajn

#endif /* CAPITAL_NAME_HERE_H_ */