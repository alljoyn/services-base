/******************************************************************************
 *  *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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

#ifndef ALLJOYN_SECURITYUTIL_H
#define ALLJOYN_SECURITYUTIL_H

#include <vector>

#include <qcc/CertificateECC.h>
#include <qcc/KeyInfoECC.h>
#include <qcc/String.h>
#include <qcc/GUID.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/Status.h>

class SecurityUtil {
  public:

    static QStatus Claim(ajn::BusAttachment& bus,
                         const qcc::String& peerBusName,
                         const ajn::SessionId sessionId,
                         const qcc::KeyInfoNISTP256& rootPublicKeyInfo,
                         const qcc::GUID128& adminGuid,
                         const qcc::KeyInfoNISTP256& adminPubKeyInfo,
                         const std::vector<qcc::CertificateX509>& certChain,
                         const std::vector<ajn::Manifest>& manifests);

    static QStatus Reset(ajn::BusAttachment& bus,
                         const qcc::String& peerBusName);

    static QStatus InstallMembership(ajn::BusAttachment& bus,
                                     const qcc::String& peerBusName,
                                     const qcc::MembershipCertificate& cert);

    static void GenerateManifest(ajn::Manifest manifest);

};


#endif //ALLJOYN_SECURITYUTIL_H