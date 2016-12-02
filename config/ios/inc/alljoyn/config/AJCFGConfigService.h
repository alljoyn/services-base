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
#import "AJNBusAttachment.h"
#import "AJNBusObject.h"
#import "AJCFGConfigServiceListenerImpl.h"
#import "AJCFGPropertyStoreImpl.h"
#import "alljoyn/services_common/AJSVCGenericLogger.h"

/**
 AJCFGConfigService is an AllJoyn BusObject that implements the org.alljoyn.Config standard interface.
 Applications that provide AllJoyn IoE services with config capability.
 */
@interface AJCFGConfigService : AJNBusObject

/**
 Designated initializer.
 Create a AJCFGConfigService Object.
 @param bus A reference to the AJNBusAttachment.
 @param propertyStore A reference to a property store.
 @param listener A reference to a Config service listener.
 @return AJCFGConfigService if successful.
 */
- (id)initWithBus:(AJNBusAttachment *)bus propertyStore:(AJCFGPropertyStoreImpl *)propertyStore listener:(AJCFGConfigServiceListenerImpl *)listener;

/**
 Register the ConfigService on the alljoyn bus.
 @return ER_OK if successful.
 */
- (QStatus)registerService;

/**
 * Unregister the AJCFGConfigService on the alljoyn bus.
 */
- (void)unregisterService;

#pragma mark - Logger methods
/**
 Receive AJSVCGenericLogger to use for logging.
 @param logger Implementation of AJSVCGenericLogger.
 @return previous logger.
 */
- (void)setLogger:(id <AJSVCGenericLogger> )logger;

/**
 Get the currently-configured logger implementation.
 @return logger Implementation of AJSVCGenericLogger.
 */
- (id <AJSVCGenericLogger> )logger;

/**
 Set log level filter for subsequent logging messages.
 @param newLogLevel New log level enum value.
 @return logLevel New log level enum value that was in effect prior to this change.
 */
- (void)setLogLevel:(QLogLevel)newLogLevel;

/**
 Get log level filter value currently in effect.
 @return logLevel Log level enum value currently in effect.
 */
- (QLogLevel)logLevel;

@end