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
#import "alljoyn/controlpanel/CPSDate.h"

/**
 * AJCPSCPSDate allows sending of a Date as a Property
 */
@interface AJCPSCPSDate : NSObject

- (id)initWithDay:(uint16_t) day month:(uint16_t) month year:(uint16_t) year;


- (id)initWithHandle:(ajn ::services ::CPSDate *)handle;

/**
 * Get the day value of the date
 * @return day value
 */
- (uint16_t)getDay;

/**
 * Set the day Value of the date
 * @param day value
 */
- (void)setDay:(uint16_t)day;

/**
 * Get the Month value of the date
 * @return month value
 */
- (uint16_t)getMonth;

/**
 * Set the Month value of the date
 * @param month value
 */
- (void)setMonth:(uint16_t)month;

/**
 * Get the Year value of the date
 * @return year value
 */
- (uint16_t)getYear;

/**
 * Set the Year value of the date
 * @param year value
 */
- (void)setYear:(uint16_t)year;

@property (nonatomic, readonly)ajn::services::CPSDate * handle;

@end