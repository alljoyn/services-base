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

#include "TestAuthorityClock.h"

using namespace ajn;
using namespace services;

//Constructor
TestAuthorityClock::TestAuthorityClock() :  TimeServiceServerAuthorityClock()
{
}

//Destructor
TestAuthorityClock::~TestAuthorityClock()
{
}

//DateTime object
const TimeServiceDateTime& TestAuthorityClock::getDateTime() const
{

    return m_Clock.getDateTime();
}

//Set the DateTime object
void TestAuthorityClock::setDateTime(TimeServiceDateTime const& dateTime)
{

    m_Clock.setDateTime(dateTime);
}

//Whether or not this clock has ever been set.
bool TestAuthorityClock::isSet() const
{

    return true;
}