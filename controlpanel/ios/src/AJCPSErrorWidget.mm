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

#import "AJCPSErrorWidget.h"
#import "AJCPSControlPanelDevice.h"

@interface AJCPSErrorWidget ()

@end


@implementation AJCPSErrorWidget

- (id)initWithHandle:(ajn::services::ErrorWidget *)handle
{
    self = [super initWithHandle:handle];
    if (self) {
    }
    return self;
}


- (AJCPSWidget *)getOriginalWidget
{
    return [[AJCPSWidget alloc] initWithHandle:((ajn::services::ErrorWidget *)self.handle)->getOriginalWidget()];
}

@end