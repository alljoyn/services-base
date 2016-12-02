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

#import "AJCPSTime.h"

@interface AJCPSTime ()
@property (nonatomic) ajn::services::CPSTime *handle;
@end

@implementation AJCPSTime

- (id)initWithHour:(uint16_t)hour minute:(uint16_t)minute second:(uint16_t)second
{
    self = [super init];
    if (self) {
        self.handle = new ajn::services::CPSTime(hour, minute, second);
    }
    return self;
}

- (id)initWithHandle:(ajn::services::CPSTime *)handle
{
    self = [super init];
    if (self) {
        self.handle = handle;
    }
    return self;
}

/**
 * Get the hour value of the date
 * @return hour value
 */
- (uint16_t)getHour
{
    return self.handle->getHour();
}

/**
 * Set the hour Value of the date
 * @param hour value
 */
- (void)setHour:(uint16_t)hour
{
    self.handle->setHour(hour);
}

/**
 * Get the Minute value of the date
 * @return minute value
 */
- (uint16_t)getMinute
{
    return self.handle->getMinute();
}

/**
 * Set the Minute value of the date
 * @param minute value
 */
- (void)setMinute:(uint16_t)minute
{
    return self.handle->setMinute(minute);
}

/**
 * Get the Second value of the date
 * @return second value
 */
- (uint16_t)getSecond
{
    return self.handle->getSecond();
}

/**
 * Set the Second value of the date
 * @param second value
 */
- (void)setSecond:(uint16_t)second
{
    return self.handle->setSecond(second);
}

@end