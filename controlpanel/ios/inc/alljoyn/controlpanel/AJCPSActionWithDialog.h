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
#import "alljoyn/controlpanel/ActionWithDialog.h"
#import "AJCPSWidget.h"
#import "AJCPSDialog.h"

/**
 * AJCPSActionWithDialog is used to display a Button.
 * Upon pressing the button a Dialog is displayed on the Controller side.
 */
@interface AJCPSActionWithDialog : AJCPSWidget

- (id)initWithHandle:(ajn::services::ActionWithDialog *)handle;


/**
 * Get the ChildDialog of the Action
 * @return dialog
 */
- (AJCPSDialog *)getChildDialog;

/**
 * Unregister the BusObjects for this and children Widgets
 * @param bus - the bus used to unregister the busObjects
 * @return status - success/failure
 */
- (QStatus)unregisterObjects:(AJNBusAttachment *)bus;

@end