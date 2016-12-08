/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
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