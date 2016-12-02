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

#include <alljoyn/controlpanel/CPSDate.h>

namespace ajn {
namespace services {

CPSDate::CPSDate(uint16_t day, uint16_t month, uint16_t year) :
    m_Day(day), m_Month(month), m_Year(year)
{
}

CPSDate::~CPSDate()
{
}

uint16_t CPSDate::getDay() const
{
    return m_Day;
}

void CPSDate::setDay(uint16_t day)
{
    m_Day = day;
}

uint16_t CPSDate::getMonth() const
{
    return m_Month;
}

void CPSDate::setMonth(uint16_t month)
{
    m_Month = month;
}

uint16_t CPSDate::getYear() const
{
    return m_Year;
}

void CPSDate::setYear(uint16_t year)
{
    m_Year = year;
}

} /* namespace services */
} /* namespace ajn */