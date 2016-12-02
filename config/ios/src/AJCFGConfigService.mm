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

#import "AJCFGConfigService.h"
#import "AJNSessionOptions.h"
#import "alljoyn/config/ConfigService.h"
#import "alljoyn/services_common/AJSVCGenericLoggerAdapter.h"
#import "alljoyn/services_common/AJSVCGenericLoggerDefaultImpl.h"

@interface AJCFGConfigService ()

@property ajn::services::ConfigService *handle;
@property id <AJSVCGenericLogger> currentLogger;
@property AJSVCGenericLoggerAdapter *AJSVCGenericLoggerAdapter;

@end

@implementation AJCFGConfigService

- (id)initWithBus:(AJNBusAttachment *)bus propertyStore:(AJCFGPropertyStoreImpl *)propertyStore listener:(AJCFGConfigServiceListenerImpl *)listener
{
	self = [super init];
	if (self) {
		self.handle = new ajn::services::ConfigService((ajn::BusAttachment&)(*bus.handle), *propertyStore.getHandle, *[listener handle]);
		// Set a default logger
		self.currentLogger = [[AJSVCGenericLoggerDefaultImpl alloc] init];
		// Create and set a generic logger adapter adapter
		self.AJSVCGenericLoggerAdapter = new AJSVCGenericLoggerAdapter(self.currentLogger);
	}
	return self;
}

- (QStatus)registerService
{
	return self.handle->Register();
}

- (void)unregisterService
{
	//self.handle->Unregister();
}

#pragma mark - Logger methods
- (void)setLogger:(id <AJSVCGenericLogger> )logger
{
	if (logger) {
		// Save the current logger
		self.currentLogger = logger;
		// Call setLoger with the adapter and save the prev Logger
	}
	else {
		[self.currentLogger warnTag:([NSString stringWithFormat:@"%@", [[self class] description]]) text:@"Failed set a logger"];
	}
}

- (id <AJSVCGenericLogger> )logger
{
	return self.currentLogger;
}

- (void)setLogLevel:(QLogLevel)newLogLevel
{
	[self.currentLogger setLogLevel:newLogLevel];
}

- (QLogLevel)logLevel
{
	return [self.currentLogger logLevel];
}

@end