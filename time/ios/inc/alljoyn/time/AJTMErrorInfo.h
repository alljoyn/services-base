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

#import <Foundation/Foundation.h>
//#import "alljoyn/time/TimeServiceConstants.h"

@interface AJTMErrorInfo : NSObject

-(id)initWithErrorName:(NSString*) errorName errorDescription:(NSString*) errorDescription;
//-(id)initWithErrorInfo:(ajn::services::tsConsts::ErrorInfo*) errorInfo;
-(NSString*)errorName;
-(NSString*)errorDescription;

@end