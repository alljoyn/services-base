/******************************************************************************
 * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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

#include "OnboardingAuthListener.h"
#include <iostream>



OnboardingAuthListener::OnboardingAuthListener()
{
}



OnboardingAuthListener::~OnboardingAuthListener()
{
}



QStatus OnboardingAuthListener::SetPSK(const qcc::String& psk)
{
    m_psk = psk;
    return ER_OK;
}



QStatus OnboardingAuthListener::SetPassword(const qcc::String& password)
{
    m_password = password;
    return ER_OK;
}


bool OnboardingAuthListener::RequestCredentials(const char* authMechanism, const char* peerName, uint16_t authCount, const char* userName, uint16_t credMask, Credentials& credentials)
{
    QCC_UNUSED(peerName);
    QCC_UNUSED(authCount);
    QCC_UNUSED(userName);
    QCC_UNUSED(credMask);

    //std::cout << "OnboardingAuthListener::RequestCredentials for " << authMechanism << "\n";

    if (strcmp(authMechanism, "ALLJOYN_ECDHE_NULL") == 0) {
        return true;
    } else if (strcmp(authMechanism, "ALLJOYN_ECDHE_PSK") == 0) {     /* Marked deprecated in 16.04 */
        if (m_psk.empty()) {
            std::cerr << "OnboardingAuthListener::RequestCredentials called for ECDHE_PSK, but no PSK value is set, authentication will fail.\n";
            return false;
        }
        credentials.SetPassword(m_psk);    /* The credentials class has only one way to store pre-shared credentials. */
        return true;
    } else if (strcmp(authMechanism, "ALLJOYN_ECDHE_SPEKE") == 0) {
        if (m_password.empty()) {
            std::cerr << "OnboardingAuthListener::RequestCredentials called for ECDHE_SPEKE, but no password value is set, authentication will fail.\n";
            return false;
        }
        credentials.SetPassword(m_password);
        return true;
    } else if (strcmp(authMechanism, "ALLJOYN_ECDHE_ECDSA") == 0) {
        return true;
    } else if (strcmp(authMechanism, "ALLJOYN_SRP_KEYX") == 0) {
        if (m_password.empty()) {
            std::cerr << "OnboardingAuthListener::RequestCredentials called for SRP_KEYX, but no password value is set, authentication will fail.\n";
            return false;
        }
        credentials.SetPassword(m_password);
        return true;
    }
    return false;
}



void OnboardingAuthListener::AuthenticationComplete(const char* authMechanism, const char* peerName, bool success)
{
    QCC_UNUSED(authMechanism);
    QCC_UNUSED(peerName);
    QCC_UNUSED(success);
}