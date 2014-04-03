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

#include <alljoyn/controlpanel/ControlPanelController.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/about/AboutClient.h>
#include "ControlPanelConstants.h"
#include <alljoyn/controlpanel/LogModule.h>

namespace ajn {
namespace services {
using namespace cpsConsts;

ControlPanelController::ControlPanelController()
{
}

ControlPanelController::~ControlPanelController()
{
}

ControlPanelDevice* ControlPanelController::createControllableDevice(qcc::String const& sender,
                                                                     const AnnounceHandler::ObjectDescriptions& objectDescs)
{
    if (sender.length() == 0) {
        QCC_DbgHLPrintf(("Sender cannot be empty"));
        return NULL;
    }

    ControlPanelDevice* device = 0;
    std::map<qcc::String, ControlPanelDevice*>::iterator iter;
    if ((iter = m_ControllableDevices.find(sender)) != m_ControllableDevices.end()) {
        QCC_DbgPrintf(("ControlPanelDevice for this sender already exists"));
        device = iter->second;
    }

    AboutClient::ObjectDescriptions::const_iterator it;
    qcc::String ControlPanelPrefix = "/ControlPanel/";
    bool hasControlPanel = false;

    for (it = objectDescs.begin(); it != objectDescs.end(); ++it) {
        qcc::String key = it->first;

        if (key.compare(0, ControlPanelPrefix.size(), ControlPanelPrefix) == 0) {
            if (!device) {
                device = getControllableDevice(sender);
            }
            if (device->addControlPanelUnit(key, it->second)) {
                QCC_DbgPrintf(("Adding ControlPanelUnit for objectPath: %s", key.c_str()));
                hasControlPanel = true;
            }
        }
    }
    if (hasControlPanel) {
        QCC_DbgPrintf(("Calling startSession for device %s", sender.c_str()));
        device->startSessionAsync();
    }
    return device;
}

ControlPanelDevice* ControlPanelController::getControllableDevice(qcc::String const& sender)
{
    if (sender.length() == 0) {
        QCC_DbgHLPrintf(("Sender cannot be empty"));
        return NULL;
    }

    std::map<qcc::String, ControlPanelDevice*>::iterator iter;
    if ((iter = m_ControllableDevices.find(sender)) != m_ControllableDevices.end()) {
        QCC_DbgPrintf(("ControlPanelDevice for this sender already exists"));
        return iter->second;
    }

    ControlPanelDevice* newDevice = new ControlPanelDevice(sender);
    m_ControllableDevices[sender] = newDevice;
    return newDevice;
}

QStatus ControlPanelController::deleteControllableDevice(qcc::String const& sender)
{
    if (sender.length() == 0) {
        QCC_DbgHLPrintf(("Sender cannot be empty"));
        return ER_BAD_ARG_1;
    }

    std::map<qcc::String, ControlPanelDevice*>::iterator iter;
    if ((iter = m_ControllableDevices.find(sender)) == m_ControllableDevices.end()) {
        QCC_DbgHLPrintf(("Sender does not exist"));
        return ER_BAD_ARG_1;
    }

    ControlPanelDevice* device = iter->second;
    QStatus status = device->shutdownDevice();
    if (status != ER_OK) {
        QCC_LogError(status, ("Could not end Session successfully"));
    }

    m_ControllableDevices.erase(iter);
    delete device;
    return status;
}

QStatus ControlPanelController::deleteAllControllableDevices()
{
    QStatus returnStatus = ER_OK;
    std::map<qcc::String, ControlPanelDevice*>::iterator iter;
    std::map<qcc::String, ControlPanelDevice*>::iterator deliter;
    for (iter = m_ControllableDevices.begin(); iter != m_ControllableDevices.end();) {

        ControlPanelDevice* device = iter->second;
        deliter = iter++;
        QStatus status = device->shutdownDevice();
        if (status != ER_OK) {
            QCC_LogError(status, ("Could not shutdown Device successfully"));
            returnStatus = status;
        }

        m_ControllableDevices.erase(deliter);
        delete device;
    }
    return returnStatus;
}

const std::map<qcc::String, ControlPanelDevice*>& ControlPanelController::getControllableDevices() const
{
    return m_ControllableDevices;
}

} /* namespace services */
} /* namespace ajn */
