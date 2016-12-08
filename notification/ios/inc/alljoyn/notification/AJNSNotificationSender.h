/******************************************************************************
 *  *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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

#import <Foundation/Foundation.h>
#import "alljoyn/notification/NotificationSender.h"
#import "alljoyn/notification/Notification.h"
#import "alljoyn/notification/NotificationEnums.h"
#import "AJNSNotificationEnums.h"
#import "AJNSNotification.h"
#import "alljoyn/about/AJNAboutPropertyStoreImpl.h"
#import "AJNAboutData.h"

/**
 AJNSNotificationSender class
 */
@interface AJNSNotificationSender : NSObject
///---------------------
/// @name Properties
///---------------------
/** NotificationSender Handler*/
@property (nonatomic) ajn::services::NotificationSender *senderHandle;


///---------------------
/// @name Initialization
///---------------------

/**
 * @deprecated initWithPropertyStore was deprecated in September 2016 for 16.10
 * Designated initializer
 * @param propertyStore property store
 */
- (AJNSNotificationSender *)initWithPropertyStore:(AJNAboutPropertyStoreImpl *)propertyStore __deprecated;

/**
 * Designated initializer
 * @param aboutData about data
 */
- (AJNSNotificationSender *)initWithAboutData:(AJNAboutData *)aboutData;

///---------------------
/// @name Instance methods
///---------------------

/**
 * Send notification
 * @param ajnsNotification `ajnsNotification` object
 * @param ttl message ttl
 * @return status
 */
- (QStatus)send:(AJNSNotification *)ajnsNotification ttl:(uint16_t)ttl;

/**
 * Delete last message that was sent with given MessageType
 * @param messageType MessageType of message to be deleted
 * @return status
 */
- (QStatus)deleteLastMsg:(AJNSNotificationMessageType)messageType;

/**
 * @deprecated getPropertyStore was deprecated in September 2016 for 16.10
 * Get the property store in this sender
 * @return The property store
 */
- (ajn::services::AboutPropertyStoreImpl *)getPropertyStore __deprecated;

/**
 * Get the about data in this sender
 * @return The about data
 */
- (ajn::AboutData *)getAboutData;

@end