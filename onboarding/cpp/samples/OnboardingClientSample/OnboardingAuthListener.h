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

#ifndef ONBOARDING_AUTH_LISTENER_H_
#define ONBOARDING_AUTH_LISTENER_H_

#include <alljoyn/AuthListener.h>
#include <qcc/String.h>

class OnboardingAuthListener : public ajn::AuthListener {

  public:
    OnboardingAuthListener();

    ~OnboardingAuthListener();

    virtual QStatus SetPSK(const qcc::String& psk);

    virtual QStatus SetPassword(const qcc::String& password);

    virtual bool RequestCredentials(const char* authMechanism, const char* peerName, uint16_t authCount, const char* userName, uint16_t credMask, Credentials& credentials) override;

    virtual void AuthenticationComplete(const char* authMechanism, const char* peerName, bool success) override;

  private:
    OnboardingAuthListener& operator=(const OnboardingAuthListener& other) = delete;
    OnboardingAuthListener(const OnboardingAuthListener& other) = delete;

    qcc::String m_psk;
    qcc::String m_password;
};


#endif /* ONBOARDING_AUTH_LISTENER_H_ */