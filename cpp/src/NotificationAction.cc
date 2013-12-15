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

#include <alljoyn/about/AboutServiceApi.h>
#include <alljoyn/controlpanel/NotificationAction.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/Dialog.h>
#include "ControlPanelConstants.h"
#include "BusObjects/NotificationActionBusObject.h"

namespace ajn {
namespace services {
using namespace cpsConsts;

#define TAG TAG_NOTIFICATIONACTION

NotificationAction* NotificationAction::createNotificationAction(LanguageSet* languageSet)
{
    if (!languageSet) {
        GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
        if (logger)
            logger->warn(TAG, "Could not create NotificationAction. LanguageSet is NULL");
        return NULL;
    }
    return new NotificationAction(*languageSet);
}

NotificationAction::NotificationAction(LanguageSet const& languageSet) :
    m_LanguageSet(languageSet), m_RootWidget(0),
    m_NotificationActionBusObject(0), m_ObjectPath(""), m_Device(0)
{

}

NotificationAction::NotificationAction(LanguageSet const& languageSet, qcc::String objectPath, ControlPanelDevice* device) :
    m_LanguageSet(languageSet), m_RootWidget(0), m_NotificationActionBusObject(0), m_ObjectPath(objectPath), m_Device(device)
{

}

NotificationAction::NotificationAction(const NotificationAction& notificationAction) : m_LanguageSet(notificationAction.m_LanguageSet)
{

}

NotificationAction& NotificationAction::operator=(const NotificationAction& notificationAction)
{
    return *this;
}

NotificationAction::~NotificationAction()
{
    std::map<qcc::String, RootWidget*>::const_iterator it;
    for (it = m_RootWidgetMap.begin(); it != m_RootWidgetMap.end(); it++) {
        delete it->second;
    }
}

QStatus NotificationAction::setRootWidget(RootWidget* rootWidget)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!rootWidget) {
        if (logger)
            logger->warn(TAG, "Could not add a NULL rootWidget");
        return ER_BAD_ARG_1;
    }

    if (m_RootWidget) {
        if (logger)
            logger->warn(TAG, "Could not set RootWidget. RootWidget already set");
        return ER_BUS_PROPERTY_ALREADY_EXISTS;
    }

    m_RootWidget = rootWidget;
    return ER_OK;
}

qcc::String NotificationAction::getNotificationActionName() const
{
    if (m_RootWidget)
        return m_RootWidget->getWidgetName();

    if (m_RootWidgetMap.size())
        return m_RootWidgetMap.begin()->second->getWidgetName();

    return "";
}

QStatus NotificationAction::registerObjects(BusAttachment* bus, qcc::String const& unitName)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (m_NotificationActionBusObject) {
        if (logger)
            logger->warn(TAG, "Could not register Object. BusObject already exists");
        return ER_BUS_OBJ_ALREADY_EXISTS;
    }

    QStatus status = ER_OK;
    qcc::String objectPath = AJ_OBJECTPATH_PREFIX + unitName + "/" + m_RootWidget->getWidgetName();
    m_NotificationActionBusObject = new NotificationActionBusObject(bus, objectPath.c_str(), status);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not create NotificationActionBusObject");
        return status;
    }

    status = bus->RegisterBusObject(*m_NotificationActionBusObject);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not register NotificationActionBusObject.");
        return status;
    }

    if (!m_RootWidget) {
        if (logger)
            logger->warn(TAG, "Root Widget not set");
        return ER_BUS_OBJECT_NOT_REGISTERED;
    }

    status = m_RootWidget->setNotificationActionBusObject(m_NotificationActionBusObject);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not set NotificationActionBusObject.");
        return status;
    }

    return m_RootWidget->registerObjects(bus, m_LanguageSet, objectPath + "/", "", true);
}

QStatus NotificationAction::unregisterObjects(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus status = ER_OK;
    if (!m_NotificationActionBusObject && !m_RootWidget) {
        if (logger)
            logger->info(TAG, "Can not unregister. BusObjects do not exist");
        return status; //this does not need to fail
    }

    if (!bus) {
        if (logger)
            logger->warn(TAG, "Could not unregister Object. Bus is NULL");
        return ER_BAD_ARG_1;
    }

    if (m_RootWidget) {
        m_NotificationActionBusObject = 0; // Gets unregistered in rootWidget
        status = m_RootWidget->unregisterObjects(bus);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not unregister RootWidget.");
            return status;
        }
    } else if (m_NotificationActionBusObject) {   // no rootWidget so need to unregister here
        if (m_Device)
            m_NotificationActionBusObject->UnregisterSignalHandler(bus);
        else
            bus->UnregisterBusObject(*m_NotificationActionBusObject);
        delete m_NotificationActionBusObject;
        m_NotificationActionBusObject = 0;
    }

    std::map<qcc::String, RootWidget*>::const_iterator it;
    for (it = m_RootWidgetMap.begin(); it != m_RootWidgetMap.end(); it++) {
        it->second->unregisterObjects(bus);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not unregister RootContainer for language " + it->first);
        }
    }

    return status;
}

