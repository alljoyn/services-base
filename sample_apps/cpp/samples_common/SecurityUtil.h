//
// Created by bkersten on 21/03/2016.
//

#ifndef ALLJOYN_SECURITYUTIL_H
#define ALLJOYN_SECURITYUTIL_H

#include <vector>

#include <qcc/CryptoECC.h>
#include <qcc/CertificateECC.h>
#include <qcc/KeyInfoECC.h>
#include <qcc/String.h>
#include <qcc/GUID.h>
#include <alljoyn/AllJoynStd.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/Status.h>
#include <alljoyn/Init.h>

class SecurityUtil {
public:

    static QStatus Claim(ajn::BusAttachment &bus,
                         const qcc::String &peerBusName,
                         const ajn::SessionId sessionId,
                         const qcc::KeyInfoNISTP256 &rootPublicKeyInfo,
                         const qcc::GUID128 adminGuid,
                         const qcc::KeyInfoNISTP256 &adminPubKeyInfo,
                         const std::vector<qcc::CertificateX509> &certChain,
                         const ajn::Manifest manifest);

    static QStatus Reset(ajn::BusAttachment &bus,
                         const qcc::String &peerBusName);

    static QStatus InstallMembership(ajn::BusAttachment &bus,
                                     const qcc::String &peerBusName,
                                     const qcc::MembershipCertificate &cert);

    static void GenerateIdentityCertificate(ajn::BusAttachment &bus,
                                            const qcc::ECCPublicKey &publicKey,
                                            const ajn::Manifest manifest,
                                            const qcc::GUID128 identityGuid,
                                            const qcc::String &identityName,
                                            qcc::IdentityCertificate &cert);

    static void GenerateMembershipCertificate(const qcc::ECCPublicKey &publicKey,
                                              const qcc::GUID128 &group,
                                              qcc::MembershipCertificate &cert);

    static void GenerateCA(ajn::BusAttachment &bus,
                           const qcc::String &caCN,
                           qcc::CertificateX509 &cert);

    static void IssueCertificate(ajn::BusAttachment &issuerBus,
                                 const qcc::String &issuerCN,
                                 qcc::CertificateX509 &cert);

    static void GenerateManifest(ajn::Manifest manifest);

    static QStatus SignManifest(ajn::BusAttachment &issuerBus,
                                const qcc::CertificateX509& subjectCertificate,
                                ajn::Manifest &manifest);

private:
    static void GenerateCertificate(const qcc::String &subjectCN,
                                    const qcc::ECCPublicKey *subjectPublicKey,
                                    uint64_t validDays,
                                    qcc::CertificateX509 &cert);

};


#endif //ALLJOYN_SECURITYUTIL_H
