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

#include <alljoyn/time/TimeServiceDateTime.h>

using namespace ajn;
using namespace services;

/**
 * Constructor
 */
TimeServiceDateTime::TimeServiceDateTime() : m_OffsetMinutes(0)
{
}

/**
 * Destructor
 */
TimeServiceDateTime::~TimeServiceDateTime()
{
}

//Initialize the DateTime object
QStatus TimeServiceDateTime::init(const TimeServiceDate& date, const TimeServiceTime& time, int16_t offsetMinutes)
{

    if (!date.isValid()) {

        QCC_LogError(ER_BAD_ARG_1, ("Date is not a valid object"));
        return ER_BAD_ARG_1;
    }

    if (!time.isValid()) {

        QCC_LogError(ER_BAD_ARG_2, ("Time is not a valid object"));
        return ER_BAD_ARG_2;
    }

    m_Date          = date;
    m_Time          = time;
    m_OffsetMinutes = offsetMinutes;

    return ER_OK;
}

//Checks whether data of the object is valid, the object variables have a correct values, date and time are valid
bool TimeServiceDateTime::isValid() const
{

    return m_Date.isValid() && m_Time.isValid();
}

//Get Date
const TimeServiceDate& TimeServiceDateTime::getDate() const
{

    return m_Date;
}

//Get Time
const TimeServiceTime& TimeServiceDateTime::getTime() const
{

    return m_Time;
}

//Get offset
const int16_t TimeServiceDateTime::getOffsetMinutes() const
{

    return m_OffsetMinutes;
}