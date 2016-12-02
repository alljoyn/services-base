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


#ifndef TESTCUSTOMTIMESERVICETIMERBUSOBJ_H_
#define TESTCUSTOMTIMESERVICETIMERBUSOBJ_H_

#include <stdio.h>
#include <alljoyn/time/TimeServiceTimerBusObj.h>

namespace ajn {
namespace services {

/**
 * Test Server Timer functionality
 */
class TestCustomTimerBusObject : public TimeServiceTimerBusObj {

  public:
    TestCustomTimerBusObject(qcc::String objectPath);

    ~TestCustomTimerBusObject() { }

    /**
     * Callback for the CoolTitle method
     * @param member - the member called
     * @param msg - the message of the method
     */
    void getCoolTitle(const InterfaceDescription::Member* member, Message& msg);

  protected:
    QStatus createCustomInterfaceHook(BusAttachment* bus);

};
}
}
#endif /* defined(TESTCUSTOMTIMESERVICETIMERBUSOBJ_H_) */