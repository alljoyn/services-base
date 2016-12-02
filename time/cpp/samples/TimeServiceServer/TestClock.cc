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

#include "TestClock.h"

using namespace ajn;
using namespace services;

//Constructor
TestClock::TestClock() : TimeServiceServerClock(), m_IsSet(false)
{
    TimeServiceTime time;
    TimeServiceDate date;
    date.init(1970, 1, 1);

    m_DateTime.init(date, time, 0);
}

//Destructor
TestClock::~TestClock() {
}


//DateTime object
const TimeServiceDateTime& TestClock::getDateTime() const
{

    std::cout << "Get DateTime has been called, returning: ";
    printDateTime(m_DateTime);
    return m_DateTime;
}

//Set the DateTime object
void TestClock::setDateTime(TimeServiceDateTime const& dateTime)
{

    std::cout << "Set DateTime has been called DateTime: ";
    printDateTime(dateTime);

    m_DateTime = dateTime;

    if (!m_IsSet) {

        m_IsSet = true;
    }
}

//Whether or not this clock has ever been set.
bool TestClock::isSet() const
{

    std::cout << "Get IsSet has been called, returning: " << m_IsSet << std::endl;
    return m_IsSet;
}

//DateTime string representation
void TestClock::printDateTime(TimeServiceDateTime const& dateTime) const
{

    const TimeServiceDate& date = dateTime.getDate();
    const TimeServiceTime& time = dateTime.getTime();

    printf("{DateTime: {Date %d-%d-%d} {Time %d:%d:%d.%d} Offset: '%d'}\n", (int)date.getDay(), (int)date.getMonth(),
           (unsigned int)date.getYear(), (int)time.getHour(), (int)time.getMinute(), time.getSecond(), time.getMillisecond(),
           dateTime.getOffsetMinutes());
}
