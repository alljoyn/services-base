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
#import "alljoyn/services_common/AJSVCGenericLogger.h"

/**
 ConfigLogger enable application logging by a given logger.
 */
@interface AJCFGConfigLogger : NSObject

/**
 * Create a ConfigLogger Shared instance.
 * @return ConfigLogger instance(created only once).
 */
+ (id)sharedInstance;

/**
 Set the logger to be in use.
 @param logger The logger to be in used.
 </br> If the given logger is nil -  will use AJSVCGenericLoggerDefaultImpl.
 */
- (void)setLogger:(id <AJSVCGenericLogger> )logger;

/**
 Return the logger that being used.
 */
- (id <AJSVCGenericLogger> )logger;

@end