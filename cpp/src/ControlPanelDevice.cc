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

#include <alljoyn/controlpanel/ControlPanelDevice.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelConstants.h"

namespace ajn {
namespace services {
using namespace cpsConsts;

#define TAG TAG_CONTROLPANELDEVICE

ControlPanelDevice::ControlPanelDevice(qcc::String const& deviceBusName) :
    m_DeviceBusName(deviceBusName), m_Listener(0)
{

}

ControlPanelDevice::~ControlPanelDevice()
{
}

QStatus ControlPanelDevice::startSession()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    BusAttachment* busAttachment = ControlPanelService::getInstance()->getBusAttachment();

    if (!busAttachment) {
        if (logger)
            logger->warn(TAG, "BusAttachment is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }

    if (m_SessionHandler.getSessionId() != 0) {
        if (logger)
            logger->info(TAG, "Session already started, firing Listener");

        ControlPanelListener* listener = getListener();
        if (listener)
            listener->sessionEstablished(this);

        return ER_OK;
    }

    busAttachment->EnableConcurrentCallbacks();

    SessionOpts opts(SessionOpts::TRAFFIC_MESSAGES, true, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);
    QStatus status = busAttachment->JoinSessionAsync(m_DeviceBusName.c_str(), (ajn::SessionPort)CONTROLPANELSERVICE_PORT, &m_SessionHandler,
                                                     opts, &m_SessionHandler, this);

    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Unable to JoinSession with " + m_DeviceBusName);
    }
    return status;
}

QStatus ControlPanelDevice::endSession()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    BusAttachment* busAttachment = ControlPanelService::getInstance()->getBusAttachment();

    if (!busAttachment) {
        if (logger)
            logger->warn(TAG, "BusAttachment is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }

    SessionId sessionId = m_SessionHandler.getSessionId();
    if (sessionId == 0) {
        if (logger)
            logger->info(TAG, "Session not started. Can't end Session");
        return ER_OK;
    }

    QStatus status = busAttachment->LeaveSession(sessionId);

    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Unable to LeaveSession with " + m_DeviceBusName);
    }

    return status;
}

QStatus ControlPanelDevice::shutdownDevice()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus status = endSession();
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to endSession failed");
    }

    std::map<qcc::String, ControlPanelUnit*>::iterator iter;
    std::map<qcc::String, ControlPanelUnit*>::iterator deliter;
    for (iter = m_DeviceUnits.begin(); iter != m_DeviceUnits.end();) {
        ControlPanelUnit* unit = iter->second;
        deliter = iter++;

        m_DeviceUnits.erase(deliter);
        unit->shutdownUnit();
        delete unit;
    }
    return status;
}

void ControlPanelDevice::handleSessionJoined()
{
    std::map<qcc::String, ControlPanelUnit*>::iterator iter;
    for (iter = m_DeviceUnits.begin(); iter != m_DeviceUnits.end(); iter++) {
        iter->second->registerObjects(); //TODO Opportunity for errorOccured
    }

    ControlPanelListener* listener = getListener();
    if (listener)
        listener->sessionEstablished(this);
}

ControlPanelUnit* ControlPanelDevice::addControlPanelUnit(qcc::String const& objectPath, std::vector<qcc::String> const& interfaces)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    bool hasControlPanel = false;
    bool hasHttpControl = false;

    for (size_t i = 0; i < interfaces.size(); i++) {
        if (interfaces[i].compare(AJ_CONTROLPANEL_INTERFACE) == 0) {
            hasControlPanel = true;
            if (logger)
                logger->debug(TAG, "ObjectPath contains ControlPanel");
        }
        if (interfaces[i].compare(AJ_HTTPCONTROL_INTERFACE) == 0) {
            hasHttpControl = true;
            if (logger)
                logger->debug(TAG, "ObjectPath contains HttpControl");
        }
    }

    if (!hasControlPanel && !hasHttpControl) {
        if (logger)
            logger->warn(TAG, "ObjectPath does not contain a ControlPanel or HttpControl");
        return NULL;
    }
    qcc::String unitName = "";
    qcc::String panelName = "";

    ControlPanelUnit* unit = getControlPanelUnit(objectPath, unitName, panelName);

    if (hasHttpControl)
        unit->addHttpControl(objectPath);

    if (hasControlPanel)
        unit->addControlPanel(objectPath, panelName);

    return unit;
}

ControlPanelListener* ControlPanelDevice::getListener() const
{
    if (m_Listener)
        return m_Listener;
    return ControlPanelService::getInstance()->getControlPanelListener();
}

QStatus ControlPanelDevice::setListener(ControlPanelListener* listener)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!listener) {
        if (logger)
            logger->warn(TAG, "Listener cannot be NULL");
        return ER_BAD_ARG_1;
    }
    m_Listener = listener;
    return ER_OK;
}

ControlPanelUnit* ControlPanelDevice::getControlPanelUnit(qcc::String const& objectPath, qcc::String& unitName, qcc::String& panel)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    std::vector<qcc::String> splitObjectPath = ControlPanelService::SplitObjectPath(objectPath);
    if (splitObjectPath.size() < 3) {
        if (logger)
            logger->warn(TAG, "ObjectPath is unexpected size");
        return NULL;
    }

    unitName = splitObjectPath[1];
    panel = splitObjectPath[2];

    std::map<qcc::String, ControlPanelUnit*>::iterator it;
    if ((it = m_DeviceUnits.find(unitName)) != m_DeviceUnits.end()) {
        return it->second;
    } else {
        if (logger)
            logger->debug(TAG, "Creating new unit " + unitName);
        ControlPanelUnit* unit = new ControlPanelUnit(unitName, this);
        m_DeviceUnits[unitName] = unit;
        return unit;
    }
}

NotificationAction* ControlPanelDevice::addNotificationAction(qcc::String const& objectPath)
{
    return NULL;
}

const qcc::String& ControlPanelDevice::getDeviceBusName() const
{
    return m_DeviceBusName;
}

const ajn::SessionId ControlPanelDevice::getSessionId() const
{
    return m_SessionHandler.getSessionId();
}

const std::map<qcc::String, ControlPanelUnit*>& ControlPanelDevice::getDeviceUnits() const
{
    return m_DeviceUnits;
}

void ControlPanelDevice::getAllControlPanels(std::vector<ControlPanel*>& controlPanelsVec)
{
    controlPanelsVec.clear();
    std::map<qcc::String, ControlPanelUnit*>::iterator iter;
    for (iter = m_DeviceUnits.begin(); iter != m_DeviceUnits.end(); iter++) {
        std::map<qcc::String, ControlPanel*> controlPanels = iter->second->getControlPanels();
        std::map<qcc::String, ControlPanel*>::iterator it;
        for (it = controlPanels.begin(); it != controlPanels.end(); it++) {
            controlPanelsVec.push_back(it->second);
        }
    }

}

} /* namespace services */
} /* namespace ajn */
