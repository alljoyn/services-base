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

#import "AJCPSConstraintRange.h"

@interface AJCPSConstraintRange ()
@property (nonatomic) ajn::services::ConstraintRange *handle;
@end

@implementation AJCPSConstraintRange

- (id)initWithHandle:(ajn::services::ConstraintRange *)handle
{
    self = [super init];
    if (self) {
        self.handle = handle;
    }
    return self;
}

- (uint16_t)getIncrementValue
{
    return self.handle->getIncrementValue().int16Value;
}

- (uint16_t)getMaxValue
{
    return self.handle->getMaxValue().int16Value;
}

- (uint16_t)getMinValue
{
    return self.handle->getMinValue().int16Value;
}

@end