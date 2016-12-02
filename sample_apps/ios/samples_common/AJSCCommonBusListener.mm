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

#import "AJSCCommonBusListener.h"
#import "AJNSessionOptions.h"

@interface AJSCCommonBusListener ()
@property AJNSessionPort servicePort;
@end

@implementation AJSCCommonBusListener


- (id)initWithServicePort:(AJNSessionPort)servicePort
{
    self = [super init];
    if (self) {
        self.servicePort = servicePort;
    }
    return self;
}

- (void)setSessionPort:(AJNSessionPort)sessionPort
{
    self.servicePort = sessionPort;
}

- (AJNSessionPort)sessionPort
{
    return self.servicePort;
}

/* protocol method */
- (BOOL)shouldAcceptSessionJoinerNamed:(NSString *)joiner onSessionPort:(AJNSessionPort)sessionPort withSessionOptions:(AJNSessionOptions *)options
{
    if (sessionPort != self.servicePort) {
        NSLog(@"Rejecting join attempt on unexpected session port %hu.", sessionPort);
        return false;
    } else {
        NSLog(@"Accepting join session request from %@ (proximity=%c, traffic=%u, transports=%hu).\n", joiner, options.proximity, options.trafficType, options.transports);
        return true;
    }
}

@end