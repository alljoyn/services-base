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
#import "alljoyn/time/TimeServiceDate.h"

@interface AJTMTimeServiceDate : NSObject

/* Constructor
 * @param handle A handle to a cpp object
 */
- (id)initWithHandle:(ajn::services::TimeServiceDate*)handle;

/**
 * Initialize the object with its data.
 *
 * @param year Expected: four digit format
 * @param month Expected range: 1-12
 * @param day Expected range: 1-31
 * @return ER_OK if the object was initialized successfully and valid arguments have been passed to this method,
 * otherwise ER_BAD_ARGUMENT status of the appropriate argument is returned
 */
-(QStatus)populateWithYear:(uint16_t) year month:(uint8_t) month day:(uint8_t) day;

/**
 * Checks whether data of the object is valid, the object variables have a correct values.
 *
 *  - year Expected: four digit format
 *  - month Expected range: 1-12
 *  - day Expected range: 1-31
 * @return TRUE of the object is valid
 */
-(bool)isValid;

/**
 * Returns year
 *
 * @return Returns year
 */
-(uint16_t)year;

/**
 * Returns month
 *
 * @return Returns month
 */
-(uint8_t)month;

/**
 * Returns day
 *
 * @return Returns day
 */
-(uint8_t)day;

-(const ajn::services::TimeServiceDate&)getHandle;

@end