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

#import "SampleTimer.h"

@interface SampleTimer ()
@property (strong, nonatomic) AJTMTimeServicePeriod* timerInterval;
@property (strong, nonatomic) AJTMTimeServicePeriod* timerLeft;
@property (strong, nonatomic) NSString* timerTitle;
@property (nonatomic) bool isRunning;
@property (nonatomic) uint16_t timerRepeat;
@end

@implementation SampleTimer

-(id)init
{
    self = [super init];
    if (self) {
        _timerInterval = [[AJTMTimeServicePeriod alloc]init];
        _timerLeft = [[AJTMTimeServicePeriod alloc]init];
        _timerTitle = @"";
    }
    return self;
}

-(AJTMTimeServicePeriod*)interval
{
    return _timerInterval;
}

-(QStatus)setInterval:(AJTMTimeServicePeriod*) interval
{
    if (!interval) {
        NSLog(@"AJTMTimeServicePeriod object is nil");
        return ER_FAIL;
    }
    _timerInterval = interval;
    return ER_OK;
}

-(AJTMTimeServicePeriod*)timeLeft
{
    return _timerLeft;
}

-(NSString*)title
{
    return _timerTitle;
}

-(void)setTitle:(NSString*) title
{
    _timerTitle = title;
}

-(bool)isRunning
{
    return _isRunning;
}

-(uint16_t)repeat
{
    return _timerRepeat;
}

-(void)setRepeat:(uint16_t) repeat
{
    _timerRepeat = repeat;
}

-(void)start
{
    NSLog(@"%@", NSStringFromSelector(_cmd));
}

-(void)reset
{
    NSLog(@"%@", NSStringFromSelector(_cmd));
}

-(void)pause
{
    NSLog(@"%@", NSStringFromSelector(_cmd));
}

-(void)releaseObject
{
    NSLog(@"%@", NSStringFromSelector(_cmd));
}

@end
