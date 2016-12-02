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

#import <Foundation/Foundation.h>
#import "alljoyn/controlpanel/CPSTime.h"

/**
 * AJCPSCPSTime allows sending of a Time as a Property
 */
@interface AJCPSTime : NSObject

- (id)initWithHour:(uint16_t)hour minute:(uint16_t)minute second:(uint16_t)second;

- (id)initWithHandle:(ajn::services::CPSTime *)handle;

/**
 * Get the hour value of the date
 * @return hour value
 */
- (uint16_t)getHour;

/**
 * Set the hour Value of the date
 * @param hour value
 */
- (void)setHour:(uint16_t)hour;

/**
 * Get the Minute value of the date
 * @return minute value
 */
- (uint16_t)getMinute;

/**
 * Set the Minute value of the date
 * @param minute value
 */
- (void)setMinute:(uint16_t)minute;

/**
 * Get the Second value of the date
 * @return second value
 */
- (uint16_t)getSecond;

/**
 * Set the Second value of the date
 * @param second value
 */
- (void)setSecond:(uint16_t)second;

@end