/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
 *    Project (AJOSP) Contributors and others.
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
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
******************************************************************************/

#include "SecurityUtil.h"

#include <iostream>
#include <assert.h>
#include <alljoyn/AllJoynStd.h>
#include <alljoyn/PermissionPolicy.h>
#include "SecurityApplicationProxy_FIXED_CORE_HEADER.h"

using namespace qcc;
using namespace ajn;

QStatus SecurityUtil::Claim(BusAttachment &bus,
                            const String &peerBusName,
                            const SessionId sessionId,
                            const KeyInfoNISTP256 &caPublicKeyInfo,
                            const GUID128 &adminGuid,
                            const KeyInfoNISTP256 &adminPubKeyInfo,
                            const std::vector <CertificateX509> &certChain,
                            const std::vector<Manifest> &manifests) {
    QStatus status = ER_FAIL;

    SecurityApplicationProxy peerProxy(bus, peerBusName.c_str(), sessionId);
    status = peerProxy.Claim(caPublicKeyInfo, adminGuid, adminPubKeyInfo,
                             certChain.data(), certChain.size(), manifests.data(), manifests.size());
    if (ER_OK != status) {
        std::cout << "WARNING - Failed to call Claim on peer '" << peerBusName << "' error: " <<
        QCC_StatusText(status) << std::endl;
    }

    return status;
}

QStatus SecurityUtil::Reset(BusAttachment &bus, const String &peerBusName) {
    QStatus status = ER_FAIL;

    SessionId sessionId = 0;
    if (peerBusName != bus.GetUniqueName()) {
        SessionOpts opts;
        status = bus.JoinSession(peerBusName.c_str(), ALLJOYN_SESSIONPORT_PERMISSION_MGMT, NULL, sessionId,
                                 opts);
        if (ER_OK != status) {
            std::cout << "WARNING - Failed to join session with '" << peerBusName << "' error: " <<
            QCC_StatusText(status) << std::endl;
            return status;
        }
    }

    SecurityApplicationProxy peerProxy(bus, peerBusName.c_str(), sessionId);
    status = peerProxy.Reset();
    if (ER_OK != status) {
        std::cout << "WARNING - Failed to call Reset on peer '" << peerBusName << "' error: " <<
        QCC_StatusText(status) << std::endl;
    }

    if (sessionId != 0) {
        QStatus sessionStatus = bus.LeaveSession(sessionId);
        if (ER_OK != sessionStatus) {
            std::cout << "WARNING - Failed to leave session with '" << peerBusName << "' error: " <<
            QCC_StatusText(sessionStatus) << std::endl;
        }
    }

    return status;
}

QStatus SecurityUtil::InstallMembership(BusAttachment &bus,
                                        const String &peerBusName,
                                        const MembershipCertificate &cert) {
    QStatus status = ER_FAIL;

    SessionId sessionId = 0;
    if (peerBusName != bus.GetUniqueName()) {
        SessionOpts opts;
        status = bus.JoinSession(peerBusName.c_str(), ALLJOYN_SESSIONPORT_PERMISSION_MGMT, NULL, sessionId,
                                 opts);
        if (ER_OK != status) {
            std::cout << "WARNING - Failed to join session with '" << peerBusName << "' error: " <<
            QCC_StatusText(status) << std::endl;
            return status;
        }
    }

    SecurityApplicationProxy peerProxy(bus, peerBusName.c_str(), sessionId);
    status = peerProxy.InstallMembership(&cert, 1);
    if (ER_OK != status) {
        std::cout << "WARNING - Failed to call InstallMembership on peer '" << peerBusName << "' error: " <<
        QCC_StatusText(status) << std::endl;
    }

    if (sessionId != 0) {
        QStatus sessionStatus = bus.LeaveSession(sessionId);
        if (ER_OK != sessionStatus) {
            std::cout << "WARNING - Failed to leave session with '" << peerBusName << "' error: " <<
            QCC_StatusText(sessionStatus) << std::endl;
        }
    }

    return status;
}

void SecurityUtil::GenerateManifest(Manifest manifest) {

    const size_t rule_count = 1;
    PermissionPolicy::Rule rules[rule_count];
    rules[0].SetObjPath("*");
    rules[0].SetInterfaceName("*");

    {
        PermissionPolicy::Rule::Member members[1];
        members[0].SetMemberName("*");
        members[0].SetMemberType(PermissionPolicy::Rule::Member::MemberType::NOT_SPECIFIED);
        members[0].SetActionMask(
                PermissionPolicy::Rule::Member::ACTION_PROVIDE |
                PermissionPolicy::Rule::Member::ACTION_MODIFY |
                PermissionPolicy::Rule::Member::ACTION_OBSERVE
        );

        rules[0].SetMembers(1, members);
    }

    manifest->SetRules(rules, rule_count);
}