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
#import "alljoyn/notification/NotificationService.h"
#import "AJNSNotificationReceiverAdapter.h"
#import "AJNSNotificationReceiver.h"
#import "AJNSNotificationSender.h"
#import "AJNBusAttachment.h"
#import "AJNAboutData.h"
#import "alljoyn/services_common/AJSVCGenericLoggerAdapter.h"

/**
 AJNSNotificationService class
 */
@interface AJNSNotificationService : NSObject

///---------------------
/// @name Properties
///---------------------

/**
 * Get Instance of AJNSNotificationServiceImpl
 * @return instance
 */
+ (id)sharedInstance;

/**
 *  @deprecated startSendWithBus:andPropertyStore: was deprecated in September 2016 for 16.10
 *  Initialize Producer side via Transport. Create and
 *  return NotificationSender.
 *  @param bus ajn bus
 *  @param store property store
 *  @return NotificationSender instance
 */
- (AJNSNotificationSender *)startSendWithBus:(AJNBusAttachment *)bus andPropertyStore:(AJNAboutPropertyStoreImpl *)store __deprecated;

/**
 *  Initialize Producer side via Transport. Create and
 *  return NotificationSender.
 *  @param bus ajn bus
 *  @param ajn about data object
 *  @return NotificationSender instance
 */
- (AJNSNotificationSender *)startSendWithBus:(AJNBusAttachment *)bus andAboutData:(AJNAboutData *)aboutData;

/**
 * Initialize Consumer side via Transport.
 * Set NotificationReceiver to given receiver
 * @param bus ajn bus
 * @param ajnsNotificationReceiver notification receiver
 * @return status
 */
- (QStatus)startReceive:(AJNBusAttachment *)bus withReceiver:(id <AJNSNotificationReceiver> )ajnsNotificationReceiver;

/**
 * Stops sender but leaves bus and other objects alive
 */
- (void)shutdownSender;

/**
 * Stops receiving but leaves bus and other objects alive
 */
- (void)shutdownReceiver;

/**
 * Cleanup and get ready for shutdown
 */
- (void)shutdown;

/**
 * Get the currently-configured logger implementation
 * @return logger Implementation of GenericLogger
 */
- (id <AJSVCGenericLogger> )logger;

/**
 * Set log level filter for subsequent logging messages
 * @param newLogLevel enum value
 * @return logLevel enum value that was in effect prior to this change
 */
- (void)setLogLevel:(QLogLevel)newLogLevel;

/**
 * Get log level filter value currently in effect
 * @return logLevel enum value currently in effect
 */
- (QLogLevel)logLevel;

/**
 * Virtual method to get the busAttachment used in the service.
 */
- (AJNBusAttachment *)busAttachment;

/**
 * Get the Version of the NotificationService
 * @return the NotificationService version
 */
- (uint16_t)version;

@end