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

#include "HttpControlBusObject.h"
#include "../ControlPanelConstants.h"
#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/HttpControl.h>

namespace ajn {
namespace services {
using namespace qcc;
using namespace cpsConsts;

#define TAG TAG_HTTPCONTROL_BUSOBJECT
#define HTTP_INTERFACE_VERSION 1

HttpControlBusObject::HttpControlBusObject(BusAttachment* bus, String const& objectPath,
                                           QStatus& status, HttpControl* httpControl) :
    BusObject(objectPath.c_str()), m_HttpControl(httpControl), m_Proxy(0), m_ObjectPath(objectPath), m_InterfaceDescription(0)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!httpControl) {
        if (logger)
            logger->warn(TAG, "HttpControl cannot be NULL");
        status = ER_BAD_ARG_4;
        return;
    }

    m_InterfaceDescription = (InterfaceDescription*) bus->GetInterface(AJ_HTTPCONTROL_INTERFACE.c_str());
    if (!m_InterfaceDescription) {
        do {
            CHECK_AND_BREAK(bus->CreateInterface(AJ_HTTPCONTROL_INTERFACE.c_str(), m_InterfaceDescription, false));
            CHECK_AND_BREAK(m_InterfaceDescription->AddProperty(AJ_PROPERTY_VERSION.c_str(), AJPARAM_UINT16.c_str(), PROP_ACCESS_READ));
            CHECK_AND_BREAK(m_InterfaceDescription->AddMethod(AJ_METHOD_GETROOTURL.c_str(), AJPARAM_EMPTY.c_str(),
                                                              AJPARAM_STR.c_str(), AJ_PROPERTY_URL.c_str()));
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

    if (m_HttpControl->getWidgetMode() == CONTROLLEE_WIDGET) {
        //Get the signal methods for future use
        const ajn::InterfaceDescription::Member* getRootUrlMember = m_InterfaceDescription->GetMember(AJ_METHOD_GETROOTURL.c_str());

        status = AddMethodHandler(getRootUrlMember, static_cast<MessageReceiver::MethodHandler>(&HttpControlBusObject::HttpControlGetUrl));
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register the SignalHandler");
            return;
        }
    }
    if (logger)
        logger->debug(TAG, "Created HttpControlBusObject successfully");
}

HttpControlBusObject::~HttpControlBusObject()
{
}

uint16_t HttpControlBusObject::getInterfaceVersion()
{
    return HTTP_INTERFACE_VERSION;
}

QStatus HttpControlBusObject::Get(const char* interfaceName, const char* propName, MsgArg& val)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (logger)
        logger->debug(TAG, "Get property was called - in HttpControlBusObject class:\n");

    if (0 == strcmp(AJ_PROPERTY_VERSION.c_str(), propName)) {
        return val.Set(AJPARAM_UINT16.c_str(), getInterfaceVersion());
    }

    return ER_BUS_NO_SUCH_PROPERTY;
}

QStatus HttpControlBusObject::Set(const char* interfaceName, const char* propName, MsgArg& val)
{
    return ER_ALLJOYN_ACCESS_PERMISSION_ERROR;
}

void HttpControlBusObject::HttpControlGetUrl(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (logger)
        logger->debug(TAG, "Get Url was called");

    MsgArg url;
    QStatus status = m_HttpControl->fillUrlArg(url);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not set Url");
        MethodReply(msg, AJ_ERROR_UNKNOWN.c_str(), AJ_ERROR_UNKNOWN_MESSAGE.c_str());
        return;
    }

    status = MethodReply(msg, &url, 1);
    if (ER_OK != status) {
        if (logger)
            logger->warn(TAG, "Did not reply successfully");
    } else {
        if (logger)
            logger->info(TAG, "Replied to GetUrl successfully");
    }
}

QStatus HttpControlBusObject::setRemoteController(BusAttachment* bus, qcc::String const& deviceBusName, SessionId sessionId)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (m_Proxy) {
        if (logger)
            logger->debug(TAG, "ProxyBusObject already set - ignoring");
        return ER_OK;
    }

    if (!m_InterfaceDescription) {
        if (logger)
            logger->warn(TAG, "InterfaceDescription is not set. Cannot set RemoteController");
        return ER_FAIL;
    }

    m_Proxy = new ProxyBusObject(*bus, deviceBusName.c_str(), m_ObjectPath.c_str(), sessionId);
    QStatus status = m_Proxy->AddInterface(*m_InterfaceDescription);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not add Interface to ProxyBusobject");
    }
    return status;
}

QStatus HttpControlBusObject::checkVersions()
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

    m_HttpControl->setVersion(version);
    return ER_OK;
}

QStatus HttpControlBusObject::GetUrl(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!m_Proxy) {
        if (logger)
            logger->warn(TAG, "Cannot get the Url. ProxyBusObject is not set");
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    const ajn::InterfaceDescription::Member* getRootUrlMember = m_InterfaceDescription->GetMember(AJ_METHOD_GETROOTURL.c_str());
    if (!getRootUrlMember) {
        if (logger)
            logger->warn(TAG, "Cannot get the Url. RootUrlMember is not set");
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    Message replyMsg(*bus);
    QStatus status = m_Proxy->MethodCall(*getRootUrlMember, NULL, 0, replyMsg);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to getUrl failed");
        return status;
    }

    const ajn::MsgArg* returnArgs;
    size_t numArgs;
    replyMsg->GetArgs(numArgs, returnArgs);
    if (numArgs != 1) {
        if (logger)
            logger->warn(TAG, "Received unexpected amount of returnArgs");
        return ER_BUS_UNEXPECTED_SIGNATURE;
    }

    status = m_HttpControl->readUrlArg(returnArgs[0]);
    return status;
}

} /* namespace services */
} /* namespace ajn */



