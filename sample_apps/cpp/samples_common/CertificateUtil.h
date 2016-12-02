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

#ifndef ALLJOYN_CERTIFICATEUTIL_H
#define ALLJOYN_CERTIFICATEUTIL_H

#include <qcc/CryptoECC.h>
#include <qcc/CertificateECC.h>
#include <qcc/String.h>
#include <qcc/GUID.h>
#include <alljoyn/Status.h>
#include <alljoyn/PermissionPolicy.h>

class CertificateUtil {
  public:
    static QStatus GenerateIdentityCertificate(const qcc::ECCPublicKey& publicKey,
                                               const qcc::GUID128 identityGuid,
                                               const qcc::String& identityName,
                                               qcc::IdentityCertificate& cert);

    static QStatus GenerateMembershipCertificate(const qcc::ECCPublicKey& publicKey,
                                                 const qcc::GUID128& group,
                                                 qcc::MembershipCertificate& cert);

    static QStatus GenerateCA(const qcc::Crypto_ECC& caKeyPair,
                              const qcc::String& caCN,
                              qcc::CertificateX509& cert);

    static QStatus IssueCertificate(const qcc::Crypto_ECC& issuerKeyPair,
                                    const qcc::String& issuerCN,
                                    qcc::CertificateX509& cert);

    static QStatus SignManifest(const qcc::ECCPrivateKey* issuerKey,
                                const qcc::CertificateX509& subjectCertificate,
                                ajn::Manifest& manifest);

    static bool SaveCertificate(const qcc::String& filename, qcc::CertificateX509& cert);
    static bool LoadCertificate(const qcc::String& filename, qcc::CertificateX509& cert);

    static bool SavePrivateKey(const qcc::String& filename, const qcc::ECCPrivateKey* privateKey);
    static bool LoadPrivateKey(const qcc::String& filename, qcc::ECCPrivateKey* privateKey);

  private:
    static QStatus GenerateCertificate(const qcc::String& subjectCN,
                                       const qcc::ECCPublicKey* subjectPublicKey,
                                       uint64_t validDays,
                                       qcc::CertificateX509& cert);
   
};


#endif //ALLJOYN_CERTIFICATEUTIL_H