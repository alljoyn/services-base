/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
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

#include "OnboardingSignalListenerImpl.h"
#include <iostream>
#include <qcc/Debug.h>

#define QCC_MODULE "ONBOARD"


OnboardingSignalListenerImpl::OnboardingSignalListenerImpl()
{
    QCC_DbgTrace(("%s", __FUNCTION__));
}

OnboardingSignalListenerImpl::~OnboardingSignalListenerImpl()
{
    QCC_DbgTrace(("%s", __FUNCTION__));
}


void OnboardingSignalListenerImpl::ConnectionResultSignalReceived(short connectionResultCode, const qcc::String& connectionResultMessage)
{
    QCC_UNUSED(connectionResultCode);
    QCC_UNUSED(connectionResultMessage);

    QCC_DbgTrace(("%s", __FUNCTION__));
    QCC_DbgHLPrintf(("code = %d, message = %s", connectionResultCode, connectionResultMessage.c_str()));
}
