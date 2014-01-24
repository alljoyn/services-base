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

#include "ActionBusObject.h"
#include "../ControlPanelConstants.h"
#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/Action.h>

namespace ajn {
namespace services {
using namespace qcc;
using namespace cpsConsts;

ActionBusObject::ActionBusObject(BusAttachment* bus, String const& objectPath, uint16_t langIndx,
                                 QStatus& status, Widget* widget) : WidgetBusObject(objectPath, langIndx,
                                                                                    TAG_ACTION_BUSOBJECT, status, widget)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not create the BusObject");
        }
        return;
    }

    String interfaceName = widget->getIsSecured() ? AJ_SECURED_ACTION_INTERFACE : AJ_ACTION_INTERFACE;
    m_InterfaceDescription = (InterfaceDescription*) bus->GetInterface(interfaceName.c_str());
    if (!m_InterfaceDescription) {
        do {
            CHECK_AND_BREAK(bus->CreateInterface(interfaceName.c_str(), m_InterfaceDescription, widget->getIsSecured()));
            CHECK_AND_BREAK(addDefaultInterfaceVariables(m_InterfaceDescription));
            CHECK_AND_BREAK(m_InterfaceDescription->AddMethod(AJ_METHOD_EXECUTE.c_str(), AJPARAM_EMPTY.c_str(),
                                                              AJPARAM_EMPTY.c_str(), AJPARAM_EMPTY.c_str()));
            m_InterfaceDescription->Activate();
        } while (0);
    }
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not create interface");
        }
        return;
    }

    status = AddInterface(*m_InterfaceDescription);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not add interface");
        }
        return;
    }

    //Get the signal methods for future use
    m_SignalPropertyChanged = m_InterfaceDescription->GetMember(AJ_SIGNAL_PROPERTIES_CHANGED.c_str());

    if (widget->getControlPanelMode() == CONTROLLER_MODE) {
        status = addSignalHandler(bus);
    } else if (widget->getWidgetType() == ACTION) {
        const ajn::InterfaceDescription::Member* execMember = m_InterfaceDescription->GetMember(AJ_METHOD_EXECUTE.c_str());

        status = AddMethodHandler(execMember, static_cast<MessageReceiver::MethodHandler>(&ActionBusObject::ActionExecute));
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not register the MethodHandler");
            }
            return;
        }
    }

    if (logger) {
        logger->debug(TAG, "Created ActionBusObject successfully");
    }
}

ActionBusObject::~ActionBusObject()
{
}

void ActionBusObject::ActionExecute(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QStatus status = ER_OK;
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "Execute was called");
    }

    if (((Action*)m_Widget)->executeCallBack()) {
        MsgArg replyArg;
        status = MethodReply(msg, &replyArg, 0);
        if (logger) {
            logger->info(TAG, "Execute completed successfully");
        }
    } else {
        status = MethodReply(msg, AJ_ERROR_UNKNOWN.c_str(), AJ_ERROR_UNKNOWN_MESSAGE.c_str());
        if (logger) {
            logger->warn(TAG, "Execute did not complete successfully");
        }
    }

    if (ER_OK != status) {
        if (logger) {
            logger->warn(TAG, "Did not reply successfully");
        }
    }
}

QStatus ActionBusObject::ExecuteAction()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!m_Proxy) {
        if (logger) {
            logger->warn(TAG, "Cannot execute the Action. ProxyBusObject is not set");
        }
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    const ajn::InterfaceDescription::Member* execMember = m_InterfaceDescription->GetMember(AJ_METHOD_EXECUTE.c_str());
    if (!execMember) {
        if (logger) {
            logger->warn(TAG, "Cannot execute the Action. ExecMember is not set");
        }
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    Message replyMsg(*bus);
    QStatus status = m_Proxy->MethodCall(*execMember, NULL, 0, replyMsg);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Call to execute the Action failed");
        }
    }
    return status;
}

QStatus ActionBusObject::Introspect(std::vector<IntrospectionNode>& childNodes)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!m_Proxy) {
        if (logger) {
            logger->warn(TAG, "Cannot Check Versions. ProxyBusObject is not set");
        }
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    QStatus status = m_Proxy->IntrospectRemoteObject();
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not introspect RemoteObject");
        }
        return status;
    }

    size_t numChildren = m_Proxy->GetChildren();
    if (numChildren == 0) {
        if (logger) {
            logger->warn(TAG, "ActionWithDialog does not have children");
        }
        return ER_FAIL;
    }

    ProxyBusObject** proxyBusObjectChildren = new ProxyBusObject *[numChildren];
    numChildren = m_Proxy->GetChildren(proxyBusObjectChildren, numChildren);

    for (size_t i = 0; i < numChildren; i++) {

        String const& objectPath = proxyBusObjectChildren[i]->GetPath();
        if (logger) {
            logger->debug(TAG, "ObjectPath is: " + objectPath);
        }

        status = proxyBusObjectChildren[i]->IntrospectRemoteObject();
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not introspect RemoteObjectChild");
            }
            delete[] proxyBusObjectChildren;
            return status;
        }

        size_t numInterfaces = proxyBusObjectChildren[i]->GetInterfaces();

        if (numInterfaces == 0) {
            continue;
        }

        const InterfaceDescription** ifaces = new const InterfaceDescription *[numInterfaces];
        numInterfaces = proxyBusObjectChildren[i]->GetInterfaces(ifaces, numInterfaces);
        for (size_t j = 0; j < numInterfaces; j++) {
            if (logger) {
                logger->debug(TAG, "InterfaceName is : " + String(ifaces[j]->GetName()));
            }
            if (strcmp(ifaces[j]->GetName(), AJ_DIALOG_INTERFACE.c_str()) == 0) {
                IntrospectionNode node(objectPath, DIALOG, false);
                childNodes.push_back(node);
            } else if (strcmp(ifaces[j]->GetName(), AJ_SECURED_DIALOG_INTERFACE.c_str()) == 0) {
                IntrospectionNode node(objectPath, DIALOG, true);
                childNodes.push_back(node);
            } else {
                if (logger) {
                    logger->debug(TAG, "Ignoring interfaceName: " + String(ifaces[j]->GetName()));
                }
            }
        }
        delete[] ifaces;
    }
    delete[] proxyBusObjectChildren;
    return ER_OK;
}

} /* namespace services */
} /* namespace ajn */



