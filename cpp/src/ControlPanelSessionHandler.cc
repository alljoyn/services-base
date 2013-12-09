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

#include <alljoyn/controlpanel/ControlPanelSessionHandler.h>
#include <alljoyn/controlpanel/ControlPanelDevice.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelConstants.h"
#include <sstream>

namespace ajn {
namespace services {
using namespace cpsConsts;

#define TAG TAG_CONTROLPANELSESSIONHANDLER

ControlPanelSessionHandler::ControlPanelSessionHandler() : m_SessionId(0)
{

}

ControlPanelSessionHandler::~ControlPanelSessionHandler()
{
}


void ControlPanelSessionHandler::SessionLost(ajn::SessionId sessionId)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (logger) {
        std::stringstream sessionIdStr;
        sessionIdStr << sessionId;
        logger->info(TAG, "Session lost for sessionId: " + qcc::String(sessionIdStr.str().c_str()));
    }
    m_SessionId = 0;
}

void ControlPanelSessionHandler::JoinSessionCB(QStatus status, ajn::SessionId id, const ajn::SessionOpts& opts, void* context)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, qcc::String("Joining session failed. Status: ") + QCC_StatusText(status));
        //TODO: Opportunity for Error Occurred
        return;
    }

    if (logger) {
        std::stringstream sessionIdStr;
        sessionIdStr << id;
        logger->info(TAG, "Joining session succeeded. SessionId: " + qcc::String(sessionIdStr.str().c_str()));
    }

    m_SessionId = id;
    ControlPanelDevice* device = (ControlPanelDevice*) context;
    device->handleSessionJoined();
}

ajn::SessionId ControlPanelSessionHandler::getSessionId() const
{
    return m_SessionId;
}

} /* namespace services */
} /* namespace ajn */
