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