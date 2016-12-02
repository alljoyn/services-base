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

#import "AJTMTimeServiceClientBase.h"

@interface AJTMTimeServiceClientBase()

@property NSString *objectPath;
@property AJTMTimeServiceClient *serviceClient;

@end

@implementation AJTMTimeServiceClientBase

-(id)initWithTimeServiceClient:(AJTMTimeServiceClient *)tsClient objectPath:(NSString *)objectPath
{
    self = [super init];
    if (self) {
        _objectPath = objectPath;
        _serviceClient = tsClient;
    }
    return self;
}

-(NSString *) getObjectPath {return self.objectPath;}

-(AJTMTimeServiceClient *) getTsClient {return self.serviceClient;}

@end