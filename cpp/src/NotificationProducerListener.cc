/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
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

#include "NotificationProducerListener.h"
#include <iostream>
#include <algorithm>
#include <alljoyn/notification/NotificationService.h>
#include "NotificationConstants.h"

using namespace ajn;
using namespace services;
using namespace qcc;
using namespace nsConsts;

static qcc::String TAG(TAG_NOTIFICATION_PRODUCER_LISTENER);

NotificationProducerListener::NotificationProducerListener() :
    SessionPortListener(), m_SessionPort(0)
{
}

NotificationProducerListener::~NotificationProducerListener()
{
}

void NotificationProducerListener::setSessionPort(ajn::SessionPort sessionPort)
{
    m_SessionPort = sessionPort;
}

SessionPort NotificationProducerListener::getSessionPort()
{
    return m_SessionPort;
}

bool NotificationProducerListener::AcceptSessionJoiner(ajn::SessionPort sessionPort, const char* joiner, const ajn::SessionOpts& opts)
{
    if (sessionPort != m_SessionPort) {
        return false;
    }

    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        String log("NotificationProducerListener::AcceptSessionJoiner() sessionPort=");
        log.append(std::to_string(sessionPort).c_str());
        log.append(" joiner:");
        log.append(joiner);
        logger->debug(TAG, log);
    }

    return true;
}

void NotificationProducerListener::SessionJoined(SessionPort sessionPort, SessionId sessionId, const char* joiner)
{

}
