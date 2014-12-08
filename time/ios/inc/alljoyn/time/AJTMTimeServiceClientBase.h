/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
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

/**
 * This class is experimental, and as such has not been tested.
 * Please help make it more robust by contributing fixes if you find issues
 **/

#import <Foundation/Foundation.h>
#import "alljoyn/time/TimeServiceClientBase.h"
#import "AJTMTimeServiceClient.h"

@interface AJTMTimeServiceClientBase : NSObject
/**
 * Constructor
 *
 * @param tsClient TimeServiceClient managing this object
 * @param objectPath Object path of the Time Service server side object
 */
-(id)initWithTimeServiceClient:(AJTMTimeServiceClient *)tsClient objectPath:(NSString *)objectPath;

/**
 * Object path of the  Time Service server side object
 *
 * @return object path
 */
-(NSString *) getObjectPath;

/**
 * Returns TimeServiceClient that is managing this object
 */
-(AJTMTimeServiceClient *) getTsClient;

@end
