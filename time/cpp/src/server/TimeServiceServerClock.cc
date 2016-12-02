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

#include <alljoyn/time/TimeServiceServerClock.h>
#include "TimeServiceClockBusObj.h"
#include <alljoyn/time/LogModule.h>

using namespace ajn;
using namespace services;

//Constructor
TimeServiceServerClock::TimeServiceServerClock() : m_ClockBusObj(NULL)
{

    QCC_DbgTrace(("%s", __FUNCTION__));
}

// Destructor
TimeServiceServerClock::~TimeServiceServerClock()
{

    QCC_DbgTrace(("%s", __FUNCTION__));
    release();
}

//Releases object resources
void TimeServiceServerClock::release()
{

    QCC_DbgTrace(("%s", __FUNCTION__));

    if (!m_ClockBusObj) {

        return;
    }

    QCC_DbgHLPrintf(("Releasing the Clock '%s'", m_ClockBusObj->getObjectPath().c_str()));

    delete m_ClockBusObj;
    m_ClockBusObj = NULL;
}

//Returns object path of this Clock
qcc::String const& TimeServiceServerClock::getObjectPath() const
{

    if (!m_ClockBusObj) {

        QCC_LogError(ER_FAIL, ("This Clock hasn't been created yet"));
        return qcc::String::Empty;
    }

    return m_ClockBusObj->getObjectPath();
}

//Set Clock Bus Object
QStatus TimeServiceServerClock::setClockBusObj(TimeServiceClockBusObj* clockBusObj)
{

    if (!clockBusObj) {

        QCC_LogError(ER_FAIL, ("No clockBusObj given"));
        return ER_FAIL;
    }

    if (m_ClockBusObj) {

        QCC_LogError(ER_FAIL, ("clockBusObj has been previously set"));
        return ER_FAIL;
    }

    m_ClockBusObj = clockBusObj;
    return ER_OK;
}

//Returns ClockBusObj
const TimeServiceClockBusObj* TimeServiceServerClock::getClockBusObj()
{

    return m_ClockBusObj;
}