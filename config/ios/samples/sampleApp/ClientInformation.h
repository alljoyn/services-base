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
#import "alljoyn/about/AJNAnnouncement.h"

// Property store strings
static NSString *const DEFAULT_LANGUAGE_STR = @"DefaultLanguage";
static NSString *const DEVICE_NAME_STR = @"DeviceName";
static NSString *const DEVICE_ID_STR = @"DeviceId";
static NSString *const PASS_CODE_STR = @"passcode";

/**
 ClientInformation is a helper class to contain the announcement data and the user preferred language.
 */
@interface ClientInformation : NSObject

@property (strong, nonatomic) AJNAnnouncement *announcement;

@property (strong, nonatomic) NSString *currLang;

@end