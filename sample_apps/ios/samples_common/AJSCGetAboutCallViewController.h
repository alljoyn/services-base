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

#import <UIKit/UIKit.h>

#import "AJNMessageArgument.h"
#import "AJNAboutProxy.h"

#import <SystemConfiguration/CaptiveNetwork.h>
#import <CoreFoundation/CFDictionary.h>

#import "AJNVersion.h"
#import "AJNProxyBusObject.h"

#import "AJSCAboutAnnouncement.h"
#import "AJSCAboutDataConverter.h"
#import "AJSCClientInformation.h"

#import "AJNBusAttachment.h"

@interface AJSCGetAboutCallViewController : UIViewController

@property (weak, nonatomic) AJSCClientInformation *clientInformation;
@property (weak, nonatomic) AJNBusAttachment *clientBusAttachment;

@end