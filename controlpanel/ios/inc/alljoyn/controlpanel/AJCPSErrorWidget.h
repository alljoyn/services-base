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
#import "alljoyn/controlpanel/ErrorWidget.h"
#import "AJCPSWidget.h"

/**
 * AJCPSErrorWidget class used to display an error widget
 */
@interface AJCPSErrorWidget : AJCPSWidget
/**
 * Constructor for AJCPSErrorWidget class
 * @param handle handle to the c++ instance
 */
- (id)initWithHandle:(ajn::services::ErrorWidget *)handle;

- (AJCPSWidget *)getOriginalWidget;

@end