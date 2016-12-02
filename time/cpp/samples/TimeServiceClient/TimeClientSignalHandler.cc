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

#include "TimeClientSignalHandler.h"

using namespace ajn;
using namespace services;

//Constructor
TimeClientSignalHandler::TimeClientSignalHandler() : TimeAuthorityHandler()
{
}

//Destructor
TimeClientSignalHandler::~TimeClientSignalHandler()
{
}

//Handle TimeSync signal
void TimeClientSignalHandler::handleTimeSync(TimeServiceClientClock* clock)
{

    printf(">>>>>> Received TimeSync signal from the Object: '%s' <<<<<< \n", clock->getObjectPath().c_str());
}

//Handle AlarmReached signal
void TimeClientSignalHandler::handleAlarmReached(TimeServiceClientAlarm* alarm)
{

    printf(">>>>>> Received AlarmReached signal from the Object: '%s' <<<<<< \n", alarm->getObjectPath().c_str());
}

// Handle TimerEvent
void TimeClientSignalHandler::handleTimerEvent(TimeServiceClientTimer*timer)
{
    printf(">>>>>> Received TimerEvent signal from the Object: '%s' <<<<<< \n", timer->getObjectPath().c_str());

}

void TimeClientSignalHandler::handleRunStateChanged(TimeServiceClientTimer*timer, bool runState)
{
    printf(">>>>>> Received run state changed signal from the Object: '%s' state is: %d <<<<<< \n", timer->getObjectPath().c_str(), runState);

}