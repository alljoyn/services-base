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
#import "AJSVCGenericLogger.h"

/**
 AJSVCGenericLoggerUtil is a util class that enable a log level covert/revert from the c++ layer to the objective-c one and vice versa.
 It also offers a log level toString capability.
 */
@interface AJSVCGenericLoggerUtil : NSObject

/**
 Convert QLogLevel(objective-c layer) to NSString.
 @param logLevel QLogLevel (objective-c layer).
 @return NSString representation of the QLogLevel content.
 */
+ (NSString *)toStringQLogLevel:(QLogLevel)logLevel;
@end