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
#import "AJCPSRootWidget.h"
#import "alljoyn/controlpanel/Container.h"

/**
 * AJCPSContainer class is used to represent a container widget,
 * container widgets container children widgets and group them together.
 */
@interface AJCPSContainer : AJCPSRootWidget

- (id)initWithHandle:(ajn::services::Container *)handle;

/**
 * Register the BusObjects for this Widget
 * @param bus A reference to the AJNBusAttachment.
 * @return status - success/failure
 */
- (QStatus)registerObjects:(AJNBusAttachment *)bus;

/**
 * Unregister the BusObjects for this widget
 * @param bus A reference to the AJNBusAttachment.
 * @return status - success/failure
 */
- (QStatus)unregisterObjects:(AJNBusAttachment *)bus;


/**
 * Get the ChildWidget Vector
 * @return children widgets
 */
//const std : : vector <Widget *>& getChildWidgets() const;
- (NSArray *)getChildWidgets;

/**
 * Get IsDismissable
 * @return isDismissable
 */
- (bool)getIsDismissable;

@end