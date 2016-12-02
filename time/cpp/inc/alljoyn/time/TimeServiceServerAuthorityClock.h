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

#ifndef TIMESERVICESERVERAUTHORITYCLOCK_H_
#define TIMESERVICESERVERAUTHORITYCLOCK_H_

#include <alljoyn/time/TimeServiceServerClock.h>

namespace ajn {
namespace services {

/**
 * Extend this class to receive events related to this TimeAuthority Clock
 * and send TimeAuthorityClock.timeSync() signal.
 */
class TimeServiceServerAuthorityClock : public TimeServiceServerClock {

    /**
     * This AuthorityClock object is a Friend of the TimeServiceServer
     */
    friend class TimeServiceServer;

  public:

    /**
     * Constructor
     */
    TimeServiceServerAuthorityClock();

    /**
     * Destructor
     */
    ~TimeServiceServerAuthorityClock();

    /**
     * Send TimeSync signal to suggest clients of this TimeAuthority Clock to synchronize their time
     *
     * @return Status of sending the signal
     */
    QStatus timeSync();
};

} /* namespace services */
} /* namespace ajn */

#endif /* TIMESERVICESERVERAUTHORITYCLOCK_H_ */