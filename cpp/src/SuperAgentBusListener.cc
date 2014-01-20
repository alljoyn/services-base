/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
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

#include "SuperAgentBusListener.h"
#include "Transport.h"
#include <alljoyn/notification/NotificationService.h>
#include <qcc/String.h>

namespace ajn {
namespace services {

SuperAgentBusListener::SuperAgentBusListener(ajn::BusAttachment* bus)
    : BusListener(), m_Bus(bus), m_SuperAgentBusUniqueName("")
{

}

SuperAgentBusListener::~SuperAgentBusListener()
{

}

void SuperAgentBusListener::FoundAdvertisedName(const char* name, TransportMask transport, const char* namePrefix)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        qcc::String log("FoundAdvertisedName name:");
        log.append(name);
        logger->debug(TAG, log);
    }
    // We must enable concurrent callbacks since some of the calls below are blocking
    m_Bus->EnableConcurrentCallbacks();

    if (0 == strcmp(name, m_SuperAgentBusUniqueName.c_str())) {
        Transport::getInstance()->listenToSuperAgent(m_SuperAgentBusUniqueName.c_str());
    }
}

void SuperAgentBusListener::LostAdvertisedName(const char* name, TransportMask transport, const char* prefix)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "LostAdvertisedName");
    }
    if (0 == ::strcmp(name, m_SuperAgentBusUniqueName.c_str())) {
        Transport::getInstance()->cancelListenToSuperAgent(m_SuperAgentBusUniqueName.c_str());
    }
}

} //services
} //ajn
