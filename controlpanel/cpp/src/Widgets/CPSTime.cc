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

#include <alljoyn/controlpanel/CPSTime.h>

namespace ajn {
namespace services {

CPSTime::CPSTime(uint16_t hour, uint16_t minute, uint16_t second) :
    m_Hour(hour), m_Minute(minute), m_Second(second)
{
}

CPSTime::~CPSTime()
{
}

uint16_t CPSTime::getHour() const
{
    return m_Hour;
}

void CPSTime::setHour(uint16_t hour)
{
    m_Hour = hour;
}

uint16_t CPSTime::getMinute() const
{
    return m_Minute;
}

void CPSTime::setMinute(uint16_t minute)
{
    m_Minute = minute;
}

uint16_t CPSTime::getSecond() const
{
    return m_Second;
}

void CPSTime::setSecond(uint16_t second)
{
    m_Second = second;
}

} /* namespace services */
} /* namespace ajn */