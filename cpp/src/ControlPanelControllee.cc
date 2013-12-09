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

#include <alljoyn/controlpanel/ControlPanelControllee.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelConstants.h"

namespace ajn {
namespace services {
using namespace cpsConsts;

#define TAG TAG_CONTROLPANELCONTROLLEE

ControlPanelControllee::ControlPanelControllee(qcc::String const& unitName) :
    m_UnitName(unitName), m_HttpControl(0)
{
}

ControlPanelControllee::~ControlPanelControllee()
{

}

const qcc::String& ControlPanelControllee::getUnitName() const
{
    return m_UnitName;
}

QStatus ControlPanelControllee::addControlPanel(ControlPanel* controlPanel)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!controlPanel) {
        if (logger)
            logger->warn(TAG, "Could not add a NULL controlPanel");
        return ER_BAD_ARG_1;
    }

    m_ControlPanels.push_back(controlPanel);
    return ER_OK;
}

const std::vector<ControlPanel*>& ControlPanelControllee::getControlPanels() const
{
    return m_ControlPanels;
}

QStatus ControlPanelControllee::addNotificationAction(NotificationAction* notificationAction)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!notificationAction) {
        if (logger)
            logger->warn(TAG, "Could not add a NULL notificationAction");
        return ER_BAD_ARG_1;
    }

    m_NotificationActions.push_back(notificationAction);
    return ER_OK;
}

const std::vector<NotificationAction*>& ControlPanelControllee::getNotificationActions() const
{
    return m_NotificationActions;
}

QStatus ControlPanelControllee::setHttpControl(HttpControl* httpControl)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!httpControl) {
        if (logger)
            logger->warn(TAG, "Could not add a NULL httpControl");
        return ER_BAD_ARG_1;
    }

    if (m_HttpControl) {
        if (logger)
            logger->warn(TAG, "Could not set httpControl. HttpControl already set");
        return ER_BUS_PROPERTY_ALREADY_EXISTS;
    }

    m_HttpControl = httpControl;
    return ER_OK;
}

const HttpControl* ControlPanelControllee::getHttpControl() const
{
    return m_HttpControl;
}

QStatus ControlPanelControllee::registerObjects(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus status = ER_OK;

    if (m_HttpControl) {
        status = m_HttpControl->registerObjects(bus, m_UnitName);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register the HttpControl");
            return status;
        }
    }

    for (size_t indx = 0; indx < m_ControlPanels.size(); indx++) {
        status = m_ControlPanels[indx]->registerObjects(bus, m_UnitName);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register Objects for the ControlPanels");
            return status;
        }
    }

    for (size_t indx = 0; indx < m_NotificationActions.size(); indx++) {
        status = m_NotificationActions[indx]->registerObjects(bus, m_UnitName);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register Objects for the ControlPanels");
            return status;
        }
    }
    return status;
}

QStatus ControlPanelControllee::unregisterObjects(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus returnStatus = ER_OK;

    if (m_HttpControl) {
        QStatus status = m_HttpControl->unregisterObjects(bus);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not unregister the HttpControl");
            returnStatus = status;
        }
    }

    for (size_t indx = 0; indx < m_ControlPanels.size(); indx++) {
        QStatus status = m_ControlPanels[indx]->unregisterObjects(bus);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register Objects for the ControlPanels");
            returnStatus = status;
        }
    }

    for (size_t indx = 0; indx < m_NotificationActions.size(); indx++) {
        QStatus status = m_NotificationActions[indx]->unregisterObjects(bus);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register Objects for the ControlPanels");
            returnStatus = status;
        }
    }
    return returnStatus;
}

} /* namespace services */
} /* namespace ajn */
