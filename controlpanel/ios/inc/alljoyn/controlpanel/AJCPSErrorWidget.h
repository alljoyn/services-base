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
 * AJCPSLabel class used to display a Label.
 */
@interface AJCPSErrorWidget : AJCPSWidget
/**
 * Constructor for AJCPSLabel class
 * @param handle handle to the c++ instance
 */
- (id)initWithHandle:(ajn ::services ::ErrorWidget *)handle;

- (AJCPSWidget*)getOriginalWidget;

@end