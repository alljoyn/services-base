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
#include "WidgetBusObject.h"
#include "../ControlPanelConstants.h"
#include "WidgetProxyBusObjectListener.h"

namespace ajn {
namespace services {
using namespace qcc;
using namespace cpsConsts;

#define DEFAULT_WIDGET_INTERFACE_VERSION 1

WidgetBusObject::WidgetBusObject(String const& objectPath, uint16_t langIndx, String const& tag,
                                 QStatus& status, Widget* widget) : BusObject(objectPath.c_str()), m_Widget(widget), m_Proxy(0),
    m_LanguageIndx(langIndx), m_ObjectPath(objectPath), m_InterfaceDescription(0), m_SignalPropertyChanged(0), TAG(tag)
{
    status = ER_OK;
    if (!widget) {
        GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
        if (logger)
            logger->warn(TAG, "Can't send in a NULL widget");
        status = ER_BAD_ARG_5;
    }
}

WidgetBusObject::~WidgetBusObject()
{
    if (m_Proxy)
        delete m_Proxy;
}

uint16_t WidgetBusObject::getInterfaceVersion()
{
    return DEFAULT_WIDGET_INTERFACE_VERSION;
}

QStatus WidgetBusObject::addDefaultInterfaceVariables(InterfaceDescription* intf)
{
    if (!intf)
        return ER_BAD_ARG_1;

    QStatus status;
    CHECK_AND_RETURN(intf->AddProperty(AJ_PROPERTY_VERSION.c_str(), AJPARAM_UINT16.c_str(), PROP_ACCESS_READ));
    CHECK_AND_RETURN(intf->AddProperty(AJ_PROPERTY_STATES.c_str(), AJPARAM_UINT32.c_str(), PROP_ACCESS_READ));
    CHECK_AND_RETURN(intf->AddProperty(AJ_PROPERTY_OPTPARAMS.c_str(), AJPARAM_ARRAY_DICT_UINT16_VAR.c_str(),
                                       PROP_ACCESS_READ));

    return intf->AddSignal(AJ_SIGNAL_PROPERTIES_CHANGED.c_str(), "", "", 0);
}

QStatus WidgetBusObject::addSignalHandler(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus status =  bus->RegisterSignalHandler(this,
                                                 static_cast<MessageReceiver::SignalHandler>(&WidgetBusObject::PropertyChanged),
                                                 m_SignalPropertyChanged,
                                                 m_ObjectPath.c_str());
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not register the SignalHandler");
    }
    return status;
}

QStatus WidgetBusObject::UnregisterSignalHandler(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus status =  bus->UnregisterSignalHandler(this,
                                                   static_cast<MessageReceiver::SignalHandler>(&WidgetBusObject::PropertyChanged),
                                                   m_SignalPropertyChanged,
                                                   m_ObjectPath.c_str());
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not unregister the SignalHandler");
    }
    return status;
}

QStatus WidgetBusObject::Get(const char* interfaceName, const char* propName, MsgArg& val)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (logger)
        logger->debug(TAG, "Get property was called - in WidgetBusObject class:");

    if (0 == strcmp(AJ_PROPERTY_VERSION.c_str(), propName)) {
        return val.Set(AJPARAM_UINT16.c_str(), getInterfaceVersion());
    } else if (0 == strcmp(AJ_PROPERTY_STATES.c_str(), propName)) {
        return m_Widget->fillStatesArg(val, m_LanguageIndx);
    } else if (0 == strcmp(AJ_PROPERTY_OPTPARAMS.c_str(), propName)) {
        return m_Widget->fillOptParamsArg(val, m_LanguageIndx);
    }

    return ER_BUS_NO_SUCH_PROPERTY;
}

QStatus WidgetBusObject::Set(const char* interfaceName, const char* propName, MsgArg& val)
{
    return ER_ALLJOYN_ACCESS_PERMISSION_ERROR;
}

void WidgetBusObject::PropertyChanged(const InterfaceDescription::Member* member, const char* srcPath, Message& msg)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (msg.unwrap()->GetSender() && strcmp(msg.unwrap()->GetSender(), m_Widget->getDevice()->getDeviceBusName().c_str()) != 0) {
        if (logger)
            logger->debug(TAG, "Received PropertyChanged signal for someone else");
        return;
    }

    if (logger)
        logger->debug(TAG, "Received PropertyChanged signal - reloading properties");

    return m_Widget->PropertyChanged();
}

