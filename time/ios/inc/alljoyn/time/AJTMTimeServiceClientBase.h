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
#import "alljoyn/time/TimeServiceClientBase.h"
#import "AJTMTimeServiceClient.h"

@interface AJTMTimeServiceClientBase : NSObject
/**
 * Constructor
 *
 * @param tsClient TimeServiceClient managing this object
 * @param objectPath Object path of the Time Service server side object
 */
-(id)initWithTimeServiceClient:(AJTMTimeServiceClient *)tsClient objectPath:(NSString *)objectPath;

/**
 * Object path of the  Time Service server side object
 *
 * @return object path
 */
-(NSString *) getObjectPath;

/**
 * Returns TimeServiceClient that is managing this object
 */
-(AJTMTimeServiceClient *) getTsClient;

@end