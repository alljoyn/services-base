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

#import "AJNSNotificationSender.h"

@interface AJNSNotificationSender ()
@property (nonatomic) ajn::services::AboutPropertyStoreImpl *propertyStore;

@end

@implementation AJNSNotificationSender

- (AJNSNotificationSender *)initWithPropertyStore:(AJNAboutPropertyStoreImpl *)propertyStore
{
	self = [super init];
	if (self) {
		self.propertyStore = [propertyStore getHandle];
		if (self.propertyStore) {
			self.senderHandle = new ajn::services::NotificationSender(self.propertyStore);
			return self;
		}
	}
	return nil;
}

/**
 * Send notification
 * @param notification
 * @param ttl message ttl
 * @return status
 */
- (QStatus)send:(AJNSNotification *)ajnsNotification ttl:(uint16_t)ttl
{
	return (self.senderHandle->send(*ajnsNotification.handle, ttl));
}

/**
 * Delete last message that was sent with given MessageType
 * @param messageType MessageType of message to be deleted
 * @return status
 */
- (QStatus)deleteLastMsg:(AJNSNotificationMessageType)messageType
{
	return(self.senderHandle->deleteLastMsg((ajn::services::NotificationMessageType)messageType));
}

- (ajn::services::AboutPropertyStoreImpl *)getPropertyStore
{
	return self.propertyStore;
}

@end