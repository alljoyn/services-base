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

#import <Foundation/Foundation.h>
#import "AJNSessionOptions.h"
#import "AJNMessageArgument.h"

/**
 * AJSCAboutAnnouncement is a storage class that holds about announcement information received
 * from the server.
 */
@interface AJSCAboutAnnouncement : NSObject

@property (strong, nonatomic) NSString *busName;
@property uint16_t version;
@property AJNSessionPort port;
@property (strong, nonatomic) AJNMessageArgument *objectDescriptionArg;
@property (strong, nonatomic) AJNMessageArgument *aboutDataArg;

/**
 * Create AJSCAboutAnnouncement object using the parameters passed in from AJNAboutListener's
 * didReceiveAnnounce method.
 */
- (id)initWithBusName:(NSString *)busName version:(uint16_t)version sessionPort:(AJNSessionPort)sessionPort objectDescriptionArg:(AJNMessageArgument *)objectDescriptionArg aboutDataArg:(AJNMessageArgument *)aboutDataArg;

@end