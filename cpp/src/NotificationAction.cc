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

#include <alljoyn/controlpanel/NotificationAction.h>
#include <alljoyn/about/AboutServiceApi.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
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
    m_NotificationActionBusObject(0)
{

}

NotificationAction::~NotificationAction() {
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
        bus->UnregisterBusObject(*m_NotificationActionBusObject);
        delete m_NotificationActionBusObject;
        m_NotificationActionBusObject = 0;
    }

    return status;
}

} /* namespace services */
} /* namespace ajn */
