//
// Created by bkersten on 21/03/2016.
//

#include "SecurityUtil.h"

#include <assert.h>
#include <qcc/Crypto.h>
#include <alljoyn/PermissionPolicy.h>
#include <alljoyn/SecurityApplicationProxy.h>

#include "../../../../../core/alljoyn/alljoyn_core/src/CredentialAccessor.h"

using namespace qcc;
using namespace ajn;

const size_t SERIAL_NUMBER_LENGTH = 20; // RFC 5280 4.1.2.2

static QStatus GenerateManifestDigest(BusAttachment &bus,
                                      const PermissionPolicy::Rule *rules,
                                      size_t count,
                                      uint8_t *digest,
                                      size_t digestSize) {
    if (digestSize != Crypto_SHA256::DIGEST_SIZE) {
        return ER_INVALID_DATA;
    }
    Message tmpMsg(bus);
    DefaultPolicyMarshaller marshaller(tmpMsg);
    return marshaller.Digest(rules, count, digest, Crypto_SHA256::DIGEST_SIZE);
}

QStatus SecurityUtil::Claim(BusAttachment &bus,
                            const String &peerBusName,
                            const SessionId sessionId,
                            const KeyInfoNISTP256 &caPublicKeyInfo,
                            const GUID128 adminGuid,
                            const KeyInfoNISTP256 &adminPubKeyInfo,
                            const std::vector <CertificateX509> &certChain,
                            const Manifest manifest) {
    QStatus status = ER_FAIL;

//    SessionId sessionId = 0;
//    if (peerBusName != bus.GetUniqueName()) {
//        SessionOpts opts;
//        status = bus.JoinSession(peerBusName.c_str(), ALLJOYN_SESSIONPORT_PERMISSION_MGMT, NULL, sessionId,
//                                 opts);
//        if (ER_OK != status) {
//            std::cout << "WARNING - Failed to join session with '" << peerBusName << "' error: " <<
//            QCC_StatusText(status) << std::endl;
//            return status;
//        }
//    }

    SecurityApplicationProxy peerProxy(bus, peerBusName.c_str(), sessionId);
    status = peerProxy.Claim(caPublicKeyInfo, adminGuid, adminPubKeyInfo,
                             &certChain[0], certChain.size(), &manifest, 1);
    if (ER_OK != status) {
        std::cout << "WARNING - Failed to call Claim on peer '" << peerBusName << "' error: " <<
        QCC_StatusText(status) << std::endl;
    }

//    if (sessionId != 0) {
//        QStatus sessionStatus = bus.LeaveSession(sessionId);
//        if (ER_OK != sessionStatus) {
//            std::cout << "WARNING - Failed to leave session with '" << peerBusName << "' error: " <<
//            QCC_StatusText(sessionStatus) << std::endl;
//        }
//    }

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

QStatus SecurityUtil::SignManifest(BusAttachment &issuerBus, const CertificateX509 &subjectCertificate, Manifest &manifest) {
    CredentialAccessor ca(issuerBus);
    ECCPrivateKey privateKey;
    QStatus status = ca.GetDSAPrivateKey(privateKey);
    if (ER_OK != status) {
        return status;
    }

    return manifest->ComputeThumbprintAndSign(subjectCertificate, &privateKey);
}

void SecurityUtil::GenerateIdentityCertificate(BusAttachment &bus,
                                               const ECCPublicKey &publicKey,
                                               const Manifest manifest,
                                               const GUID128 identityGuid,
                                               const String &identityName,
                                               IdentityCertificate &cert) {
    cert.SetAlias(identityGuid.ToString());
    cert.SetSubjectOU((const uint8_t *) identityName.c_str(), identityName.length());

    uint8_t digest[Crypto_SHA256::DIGEST_SIZE];

    const std::vector <PermissionPolicy::Rule> rules = manifest->GetRules();

    GenerateManifestDigest(bus, &rules[0], rules.size(), digest, Crypto_SHA256::DIGEST_SIZE);
    cert.SetDigest(digest, Crypto_SHA256::DIGEST_SIZE);

    String aki;
    CertificateX509::GenerateAuthorityKeyId(&publicKey, aki);

    GenerateCertificate(aki, &publicKey, 365 * 5, cert);
}

void SecurityUtil::GenerateMembershipCertificate(const ECCPublicKey &publicKey,
                                                 const GUID128 &group,
                                                 MembershipCertificate &cert) {
    cert.SetGuild(group);

    String aki;
    CertificateX509::GenerateAuthorityKeyId(&publicKey, aki);

    GenerateCertificate(aki, &publicKey, 365 * 5, cert);
}

void SecurityUtil::IssueCertificate(BusAttachment &issuerBus,
                                    const String &issuerCN,
                                    CertificateX509 &cert) {
    std::cout << "IssueCertificate. issuer CN: " << issuerCN << std::endl;

    cert.SetIssuerCN((const uint8_t *) issuerCN.c_str(), issuerCN.length());

    issuerBus.GetPermissionConfigurator().SignCertificate(cert);

    std::cout << "IssueCertificate " << cert.ToString() << std::endl;
}

void SecurityUtil::GenerateCA(BusAttachment &bus, const String &caCN, CertificateX509 &cert) {
    std::cout << "GenerateCA. CN: " << caCN << std::endl;
    cert.SetCA(true);

    KeyInfoNISTP256 publicKeyInfo;
    bus.GetPermissionConfigurator().GetSigningPublicKey(publicKeyInfo);

    GenerateCertificate(caCN, publicKeyInfo.GetPublicKey(), 365 * 10, cert);

    IssueCertificate(bus, caCN, cert);
}

void SecurityUtil::GenerateCertificate(const String &subjectCN,
                                       const ECCPublicKey *subjectPublicKey,
                                       uint64_t validDays,
                                       CertificateX509 &cert) {
    std::cout << "GenerateCertificate. subject CN: " << subjectCN << std::endl;
    assert(subjectPublicKey != NULL && "subjectPublicKey should not be NULL");

    cert.SetSubjectCN((uint8_t *) subjectCN.c_str(), subjectCN.length());
    cert.SetSubjectPublicKey(subjectPublicKey);

    CertificateX509::ValidPeriod period;
    uint64_t currentTime = GetEpochTimestamp() / 1000;
    period.validFrom = currentTime;
    period.validTo = period.validFrom + 3600 * 24 * validDays;
    period.validFrom = period.validFrom - 3600;
    cert.SetValidity(&period);

    uint8_t serialNumber[SERIAL_NUMBER_LENGTH];
    QStatus status = Crypto_GetRandomBytes(serialNumber, sizeof(serialNumber));
    if (ER_OK != status) {
        std::cout << "WARNING - Could not generate random serial number; status is " << QCC_StatusText(status) <<
        std::endl;
    }
    serialNumber[0] &= 0x7F;
    cert.SetSerial(serialNumber, sizeof(serialNumber));
}
