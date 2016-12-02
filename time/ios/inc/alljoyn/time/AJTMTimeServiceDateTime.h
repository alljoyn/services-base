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
#import "AJTMTimeServiceDate.h"
#import "AJTMTimeServiceTime.h"
#import "alljoyn/time/TimeServiceServerClock.h"


@interface AJTMTimeServiceDateTime : NSObject

/**
 * Initialize the object with data.
 *
 * @param handle
 * @return ER_OK if the object was initialized successfully and valid arguments have been passed to this method,
 * otherwise ER_BAD_ARGUMENT of the appropriate argument is returned
 */
-(id)initWithHandle:(ajn::services::TimeServiceDateTime const*) handle;

/**
 * Initialize the object with data.
 *
 * @param date
 * @param time
 * @param offsetMinutes
 * @return ER_OK if the object was initialized successfully and valid arguments have been passed to this method,
 * otherwise ER_BAD_ARGUMENT of the appropriate argument is returned
 */
-(QStatus)populateWithDate:(AJTMTimeServiceDate*) date time:(AJTMTimeServiceTime*) time offsetMinutes:(int16_t) offsetMinutes;

/**
 * Checks whether data of the object is valid, the object variables have a correct values, date and time are valid
 */
-(bool)isValid;

/**
 * Returns Date
 *
 * @return Date
 */
-(AJTMTimeServiceDate*)date;

/**
 * Returns Time
 *
 * @return Time
 */
-(AJTMTimeServiceTime*)time;

/**
 * Returns Offset minutes
 *
 * @return Offset minutes
 */
-(int16_t)offsetMinutes;

-(ajn::services::TimeServiceDateTime*)getHandle;


@end