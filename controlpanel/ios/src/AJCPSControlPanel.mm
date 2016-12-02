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

#import "AJCPSControlPanel.h"
#import "alljoyn/services_common/AJSVCConvertUtil.h"

@interface AJCPSControlPanel ()

@property (nonatomic) ajn::services::ControlPanel *handle;

@end

@implementation AJCPSControlPanel


- (id)initWithHandle:(ajn::services::ControlPanel *)handle
{
    self = [super init];
    if (self) {
        self.handle = handle;
    }
    return self;
}

// original cpp constructor: ControlPanel(LanguageSet const& languageSet, qcc::String objectPath, ControlPanelDevice* device);

- (NSString *)getPanelName
{
    return [AJSVCConvertUtil convertQCCStringtoNSString:self.handle->getPanelName()];
}

- (QStatus)registerObjects:(AJNBusAttachment *)bus
{
    return self.handle->registerObjects((ajn::BusAttachment *)[bus handle]);
}

- (QStatus)unregisterObjects:(AJNBusAttachment *)bus
{
    return self.handle->unregisterObjects((ajn::BusAttachment *)[bus handle]);
}

- (AJCPSLanguageSet *)getLanguageSet
{
    return [[AJCPSLanguageSet alloc] initWithHandle:(ajn::services::LanguageSet *)&self.handle->getLanguageSet()];
}

- (AJCPSControlPanelDevice *)getDevice
{
    return [[AJCPSControlPanelDevice alloc] initWithHandle:self.handle->getDevice()];
}

- (NSString *)getObjectPath
{
    return [AJSVCConvertUtil convertQCCStringtoNSString:self.handle->getObjectPath()];
}

- (AJCPSContainer *)getRootWidget:(NSString *)Language
{
    return [[AJCPSContainer alloc] initWithHandle:self.handle->getRootWidget([AJSVCConvertUtil convertNSStringToQCCString:Language])];
}

@end