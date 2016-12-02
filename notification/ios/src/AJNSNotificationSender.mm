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
@property (nonatomic) ajn::AboutData *aboutData;
@end

@interface AJNAboutData (Private)
@property (nonatomic, readonly) ajn::AboutData *aboutData;
@end

@implementation AJNSNotificationSender

/**
 * @deprecated initWithPropertyStore was deprecated in September 2016 for 16.10
 * Initialize notification sender
 * @param property store
 * @return ajns notification sender
 */
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
 * Initialize notification sender
 * @param about data
 * @return ajns notification sender
 */
- (AJNSNotificationSender *)initWithAboutData:(AJNAboutData *)aboutData
{
    self = [super init];
    if (self) {
        self.aboutData = aboutData.aboutData;
        if (self.aboutData) {
            self.senderHandle = new ajn::services::NotificationSender(self.aboutData);
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
    return self.senderHandle->send(*ajnsNotification.handle, ttl);
}

/**
 * Delete last message that was sent with given MessageType
 * @param messageType MessageType of message to be deleted
 * @return status
 */
- (QStatus)deleteLastMsg:(AJNSNotificationMessageType)messageType
{
    return self.senderHandle->deleteLastMsg((ajn::services::NotificationMessageType)messageType);
}

- (ajn::services::AboutPropertyStoreImpl *)getPropertyStore
{
    return self.propertyStore;
}

- (ajn::AboutData *)getAboutData
{
    return self.aboutData;
}

@end