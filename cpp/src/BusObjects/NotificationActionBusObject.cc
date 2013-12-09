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

#include <alljoyn/controlpanel/ControlPanelService.h>
#include "NotificationActionBusObject.h"
#include "../ControlPanelConstants.h"

namespace ajn {
namespace services {
using namespace qcc;
using namespace cpsConsts;

#define NOTIFICATIONACTION_INTERFACE_VERSION 1
#define TAG TAG_NOTIFICATIONACTIONBUSOBJECT

NotificationActionBusObject::NotificationActionBusObject(BusAttachment* bus, String const& objectPath, QStatus& status) :
    BusObject(objectPath.c_str()), m_SignalDismiss(0)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    status = ER_OK;

    String interfaceName = AJ_NOTIFICATIONACTION_INTERFACE;
    InterfaceDescription* intf = (InterfaceDescription*) bus->GetInterface(interfaceName.c_str());
    if (!intf) {
        do {
            CHECK_AND_BREAK(bus->CreateInterface(interfaceName.c_str(), intf));
            CHECK_AND_BREAK(intf->AddProperty(AJ_PROPERTY_VERSION.c_str(), AJPARAM_UINT16.c_str(), PROP_ACCESS_READ));
            CHECK_AND_BREAK(intf->AddSignal(AJ_SIGNAL_DISMISS.c_str(), AJPARAM_EMPTY.c_str(), AJPARAM_EMPTY.c_str()));
            intf->Activate();
        } while (0);
    }
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not create interface");
        return;
    }

    status = AddInterface(*intf);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not add interface");
        return;
    }

    //Get the signal methods for future use
    m_SignalDismiss = intf->GetMember(AJ_SIGNAL_DISMISS.c_str());
    if (logger)
        logger->debug(TAG, "Created NotificationActionBusObject successfully");
}

NotificationActionBusObject::~NotificationActionBusObject()
{
}

QStatus NotificationActionBusObject::Get(const char* interfaceName, const char* propName, MsgArg& val)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (logger)
        logger->debug(TAG, "Get property was called - in NotificationActionBusObject class:\n");

    if (0 == strcmp(AJ_PROPERTY_VERSION.c_str(), propName)) {
        return val.Set(AJPARAM_UINT16.c_str(), NOTIFICATIONACTION_INTERFACE_VERSION);
    }
    return ER_BUS_NO_SUCH_PROPERTY;
}

QStatus NotificationActionBusObject::Set(const char* interfaceName, const char* propName, MsgArg& val)
{
    return ER_ALLJOYN_ACCESS_PERMISSION_ERROR;
}


QStatus NotificationActionBusObject::SendDismissSignal()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    ControlPanelBusListener* busListener = ControlPanelService::getInstance()->getBusListener();
    QStatus status = ER_BUS_PROPERTY_VALUE_NOT_SET;

    if (!m_SignalDismiss) {
        if (logger)
            logger->warn(TAG, "Can't send Dismiss signal. Signal to set");
        return status;
    }

    if (!busListener) {
        if (logger)
            logger->warn(TAG, "Can't send valueChanged signal. SessionIds are unknown");
        return status;
    }

    const std::vector<SessionId>& sessionIds = busListener->getSessionIds();
    for (size_t indx = 0; indx < sessionIds.size(); indx++) {
        status = Signal(NULL, sessionIds[indx], *m_SignalDismiss, NULL, 0);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not send PropertyChanged Signal for sessionId: " + sessionIds[indx]);
        }
    }
    return status;
}

} /* namespace services */
} /* namespace ajn */



