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
#import "AJNSessionListener.h"

@class AJTMTimeServiceClient;

@protocol AJTMTimeServiceSessionListener <NSObject>

/**
 * The method is called when the existing session is lost.
 * Avoid blocking this thread with long running tasks.
 *
 * @param timeServiceClient TimeServiceClient that was used to create the session
 * @param reason Session lost reason
 */
-(void)sessionLost:(AJTMTimeServiceClient*)timeServiceClient SessionListener:(AJNSessionLostReason)reason;

/**
 * The method is called when a session was established following the call to
 * TimeServiceClient.joinSessionAsync().
 * Avoid blocking this thread with long running tasks.
 *
 * @param timeServiceClient TimeServiceClient that was used to create the session
 * @param status Check this QStatus to ensure that the session was created successfully
 */
-(void)sessionJoined:(AJTMTimeServiceClient*)timeServiceClient staus:(QStatus)status;

@end