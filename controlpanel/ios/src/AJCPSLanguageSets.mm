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

#import "AJCPSLanguageSets.h"
#import "alljoyn/services_common/AJSVCConvertUtil.h"


@interface AJCPSLanguageSets ()
@property (nonatomic) ajn::services::LanguageSets *handle;
@end

@implementation AJCPSLanguageSets
- (id)initWithHandle:(ajn::services::LanguageSets *)handle
{
    self = [super init];
    if (self) {
        self.handle = handle;
    }
    return self;
}

- (AJCPSLanguageSet *)getLanguageSet:(NSString *)languageSetName
{
    return [[AJCPSLanguageSet alloc] initWithHandle:self.handle->LanguageSets::get([AJSVCConvertUtil convertNSStringToQCCString:languageSetName])];
}

@end