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


#ifndef BS_SECURITY_H_
#define BS_SECURITY_H_

#include <alljoyn/Status.h>
#include <qcc/String.h>

namespace ajn {

class BusAttachment;
class PermissionConfigurationListener;

namespace services {
//======================================================================


class BsSecurity
{
  public:

    static const char* KEYX_ECDHE_NULL;
    static const char* KEYX_ECDHE_PSK;
    static const char* KEYX_ECDHE_SPEKE;
    static const char* KEYX_ECDHE_ECDSA;
    static const char* ECDHE_KEYX;

    /**
     * Constructor
     * @param[in] busAttachment
     */
    BsSecurity(ajn::BusAttachment& busAttachment);

    ~BsSecurity();

    /**
     * After setting the security parameters, call this. It will call
     * BusAttachment::EnablePeerSecurity for you.
     */
    QStatus Enable();

    /**
     * After setting the security parameters, call this. It will call
     * BusAttachment::EnablePeerSecurity for you.
     *
     * @param[in] keyStoreFileName  filepath to the keystore
     * @param[in] isShared  is the keystore shared
     */
    QStatus Enable(const char* keyStoreFileName, bool isShared);

    /**
     * There may be multiple concatenated certificates within the string.
     */
    QStatus AddTrustedRoot(const qcc::String& cert);

    /**
     * Set the private key for ECDHE_ECDSA authentication.
     *
     * @param[in] key a PEM encoded key 
     */
    BsSecurity& SetEcdsaPrivateKeyPEM(const qcc::String& key);

    /**
     * Set the certificate chain for ECDHE_ECDSA authentication.
     * The application may provide the DSA private key and public key in the certificate.
     *
     * @param[in] certChain one or more certificates in PEM format and concatenated together.
     */
    BsSecurity& SetEcdsaCertChainX509PEM(const qcc::String& certChain);

    /**
     * @param[in] pwd a pre-shared key for the old ECDHE_PSK authentication method or
     * a password for the ECDHE_SPEKE method.
     */
    BsSecurity& SetPassword(const qcc::String& pwd);

    BsSecurity& SetMechanism(const qcc::String& mech);

    /**
     * @param expirationSeconds  The master secret's expiration time in seconds.
     */
    BsSecurity& SetExpiration(uint32_t expirationSeconds);

    /**
     * @param listener for security events.  This is optional.
     */
    BsSecurity& SetPermissionListener(std::shared_ptr<ajn::PermissionConfigurationListener> listener);

    /**
     * @param pathPrefix Extensions of .pub and .priv will be used
     * to read the files.
     */
    BsSecurity& LoadFiles(const qcc::String& pathPrefix);

  private:
    class Impl;

    Impl* m_impl;
};

//======================================================================
} // namespace services
} // namespace ajn

#endif // BS_SECURITY_H_