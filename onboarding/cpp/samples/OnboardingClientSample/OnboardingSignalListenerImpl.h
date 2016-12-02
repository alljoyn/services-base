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

#ifndef ONBOARDINGSIGNALLISTENERIMPL_H_
#define ONBOARDINGSIGNALLISTENERIMPL_H_

#include <alljoyn/onboarding/OnboardingClientListener.h>



class OnboardingSignalListenerImpl : public ajn::services::OnboardingClientListener {
  public:

    OnboardingSignalListenerImpl();

    ~OnboardingSignalListenerImpl();

    void ConnectionResultSignalReceived(short connectionResultCode, const qcc::String& connectionResultMessage);
};

#endif /* ONBOARDINGSIGNALLISTENERIMPL_H_ */