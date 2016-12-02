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


#ifndef __TIMECLIENT__TIMERSIGNALHANDLER__
#define __TIMECLIENT__TIMERSIGNALHANDLER__

#include <alljoyn/InterfaceDescription.h>
#include <alljoyn/MessageReceiver.h>
#include <alljoyn/services_common/AsyncTaskQueue.h>
#include <alljoyn/time/TimeServiceClientTimer.h>
#include "../common/Mutex.h"

namespace ajn {
namespace services {

class TimerSignalTask;

class TimerSignalHandler : public MessageReceiver {

  public:
    /**
     * GetInstance
     * @return TimeServiceSignalHandler created only once.
     */
    static TimerSignalHandler* getInstance();

    /**
     * Adds TimeServiceClientTimer event listener
     * @param timer
     * @return status
     */
    QStatus registerTimer(TimeServiceClientTimer*Timer);

    /**
     * Remove TimeServiceClientTimer event listener
     * @param timer
     */
    void unRegisterTimer(TimeServiceClientTimer*Timer);

  private:

    /**
     * Constructor
     */
    TimerSignalHandler();
    /**
     * Desctructor
     */
    virtual ~TimerSignalHandler();

    /**
     *  pointer to TimerSignalHandler
     */
    static TimerSignalHandler* s_Instance;

    std::vector<TimeServiceClientTimer*> m_Timers;

    Mutex m_Protector;

    const ajn::InterfaceDescription::Member* m_TimerSignalMethod;
    const ajn::InterfaceDescription::Member* m_TimerRunStateChangedSignalMethod;

    static AsyncTaskQueue s_SignalQueue;

    static TimerSignalTask s_SignalTask;

    void handleTimerSignal(const ajn::InterfaceDescription::Member* member,
                           const char* srcPath, ajn::Message& msg);

    TimeServiceClientTimer* findTimerByObjectPathForDevice(qcc::String objPath, qcc::String sender);
};

} /* namespace services */
} /* namespace ajn */

#endif /* defined(__TIMECLIENT__TIMERSIGNALHANDLER__) */