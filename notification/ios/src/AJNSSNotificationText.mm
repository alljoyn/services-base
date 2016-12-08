/******************************************************************************
 * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#import "AJNSNotificationText.h"
#import "alljoyn/services_common/AJSVCConvertUtil.h"

@interface AJNSNotificationText ()
/** cpp notificationText Handler*/
@property (nonatomic) ajn::services::NotificationText *handle;
@end

@implementation AJNSNotificationText

- (AJNSNotificationText *)initWithLang:(NSString *)language andText:(NSString *)text
{
    self = [super init];
    if (self) {
        self.handle = new ajn::services::NotificationText([AJSVCConvertUtil convertNSStringToQCCString:language], [AJSVCConvertUtil convertNSStringToQCCString:text]);
    }
    return self;
}

/**
 * Set Language for Notification
 * @param language
 */
- (void)setLanguage:(NSString *)language
{
    self.handle->setLanguage([AJSVCConvertUtil convertNSStringToQCCString:language]);
}

/**
 * Get Language for Notification
 * @return language
 */
- (NSString *)getLanguage
{
    return [AJSVCConvertUtil convertQCCStringtoNSString:self.handle->getLanguage()];
}

/**
 * Set Text for Notification
 * @param text the text to set
 */
- (void)setText:(NSString *)text
{
    self.handle->setText([AJSVCConvertUtil convertNSStringToQCCString:text]);
}

/**
 * Get Text for Notification
 * @return text
 */
- (NSString *)getText
{
    return [AJSVCConvertUtil convertQCCStringtoNSString:(self.handle->getText())];
}

@end