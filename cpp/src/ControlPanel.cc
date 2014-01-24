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

#include <alljoyn/controlpanel/ControlPanel.h>
#include <algorithm>
#include <alljoyn/about/AboutServiceApi.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelConstants.h"
#include "BusObjects/ControlPanelBusObject.h"
#include "BusObjects/NotificationActionBusObject.h"
#include "BusObjects/IntrospectionNode.h"

namespace ajn {
namespace services {
using namespace cpsConsts;

#define TAG TAG_CONTROLPANEL

ControlPanel* ControlPanel::createControlPanel(LanguageSet* languageSet)
{
    if (!languageSet) {
        GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
        if (logger) {
            logger->warn(TAG, "Could not create ControlPanel. LanguageSet is NULL");
        }
        return NULL;
    }
    return new ControlPanel(*languageSet);
}

ControlPanel::ControlPanel(LanguageSet const& languageSet) :
    m_LanguageSet(languageSet), m_RootWidget(0), m_ControlPanelBusObject(0), m_ObjectPath(""), m_Device(0)
{

}

ControlPanel::ControlPanel(LanguageSet const& languageSet, qcc::String objectPath, ControlPanelDevice* device) :
    m_LanguageSet(languageSet), m_RootWidget(0), m_ControlPanelBusObject(0), m_ObjectPath(objectPath), m_Device(device)
{

}

ControlPanel::ControlPanel(const ControlPanel& controlPanel) : m_LanguageSet(controlPanel.m_LanguageSet)
{

}

ControlPanel& ControlPanel::operator=(const ControlPanel& controlPanel)
{
    return *this;
}

ControlPanel::~ControlPanel()
{
    std::map<qcc::String, Container*>::const_iterator it;
    for (it = m_RootWidgetMap.begin(); it != m_RootWidgetMap.end(); it++) {
        delete it->second;
    }
}

QStatus ControlPanel::setRootWidget(Container* rootWidget)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!rootWidget) {
        if (logger) {
            logger->warn(TAG, "Could not add a NULL rootWidget");
        }
        return ER_BAD_ARG_1;
    }

    if (m_RootWidget) {
        if (logger) {
            logger->warn(TAG, "Could not set the RootWidget. RootWidget already set");
        }
        return ER_BUS_PROPERTY_ALREADY_EXISTS;
    }

    m_RootWidget = rootWidget;
    return ER_OK;
}

qcc::String ControlPanel::getPanelName() const
{
    if (m_RootWidget) {
        return m_RootWidget->getWidgetName();
    }

    if (m_RootWidgetMap.size()) {
        return m_RootWidgetMap.begin()->second->getWidgetName();
    }

    return "";
}

Container* ControlPanel::getRootWidget() const
{
    return m_RootWidget;
}

QStatus ControlPanel::registerObjects(BusAttachment* bus, qcc::String const& unitName)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (m_ControlPanelBusObject) {
        if (logger) {
            logger->warn(TAG, "Could not register Object. BusObject already exists");
        }
        return ER_BUS_OBJ_ALREADY_EXISTS;
    }

    if (!bus) {
        if (logger) {
            logger->warn(TAG, "Could not register Object. Bus is NULL");
        }
        return ER_BAD_ARG_1;
    }

    if (!(bus->IsStarted() && bus->IsConnected())) {
        if (logger) {
            logger->warn(TAG, "Could not register Object. Bus is not started or not connected");
        }
        return ER_BAD_ARG_1;
    }

    if (!m_RootWidget) {
        if (logger) {
            logger->warn(TAG, "RootWidget has not been initialized. Can't continue");
        }
        return ER_BUS_TRANSPORT_NOT_STARTED;
    }

    AboutServiceApi* aboutService = AboutServiceApi::getInstance();
    if (!aboutService) {
        if (logger) {
            logger->warn(TAG, "Could not retrieve AboutService. It has not been initialized");
        }
        return ER_BUS_TRANSPORT_NOT_STARTED;
    }

    QStatus status = ER_OK;
    qcc::String objectPath = AJ_OBJECTPATH_PREFIX + unitName + "/" + m_RootWidget->getWidgetName();
    m_ControlPanelBusObject = new ControlPanelBusObject(bus, objectPath.c_str(), status);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not create ControlPanelBusObject");
        }
        return status;
    }
    status = bus->RegisterBusObject(*m_ControlPanelBusObject);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not register ControlPanelBusObject.");
        }
        return status;
    }

    std::vector<qcc::String> interfaces;
    interfaces.push_back(AJ_CONTROLPANEL_INTERFACE);
    aboutService->AddObjectDescription(objectPath, interfaces);

    status = m_RootWidget->registerObjects(bus, m_LanguageSet, objectPath + "/", "", true);
    return status;
}