QStatus WidgetBusObject::SendPropertyChangedSignal()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    ControlPanelBusListener* busListener = ControlPanelService::getInstance()->getBusListener();
    QStatus status = ER_BUS_PROPERTY_VALUE_NOT_SET;

    if (!m_SignalPropertyChanged) {
        if (logger)
            logger->warn(TAG, "Can't send propertyChanged signal. Signal to set");
        return status;
    }

    if (!busListener) {
        if (logger)
            logger->warn(TAG, "Can't send valueChanged signal. SessionIds are unknown");
        return status;
    }

    const std::vector<SessionId>& sessionIds = busListener->getSessionIds();
    for (size_t indx = 0; indx < sessionIds.size(); indx++) {
        status = Signal(NULL, sessionIds[indx], *m_SignalPropertyChanged, NULL, 0);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not send PropertyChanged Signal for sessionId: " + sessionIds[indx]);
        }
    }
    return status;
}

QStatus WidgetBusObject::setRemoteController(BusAttachment* bus, qcc::String const& deviceBusName, SessionId sessionId)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (m_Proxy && m_Proxy->GetSessionId() == sessionId) {
        if (logger)
            logger->debug(TAG, "ProxyBusObject already set - ignoring");
        return ER_OK;
    }

    if (!m_InterfaceDescription) {
        if (logger)
            logger->warn(TAG, "InterfaceDescription is not set. Cannot set RemoteController");
        return ER_FAIL;
    }

    if (m_Proxy)
        delete m_Proxy;  // delete existing proxyBusObject. create new one with new sessionId

    m_Proxy = new ProxyBusObject(*bus, deviceBusName.c_str(), m_ObjectPath.c_str(), sessionId);
    QStatus status = m_Proxy->AddInterface(*m_InterfaceDescription);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not add Interface to ProxyBusobject");
    }
    return status;
}

QStatus WidgetBusObject::checkVersions()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!m_Proxy) {
        if (logger)
            logger->warn(TAG, "Cannot Check Versions. ProxyBusObject is not set");
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    MsgArg value;
    QStatus status = m_Proxy->GetProperty(m_InterfaceDescription->GetName(), AJ_PROPERTY_VERSION.c_str(), value);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to get Version Property failed");
        return status;
    }

    uint16_t version = 1;
    status = value.Get(AJPARAM_UINT16.c_str(), &version);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not unmarshal version property");
        return status;
    }

    if (getInterfaceVersion() < version) {
        if (logger)
            logger->warn(TAG, "The versions of the interface are not compatible");
        return ER_BUS_INTERFACE_MISMATCH;
    }
    return ER_OK;
}

QStatus WidgetBusObject::fillProperties()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!m_Proxy) {
        if (logger)
            logger->warn(TAG, "Cannot fill Properties. ProxyBusObject is not set");
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    MsgArg allPropValues;
    QStatus status = m_Proxy->GetAllProperties(m_InterfaceDescription->GetName(), allPropValues);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to getAllProperties failed");
        return status;
    }
    return fillAllProperties(allPropValues);
}

QStatus WidgetBusObject::refreshProperties()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!m_Proxy) {
        if (logger)
            logger->warn(TAG, "Cannot refresh Properties. ProxyBusObject is not set");
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    WidgetProxyBusObjectListener* listener = new WidgetProxyBusObjectListener(m_Widget, this, TAG);
    QStatus status = m_Proxy->GetAllPropertiesAsync(m_InterfaceDescription->GetName(), listener,
                                                    static_cast<ProxyBusObject::Listener::GetAllPropertiesCB>(&WidgetProxyBusObjectListener::GetAllPropertiesCallBack),
                                                    NULL);

    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to getAllProperties failed");
    }
    return status;
}

QStatus WidgetBusObject::fillAllProperties(MsgArg const& allPropValues)
{
    QStatus status;
    do {
        MsgArg* allPropEntries;
        size_t allPropNum;
        CHECK_AND_BREAK(allPropValues.Get(AJPARAM_ARRAY_DICT_STR_VAR.c_str(), &allPropNum, &allPropEntries));

        for (size_t i = 0; i < allPropNum; i++) {
            char* key;
            MsgArg* variant;

            CHECK_AND_BREAK(allPropEntries[i].Get(AJPARAM_DICT_STR_VAR.c_str(), &key, &variant));
            if (strcmp(key, AJ_PROPERTY_VERSION.c_str()) == 0) {
                CHECK_AND_BREAK(m_Widget->readVersionArg(variant))
            } else if (strcmp(key, AJ_PROPERTY_STATES.c_str()) == 0) {
                CHECK_AND_BREAK(m_Widget->readStatesArg(variant))
            } else if (strcmp(key, AJ_PROPERTY_OPTPARAMS.c_str()) == 0) {
                CHECK_AND_BREAK(m_Widget->readOptParamsArg(variant))
            } else {
                CHECK_AND_BREAK(fillProperty(key, variant))
            }
        }
    } while (0);
    return status;
}

QStatus WidgetBusObject::fillProperty(char* key, MsgArg* variant)
{
    return ER_BUS_NO_SUCH_PROPERTY;
}

QStatus WidgetBusObject::Introspect(std::vector<IntrospectionNode>& childNodes)
{
    return ER_OK;
}

} /* namespace services */
} /* namespace ajn */
