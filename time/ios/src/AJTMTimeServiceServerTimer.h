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
#import "AJTMTimeServicePeriod.h"

@protocol AJTMTimeServiceServerTimer <NSObject>

/**
 * Returns TimeServiceSchedule of this Timer
 *
 * @return interval
 */
-(AJTMTimeServicePeriod*)interval;

/**
 * Set interval of this Timer object
 *
 * @param schedule TimeServiceSchedule
 * @return Status ER_OK If succeeded to set the Schedule, otherwise, if this Timer does not support
 * one of the interval values, return an appropriate failure status.
 */
-(QStatus)setInterval:(AJTMTimeServicePeriod*) interval;

/**
 * Returns time left for this Timer
 *
 * @return time left
 */
-(AJTMTimeServicePeriod*)timeLeft;

/**
 * Returns Timer title.
 *
 * @return Optional textual description of what this Timer is
 */
-(NSString*)title;

/**
 * Set title of this Timer.
 *
 * @param title Timer title.
 * Title is an optional textual description of what this Timer is set for.
 */
-(void)setTitle:(NSString*) title;

/**
 * Returns whether or not this Timer is running
 *
 * @return Is Timer running
 */
-(bool)isRunning;

/**
 * Returns number of repeats for this Timer
 *
 * @return repeats
 */
-(uint16_t)repeat;

/**
 * Sets number of repeats for this Timer
 *
 * @return time left
 */
-(void)setRepeat:(uint16_t) repeat;

/**
 * Start Timer
 */
-(void)start;

/**
 * Reset Timer
 */
-(void)reset;

/**
 * Pause
 */
-(void)pause;

/**
 * Releases object resources
 */
-(void)releaseObject;


@end