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

#import "AJCPSNotificationAction.h"
#import "alljoyn/about/AJNConvertUtil.h"
#import "AJCPSControlPanelDevice.h"

@interface AJCPSNotificationAction ()
@property (nonatomic) ajn::services::NotificationAction *handle;
@end


@implementation AJCPSNotificationAction

- (id)initWithHandle:(ajn::services::NotificationAction *)handle
{
	self = [super init];
	if (self) {
		self.handle = handle;
	}
	return self;
}

- (NSString *)getNotificationActionName
{
	return [AJNConvertUtil convertQCCStringtoNSString:self.handle->getNotificationActionName()];
}

/**
 * Register the BusObjects for this Widget
 * @param bus - bus used to register the busObjects
 * @return status - success/failure
 */
- (QStatus)registerObjects:(AJNBusAttachment *)bus
{
	return self.handle->registerObjects((ajn::BusAttachment *)[bus handle]);
}

/**
 * Unregister the BusObjects of the NotificationAction class
 * @param bus - bus used to unregister the objects
 * @return status - success/failure
 */
- (QStatus)unregisterObjects:(AJNBusAttachment *)bus
{
	return self.handle->unregisterObjects((ajn::BusAttachment *)[bus handle]);
}

/**
 * Get the LanguageSet of the NotificationAction
 * @return
 */
- (AJCPSLanguageSet *)getLanguageSet
{
	return [[AJCPSLanguageSet alloc] initWithHandle:(ajn::services::LanguageSet *)&self.handle->getLanguageSet()];
}

/**
 * Get the Device of the NotificationAction
 * @return controlPanelDevice
 */
- (AJCPSControlPanelDevice *)getDevice
{
	return [[AJCPSControlPanelDevice alloc]initWithHandle:self.handle->getDevice()];
}

/**
 * Get the objectPath
 * @return
 */
- (NSString *)getObjectPath
{
	return [AJNConvertUtil convertQCCStringtoNSString:self.handle->getObjectPath()];
}

/**
 * Get the RootWidget of the NotificationAction
 * @param Language - languageSet of RootWidget to retrieve
 * @return rootWidget
 */
- (AJCPSRootWidget *)getRootWidget:(NSString *)Language
{
	return [[AJCPSRootWidget alloc]initWithHandle:self.handle->getRootWidget([AJNConvertUtil convertNSStringToQCCString:Language])];
}

@end