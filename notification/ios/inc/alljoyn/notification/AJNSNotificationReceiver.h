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

#ifndef AJNSNOTIFICATIONRECEIVER_H_
#define AJNSNOTIFICATIONRECEIVER_H_
#import <Foundation/Foundation.h>
#import "alljoyn/notification/NotificationReceiver.h"
#import "AJNSNotification.h"

/**
 AJNSNotificationReceiver class
 */
@protocol AJNSNotificationReceiver <NSObject>

/**
 * Pure abstract function that receives a notification
 * Consumer Application must override this method
 * @param notification the notification that is received
 */

- (void)receive:(AJNSNotification *)notification;

/**
 *   Dismiss handler
 * @param msgId message ID to dismiss
 * @param appId app ID to use
 */
- (void)dismissMsgId:(const int32_t)msgId appId:(NSString *)appId;

@end
#endif