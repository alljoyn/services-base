/******************************************************************************
 * Copyright (c) 2016 Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright 2016 Open Connectivity Foundation and Contributors to
 *    AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#import <Foundation/Foundation.h>
#import "AJNBusAttachment.h"
#import "alljoyn/controlpanel/HttpControl.h"
#import "AJCPSControlPanelEnums.h"

@class AJCPSControlPanelDevice;

/**
 * AJCPSHttpControl allows definition of a url.
 */
@interface AJCPSHttpControl : NSObject

- (id)initWithHandle:(ajn::services::HttpControl *)handle;

/**
 * Get the Interface Version of the HttpControl
 * @return interface Version
 */
- (uint16_t)getInterfaceVersion;

/**
 * Register the HttpControl BusObject
 * @param bus - bus used for registering the object
 * @return status - success/failure
 */
- (QStatus)registerObjects:(AJNBusAttachment *)bus;

/**
 * Refresh the HttpControl
 * @param bus - bus used for refreshing the object
 * @return status - success/failure
 */
- (QStatus)refreshObjects:(AJNBusAttachment *)bus;

/**
 * Unregister the HttpControl BusObject
 * @param bus - bus used to unregister the object
 * @return status - success/failure
 */
- (QStatus)unregisterObjects:(AJNBusAttachment *)bus;

/**
 * Get the Device that contains this HttpControl
 * @return ControlPanelDevice
 */
- (AJCPSControlPanelDevice *)getDevice;

/**
 * Get the Url for the HttpControl
 * @return url
 */
- (NSString *)getUrl;

/**
 * Get the ControlPanelMode of this HttpControl
 * @return ControlPanelMode
 */
- (AJCPSControlPanelMode)getControlPanelMode;

@end