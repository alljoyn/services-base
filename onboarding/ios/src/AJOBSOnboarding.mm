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

#import "AJOBSOnboarding.h"
#import "alljoyn/services_common/AJSVCConvertUtil.h"

static NSString *const PASSCODE_FORMAT = @"%02X";

@implementation AJOBSOnboarding

+ (bool)isValidWEPKey:(NSString *)key
{
    size_t size = [key length];
    return size == 5 || // 40-bit ascii
           size == 10 ||  // 40 bit hex
           size == 13 ||  // 104 bit ascii
           size == 26 ||  // 104 bit hex
           size == 16 ||  // 152 bit ascii
           size == 32 ||  // 152-bit hex
           size == 29 ||  // 256-bit ascii
           size == 64;    // 256-bit hex
}

+ (bool)isValidWPAKey:(NSString *)key
{
    return [key length] >= MIN_PSK_SIZE && [key length] <= MAX_PSK_SIZE;
}

+ (NSString *)passcodeToHex:(NSString *)passcode
{
    const char *pass = [passcode UTF8String];
    NSMutableString *passcodeHex = [NSMutableString string];
    while (*pass) {
        [passcodeHex appendFormat:PASSCODE_FORMAT, *pass++ & BIT_MASK];
    }
    return passcodeHex;
}

+ (ajn::services::OBInfo)toOBInfo:(AJOBInfo)ajOBInfo
{
    ajn::services::OBInfo obInfo;
    obInfo.SSID = [AJSVCConvertUtil convertNSStringToQCCString:ajOBInfo.SSID];
    obInfo.state = (ajn::services::OBState)ajOBInfo.state;
    obInfo.authType = (ajn::services::OBAuthType)ajOBInfo.authType;
    obInfo.passcode = [AJSVCConvertUtil convertNSStringToQCCString:ajOBInfo.passcode];
    return obInfo;
}

+ (AJOBInfo)toAJOBInfo:(ajn::services::OBInfo)obInfo
{
    AJOBInfo ajOBInfo;
    ajOBInfo.SSID = [AJSVCConvertUtil convertQCCStringtoNSString:obInfo.SSID];
    ajOBInfo.state = obInfo.state;
    ajOBInfo.authType = obInfo.authType;
    ajOBInfo.passcode = [AJSVCConvertUtil convertQCCStringtoNSString:obInfo.passcode];
    return ajOBInfo;
}

+ (AJOBLastError)toAJOBLastError:(ajn::services::OBLastError)obLastError
{
    AJOBLastError ajOBLastError;
    ajOBLastError.validationState = obLastError.validationState;
    ajOBLastError.message = [AJSVCConvertUtil convertQCCStringtoNSString:obLastError.message];
    return ajOBLastError;
}

@end