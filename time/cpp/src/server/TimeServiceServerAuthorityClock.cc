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

#include <alljoyn/time/TimeServiceServerAuthorityClock.h>
#include <alljoyn/time/LogModule.h>
#include "TimeServiceAuthorityClockBusObj.h"

using namespace ajn;
using namespace services;

//Constructor
TimeServiceServerAuthorityClock::TimeServiceServerAuthorityClock() : TimeServiceServerClock()
{

    QCC_DbgTrace(("%s", __FUNCTION__));
}

//Destructor
TimeServiceServerAuthorityClock::~TimeServiceServerAuthorityClock()
{

    QCC_DbgTrace(("%s", __FUNCTION__));
}

//Emit TimeSync signal
QStatus TimeServiceServerAuthorityClock::timeSync()
{

    QCC_DbgTrace(("%s", __FUNCTION__));

    TimeServiceAuthorityClockBusObj* authClock = (TimeServiceAuthorityClockBusObj*)TimeServiceServerClock::getClockBusObj();

    if (!authClock) {

        QCC_LogError(ER_FAIL, ("This TimeAuthority Clock hasn't been created yet"));
        return ER_FAIL;
    }

    return authClock->sendTimeSync();
}