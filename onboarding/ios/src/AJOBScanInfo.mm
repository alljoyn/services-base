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

#import "AJOBScanInfo.h"

@interface AJOBScanInfo()

@property(strong, nonatomic) NSString *ssid;
@property(nonatomic) AJOBAuthType authType;

@end

@implementation AJOBScanInfo

- (id) initWithSSID:(NSString *)ssid AuthType:(AJOBAuthType)authType
{
    self = [super init];

    if (self) {
        self.ssid = ssid;
        self.authType = authType;
    }

    return self;
}

@end