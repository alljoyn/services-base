/******************************************************************************
 *  * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#import "AJOBSOnboardingClient.h"
#import "alljoyn/onboarding/OnboardingClient.h"
#import "alljoyn/onboarding/Onboarding.h"
#import "alljoyn/services_common/AJSVCConvertUtil.h"
#import "AJOBOnboardingClientListenerAdapter.h"
#import "AJOBScanInfo.h"

@interface AJOBSOnboardingClient ()
@property ajn::services::OnboardingClient *handle;
@property (nonatomic) AJOBOnboardingClientListenerAdapter *onboardingListenerAdapter;
@end

@implementation AJOBSOnboardingClient

- (void)dealloc
{
    delete self.handle;
    delete self.onboardingListenerAdapter;
}

- (id)initWithBus:(AJNBusAttachment *)bus listener:(id <AJOBOnboardingClientListener> )listener
{
    self = [super init];
    if (self) {
        self.onboardingListenerAdapter = new AJOBOnboardingClientListenerAdapter(listener);
        self.handle = new ajn::services::OnboardingClient((ajn::BusAttachment &)(*bus.handle), *self.onboardingListenerAdapter);
    }
    return self;
}

- (QStatus)getScanInfo:(NSString *)busName age:(unsigned short &)age scanInfo:(NSMutableArray *)scanInfos sessionId:(AJNSessionId)sessionId
{
    if(busName == nil) {
        return ER_BAD_ARG_1;
    }
    
    if(scanInfos == nil) {
        return ER_BAD_ARG_3;
    }
    
    ajn::services::OnboardingClient::ScanInfos internalScanInfos;
    QStatus status = self.handle->GetScanInfo([AJSVCConvertUtil convertNSStringToConstChar:busName], age, internalScanInfos, sessionId);

    if(status == ER_OK) {
        ajn::services::OnboardingClient::ScanInfos::iterator it;
        for(it = internalScanInfos.begin(); it != internalScanInfos.end(); ++it) {
            NSString *ssid = [AJSVCConvertUtil convertConstCharToNSString:it->SSID.c_str()];
            AJOBScanInfo *scanInfo = [[AJOBScanInfo alloc] initWithSSID:ssid AuthType:it->authType];
            [scanInfos addObject:scanInfo];
        }
    }

    return status;
}

- (QStatus)configureWiFi:(NSString *)busName obInfo:(AJOBInfo &)ajOBInfo resultStatus:(short &)resultStatus sessionId:(AJNSessionId)sessionId
{
    // prepare OBInfo
    ajn::services::OBInfo obInfo;
    obInfo = [AJOBSOnboarding toOBInfo:ajOBInfo];
    QStatus status = self.handle->ConfigureWiFi([AJSVCConvertUtil convertNSStringToConstChar:busName], obInfo, resultStatus, sessionId);
    return status;
}

- (QStatus)configureWiFi:(NSString *)busName obInfo:(AJOBInfo &)obInfo resultStatus:(short &)resultStatus
{
    NSLog(@"No sessionId has been specified - the service will find an existing route to end point rather than access directly.");
    return [self configureWiFi:busName obInfo:obInfo resultStatus:resultStatus sessionId:0];
}

- (QStatus)connectTo:(NSString *)busName sessionId:(AJNSessionId)sessionId
{
    return self.handle->ConnectTo([AJSVCConvertUtil convertNSStringToConstChar:busName], sessionId);
}

- (QStatus)connectTo:(NSString *)busName
{
    NSLog(@"No sessionId has been specified - the service will find an existing route to end point rather than access directly.");
    return [self connectTo:busName sessionId:0];
}


- (QStatus)offboardFrom:(NSString *)busName sessionId:(AJNSessionId)sessionId
{
    return self.handle->OffboardFrom([AJSVCConvertUtil convertNSStringToConstChar:busName], sessionId);
}


- (QStatus)offboardFrom:(NSString *)busName
{
    NSLog(@"No sessionId has been specified - the service will find an existing route to end point rather than access directly.");
    return [self offboardFrom:busName sessionId:0];
}

- (QStatus)version:(NSString *)busName version:(int &)version sessionId:(AJNSessionId)sessionId
{
    return self.handle->GetVersion([AJSVCConvertUtil convertNSStringToConstChar:busName], version, sessionId);
}

- (QStatus)version:(NSString *)busName version:(int &)version
{
    NSLog(@"No sessionId has been specified - the service will find an existing route to end point rather than access directly.");
    return [self version:busName version:version sessionId:0];
}


- (QStatus)state:(NSString *)busName state:(short &)state sessionId:(AJNSessionId)sessionId
{
    return self.handle->GetState([AJSVCConvertUtil convertNSStringToConstChar:busName], state, sessionId);
}

- (QStatus)state:(NSString *)busName state:(short &)state
{
    NSLog(@"No sessionId has been specified - the service will find an existing route to end point rather than access directly.");
    return [self state:busName state:state sessionId:0];
}


- (QStatus)lastError:(NSString *)busName lastError:(AJOBLastError &)lastError sessionId:(AJNSessionId)sessionId
{
    ajn::services::OBLastError obLastError;
    QStatus status = self.handle->GetLastError([AJSVCConvertUtil convertNSStringToConstChar:busName], obLastError, sessionId);
    lastError = [AJOBSOnboarding toAJOBLastError:obLastError];
    return status;
}


- (QStatus)lastError:(NSString *)busName lastError:(AJOBLastError &)lastError
{
    NSLog(@"No sessionId has been specified - the service will find an existing route to end point rather than access directly.");
    return [self lastError:busName lastError:lastError sessionId:0];
}
@end