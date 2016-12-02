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
#include "qcc/String.h"

/**
 AJSVCConvertUtil class includes a string convert methods.
 */
@interface AJSVCConvertUtil : NSObject

/**
 Convert NSString to qcc::String.
 @param nsstring String from type NSString.
 @return qcc::String.
 */
+ (qcc::String)convertNSStringToQCCString:(NSString *)nsstring;

/**
 Convert qcc::String to NSString.
 @param qccstring String from type qcc::String.
 @return NSString.
 */
+ (NSString *)convertQCCStringtoNSString:(qcc::String)qccstring;

/**
 Convert NSString to const char.
 @param nsstring String from type NSString.
 @return const char.
 */
+ (const char *)convertNSStringToConstChar:(NSString *)nsstring;

/**
 Convert const char to NSString.
 @param constchar const char.
 @return NSString.
 */
+ (NSString *)convertConstCharToNSString:(const char *)constchar;
@end