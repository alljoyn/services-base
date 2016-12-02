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

#import "ConfigServiceListener.h"
#import "alljoyn/about/AJNAboutServiceApi.h"
#import "alljoyn/config/AJCFGConfigLogger.h"

@interface ConfigServiceListener ()

@property AJCFGPropertyStoreImpl *propertyStore;

@property AJNBusAttachment *bus;

@end

@implementation ConfigServiceListener

- (id)initWithPropertyStore:(AJCFGPropertyStoreImpl *)propertyStore andBus:(AJNBusAttachment *)bus
{
	self = [super init];
	if (self) {
		self.propertyStore = propertyStore;
		self.bus = bus;
	}
	return self;
}

- (QStatus)restart
{
	[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:@"Restart has been called !"];
	return ER_OK;
}

- (QStatus)factoryReset
{
	[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:@"Factory Reset called"];
    
	[self.propertyStore factoryReset];
    
	[self.bus clearKeyStore];
    
	return [[AJNAboutServiceApi sharedInstance] announce];
}

- (QStatus)setPassphrase:(NSString *)daemonRealm withPasscode:(NSString *)passcode
{
	QStatus status = [self.propertyStore setPasscode:passcode];
    
	[self.bus clearKeyStore];
    
	return status;
}

@end