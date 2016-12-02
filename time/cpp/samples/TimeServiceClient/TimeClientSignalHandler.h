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

#ifndef TIMECLIENTSIGNALHANDLER_H_
#define TIMECLIENTSIGNALHANDLER_H_

#include <alljoyn/time/TimeServiceClient.h>

namespace ajn {
namespace services {

/**
 * Receive events about TimeService signals
 */
class TimeClientSignalHandler : public TimeAuthorityHandler, public AlarmHandler, public TimerHandler {

  public:

    /**
     * Constructor
     */
    TimeClientSignalHandler();

    /**
     * Destructor
     */
    ~TimeClientSignalHandler();

    /**
     * Handle TimeSync signals
     */
    void handleTimeSync(TimeServiceClientClock* clock);

    /**
     * Handle Alarm reached event, which was sent from the given TimeServiceClientAlarm
     */
    void handleAlarmReached(TimeServiceClientAlarm* alarm);

    void handleTimerEvent(TimeServiceClientTimer*timer);

    void handleRunStateChanged(TimeServiceClientTimer*timer, bool runState);

};

} /* namespace services */
} /* namespace ajn */

#endif /* TIMECLIENTSIGNALHANDLER_H_ */