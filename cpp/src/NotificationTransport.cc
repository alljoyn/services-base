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

#include <alljoyn/notification/NotificationService.h>

#include "NotificationTransport.h"
#include "NotificationConstants.h"

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

NotificationTransport::NotificationTransport(ajn::BusAttachment* bus,
                                             qcc::String const& servicePath, QStatus& status, String const& interfaceName, String const& tag) :
    BusObject(servicePath.c_str()), m_SignalMethod(0), TAG(tag)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    InterfaceDescription* intf = NULL;
    status = bus->CreateInterface(interfaceName.c_str(), intf);

    if (status == ER_OK) {
        intf->AddSignal(AJ_SIGNAL_METHOD.c_str(), AJ_NOTIFY_PARAMS.c_str(), AJ_NOTIFY_PARAM_NAMES.c_str(), 0);
        intf->AddProperty(AJ_PROPERTY_VERSION.c_str(), AJPARAM_UINT16.c_str(), PROP_ACCESS_READ);
        intf->Activate();
    } else if (status == ER_BUS_IFACE_ALREADY_EXISTS) {
        intf = (InterfaceDescription*) bus->GetInterface(interfaceName.c_str());
        if (!intf) {
            if (logger) {
                logger->warn(TAG, "Could not get interface");
            }
            status = ER_BUS_UNKNOWN_INTERFACE;
            return;
        }
    } else {
        if (logger) {
            logger->warn(TAG, "Could not create interface");
        }
        return;
    }

    status = AddInterface(*intf);
    if (status != ER_OK) {
        if (logger) {
            logger->debug(TAG, "Could not add interface");
        }
        return;
    }

    // Get the signal method for future use
    m_SignalMethod = intf->GetMember(AJ_SIGNAL_METHOD.c_str());
}

NotificationTransport::~NotificationTransport()
{

}

QStatus NotificationTransport::Get(const char* ifcName, const char* propName, MsgArg& val)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "Get property was called :\n");
    }

    if (0 != strcmp(AJ_PROPERTY_VERSION.c_str(), propName)) {
        if (logger) {
            logger->warn(TAG, "Called for property different than version. Exiting :\n");
        }
        return ER_BUS_NO_SUCH_PROPERTY;
    }

    val.typeId = ALLJOYN_UINT16;
    val.v_uint16 = NotificationService::getVersion();
    return ER_OK;
}

QStatus NotificationTransport::Set(const char* ifcName, const char* propName, MsgArg& val)
{
    return ER_ALLJOYN_ACCESS_PERMISSION_ERROR;
}
