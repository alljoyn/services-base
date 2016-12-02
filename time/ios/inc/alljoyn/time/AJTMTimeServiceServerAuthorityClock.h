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
#import "alljoyn/time/TimeServiceServerAuthorityClock.h"
#import "alljoyn/time/AJTMTimeServiceServerTimeAuthorityBase.h"
#import "alljoyn/Status.h"

@interface AJTMTimeServiceServerAuthorityClock : AJTMTimeServiceServerTimeAuthorityBase

/**
 * Send TimeSync signal to suggest clients of this TimeAuthority Clock to synchronize their time
 *
 * @return Status of sending the signal
 */
-(QStatus)timeSync;

//-(const ajn::services::TimeServiceServerAuthorityClock&)getHandle;
-(ajn::services::TimeServiceServerAuthorityClock&)getHandle;


@end