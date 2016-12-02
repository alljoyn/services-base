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
#import "AJTMTimeServiceDateTime.h"

@protocol AJTMTimeServiceServerClock <NSObject>

/**
 * Returns DateTime object
 *
 * @return DateTime object
 */
-(AJTMTimeServiceDateTime*)dateTime;

/**
 * Set the DateTime object
 *
 * @param dateTime DateTime
 */
-(void)setDateTime:(AJTMTimeServiceDateTime*) dateTime;

/**
 * Whether or not this clock has been set.
 *
 * @return Returns TRUE if the Clock has been set since the last reboot
 */
-(bool)isSet;

@end