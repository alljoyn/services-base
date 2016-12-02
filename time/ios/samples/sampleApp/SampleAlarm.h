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

#import "alljoyn/time/AJTMTimeServiceServerAlarmBase.h"
#import "alljoyn/time/AJTMTimeServiceServerAlarm.h"

@interface SampleAlarm : AJTMTimeServiceServerAlarmBase <AJTMTimeServiceServerAlarm>

-(id)init;

#pragma mark AJTMTimeServiceServerAlarm protocol methods
/**
 * Returns TimeServiceSchedule of this Alarm
 *
 * @return Alarm
 */
-(AJTMTimeServiceSchedule*)schedule;

/**
 * Set TimeServiceSchedule to this Alarm object
 *
 * @param schedule TimeServiceSchedule
 * @return Status ER_OK If succeeded to set the Schedule, otherwise, if this Alarm does not support
 * one of the Schedule values, return an appropriate failure status.
 */
-(QStatus)setSchedule:(AJTMTimeServiceSchedule *) schedule;

/**
 * Returns Alarm title.
 *
 * @return Optional textual description of what this Alarm is
 */
-(NSString*)title;

/**
 * Set title to this Alarm.
 *
 * @param title Alarm title.
 * Title is an optional textual description of what this Alarm is set for.
 */
-(void)setTitle:(NSString*) title;

/**
 * Returns whether or not this Alarm is enabled
 *
 * @return Is Alarm enabled
 */
-(bool)isEnabled;

/**
 * Set whether or not this Alarm is enabled
 *
 * @param enabled Is Alarm enabled
 */
-(void)setEnabled:(bool) enabled;

@end