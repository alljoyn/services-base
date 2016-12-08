/******************************************************************************
 *  * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
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

#import "AJCPSControlPanelController.h"
#import "alljoyn/services_common/AJSVCConvertUtil.h"

@interface AJCPSControlPanelController ()
@property (nonatomic) ajn::services::ControlPanelController *handle;
@end

// This exposes the private AboutObjectDescription C++ handle in AJNAboutObjectDescription, limiting
// its scope to this implementation file. This follows the pattern in the core ios bindings.
@interface AJNAboutObjectDescription(Private)
@property (nonatomic, readonly) ajn::AboutObjectDescription *aboutObjectDescription;
@end

@implementation AJCPSControlPanelController

- (id)init
{
    self = [super init];
    if (self) {
        self.handle = new ajn::services::ControlPanelController();
    }
    return self;
}

- (id)initWithHandle:(ajn::services::ControlPanelController *)handle
{
    self = [super init];
    if (self) {
        self.handle = handle;
    }
    return self;
}

- (AJCPSControlPanelDevice *)createControllableDevice:(NSString *)deviceBusName ObjectDescs:(NSDictionary *)objectDescs
{
    ajn::services::AnnounceHandler::ObjectDescriptions *cpp_ObjectDescs = new ajn::services::AnnounceHandler::ObjectDescriptions;

    for (NSString *key in objectDescs) {
        NSArray *strings = [objectDescs objectForKey:key];

        std::vector <qcc::String> cpp_strings;

        for (NSString *string in strings) {
            cpp_strings.push_back([AJSVCConvertUtil convertNSStringToQCCString:string]);
        }

        cpp_ObjectDescs->insert(std::make_pair([AJSVCConvertUtil convertNSStringToQCCString:key], cpp_strings));
    }

    return [[AJCPSControlPanelDevice alloc] initWithHandle:self.handle->createControllableDevice([AJSVCConvertUtil convertNSStringToQCCString:deviceBusName], *cpp_ObjectDescs)];
}

- (AJCPSControlPanelDevice *)createControllableDevice:(NSString *)deviceBusName withObjectDesc:(AJNAboutObjectDescription *)objectDesc
{
    return [[AJCPSControlPanelDevice alloc] initWithHandle:self.handle->createControllableDevice([AJSVCConvertUtil convertNSStringToQCCString:deviceBusName], *(objectDesc.aboutObjectDescription))];
}

- (AJCPSControlPanelDevice *)getControllableDevice:(NSString *)deviceBusName
{
    return [[AJCPSControlPanelDevice alloc] initWithHandle:self.handle->getControllableDevice([AJSVCConvertUtil convertNSStringToQCCString:deviceBusName])];
}

- (QStatus)deleteControllableDevice:(NSString *)deviceBusName
{
    return self.handle->deleteControllableDevice([AJSVCConvertUtil convertNSStringToQCCString:deviceBusName]);
}

- (QStatus)deleteAllControllableDevices
{
    return self.handle->deleteAllControllableDevices();
}

- (NSDictionary *)getControllableDevices
{
    std::map <qcc::String, ajn::services::ControlPanelDevice *> cpp_ControllableDevices = self.handle->getControllableDevices();

    NSMutableDictionary *controllableDevices = [[NSMutableDictionary alloc] init];

    for (std::map <qcc::String, ajn::services::ControlPanelDevice *>::const_iterator itr = cpp_ControllableDevices.begin(); itr != cpp_ControllableDevices.end(); itr++) {
        NSString *key = [AJSVCConvertUtil convertQCCStringtoNSString:itr->first];
        AJCPSControlPanelDevice *value = [[AJCPSControlPanelDevice alloc] initWithHandle:itr->second];

        [controllableDevices setObject:value forKey:key];
    }

    return controllableDevices;
}

@end