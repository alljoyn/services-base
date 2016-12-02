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
#import "alljoyn/notification/NotificationText.h"

/**
 AJNSNotificationText class.
 */
@interface AJNSNotificationText : NSObject

///---------------------
/// @name Properties
///---------------------

/**
 Initialize notification text with language and text
 @param language The language of this notification text
 @param text The text of the notification text
 @return pointer to the noficiation text object
 */
- (AJNSNotificationText *)initWithLang:(NSString *)language andText:(NSString *)text;

/**
 * Set Language for Notification
 * @param language set the language of the notification text
 */
- (void)setLanguage:(NSString *)language;

/**
 * Get Language for Notification
 * @return language of this notification text
 */
- (NSString *)getLanguage;

/**
 * Set Text for Notification
 * @param text set the text for this notification text
 */
- (void)setText:(NSString *)text;

/**
 * Get Text for Notification
 * @return text
 */
- (NSString *)getText;

@property (nonatomic, readonly) ajn::services::NotificationText *handle;

@end