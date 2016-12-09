/******************************************************************************
 * Copyright (c) 2016 Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright 2016 Open Connectivity Foundation and Contributors to
 *    AllSeen Alliance. All rights reserved.
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

#include "ConfigServiceListenerImpl.h"
#include <IniParser.h>

using namespace ajn;
using namespace services;

ConfigServiceListenerImpl::ConfigServiceListenerImpl(PropertyStoreImpl& store, ajn::BusAttachment& bus,
                                                     CommonBusListener& busListener) :
    ConfigService::Listener(), m_PropertyStore(&store), m_Bus(&bus), m_BusListener(&busListener)
{
}

QStatus ConfigServiceListenerImpl::Restart()
{
    printf("Restart has been called !!!\n");
    return ER_OK;
}

QStatus ConfigServiceListenerImpl::FactoryReset()
{
    printf("FactoryReset has been called!!!\n");

    QStatus status = ER_OK;
    m_PropertyStore->FactoryReset();
    printf("Clearing Key Store\n");
    m_Bus->ClearKeyStore();

    AboutServiceApi* aboutService = AboutServiceApi::getInstance();
    if (aboutService) {
        status = aboutService->Announce();
        std::cout << "Announce for " << m_Bus->GetUniqueName().c_str() << " = " << QCC_StatusText(status) << std::endl;
    }

    return status;
}

QStatus ConfigServiceListenerImpl::SetPassphrase(const char* daemonRealm, size_t passcodeSize, const char* passcode, ajn::SessionId sessionId)
{
    qcc::String passCodeString(passcode, passcodeSize);
    printf("SetPassphrase has been called daemonRealm=%s passcode=%s passcodeLength=%lu\n", daemonRealm,
           passCodeString.c_str(), passcodeSize);
    PersistPassword(daemonRealm, passCodeString.c_str());

    printf("Clearing Key Store\n");
    m_Bus->ClearKeyStore();
    m_Bus->EnableConcurrentCallbacks();

    std::vector<SessionId> sessionIds = m_BusListener->getSessionIds();
    for (size_t i = 0; i < sessionIds.size(); i++) {
        if (sessionIds[i] == sessionId) {
            continue;
        }
        m_Bus->LeaveSession(sessionIds[i]);
        std::cout << "Leaving session with id: " << sessionIds[i];
    }

    return ER_OK;
}

ConfigServiceListenerImpl::~ConfigServiceListenerImpl()
{
}

void ConfigServiceListenerImpl::PersistPassword(const char* daemonRealm, const char* passcode)
{
    std::map<std::string, std::string> data;
    data["daemonrealm"] = daemonRealm;
    data["passcode"] = passcode;
    IniParser::UpdateFile(m_PropertyStore->GetConfigFileName().c_str(), data);
}