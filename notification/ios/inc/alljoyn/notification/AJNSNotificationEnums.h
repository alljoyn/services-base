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

#ifndef AJNSNOTIFICATIONENUMS_H_
#define AJNSNOTIFICATIONENUMS_H_

/**
 AJNSNotificationEnums class
 */
@interface AJNSNotificationEnums : NSObject

/**
 @enum AJNSNotificationMessageType enum
 @abstract AJNSNotificationMessageType DBus request status return values.
 */
    typedef NS_ENUM (NSInteger, AJNSNotificationMessageType) {
    /** EMERGENCY - Urgent Message */
    EMERGENCY = 0,
    /** WARNING  - Warning Message */
    WARNING = 1,
    /** INFO - Informational Message */
    INFO = 2,
    /** MESSAGE_TYPE_CNT  - Number of Message Types Defined */
    MESSAGE_TYPE_CNT = 3,
    UNSET = 4
};

/**
 Convet AJNSNotificationMessageType to an NSString format
 @param msgType DBus request status
 @return message type in an NSString format(Capital letters)
 */
+ (NSString *)AJNSMessageTypeToString:(AJNSNotificationMessageType)msgType;

@end
#endif