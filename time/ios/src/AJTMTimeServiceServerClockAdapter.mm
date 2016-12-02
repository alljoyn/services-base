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

/**
 * This class is experimental, and as such has not been tested.
 * Please help make it more robust by contributing fixes if you find issues
 **/

#include "AJTMTimeServiceServerClockAdapter.h"

AJTMTimeServiceServerClockAdapter::AJTMTimeServiceServerClockAdapter(id<AJTMTimeServiceServerClock> timeServiceServerClock)
{
    handle = timeServiceServerClock;
}

AJTMTimeServiceServerClockAdapter::~AJTMTimeServiceServerClockAdapter()
{
}

const ajn::services::TimeServiceDateTime& AJTMTimeServiceServerClockAdapter::getDateTime() const
{
    return *[[handle dateTime] getHandle];

}

void AJTMTimeServiceServerClockAdapter::setDateTime(ajn::services::TimeServiceDateTime const& dateTime)
{
    ajn::services::TimeServiceDateTime *dt = new ajn::services::TimeServiceDateTime;

    dt->init(dateTime.getDate(), dateTime.getTime(), dateTime.getOffsetMinutes());

    AJTMTimeServiceDateTime *dateTimeTmp;
    dateTimeTmp = [[AJTMTimeServiceDateTime alloc] initWithHandle:dt];
    [handle setDateTime:dateTimeTmp];
}

bool AJTMTimeServiceServerClockAdapter::isSet() const
{
    return [handle isSet];
}
