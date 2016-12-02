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
#import "alljoyn/time/TimeServicePeriod.h"

@interface AJTMTimeServicePeriod : NSObject

- (id)initWithHandle:(const ajn::services::TimeServicePeriod *)handle;

/**
 * Initialize the object with its data.
 *
 * @param hour A positive number
 * @param minute Expected range: 0-59
 * @param second Expected range: 0-59
 * @param millisecond Expected range: 0-999
 * @return ER_OK if the object was initialized successfully and valid arguments have been passed to this method,
 * otherwise ER_BAD_ARGUMENT status of the appropriate argument is returned
 */
-(QStatus)populateWithHour:(uint8_t) hour minute:(uint8_t) minute second:(uint8_t) second millisecond:(uint16_t) millisecond;

/**
 * Checks whether data of the object is valid, the object variables have a correct values.
 *
 * @return TRUE of the object is valid
 */
-(bool)isValid;

/**
 * Returns Hour
 *
 * @return Returns hour
 */
-(uint8_t)hour;

/**
 * Returns Minute
 *
 * @return Returns minute
 */
-(uint8_t)minute;

/**
 * Returns second
 *
 * @return Returns second
 */
-(uint8_t)second;

/**
 * Returns millisecond
 *
 * @return Returns millisecond
 */
-(uint8_t)millisecond;

-(const ajn::services::TimeServicePeriod&)getHandle;

@end