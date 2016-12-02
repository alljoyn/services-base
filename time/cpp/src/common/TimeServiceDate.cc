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

#include <alljoyn/time/TimeServiceDate.h>

using namespace ajn;
using namespace services;

//Constructor
TimeServiceDate::TimeServiceDate() : m_Year(0), m_Month(0), m_Day(0)
{
}

//Destructor
TimeServiceDate::~TimeServiceDate() {
}

//Initialize the object with its data.
QStatus TimeServiceDate::init(uint16_t year, uint8_t month, uint8_t day)
{

    QStatus status;
    if (!isValid(year, month, day, status)) {

        return status;
    }

    m_Year   = year;
    m_Month  = month;
    m_Day    = day;

    return ER_OK;
}

//Checks whether data of the object is valid, the object variables have a correct values.
bool TimeServiceDate::isValid() const
{

    QStatus status;
    return isValid(m_Year, m_Month, m_Day, status);
}

//Checks whether data of the object is valid, the object variables have a correct values.
bool TimeServiceDate::isValid(uint16_t year, uint8_t month, uint8_t day, QStatus& status) const
{

    if (year <= 0) {

        status = ER_BAD_ARG_1;
        QCC_LogError(ER_BAD_ARG_1, ("Year is not a positive number"));
        return false;
    }

    if (month < 1 || month > 12) {

        status = ER_BAD_ARG_2;
        QCC_LogError(ER_BAD_ARG_2, ("Month is not in the expected range"));
        return false;
    }

    if (day < 1 || day > 31) {

        status = ER_BAD_ARG_3;
        QCC_LogError(ER_BAD_ARG_3, ("Day is not in the correct range"));
        return false;
    }

    status = ER_OK;
    return true;
}

//Returns year
const uint16_t TimeServiceDate::getYear() const
{

    return m_Year;
}


//Returns month
const uint8_t TimeServiceDate::getMonth() const
{

    return m_Month;
}

//Returns day
const uint8_t TimeServiceDate::getDay() const
{

    return m_Day;
}