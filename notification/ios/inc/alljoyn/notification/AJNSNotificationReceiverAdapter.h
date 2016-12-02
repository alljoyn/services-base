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

#import <vector>
#import "alljoyn/notification/NotificationReceiver.h"
#import "alljoyn/notification/Notification.h"
#import "AJNSNotificationReceiver.h"

/**
 NotificationReceiverAdapter class
 */

class AJNSNotificationReceiverAdapter : public ajn::services::NotificationReceiver {
public:
	/**
     NotificationReceiverAdapter
     @param notificationReceiverHandler a AJNSNotificationReceiver handler
	 */
	AJNSNotificationReceiverAdapter(id <AJNSNotificationReceiver> notificationReceiverHandler);
    
	/**
     receive a notification
     @param notification the notification will be populated inside this param
	 */
	void Receive(ajn::services::Notification const& notification);
    
    /**
     Dismiss handler
     @param msgId message ID to dismiss
     @param appId app ID to use
     */
    void Dismiss(const int32_t msgId, const qcc::String appId);

    
	/**
     The handler of the receiver
	 */
	id <AJNSNotificationReceiver> ajnsNotificationReceiverHandler;
};