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


#ifndef __TIMECLIENT__ALARMSIGNALHANDLER__
#define __TIMECLIENT__ALARMSIGNALHANDLER__

#include <alljoyn/InterfaceDescription.h>
#include <alljoyn/MessageReceiver.h>
#include <alljoyn/services_common/AsyncTaskQueue.h>
#include <alljoyn/time/TimeServiceClientAlarm.h>
#include "../common/Mutex.h"

namespace ajn {
namespace services {

class AlarmSignalTask;

class AlarmSignalHandler : public MessageReceiver {

  public:

    /**
     * GetInstance
     * @return TimeServiceSignalHandler created only once.
     */
    static AlarmSignalHandler* getInstance();

    /**
     * Adds TimeServiceClientAlarm event listener
     * @param alarm
     * @return status
     */
    QStatus registerAlarm(TimeServiceClientAlarm* alarm);

    /**
     * Remove TimeServiceClientAlarm event listener
     * @param alarm
     */
    void unRegisterAlarm(TimeServiceClientAlarm* alarm);

  private:

    /**
     * Constructor
     */
    AlarmSignalHandler();

    /**
     * Desctructor
     */
    virtual ~AlarmSignalHandler();

    /**
     *  Pointer to the AlarmSignalHandler
     */
    static AlarmSignalHandler* s_Instance;

    std::vector<TimeServiceClientAlarm*> m_Alarms;

    Mutex m_Protector;

    const ajn::InterfaceDescription::Member* m_AlarmSignalMethod;

    static AsyncTaskQueue s_SignalQueue;

    static AlarmSignalTask s_SignalTask;

    void handleAlarmSignal(const ajn::InterfaceDescription::Member* member,
                           const char* srcPath, ajn::Message& msg);

    TimeServiceClientAlarm* findAlarmByObjectPathForDevice(qcc::String objPath, qcc::String sender);
};

} /* namespace services */
} /* namespace ajn */

#endif /* defined(__TIMECLIENT__ALARMSIGNALHANDLER__) */