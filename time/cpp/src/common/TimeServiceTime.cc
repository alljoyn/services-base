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

#include <alljoyn/time/TimeServiceTime.h>

using namespace ajn;
using namespace services;

//Constructor
TimeServiceTime::TimeServiceTime() : m_Hour(0), m_Minute(0), m_Second(0), m_Millisecond(0)
{
}

//Destructor
TimeServiceTime::~TimeServiceTime()
{
}

//Initialize the object with its data.
QStatus TimeServiceTime::init(uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond)
{

    QStatus status;
    if (!isValid(hour, minute, second, millisecond, status)) {

        return status;
    }

    m_Hour          = hour;
    m_Minute        = minute;
    m_Second        = second;
    m_Millisecond   = millisecond;

    return ER_OK;
}

//Checks whether data of the object is valid, the object variables have a correct values.
bool TimeServiceTime::isValid() const
{

    QStatus status;
    return isValid(m_Hour, m_Minute, m_Second, m_Millisecond, status);
}

//Checks whether data of the object is valid, the object variables have a correct values.
bool TimeServiceTime::isValid(uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond, QStatus& status) const
{

    if (hour > 23) {

        status = ER_BAD_ARG_1;
        QCC_LogError(ER_BAD_ARG_1, ("Hour is not in the expected range"));
        return false;
    }

    if (minute > 59) {

        status = ER_BAD_ARG_2;
        QCC_LogError(ER_BAD_ARG_2, ("Minute is not in the expected range"));
        return false;
    }

    if (second > 59) {

        status = ER_BAD_ARG_3;
        QCC_LogError(ER_BAD_ARG_3, ("Second is not in the expected range"));
        return false;
    }

    if (millisecond > 999) {

        status = ER_BAD_ARG_4;
        QCC_LogError(ER_BAD_ARG_4, ("Milliseconds is not in the expected range"));
        return false;
    }

    status = ER_OK;
    return true;
}

//Returns hour
const uint8_t TimeServiceTime::getHour() const
{

    return m_Hour;
}

//Returns minute
const uint8_t TimeServiceTime::getMinute() const
{

    return m_Minute;
}

// Returns second
const uint8_t TimeServiceTime::getSecond() const
{

    return m_Second;
}

//Returns milliseconds
const uint16_t TimeServiceTime::getMillisecond() const
{

    return m_Millisecond;
}