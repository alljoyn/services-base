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

#include <alljoyn/AuthListener.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/services_common/BsSecurity.h>

#include <qcc/String.h>
#include <qcc/CertificateECC.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

using namespace std;

namespace ajn {
namespace services {
//======================================================================

// REVISIT concurrency?

const char* BsSecurity::KEYX_ECDHE_NULL = "ALLJOYN_ECDHE_NULL";
const char* BsSecurity::KEYX_ECDHE_PSK = "ALLJOYN_ECDHE_PSK";
const char* BsSecurity::KEYX_ECDHE_SPEKE = "ALLJOYN_ECDHE_SPEKE";
const char* BsSecurity::KEYX_ECDHE_ECDSA = "ALLJOYN_ECDHE_ECDSA";
const char* BsSecurity::ECDHE_KEYX = "ALLJOYN_ECDHE_ECDSA ALLJOYN_ECDHE_PSK ALLJOYN_ECDHE_SPEKE ALLJOYN_ECDHE_NULL";

using qcc::CertificateX509;

typedef vector<CertificateX509> CertificateVec;
typedef vector<qcc::String> StringVec;

static StringVec splitPEM(const qcc::String& encoded);

static QStatus DecodeCertificatePEM(const qcc::String& encoded, CertificateVec& result);

static bool ValidateCertificateTypeInCertChain(CertificateVec& certChain);

static bool VerifyCertificateChain(
    const AuthListener::Credentials& creds,
    const CertificateVec& trustedRoots
    );

static bool startsWith(const string& s, const string& prefix);
static string trimLeft(const string& s);
static string trimRight(const string& s);

//======================================================================

class BsSecurity::Impl
{
  public:

    /*
     * This is the local implementation of the an AuthListener.  ECDHEKeyXListener is
     * designed to only handle ECDHE Key Exchange Authentication requests.
     *
     * If any other authMechanism is used, other than ECDHE Key Exchange, then authentication
     * will fail.
     */
    class MyAuthListener : public AuthListener
    {
      public:
        MyAuthListener(BsSecurity::Impl& impl)
          : m_impl(impl)
        {
        }

        virtual bool RequestCredentials(const char* authMechanism, const char* authPeer, uint16_t authCount, const char* userId, uint16_t credMask, Credentials& creds) override;
        virtual bool VerifyCredentials(const char* authMechanism, const char* authPeer, const Credentials& creds) override;
        virtual void AuthenticationComplete(const char* authMechanism, const char* authPeer, bool success) override;

        BsSecurity::Impl& m_impl;
    };


    Impl(BusAttachment& busAttachment)
      : m_bus(busAttachment),
        m_authListener(std::make_shared<MyAuthListener>(*this))
    {
    }

    ~Impl()
    {
    }

    QStatus Enable();
    QStatus Enable(const char* keyStoreFileName, bool isShared);
    QStatus AddTrustedRoot(const qcc::String& cert);
    void SetEcdsaPrivateKeyPEM(const qcc::String& key);
    void SetEcdsaCertChainX509PEM(const qcc::String& certChain);
    void SetExpiration(uint32_t expirationSeconds);
    void SetPassword(const qcc::String& pwd);
    void SetMechanism(const qcc::String& mech);
    void SetPermissionListener(std::shared_ptr<PermissionConfigurationListener> listener);

    enum class FilePart {None, Identity, Root, Password, Key};

    void LoadFiles(const qcc::String& pathPrefix);
    void LoadOneFile(const string& path);
    void FlushFilePart(FilePart part, string& chunk);

