/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifndef AJNSPROPERTYSTORE_H_
#define AJNSPROPERTYSTORE_H_

#import <Foundation/Foundation.h>
#import "Status.h"

/**
 AJNSPropertyStore protocol overview
 @deprecated AJNSPropertyStore was deprecated in September 2016 for 16.10
 */
__deprecated
@protocol AJNSPropertyStore <NSObject>

#pragma mark – Protocol enum
///---------------------
/// @name Protocol enum
///---------------------
/**
 * Filter has three possible values ANNOUNCE, READ,WRITE
 * READ is for data that is marked as read
 * ANNOUNCE is for data that is marked as announce
 * WRITE is for data that is marked as write
 */
typedef NS_ENUM (NSInteger, AJNSFilter) {
    ANNOUNCE, /** ANNOUNCE Property that has  ANNOUNCE  enabled */
    READ,    /** READ     Property that has READ  enabled */
    WRITE,   /* WRITE    Property that has  WRITE  enabled */
};

#pragma mark – Protocol methods

/**
 Calls Reset() implemented only for  ConfigService
 @deprecated Reset was deprecated in September 2016 for 16.10
 @return status
 */
- (QStatus)Reset __deprecated;

/**
 Reads all properties method
 @deprecated ReadAllWithLanguageTag:andFilter:andAllMsgArg: was deprecated in September 2016 for 16.10
 @param languageTag is the language to use for the action can be NULL meaning default.
 @param filter filter describe which properties to read.
 @param all all reference to MsgArg
 @return status
 */
- (QStatus)ReadAllWithLanguageTag:(NSString *)languageTag andFilter:(AJNSFilter)filter andAllMsgArg:(ajn::MsgArg &)all __deprecated;

/**
 Update properties method
 @deprecated UpdatePropertyName:andLanguageTag:andValue was deprecated in September 2016 for 16.10
 @param name name of the property
 @param languageTag languageTag is the language to use for the action can be NULL meaning default.
 @param value value is a pointer to the data to change.
 @return status
 */
- (QStatus)UpdatePropertyName:(NSString *)name andLanguageTag:(NSString *)languageTag andValue:(ajn::MsgArg *)value __deprecated;

/**
 Delete property method
 @deprecated DeletePropertyName:andLanguageTag: was deprecated in September 2016 for 16.10
 @param name name of the property
 @param languageTag languageTag is the language to use for the action can't be NULL.
 @return status
 */
- (QStatus)DeletePropertyName:(NSString *)name andLanguageTag:(NSString *)languageTag __deprecated;


@end

#endif /* AJNSPROPERTYSTORE_H_ */
