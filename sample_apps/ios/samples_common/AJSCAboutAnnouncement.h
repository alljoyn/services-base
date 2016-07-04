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

#import <Foundation/Foundation.h>
#import "AJNSessionOptions.h"
#import "AJNMessageArgument.h"

/**
 * AJSCAboutAnnouncement is a storage class that holds about announcement information received
 * from the server.
 * To be backwards compatible with Core's deprecated About service class AJNAnnouncement, and to
 * aid in the deprecation cleanup of one iOS sample at a time, the old deprecated 'objectDescriptions'
 * and 'aboutData' properties have been included as part of this class. A temporary property
 * 'usesDeprecatedAnnounce' is set internally depending on which init method is called, and can be
 * used for separating deprecated from non-deprecated code used in files shared by multiple samples.
 * After all dependencies on the initWithVersion method have been removed within the iOS samples,
 * all properties / methods marked as __deprecated in this class can be removed.
 */
@interface AJSCAboutAnnouncement : NSObject

@property (strong, nonatomic) NSString *busName;
@property uint16_t version;
@property AJNSessionPort port;
@property (strong, nonatomic) AJNMessageArgument *objectDescriptionArg;
@property (strong, nonatomic) AJNMessageArgument *aboutDataArg;

@property (strong, nonatomic) NSMutableDictionary *objectDescriptions __deprecated;
@property (strong, nonatomic) NSMutableDictionary *aboutData __deprecated;

@property (readonly) BOOL usesDeprecatedAnnounce __deprecated;

/**
 * Create AJSCAboutAnnouncement object using the parameters passed in from AJNAboutListener's
 * didReceiveAnnounce method.
 */
- (id)initWithBusName:(NSString *)busName version:(uint16_t)version sessionPort:(AJNSessionPort)sessionPort objectDescriptionArg:(AJNMessageArgument *)objectDescriptionArg aboutDataArg:(AJNMessageArgument *)aboutDataArg;

/**
 * @deprecated
 * Create AJSCAboutAnnouncement object using the parameters passed in from the deprecated
 * AJNAnnouncementListener's announce method.
 */
- (id)initWithVersion:(uint16_t)version port:(uint16_t)port busName:(NSString *)busName objectDescriptions:(NSMutableDictionary *)objectDescs aboutData:(NSMutableDictionary **)aboutData __deprecated;

@end