    BusAttachment& m_bus;
    std::shared_ptr<AuthListener> m_authListener;
    CertificateVec m_trustedRoots;
    qcc::String m_ecdsaPrivateKeyPEM;
    qcc::String m_ecdsaCertChainX509PEM;
    qcc::String m_pwd;
    qcc::String m_mech = ECDHE_KEYX;
    uint32_t m_expirationSeconds = 100;
    std::shared_ptr<PermissionConfigurationListener> m_permListener;
};



QStatus BsSecurity::Impl::Enable()
{
    return Enable(nullptr, false);
}



QStatus BsSecurity::Impl::Enable(const char* keyStoreFileName, bool isShared)
{
    return m_bus.EnablePeerSecurity(m_mech.c_str(), m_authListener.get(), keyStoreFileName, isShared, m_permListener.get());
}



QStatus 
BsSecurity::Impl::AddTrustedRoot(const qcc::String& cert)
{
    return DecodeCertificatePEM(cert, m_trustedRoots);
}



void BsSecurity::Impl::SetEcdsaPrivateKeyPEM(const qcc::String& key)
{
    m_ecdsaPrivateKeyPEM = key;
}



void BsSecurity::Impl::SetEcdsaCertChainX509PEM(const qcc::String& certChain)
{
    m_ecdsaCertChainX509PEM = certChain;
}



void BsSecurity::Impl::SetExpiration(uint32_t expirationSeconds)
{
    m_expirationSeconds = expirationSeconds;
}



void BsSecurity::Impl::SetPassword(const qcc::String& pwd)
{
    m_pwd = pwd;
}

void BsSecurity::Impl::SetMechanism(const qcc::String &mech)
{
    m_mech = mech;
}

void BsSecurity::Impl::SetPermissionListener(std::shared_ptr<PermissionConfigurationListener> listener)
{
    m_permListener = listener;
}



void BsSecurity::Impl::LoadFiles(const qcc::String& pathPrefix)
{
    string prefix = pathPrefix.c_str();

    LoadOneFile(prefix + ".pub");
    LoadOneFile(prefix + ".priv");
}



void BsSecurity::Impl::LoadOneFile(const string& path)
{
    cout << "BsSecurity::LoadOneFile " << path << endl;

    ifstream strm(path);
    size_t lnum = 0;
    string chunk;
    FilePart which = FilePart::None;

    while (strm) {
        string line;
        getline(strm, line);

        ++lnum;

        if (startsWith(line, "identity")) {
            FlushFilePart(which, chunk);
            which = FilePart::Identity;
            cout << "BsSecurity- Reading identity" << std::endl;
        } else if (startsWith(line, "root")) {
            FlushFilePart(which, chunk);
            which = FilePart::Root;
            cout << "BsSecurity- Reading root" << std::endl;
        } else if (startsWith(line, "password")) {
            FlushFilePart(which, chunk);
            which = FilePart::Password;
            cout << "BsSecurity- Reading password" << std::endl;
        } else if (startsWith(line, "key")) {
            FlushFilePart(which, chunk);
            which = FilePart::Key;
            cout << "BsSecurity- Reading key" << std::endl;
        } else if (startsWith(line, " ") || startsWith(line, "\t")) {
            // This will trim a trailing \n and \r
            line = trimLeft(trimRight(line));

            if (line.empty() || line[0] == '#') {
                continue;
            }

            chunk += line;
            chunk += "\n";
        } else if (!line.empty()) {
            cerr << path << ": line " << lnum << ": Unrecognised line\n";
        }
    }

    FlushFilePart(which, chunk);
}



void BsSecurity::Impl::FlushFilePart(FilePart part, string& chunk)
{
    if (chunk.empty())
    {
        return;
    }

    //cout << "flushing\n" << chunk;

    switch (part)
    {
    case FilePart::Identity:
        SetEcdsaCertChainX509PEM(qcc::String(chunk));
        break;

    case FilePart::Root:
        AddTrustedRoot(qcc::String(chunk));
        break;

    case FilePart::Password:
        // Trimming to remove the added \n
        SetPassword(qcc::String(trimRight(chunk)));
        break;

    case FilePart::Key:
        SetEcdsaPrivateKeyPEM(qcc::String(chunk));
        break;

    case FilePart::None:
        break;
    }

    chunk.clear();
}

//======================================================================

bool BsSecurity::Impl::MyAuthListener::RequestCredentials(
    const char* authMechanism,
    const char* authPeer,
    uint16_t authCount,
    const char* userId,
    uint16_t credMask,
    Credentials& creds
    )
{
    QCC_UNUSED(userId);

    cout << "BsSecurity::MAL::RequestCredentials for authenticating " << authPeer
         << " using mechanism " << authMechanism
         << " and password " << m_impl.m_pwd
         << " authCount " << authCount << std::endl;

    if (strcmp(authMechanism, KEYX_ECDHE_NULL) == 0) {
        creds.SetExpiration(m_impl.m_expirationSeconds);
        return true;
    } 
    
    if (strcmp(authMechanism, KEYX_ECDHE_PSK) == 0 || strcmp(authMechanism, KEYX_ECDHE_SPEKE) == 0) {

        if (authCount > 10) {
                /* If the peer is making a large number of attempts they may be trying to guess the password. */
                return false;
            }

        creds.SetPassword(m_impl.m_pwd);
        creds.SetExpiration(m_impl.m_expirationSeconds);
        return true;
    }
    
    if (strcmp(authMechanism, KEYX_ECDHE_ECDSA) == 0) {
        if (credMask & AuthListener::CRED_PRIVATE_KEY) {
            creds.SetPrivateKey(m_impl.m_ecdsaPrivateKeyPEM);
        }

        if (credMask & AuthListener::CRED_CERT_CHAIN) {
            creds.SetCertChain(m_impl.m_ecdsaCertChainX509PEM);
        }

        creds.SetExpiration(m_impl.m_expirationSeconds);
        return true;
    }

    return false;
}



bool BsSecurity::Impl::MyAuthListener::VerifyCredentials(const char* authMechanism, const char* authPeer, const Credentials& creds)
{
    QCC_UNUSED(authPeer);

    cout << "BsSecurity::MAL::VerifyCredentials " << authPeer
         << " using mechanism " << authMechanism
         << " and peer " << authPeer << std::endl;

    /* only the ECDHE_ECDSA calls for peer credential verification */
    if (strcmp(authMechanism, KEYX_ECDHE_ECDSA) == 0) {
        if (creds.IsSet(AuthListener::CRED_CERT_CHAIN)) {
            /*
             * AllJoyn sends back the certificate chain for the application to verify.
             * The application has to option to verify the certificate chain.
             * Return true if the cert chain is validated and trusted.
             */
            cout << "VerifyCredentials receives cert chain " << creds.GetCertChain().c_str() << std::endl;
            return VerifyCertificateChain(creds, m_impl.m_trustedRoots);
        }
    }
    return false;
}



void BsSecurity::Impl::MyAuthListener::AuthenticationComplete(const char* authMechanism, const char* authPeer, bool success)
{
    QCC_UNUSED(authMechanism);
    QCC_UNUSED(authPeer);
    QCC_UNUSED(success);
    cout << "BsSecurity::MAL::AuthenticationComplete using mechanism " << authMechanism << (success ? " successful" : " failed") << std::endl;
}



bool VerifyCertificateChain(
    const AuthListener::Credentials& creds,
    const CertificateVec& trustedRoots
    )
{
    /* This certificate chain verification engine does the following:
     *
     * 1. Verifies that all certificates in the chain have not expired.
     * 2. Verifies that all CAs in the chain have the CA flag set to TRUE,
     * 3. Verifies the cryptographic binding between each certificate.
     * 4. Verifies the certificates chain up to one of the trusted roots.
     * 5. Verifies the end-entity certificate is an identity certificate, and the chain is valid
     *    for this purpose.
     *
     * Other implementations may make app-dependent decisions, such as verifying the certificate's
     * subject name equals some known value.
     */
    QStatus status = ER_OK;

    CertificateVec certChain;
    status = DecodeCertificatePEM(creds.GetCertChain(), certChain);

    /* Here is where you could check additional properties of the certificate, depending on your application and scenario's
     * needs.
     *
     * If you make use of the CN or OU fields of the Distinguished Name, remember that these are UTF-8 strings.
     */
    bool trusted = false;

    if (ER_OK == status) {
        /* Most of the time in your code you'll be writing code to check identity certificates, and so we check for that
         * type of certificate in this sample. */
        if (certChain[0].GetType() == qcc::CertificateX509::IDENTITY_CERTIFICATE) {
            /* Ensure that the Extended Key Usages are valid for the whole chain. In AllJoyn, we insist the end-entity
             * certificate is not unrestricted (has at least one EKU). We then make sure every Certificate Authority to the
             * root has that EKU present or is unrestricted. We recommend all CA's, including roots, be issued with AllJoyn
             * EKUs to ensure they are not used for other purposes. */
            trusted = ValidateCertificateTypeInCertChain(certChain);
        }

        /* However, we also need to check for unrestricted certificates here.
         * The reason to allow unrestricted here is for backwards compatibility
         * with pre-1509 peers that use certificates without EKUs in Security 1.0
         * contexts. Certificates created by AllJoyn in 1509 and above will always
         * have an EKU indicating the type. */
        if (certChain[0].GetType() == qcc::CertificateX509::UNRESTRICTED_CERTIFICATE) {
            /* Loop through and determine if the entire cert chain has type UNRESTRICTED_CERTIFICATE. */
            trusted = true;
            for (size_t iCert = 1; iCert < certChain.size(); iCert++) {
                if (certChain[iCert].GetType() != qcc::CertificateX509::UNRESTRICTED_CERTIFICATE) {
                    trusted = false;
                    break;
                }
            }
        }
        /* trusted will remain false if certificate is not an identity certificate or the whole cert chain is unrestricted. */
    }

    if (trusted && (ER_OK == status)) {

        trusted = false;

        for (size_t iCert = 0; iCert < certChain.size(); iCert++) {
            /* Every certificate must be time-valid. */
            status = certChain[iCert].VerifyValidity();

            if (ER_OK != status) {
                cout << "VerifyCertificatechain FAILED; following certificate is not time valid:\n"
                     << certChain[iCert].ToString().c_str() << "\n";
                status = ER_OK; /* Reset to ER_OK to signal there was no internal failure. trusted is false. */
                break;
            }

            /* If the current certificate is issued by a trusted root, we're done. */
            for (auto& root : trustedRoots) {
                if (root.IsIssuerOf(certChain[iCert])) {
                    cout << "VerifyCertificateChain SUCCEEDED; trusted root certificate is:\n" 
                         << root.ToString().c_str() << "\n";
                    trusted = true;
                    /* This break gets us out of this iRoot for loop. */
                    break;
                }
            }

            if (trusted) {
                /* And if we've decided the chain is trusted, break out of the iCert for loop too. */
                break;
            }

            /* If not, and there's a next certificate in the chain, check the chaining between the i'th certificate
             * and the i+1'th.
             */
            if (iCert + 1 < certChain.size()) {
                /* First, the next certificate in the chain must be a CA certificate. */
                if (!certChain[iCert + 1].IsCA()) {
                    cout << "VerifyCertificateChain FAILED: following certificate is not a CA certificate:\n"
                         << certChain[iCert + 1].ToString().c_str() << "\n";
                    break;
                }
                /* Now check the chaining. IsIssuerOf checks both that the issuer DN of the i'th certificate equals
                 * the subject DN of the i+1'th certificate in the chain, and verifies the cryptographic signature
                 * was produced by the i+1'th certificate. */
                if (!certChain[iCert + 1].IsIssuerOf(certChain[iCert])) {
                    /* Note that PRIuSIZET is not a standards-defined constant, but is defined in AllJoyn to select the
                     * correct platform-dependent format specifier for type size_t.
                     */
                    cout << "VerifyCertificateChain FAILED: certificate at position " << iCert + 1
                         << " did not issue certificate at position " << iCert << "\n";
                    cout << "Certificate[" << iCert + 1 << "]:\n" << certChain[iCert + 1].ToString().c_str() << "\n";
                    cout << "Certificate[" << iCert << "]:\n" << certChain[iCert].ToString().c_str() << "\n";
                    break;
                }
            }
        }
    }

    /* At this point one of three things has happened:
     * status is not ER_OK: something failed before we could even check the chain, and so we're failing;
     * status is ER_OK and trusted is false: we successfully checked the chain but no trusted root was found
     * or a certificate was not time-valid;
     * or status is ER_OK and trusted is true: we successfully checked the chain and found a trusted root in the path.
     */

    /* Otherwise, return the trusted result. */
    if (!trusted) {
        printf("VerifyCertificateChain FAILED: did not see a trusted root in the chain.\n");
    }

    return status == ER_OK && trusted;
}



static bool ValidateCertificateTypeInCertChain(CertificateVec& certChain)
{
    if (certChain.empty()) {
        return false;
    }

    /* leaf cert must have a type */
    if (certChain[0].GetType() == qcc::CertificateX509::UNRESTRICTED_CERTIFICATE) {
        return false;
    }

    if (certChain.size() == 1) {
        return true;
    }

    /**
     * Any signing cert in the chain must have the same type as the end entity certificate
     * or be unrestricted in order to sign the next cert in the chain.
     */
    for (size_t cnt = 1; cnt < certChain.size(); cnt++) {
        if ((certChain[cnt].GetType() != qcc::CertificateX509::UNRESTRICTED_CERTIFICATE) &&
            (certChain[cnt].GetType() != certChain[0].GetType())) {
            return false;
        }
    }
    return true;
}



static QStatus DecodeCertificatePEM(const qcc::String& encoded, CertificateVec& result)
{
    // Append the decoded certificates to the result vector.
    auto parts = splitPEM(encoded);

    for (size_t i = 0; i < parts.size(); ++i) {
        CertificateX509 cert;

        auto status = cert.LoadPEM(parts[i]);

        if (status != ER_OK) {
            return status;
        }

        result.push_back(cert);
    }

    return ER_OK;
}



static StringVec splitPEM(const qcc::String& encoded)
{
    StringVec result;

    qcc::String beginToken = "-----BEGIN CERTIFICATE-----";
    qcc::String endToken = "-----END CERTIFICATE-----";
    qcc::String remainder = encoded;

    for (;;) {
        auto pos = remainder.find(beginToken);
        if (pos == qcc::String::npos) {
            break;
        }
        remainder = remainder.substr(pos + beginToken.size());
        pos = remainder.find(endToken);
        if (pos == qcc::String::npos) {
            break;
        }

        auto part = beginToken;
        part += remainder.substr(0, pos);
        part += endToken;
        result.push_back(part);
        remainder = remainder.substr(pos + endToken.size());
    }

    return result;
}



static bool startsWith(const string& s, const string& prefix)
{
    if (prefix.size() <= s.size()) {
        return s.compare(0, prefix.size(), prefix) == 0;
    }

    return false;
}



static string trimLeft(const string& s)
{
    for (size_t i = 0; i < s.size(); ++i) {
        if (!isspace(s[i])) {
            return s.substr(i);
        }
    }

    // no non-blank chars were found
    return "";
}



static string trimRight(const string& s)
{
    for (size_t i = s.size(); i > 0; --i) {
        if (!isspace(s[i - 1])) {
            if (i == s.size()) {
                // the common case
                return s;
            }

            return s.substr(0, i);
        }
    }

    // no non-blank chars were found
    return "";
}

//======================================================================


BsSecurity::BsSecurity(ajn::BusAttachment& busAttachment)
  : m_impl(new Impl(busAttachment))
{
}


BsSecurity::~BsSecurity()
{
    delete m_impl;
}


QStatus BsSecurity::Enable()
{
    return m_impl->Enable();
}


QStatus BsSecurity::Enable(const char* keyStoreFileName, bool isShared)
{
    return m_impl->Enable(keyStoreFileName, isShared);
}


QStatus BsSecurity::AddTrustedRoot(const qcc::String& cert)
{
    return m_impl->AddTrustedRoot(cert);
}


BsSecurity& BsSecurity::SetEcdsaPrivateKeyPEM(const qcc::String& key)
{
    m_impl->SetEcdsaPrivateKeyPEM(key);
    return *this;
}


BsSecurity& BsSecurity::SetEcdsaCertChainX509PEM(const qcc::String& certChain)
{
    m_impl->SetEcdsaCertChainX509PEM(certChain);
    return *this;
}


BsSecurity& BsSecurity::SetExpiration(uint32_t expirationSeconds)
{
    m_impl->SetExpiration(expirationSeconds);
    return *this;
}


BsSecurity& BsSecurity::SetPassword(const qcc::String& pwd)
{
    m_impl->SetPassword(pwd);
    return *this;
}

BsSecurity& BsSecurity::SetMechanism(const qcc::String &mech)
{
    m_impl->SetMechanism(mech);
    return *this;
}

BsSecurity& BsSecurity::SetPermissionListener(std::shared_ptr<ajn::PermissionConfigurationListener> listener)
{
    m_impl->SetPermissionListener(listener);
    return *this;
}


BsSecurity& BsSecurity::LoadFiles(const qcc::String& pathPrefix)
{
    m_impl->LoadFiles(pathPrefix);
    return *this;
}

//======================================================================
} //namespace services
} //namespace ajn