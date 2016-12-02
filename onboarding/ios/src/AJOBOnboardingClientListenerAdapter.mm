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

#import "AJOBOnboardingClientListenerAdapter.h"
#import "alljoyn/services_common/AJSVCConvertUtil.h"


AJOBOnboardingClientListenerAdapter::AJOBOnboardingClientListenerAdapter(id <AJOBOnboardingClientListener> onboardingClientListener)
{
    ajOnboardingClientListener = onboardingClientListener;
}

AJOBOnboardingClientListenerAdapter::~AJOBOnboardingClientListenerAdapter()
{
}

void AJOBOnboardingClientListenerAdapter::ConnectionResultSignalReceived(short connectionResultCode, const qcc::String &connectionResultMessage)
{
    NSString *connectionResultMessageString;

    connectionResultMessageString = [AJSVCConvertUtil convertQCCStringtoNSString:connectionResultMessage];
    [ajOnboardingClientListener connectionResultSignalReceived:connectionResultCode connectionResultMessage:connectionResultMessageString];

}