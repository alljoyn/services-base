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

#import "AJNSNotificationReceiverAdapter.h"
#import "AJNSNotificationReceiver.h"
#import "alljoyn/services_common/AJSVCConvertUtil.h"

AJNSNotificationReceiverAdapter::AJNSNotificationReceiverAdapter(id <AJNSNotificationReceiver> notificationReceiverHandler)
{
    ajnsNotificationReceiverHandler = notificationReceiverHandler;
}

void AJNSNotificationReceiverAdapter::Receive(ajn::services::Notification const &notification)
{
    AJNSNotification *t_ajnsNotification = [[AJNSNotification alloc] initWithHandle:(new ajn::services::Notification(notification))];

    [t_ajnsNotification createAJNSNotificationTextArray];
    [ajnsNotificationReceiverHandler receive:t_ajnsNotification];
}

void AJNSNotificationReceiverAdapter::Dismiss(const int32_t msgId, const qcc::String appId)
{
    NSLog(@"Got Dissmiss of msgId %d and appId %@", msgId, [AJSVCConvertUtil convertQCCStringtoNSString:appId]);
    [ajnsNotificationReceiverHandler dismissMsgId:msgId appId:[AJSVCConvertUtil convertQCCStringtoNSString:appId]];
}