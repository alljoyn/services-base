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

#import "AJTMTimeServiceDate.h"

@interface AJTMTimeServiceDate ()
@property ajn::services::TimeServiceDate* handle;
@end

@implementation AJTMTimeServiceDate

-(id)init
{
    self = [super init];
    if (self) {
        self.handle = new ajn::services::TimeServiceDate();
    }
    return self;
}

- (id)initWithHandle:(ajn::services::TimeServiceDate *)handle
{
    self = [super init];
    if (self) {
        self.handle = (ajn::services::TimeServiceDate *)handle;
    }
    return self;
}

-(QStatus)populateWithYear:(uint16_t) year month:(uint8_t) month day:(uint8_t) day
{
    QStatus status;
    status = self.handle->init(year, month, day);
    return status;
}

-(bool)isValid
{
    return self.handle->isValid();
}

-(uint16_t)year
{
    return self.handle->getYear();
}

-(uint8_t)month
{
    return self.handle->getMonth();
}

-(uint8_t)day
{
    return self.handle->getDay();
}

-(const ajn::services::TimeServiceDate&)getHandle
{
    return *self.handle;
}

@end