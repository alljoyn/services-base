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
#import "alljoyn/controlpanel/NotificationAction.h"
#import "AJCPSLanguageSet.h"
#import "AJCPSRootWidget.h"

/**
 * AJCPSNotificationAction
 */
@interface AJCPSNotificationAction : NSObject

- (id)initWithHandle:(ajn ::services ::NotificationAction *)handle;


/**
 * Get the name of the NotificationAction - the name of the rootWidget
 * @return name of the NotificationAction
 */
- (NSString *)getNotificationActionName;

/**
 * Register the BusObjects for this Widget
 * @param bus - bus used to register the busObjects
 * @return status - success/failure
 */
- (QStatus)registerObjects:(AJNBusAttachment *)bus;

/**
 * Unregister the BusObjects of the NotificationAction class
 * @param bus - bus used to unregister the objects
 * @return status - success/failure
 */
- (QStatus)unregisterObjects:(AJNBusAttachment *)bus;

/**
 * Get the LanguageSet of the NotificationAction
 * @return
 */
- (AJCPSLanguageSet *)getLanguageSet;

/**
 * Get the Device of the NotificationAction
 * @return controlPanelDevice
 */
- (AJCPSControlPanelDevice *)getDevice;

/**
 * Get the objectPath
 * @return
 */
- (NSString *)getObjectPath;

/**
 * Get the RootWidget of the NotificationAction
 * @param Language - languageSet of RootWidget to retrieve
 * @return rootWidget
 */
- (AJCPSRootWidget *)getRootWidget:(NSString *)Language;


@property (nonatomic, readonly)ajn::services::NotificationAction * handle;

@end