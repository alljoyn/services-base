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
#import "AJNMessageArgument.h"

/**
 * AJSCAboutDataConverter is a utility class to convert About data structures into other
 * representations. String conversion methods will return @"" on failure.
 */
@interface AJSCAboutDataConverter : NSObject

/**
 * @deprecated Deprecated in October 2016 for 16.10
 * Convert AJNMessageArgument to NSString.
 */
+ (NSString *)messageArgumentToString:(AJNMessageArgument *)ajnMsgArg __deprecated;

/**
 * @deprecated Deprecated in October 2016 for 16.10
 * Convert NSMutableDictionary of about data (in the format NSString/AJNMessageArgument) to
 * NSString.
 */
+ (NSString *)aboutDataDictionaryToString:(NSMutableDictionary *)aboutDataDict __deprecated;

/**
 * @deprecated Deprecated in October 2016 for 16.10
 * Convert NSMutableDictionary of object descriptions (in the format of NSString/NSMutableArray)
 * to NSString.
 */
+ (NSString *)objectDescriptionsDictionaryToString:(NSMutableDictionary *)objectDescDict __deprecated;

/**
 * Convert AJNMessageArgument containing the announced About Data to a NSMutableDictionary
 */
+ (NSMutableDictionary *)aboutDataArgToDict:(AJNMessageArgument *)aboutDataArg;

/**
 * Convert AJNMessageArgument containing the object descriptions to a NSMutableDictionary
 */
+ (NSMutableDictionary *)objectDescriptionArgToDict:(AJNMessageArgument *)objectDescriptionsArg;

/**
 * Convert AJNMessageArgument containing the announced About Data to a String 
 */
+ (NSString *)aboutDataArgString:(AJNMessageArgument *)aboutDataArg;

/**
 * Convert AJNMessageArgument containing the Object Descriptions to a String
 */
+ (NSString *)objectDescriptionArgString:(AJNMessageArgument *)objectDescriptionsArg;

@end