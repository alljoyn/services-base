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

#import "AJTMTimeServiceServerAlarmBase.h"

@interface AJTMTimeServiceServerAlarmBase ()
@property AJTMTimeServiceServerAlarmAdapter* handle;
@end

@implementation AJTMTimeServiceServerAlarmBase

-(id)init
{
    self = [super init];
    if (self) {
    }
    return self;
}

-(void)registerAdapter:(AJTMTimeServiceServerAlarmAdapter*) adapter
{
    self.handle = adapter;
}

-(void)releaseObject
{
    if (self.handle == nil) {
        return NSLog(@"Alarm handle is nil");
    }
    self.handle->release();
}

-(NSString*)objectPath
{
    if (self.handle == nil) {
        NSLog(@"Alarm handle is nil");
        return @"";
    }
    return @((self.handle->getObjectPath()).c_str());
}

-(QStatus)alarmReached
{
    return self.handle->alarmReached();
}

-(id)getHandle
{
    return (id)self.handle;
}



@end