QStatus ControlPanel::unregisterObjects(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus status = ER_OK;
    if (!m_ControlPanelBusObject && !m_RootWidget) {
        if (logger) {
            logger->info(TAG, "Can not unregister. BusObjects do not exist");
        }
        return status; //this does not need to fail
    }

    if (!bus) {
        if (logger) {
            logger->warn(TAG, "Could not unregister Object. Bus is NULL");
        }
        return ER_BAD_ARG_1;
    }

    if (m_ControlPanelBusObject) {
        bus->UnregisterBusObject(*m_ControlPanelBusObject);

        delete m_ControlPanelBusObject;
        m_ControlPanelBusObject = 0;
    }

    if (m_RootWidget) {
        QStatus status = m_RootWidget->unregisterObjects(bus);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not unregister RootContainer.");
            }
        }
    }

    std::map<qcc::String, Container*>::const_iterator it;
    for (it = m_RootWidgetMap.begin(); it != m_RootWidgetMap.end(); it++) {
        it->second->unregisterObjects(bus);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not unregister RootContainer for language " + it->first);
            }
        }
    }

    return status;
}

QStatus ControlPanel::registerObjects(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!bus) {
        if (logger) {
            logger->warn(TAG, "Could not register Object. Bus is NULL");
        }
        return ER_BAD_ARG_1;
    }

    if (!(bus->IsStarted() && bus->IsConnected())) {
        if (logger) {
            logger->warn(TAG, "Could not register Object. Bus is not started or not connected");
        }
        return ER_BAD_ARG_1;
    }

    if (m_ControlPanelBusObject) {
        if (logger) {
            logger->debug(TAG, "BusObject already exists, just refreshing remote controller");
        }
        return m_ControlPanelBusObject->setRemoteController(bus, m_Device->getDeviceBusName(), m_Device->getSessionId());
    }

    QStatus status = ER_OK;
    m_ControlPanelBusObject = new ControlPanelBusObject(bus, m_ObjectPath, status);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not create ControlPanelBusObject");
        }
        return status;
    }

    status = m_ControlPanelBusObject->setRemoteController(bus, m_Device->getDeviceBusName(), m_Device->getSessionId());
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Call to SetRemoteController failed");
        }
        return status;
    }

    status = checkVersions();
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Call to CheckVersions failed");
        }
        return status;
    }

    status = addChildren();
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Call to AddChildren failed");
        }
        return status;
    }
    return status;
}

QStatus ControlPanel::checkVersions()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!m_ControlPanelBusObject) {
        if (logger) {
            logger->warn(TAG, "ControlPanelBusObject is not set");
        }
        return ER_BUS_BUS_NOT_STARTED;
    }
    return m_ControlPanelBusObject->checkVersions();
}

QStatus ControlPanel::addChildren()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!m_ControlPanelBusObject) {
        if (logger) {
            logger->warn(TAG, "ControlPanelBusObject is not set");
        }
        return ER_BUS_BUS_NOT_STARTED;
    }

    std::vector<IntrospectionNode> childNodes;
    QStatus status = m_ControlPanelBusObject->Introspect(childNodes);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Introspection failed");
        }
        return status;
    }

    for (size_t i = 0; i < childNodes.size(); i++) {
        qcc::String const& objectPath = childNodes[i].getObjectPath();

        std::vector<qcc::String> splitPath = ControlPanelService::SplitObjectPath(objectPath);
        if (splitPath.size() < 4) {
            return ER_FAIL;
        }

        qcc::String const& containerName = splitPath[2];
        qcc::String language = splitPath[3];
        std::replace(language.begin(), language.end(), '_', '-'); //languages that have '_' should be replaced with '-'
        m_LanguageSet.addLanguage(language);
        Container* container = new Container(containerName, NULL, childNodes[i].getObjectPath(), m_Device);
        container->setIsSecured(childNodes[i].isSecured());
        m_RootWidgetMap[language] = container;
    }
    return ER_OK;
}

const LanguageSet& ControlPanel::getLanguageSet() const
{
    return m_LanguageSet;
}

ControlPanelDevice* ControlPanel::getDevice() const
{
    return m_Device;
}

const qcc::String& ControlPanel::getObjectPath() const
{
    return m_ObjectPath;
}

Container* ControlPanel::getRootWidget(qcc::String const& language) const
{
    std::map<qcc::String, Container*>::const_iterator it;
    it = m_RootWidgetMap.find(language);
    if (it != m_RootWidgetMap.end()) {
        BusAttachment* bus = ControlPanelService::getInstance()->getBusAttachment();
        it->second->registerObjects(bus);
        return it->second;
    }
    return NULL;
}

} /* namespace services */
} /* namespace ajn */
