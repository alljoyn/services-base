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

#import "SampleAlarm.h"

@interface SampleAlarm ()

@property (strong, nonatomic) NSString* alarmTitle;
@property (nonatomic) bool enableFlag;
@property (strong, nonatomic) AJTMTimeServiceSchedule* alarmSchedule;
@end

@implementation SampleAlarm

-(id)init
{
    self = [super init];
    if (self) {
        _alarmTitle = @"";
        _alarmSchedule = [[AJTMTimeServiceSchedule alloc]init];
    }
    return self;
}

-(AJTMTimeServiceSchedule*)schedule
{
    return _alarmSchedule;
}

-(QStatus)setSchedule:(AJTMTimeServiceSchedule *) schedule
{
    if (!schedule)
    {
        NSLog(@"AJTMTimeServiceSchedule is nil");
        return ER_FAIL;
    } else {
    _alarmSchedule = schedule;
        return ER_OK;
    }
}

-(NSString*)title
{
    return _alarmTitle;
}

-(void)setTitle:(NSString*) title
{
    _alarmTitle = title;
}

-(bool)isEnabled
{
    return _enableFlag;
}

-(void)setEnabled:(bool) enabled
{
    _enableFlag = enabled;
}

@end