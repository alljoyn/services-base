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

NotificationActionBusObject::NotificationActionBusObject(BusAttachment* bus, String const& objectPath, QStatus& status,
                                                         NotificationAction* notificationAction) : BusObject(objectPath.c_str()),
    m_SignalDismiss(0), m_NotificationAction(notificationAction), m_Proxy(0), m_ObjectPath(objectPath), m_InterfaceDescription(0)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    status = ER_OK;

    String interfaceName = AJ_NOTIFICATIONACTION_INTERFACE;
    m_InterfaceDescription = (InterfaceDescription*) bus->GetInterface(interfaceName.c_str());
    if (!m_InterfaceDescription) {
        do {
            CHECK_AND_BREAK(bus->CreateInterface(interfaceName.c_str(), m_InterfaceDescription));
            CHECK_AND_BREAK(m_InterfaceDescription->AddProperty(AJ_PROPERTY_VERSION.c_str(), AJPARAM_UINT16.c_str(), PROP_ACCESS_READ));
            CHECK_AND_BREAK(m_InterfaceDescription->AddSignal(AJ_SIGNAL_DISMISS.c_str(), AJPARAM_EMPTY.c_str(), AJPARAM_EMPTY.c_str()));
            m_InterfaceDescription->Activate();
        } while (0);
    }
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not create interface");
        return;
    }

    status = AddInterface(*m_InterfaceDescription);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not add interface");
        return;
    }

    //Get the signal methods for future use
    m_SignalDismiss = m_InterfaceDescription->GetMember(AJ_SIGNAL_DISMISS.c_str());
    if (m_NotificationAction) {
        status =  bus->RegisterSignalHandler(this, static_cast<MessageReceiver::SignalHandler>(&NotificationActionBusObject::DismissSignal),
                                             m_SignalDismiss, m_ObjectPath.c_str());
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register the SignalHandler");
            return;
        }
    }
    if (logger)
        logger->debug(TAG, "Created NotificationActionBusObject successfully");
}

QStatus NotificationActionBusObject::UnregisterSignalHandler(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus status =  bus->UnregisterSignalHandler(this,
                                                   static_cast<MessageReceiver::SignalHandler>(&NotificationActionBusObject::DismissSignal),
                                                   m_SignalDismiss,
                                                   m_ObjectPath.c_str());
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not unregister the SignalHandler");
    }
    return status;
}

NotificationActionBusObject::~NotificationActionBusObject()
{
    if (m_Proxy)
        delete m_Proxy;
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

    if (logger)
        logger->info(TAG, "Sending dismiss Signal to all connected sessions");

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

void NotificationActionBusObject::DismissSignal(const InterfaceDescription::Member* member, const char* srcPath, Message& msg)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (logger)
        logger->debug(TAG, "Received DismissSignal signal");

    return m_NotificationAction->DismissSignal();
}

QStatus NotificationActionBusObject::setRemoteController(BusAttachment* bus, qcc::String const& deviceBusName, SessionId sessionId)
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

QStatus NotificationActionBusObject::checkVersions()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!m_Proxy) {
        if (logger)
            logger->warn(TAG, "Cannot Check Versions. ProxyBusObject is not set");
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    MsgArg value;
    QStatus status = m_Proxy->GetProperty(AJ_NOTIFICATIONACTION_INTERFACE.c_str(), AJ_PROPERTY_VERSION.c_str(), value);
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

    if (NOTIFICATIONACTION_INTERFACE_VERSION < version) {
        if (logger)
            logger->warn(TAG, "The versions of the interface are not compatible");
        return ER_BUS_INTERFACE_MISMATCH;
    }
    return ER_OK;
}

QStatus NotificationActionBusObject::Introspect(std::vector<IntrospectionNode>& childNodes)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!m_Proxy) {
        if (logger)
            logger->warn(TAG, "Cannot Check Versions. ProxyBusObject is not set");
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    QStatus status = m_Proxy->IntrospectRemoteObject();
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not introspect RemoteObject");
        return status;
    }

    size_t numChildren = m_Proxy->GetChildren();
    if (numChildren == 0)
        return ER_OK;

    ProxyBusObject** proxyBusObjectChildren = new ProxyBusObject *[numChildren];
    numChildren = m_Proxy->GetChildren(proxyBusObjectChildren, numChildren);

    for (size_t i = 0; i < numChildren; i++) {

        String const& objectPath = proxyBusObjectChildren[i]->GetPath();
        if (logger)
            logger->debug(TAG, "ObjectPath is: " + objectPath);

        status = proxyBusObjectChildren[i]->IntrospectRemoteObject();
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not introspect RemoteObjectChild");
            delete[] proxyBusObjectChildren;
            return status;
        }

        size_t numInterfaces = proxyBusObjectChildren[i]->GetInterfaces();

        if (numInterfaces == 0)
            continue;

        const InterfaceDescription** ifaces = new const InterfaceDescription *[numInterfaces];
        numInterfaces = proxyBusObjectChildren[i]->GetInterfaces(ifaces, numInterfaces);
        for (size_t j = 0; j < numInterfaces; j++) {
            if (logger)
                logger->debug(TAG, "InterfaceName is : " + String(ifaces[j]->GetName()));
            if (strcmp(ifaces[j]->GetName(), AJ_CONTAINER_INTERFACE.c_str()) == 0) {
                IntrospectionNode node(objectPath, CONTAINER, false);
                childNodes.push_back(node);
            } else if (strcmp(ifaces[j]->GetName(), AJ_SECURED_CONTAINER_INTERFACE.c_str()) == 0) {
                IntrospectionNode node(objectPath, CONTAINER, true);
                childNodes.push_back(node);
            } else if (strcmp(ifaces[j]->GetName(), AJ_DIALOG_INTERFACE.c_str()) == 0) {
                IntrospectionNode node(objectPath, DIALOG, false);
                childNodes.push_back(node);
            } else if (strcmp(ifaces[j]->GetName(), AJ_SECURED_DIALOG_INTERFACE.c_str()) == 0) {
                IntrospectionNode node(objectPath, DIALOG, true);
                childNodes.push_back(node);
            } else {
                if (logger)
                    logger->debug(TAG, "Ignoring interface - not a container or dialog interface");
            }
        }
        delete[] ifaces;
    }
    delete[] proxyBusObjectChildren;
    return ER_OK;
}

} /* namespace services */
} /* namespace ajn */



