/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifndef SRPKEYLISTENER_H_
#define SRPKEYLISTENER_H_

#include <alljoyn/AuthListener.h>

/*
 * A listener for Authentication
 */
class SrpKeyXListener : public ajn::AuthListener {
  public:
    SrpKeyXListener();

    virtual ~SrpKeyXListener();

    void setPassCode(qcc::String const& passCode);

    void setGetPassCode(const char* (*getPassCode)());

    bool RequestCredentials(const char* authMechanism, const char* authPeer, uint16_t authCount, const char* userId,
                            uint16_t credMask, Credentials& creds);

    void AuthenticationComplete(const char* authMechanism, const char* authPeer, bool success);

  private:
    qcc::String m_PassCode;

    const char* (*m_GetPassCode)();
};

#endif /* SRPKEYLISTENER_H_ */


