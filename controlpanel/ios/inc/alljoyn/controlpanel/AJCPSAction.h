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
#import "alljoyn/controlpanel/Action.h"
#import "AJCPSWidget.h"

/**
 * AJCPSAction Class is used to display a Button.
 */
@interface AJCPSAction : AJCPSWidget

- (id)initWithHandle:(ajn::services::Action *)handle;

/**
 * Call to execute this Action remotely
 * @return ER_OK if successful.
 */
- (QStatus)executeAction;

@end