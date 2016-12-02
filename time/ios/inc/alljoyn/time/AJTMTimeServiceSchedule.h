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
#import "alljoyn/Status.h"
#import "AJTMTimeServiceTime.h"
#import "alljoyn/time/TimeServiceSchedule.h"

@interface AJTMTimeServiceSchedule : NSObject

/**
 * Days of week used to define Alarm schedule.
 */
typedef NS_ENUM (NSInteger, AJTMWeekDay) { 

    SUNDAY      = 0x1,
    MONDAY      = 0x2,
    TUESDAY     = 0x4,
    WEDNESDAY   = 0x8,
    THURSDAY    = 0x10,
    FRIDAY      = 0x20,
    SATURDAY    = 0x40,
};

-(id)init;


/**
 * Initialize the object
 *
 * @param time Time of the Alarm
 * @param weekDays Bitmap of WeekDay(s) when the Alarm should fire.
 * Value of zero means that the Alarm is fired only once.
 * @return ER_OK if the object was initialized successfully and valid arguments have been passed to this method
 * otherwise  ER_BAD_ARGUMENT of the appropriate argument is returned
 */
-(QStatus)populateWithTime:(AJTMTimeServiceTime*) time weekDays:(uint8_t) weekDays;

/**
 * Checks whether data of the object is valid.
 */
-(bool)isValid;

/**
 * Returns Time
 *
 * @return Time
 */
-(AJTMTimeServiceTime*)time;

/**
 * Returns Bitmap of WeekDay(s) when the Alarm should fire.
 * Value of zero means that the Alarm is fired only once.
 */
-(uint8_t)weekDays;

-(const ajn::services::TimeServiceSchedule&)getHandle;

@end