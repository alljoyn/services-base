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

#include <alljoyn/time/TimeServiceSchedule.h>

using namespace ajn;
using namespace services;

//Constructor
TimeServiceSchedule::TimeServiceSchedule() : m_WeekDays(0)
{
}

//Destructor
TimeServiceSchedule::~TimeServiceSchedule() {
}

// Initialize the object
QStatus TimeServiceSchedule::init(const TimeServiceTime& time, uint8_t weekDays)
{

    if (!time.isValid()) {

        QCC_LogError(ER_BAD_ARG_1, ("Time is not a valid object"));
        return ER_BAD_ARG_1;
    }

    m_Time      = time;
    m_WeekDays  = weekDays;

    return ER_OK;
}

//Is valid
bool TimeServiceSchedule::isValid() const
{

    return m_Time.isValid();
}

//Returns Time
const TimeServiceTime& TimeServiceSchedule::getTime() const
{

    return m_Time;
}

//Returns Bitmap of WeekDay(s) when the Alarm should fire.
const uint8_t TimeServiceSchedule::getWeekDays() const
{

    return m_WeekDays;
}