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
#import "alljoyn/notification/NotificationSender.h"
#import "alljoyn/notification/Notification.h"
#import "alljoyn/notification/NotificationEnums.h"
#import "AJNSNotificationEnums.h"
#import "AJNSNotification.h"
#import "alljoyn/about/AJNPropertyStore.h"
#import "alljoyn/about/AJNAboutPropertyStoreImpl.h"

/**
 AJNSNotificationSender class
 */
@interface AJNSNotificationSender : NSObject
///---------------------
/// @name Properties
///---------------------
/** NotificationSender Handler*/
@property (nonatomic)ajn::services::NotificationSender * senderHandle;


///---------------------
/// @name Initialization
///---------------------

/**
 * Designated initializer
 * @param propertyStore property store
 */
- (AJNSNotificationSender *)initWithPropertyStore:(AJNAboutPropertyStoreImpl *)propertyStore;

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
 * Get the property store in this sender
 * @return The property store
 */
- (ajn ::services ::AboutPropertyStoreImpl *)getPropertyStore;

@end