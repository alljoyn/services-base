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


#ifndef __TIMECLIENT__TIMESERVICESIGNALHANDLER__
#define __TIMECLIENT__TIMESERVICESIGNALHANDLER__

#include <alljoyn/InterfaceDescription.h>
#include <alljoyn/MessageReceiver.h>
#include <alljoyn/services_common/AsyncTaskQueue.h>
#include <alljoyn/time/TimeServiceClientClock.h>
#include "../common/Mutex.h"

namespace ajn {
namespace services {

class TimeAuthoritySignalTask;

class TimeAuthoritySignalHandler : public MessageReceiver {

  public:

    /**
     * GetInstance
     * @return TimeServiceSignalHandler created only once.
     */
    static TimeAuthoritySignalHandler* getInstance();

    /**
     * Adds TimeServiceClientClock event listener
     * @param clock
     * @return status
     */
    QStatus registerClock(TimeServiceClientClock*clock);

    /**
     * Remove TimeServiceClientClock event listener
     * @param clock
     */
    void unRegisterClock(TimeServiceClientClock*clock);

  private:

    /**
     * Constructor
     */
    TimeAuthoritySignalHandler();

    /**
     * Desctructor
     */
    virtual ~TimeAuthoritySignalHandler();

    /**
     *  Pointer to the TimeAuthoritySignalHandler
     */
    static TimeAuthoritySignalHandler* s_Instance;

    std::vector<TimeServiceClientClock*> m_Clocks;

    Mutex m_Protector;

    const ajn::InterfaceDescription::Member* m_ClockSignalMethod;

    static AsyncTaskQueue s_SignalQueue;

    static TimeAuthoritySignalTask s_SignalTask;

    void handleClockSignal(const ajn::InterfaceDescription::Member* member,
                           const char* srcPath, ajn::Message& msg);

    TimeServiceClientClock* findClockByObjectPathForDevice(qcc::String objPath, qcc::String sender);
};

} /* namespace services */
} /* namespace ajn */

#endif /* defined(__TIMECLIENT__TIMESERVICESIGNALHANDLER__) */