QStatus NotificationAction::registerObjects(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!bus) {
        if (logger)
            logger->warn(TAG, "Could not register Object. Bus is NULL");
        return ER_BAD_ARG_1;
    }

    if (!(bus->IsStarted() && bus->IsConnected())) {
        if (logger)
            logger->warn(TAG, "Could not register Object. Bus is not started or not connected");
        return ER_BAD_ARG_1;
    }

    if (m_NotificationActionBusObject) {
        if (logger)
            logger->debug(TAG, "BusObject already exists, just refreshing remote controller");
        return m_NotificationActionBusObject->setRemoteController(bus, m_Device->getDeviceBusName(), m_Device->getSessionId());
    }

    QStatus status = ER_OK;
    m_NotificationActionBusObject = new NotificationActionBusObject(bus, m_ObjectPath, status, this);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not create NotificationActionBusObject");
        return status;
    }

    status = m_NotificationActionBusObject->setRemoteController(bus, m_Device->getDeviceBusName(), m_Device->getSessionId());
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to SetRemoteController failed");
        return status;
    }

    status = checkVersions();
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to CheckVersions failed");
        return status;
    }

    status = addChildren();
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to AddChildren failed");
        return status;
    }
    return status;
}

QStatus NotificationAction::checkVersions()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!m_NotificationActionBusObject) {
        if (logger)
            logger->warn(TAG, "NotificationActionBusObject is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }
    return m_NotificationActionBusObject->checkVersions();
}

QStatus NotificationAction::addChildren()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!m_NotificationActionBusObject) {
        if (logger)
            logger->warn(TAG, "NotificationActionBusObject is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }

    std::vector<IntrospectionNode> childNodes;
    QStatus status = m_NotificationActionBusObject->Introspect(childNodes);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Introspection failed");
        return status;
    }

    for (size_t i = 0; i < childNodes.size(); i++) {
        qcc::String const& objectPath = childNodes[i].getObjectPath();

        std::vector<qcc::String> splitPath = ControlPanelService::SplitObjectPath(objectPath);
        if (splitPath.size() < 4)
            return ER_FAIL;

        qcc::String const& containerName = splitPath[2];
        qcc::String const& language = splitPath[3];
        m_LanguageSet.addLanguage(language);
        RootWidget* rootWidget = 0;
        if (childNodes[i].getWidgetType() == CONTAINER)
            rootWidget = new Container(containerName, NULL, childNodes[i].getObjectPath(), m_Device);
        else
            rootWidget = new Dialog(containerName, NULL, childNodes[i].getObjectPath(), m_Device);
        rootWidget->setIsSecured(childNodes[i].isSecured());
        m_RootWidgetMap[language] = rootWidget;
    }
    return ER_OK;
}

const LanguageSet& NotificationAction::getLanguageSet() const
{
    return m_LanguageSet;
}

ControlPanelDevice* NotificationAction::getDevice() const
{
    return m_Device;
}

const qcc::String& NotificationAction::getObjectPath() const
{
    return m_ObjectPath;
}

RootWidget* NotificationAction::getRootWidget(qcc::String const& language) const
{
    std::map<qcc::String, RootWidget*>::const_iterator it;
    it = m_RootWidgetMap.find(language);
    if (it != m_RootWidgetMap.end()) {
        BusAttachment* bus = ControlPanelService::getInstance()->getBusAttachment();
        it->second->registerObjects(bus);
        return it->second;
    }
    return NULL;
}

void NotificationAction::DismissSignal()
{
    BusAttachment* busAttachment = ControlPanelService::getInstance()->getBusAttachment();
    if (busAttachment)
        busAttachment->EnableConcurrentCallbacks();

    ControlPanelListener* listener = m_Device->getListener();
    if (listener)
        listener->signalDismiss(m_Device, this);
}

} /* namespace services */
} /* namespace ajn */
