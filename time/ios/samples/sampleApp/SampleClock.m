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

#import <Foundation/Foundation.h>
#import "sampleClock.h"

@interface SampleClock ()
@property (strong, nonatomic) AJTMTimeServiceDateTime* service_DateTime;
@end

@implementation SampleClock

-(id)init
{
    self = [super init];
    if (self) {
        _service_DateTime = [[AJTMTimeServiceDateTime alloc]init];
    }
    return self;
}

-(AJTMTimeServiceDateTime*)dateTime
{
    return _service_DateTime;
}

-(void)setDateTime:(AJTMTimeServiceDateTime*) dateTime
{
    if (!dateTime) {
        NSLog(@"AJTMTimeServicePeriod object is nil");
        return;
    }
    _service_DateTime = dateTime;

}

-(bool)isSet
{
    return false;
}

@end