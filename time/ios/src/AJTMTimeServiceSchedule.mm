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

#import "AJTMTimeServiceSchedule.h"
#import "alljoyn/time/TimeServiceSchedule.h"

@interface AJTMTimeServiceSchedule ()
@property ajn::services::TimeServiceSchedule* handle;
@end

@implementation AJTMTimeServiceSchedule

-(id)init
{
    self = [super init];
    if (self) {
        self.handle = new ajn::services::TimeServiceSchedule();
    }
    return self;
}

-(QStatus)populateWithTime:(AJTMTimeServiceTime*) time weekDays:(uint8_t) weekDays
{
   return self.handle->init([time getHandle], weekDays);
}

-(bool)isValid
{
    return self.handle->isValid();
}

-(AJTMTimeServiceTime*)time
{

    ajn::services::TimeServiceTime timeServiceTime  = self.handle->getTime();
    return [[AJTMTimeServiceTime alloc] initWithHandle:&timeServiceTime]; 
}

-(uint8_t)weekDays
{
    return self.handle->getWeekDays();
}

-(const ajn::services::TimeServiceSchedule&)getHandle
{
    return *self.handle;
}
@end