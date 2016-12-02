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

#import "AJCPSControlPanelService.h"
#import "AJSVCGenericLoggerAdapter.h"
#import "AJCPSControlPanelListenerAdapter.h"

@interface AJCPSControlPanelService ()
@property (nonatomic) ajn::services::ControlPanelService *handle;
@property (nonatomic) AJCPSControlPanelListenerAdapter *listenerAdapter;

@property id <AJSVCGenericLogger> currentLogger;
@property AJSVCGenericLoggerAdapter *genericLoggerAdapter;
@end


@implementation AJCPSControlPanelService

+ (AJCPSControlPanelService *)getInstance
{
	static AJCPSControlPanelService *aboutLogger;
	static dispatch_once_t donce;
	dispatch_once(&donce, ^{
	    aboutLogger = [[self alloc] init];
	});
	return aboutLogger;
}

- (id)init
{
	self = [super init];
	self.handle = ajn::services::ControlPanelService::getInstance();
	return self;
}

- (QStatus)initController:(AJNBusAttachment *)bus controlPanelController:(AJCPSControlPanelController *)controlPanelController
     controlPanelListener:(id <AJCPSControlPanelListener> )controlPanelListener
{
	self.listenerAdapter = new AJCPSControlPanelListenerAdapter(controlPanelListener);
    
	return self.handle->initController((ajn::BusAttachment *)[bus handle], [controlPanelController handle], self.listenerAdapter);
}

/**
 * Remove locally stored controller. Allows a new call to initController to be made
 * @return status
 */
- (QStatus)shutdownController
{
	return self.handle->shutdown();
}

- (AJNBusAttachment *)getBusAttachment
{
	return [[AJNBusAttachment alloc]initWithHandle:self.handle->getBusAttachment()];
}

/**
 * Get the ControlPanelListener
 * @return ControlPanelListener
 */
- (id <AJCPSControlPanelListener> )getControlPanelListener
{
	AJCPSControlPanelListenerAdapter *adapter = (AJCPSControlPanelListenerAdapter *)self.handle->getControlPanelListener();
    
	return adapter->getListener();
}

/**
 * Get the Version of the ControlPanelService
 * @return the ControlPanelService version
 */
- (uint16_t)getVersion
{
	return self.handle->getVersion();
}

#pragma mark - Logger methods
- (void)setLogger:(id <AJSVCGenericLogger> )logger
{
	if (logger) {
		// save current logger
		self.currentLogger = logger;
		// call setLoger with the adapter and save the prev Logger
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