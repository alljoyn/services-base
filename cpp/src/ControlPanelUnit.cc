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

#include <alljoyn/controlpanel/ControlPanelUnit.h>
#include <alljoyn/controlpanel/ControlPanelDevice.h>
#include <alljoyn/controlpanel/ControlPanel.h>
#include <alljoyn/controlpanel/LanguageSets.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelConstants.h"

#define TAG TAG_CONTROLPANELUNIT

namespace ajn {
namespace services {
using namespace cpsConsts;

ControlPanelUnit::ControlPanelUnit(qcc::String const& unitName, ControlPanelDevice* device) :
    m_UnitName(unitName), m_Device(device), m_HttpControl(0)
{

}

ControlPanelUnit::~ControlPanelUnit()
{
}

QStatus ControlPanelUnit::addControlPanel(qcc::String const& objectPath, qcc::String const& panelName)
{
    BusAttachment* busAttachment = ControlPanelService::getInstance()->getBusAttachment();
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!busAttachment) {
        if (logger)
            logger->warn(TAG, "BusAttachment is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }

    std::map<qcc::String, ControlPanel*>::iterator it;
    ControlPanel* controlPanel = 0;

    if ((it = m_ControlPanels.find(panelName)) == m_ControlPanels.end()) {
        LanguageSet myLanguages(m_UnitName + "-" + panelName);
        LanguageSets::add(myLanguages.getLanguageSetName(), myLanguages);
        controlPanel = new ControlPanel(*LanguageSets::get(myLanguages.getLanguageSetName()), objectPath, m_Device);
        m_ControlPanels[panelName] = controlPanel;
    } else {
        if (logger)
            logger->debug(TAG, "ControlPanel for " + panelName + " already exists");
        controlPanel = it->second;
    }

    if (m_Device->getSessionId() != 0) {
        controlPanel->registerObjects(busAttachment);
    }
    return ER_OK;
}

QStatus ControlPanelUnit::registerObjects()
{
    BusAttachment* busAttachment = ControlPanelService::getInstance()->getBusAttachment();
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!busAttachment) {
        if (logger)
            logger->warn(TAG, "BusAttachment is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }

    busAttachment->EnableConcurrentCallbacks();

    QStatus status;
    QStatus returnStatus = ER_OK;

    if (m_HttpControl)
        returnStatus = m_HttpControl->registerObjects(busAttachment);

    std::map<qcc::String, ControlPanel*>::iterator it;
    for (it = m_ControlPanels.begin(); it != m_ControlPanels.end(); it++) {
        if ((status = it->second->registerObjects(busAttachment)) != ER_OK) {
            if (logger)
                logger->warn(TAG, "Registering ControlPanel Objects failed");
            returnStatus = status;
        }
    }
    return returnStatus;
}

QStatus ControlPanelUnit::shutdownUnit()
{
    BusAttachment* busAttachment = ControlPanelService::getInstance()->getBusAttachment();
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!busAttachment) {
        if (logger)
            logger->warn(TAG, "BusAttachment is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }

    busAttachment->EnableConcurrentCallbacks();

    std::map<qcc::String, ControlPanel*>::iterator iter;
    std::map<qcc::String, ControlPanel*>::iterator deliter;
    for (iter = m_ControlPanels.begin(); iter != m_ControlPanels.end();) {
        ControlPanel* controlpanel = iter->second;
        deliter = iter++;

        m_ControlPanels.erase(deliter);
        controlpanel->unregisterObjects(busAttachment);
        delete controlpanel;
    }

    if (m_HttpControl) {
        m_HttpControl->unregisterObjects(busAttachment);
        delete m_HttpControl;
        m_HttpControl = 0;
    }

    return ER_OK;
}

QStatus ControlPanelUnit::addHttpControl(qcc::String const& objectPath)
{
    BusAttachment* busAttachment = ControlPanelService::getInstance()->getBusAttachment();
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    if (!busAttachment) {
        if (logger)
            logger->warn(TAG, "BusAttachment is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }

    if (m_HttpControl) {
        if (logger)
            logger->info(TAG, "HttpControl for this unit already exists");
        return ER_OK;
    }

    m_HttpControl = new HttpControl(objectPath, m_Device);
    QStatus status = ER_OK;

    if (m_Device->getSessionId() != 0) {
        status = m_HttpControl->registerObjects(busAttachment);
    }
    return status;
}

ControlPanelDevice* ControlPanelUnit::getDevice() const
{
    return m_Device;
}

const qcc::String& ControlPanelUnit::getUnitName() const
{
    return m_UnitName;
}

const std::map<qcc::String, ControlPanel*>& ControlPanelUnit::getControlPanels() const
{
    return m_ControlPanels;
}

HttpControl* ControlPanelUnit::getHttpControl() const
{
    return m_HttpControl;
}

} /* namespace services */
} /* namespace ajn */

