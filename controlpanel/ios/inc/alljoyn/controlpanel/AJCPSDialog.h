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
#import "alljoyn/controlpanel/Dialog.h"
#import "AJCPSRootWidget.h"

/**
 * AJCPSDialog is used to display a Dialog.
 */
@interface AJCPSDialog : AJCPSRootWidget

- (id)initWithHandle:(ajn::services::Dialog *)handle;


/**
 * Get the Number of Actions in the Dialog
 * @return Number of Actions
 */
- (uint16_t)getNumActions;

/**
 * Get the Message of the Dialog
 * @return the message
 */
- (NSString *)getMessage;

/**
 * Get the LabelAction1 of the Dialog
 * @return the message
 */
- (NSString *)getLabelAction1;

/**
 * Get the LabelAction2 of the Dialog
 * @return the message
 */
- (NSString *)getLabelAction2;

/**
 * Get the LabelAction3 of the Dialog
 * @return the message
 */
- (NSString *)getLabelAction3;

/**
 * Call to execute this Dialog's Action 1 remotely
 * @return status - success/failure
 */
- (QStatus)executeAction1;

/**
 * Call to execute this Dialog's Action 2 remotely
 * @return status - success/failure
 */
- (QStatus)executeAction2;

/**
 * Call to execute this Dialog's Action 3 remotely
 * @return status - success/failure
 */
- (QStatus)executeAction3;



@end