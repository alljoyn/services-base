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

#import "AJTMTimeServiceDateTime.h"
#import "alljoyn/time/TimeServiceDateTime.h"

@interface AJTMTimeServiceDateTime ()
@property ajn::services::TimeServiceDateTime* handle;
@end

@implementation AJTMTimeServiceDateTime

-(id)init
{
    self = [super init];
    if (self) {
        self.handle = new ajn::services::TimeServiceDateTime();
    }
    return self;
}

-(id)initWithHandle:(const ajn::services::TimeServiceDateTime*) handle
{
    self = [super init];
    if (self) {
        self.handle = (ajn::services::TimeServiceDateTime*)handle;
    }
    return self;
}

-(QStatus)populateWithDate:(AJTMTimeServiceDate*) date time:(AJTMTimeServiceTime*) time offsetMinutes:(int16_t) offsetMinutes
{
    return self.handle->init([date getHandle], [time getHandle], offsetMinutes);
}

-(bool)isValid
{
    return self.handle->isValid();
}

-(AJTMTimeServiceDate*)date
{
   
    return [[AJTMTimeServiceDate alloc] initWithHandle:(ajn::services::TimeServiceDate*)&self.handle->getDate()];
}

-(AJTMTimeServiceTime*)time
{
    return [[AJTMTimeServiceTime alloc] initWithHandle:(ajn::services::TimeServiceTime*)&self.handle->getTime()];
}

-(int16_t)offsetMinutes
{
    return self.handle->getOffsetMinutes();
}

-(ajn::services::TimeServiceDateTime*)getHandle
{
    return _handle;
}

@end