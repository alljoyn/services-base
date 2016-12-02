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

#import "AJSCAboutAnnouncement.h"

@implementation AJSCAboutAnnouncement

- (id)initWithBusName:(NSString *)busName
    version:(uint16_t)version
    sessionPort:(AJNSessionPort)port
    objectDescriptionArg:(AJNMessageArgument *)objectDescriptionArg
    aboutDataArg:(AJNMessageArgument *)aboutDataArg
{
    self = [super init];
    if (self) {
        self.busName = busName;
        self.version = version;
        self.port = port;
        self.objectDescriptionArg = objectDescriptionArg;
        self.aboutDataArg = aboutDataArg;
    }
    return self;
}

@end