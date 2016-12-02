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

#include "AJTMTimeServiceServerTimeAuthorityAdapter.h"

AJTMTimeServiceServerTimeAuthorityAdapter::AJTMTimeServiceServerTimeAuthorityAdapter(id<AJTMTimeServiceServerClock> timeServiceServerClock)
{
    handle = timeServiceServerClock;
}

AJTMTimeServiceServerTimeAuthorityAdapter::~AJTMTimeServiceServerTimeAuthorityAdapter()
{
}


const ajn::services::TimeServiceDateTime& AJTMTimeServiceServerTimeAuthorityAdapter::getDateTime() const
{
    return *[[handle dateTime] getHandle];

}

void AJTMTimeServiceServerTimeAuthorityAdapter::setDateTime(ajn::services::TimeServiceDateTime const& dateTime)
{
    ajn::services::TimeServiceDateTime *cDateTime = new ajn::services::TimeServiceDateTime();
    cDateTime->init(dateTime.getDate(), dateTime.getTime(), dateTime.getOffsetMinutes());
    AJTMTimeServiceDateTime *dateTimeTmp = [[AJTMTimeServiceDateTime alloc] initWithHandle:cDateTime];
    [handle setDateTime:dateTimeTmp];
}

bool AJTMTimeServiceServerTimeAuthorityAdapter::isSet() const
{
    return [handle isSet];
}