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

/**
 * This class is experimental, and as such has not been tested.
 * Please help make it more robust by contributing fixes if you find issues
 **/

#import "AJTMErrorInfo.h"

@interface AJTMErrorInfo ()

@property (strong, nonatomic) NSString* errorName;
@property (strong, nonatomic) NSString* errorDescription;

@end

@implementation AJTMErrorInfo

-(id)init
{
    self = [super init];
    if (self)
    {
        self.errorName = @"";
        self.errorDescription = @"";
    }
    return self;
}

-(id)initWithErrorName:(NSString*) errorName errorDescription:(NSString*) errorDescription
{
    self = [super init];
    if (self)
    {
        self.errorName = errorName;
        self.errorDescription = errorDescription;
    }
    return self;
}

//-(id)initWithErrorInfo:( ajn::services::tsConsts::ErrorInfo*) errorInfo
//{
//    self = [super init];
//    if (self)
//    {
//        self.errorName = @((errorInfo->errorName).c_str());
//        self.errorDescription = @((errorInfo->errorDescription).c_str());
//    }
//    return self;
//}

-(NSString*)errorName
{
    return self.errorName;
}

@end