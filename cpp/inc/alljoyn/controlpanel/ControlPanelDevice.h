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

#ifndef CONTROLPANELDEVICE2_H_
#define CONTROLPANELDEVICE2_H_

#include <alljoyn/controlpanel/ControlPanelSessionHandler.h>
#include <alljoyn/controlpanel/ControlPanelUnit.h>
#include <alljoyn/controlpanel/NotificationAction.h>
#include <map>

namespace ajn {
namespace services {

class ControlPanelListener;

/**
 * ControlPanelDevice
 */
class ControlPanelDevice {
  public:

    friend class ControlPanelSessionHandler;

    /**
     * ControlPanelDevice
     * @param deviceBusName
     */
    ControlPanelDevice(qcc::String const& deviceBusName);

    /**
     * ~ControlPanelDevice()
     */
    virtual ~ControlPanelDevice();

    /**
     * startSession
     * @return status - success/failure
     */
    QStatus startSession();

    /**
     * endSession
     * @return status - success/failure
     */
    QStatus endSession();

    /**
     * ShutDown device - end Session and release units
     * @return
     */
    QStatus shutdownDevice();

    /**
     * getDeviceBusName
     * @return deviceBusName
     */
    const qcc::String& getDeviceBusName() const;

    /**
     * getSessionId
     * @return const ajn::SessionId
     */
    const ajn::SessionId getSessionId() const;

    /**
     * getDeviceUnits
     * @return the ControlPanelUnits of this Device
     */
    const std::map<qcc::String, ControlPanelUnit*>& getDeviceUnits() const;

    /**
     * getAllControlPanels
     * @return a vector with all the controlPanel defined as children of this DeviceUnit
     */
    void getAllControlPanels(std::vector<ControlPanel*>& controlPanelsVec);

    /**
     * addControlPanelUnit
     * @param objectPath
     * @param interfaces
     * @return const ControlPanelUnit pointer
     */
    ControlPanelUnit* addControlPanelUnit(qcc::String const& objectPath, std::vector<qcc::String> const& interfaces);

    /**
     * addNotificationAction
     * @param objectPath
     * @return NotificationAction pointer
     */
    NotificationAction* addNotificationAction(qcc::String const& objectPath);

    /**
     * Get the Listener defined for this SessionHandler
     * @return
     */
    ControlPanelListener* getListener() const;

    /**
     * Set the Listener defined for this SessionHandler
     * @param listener
     * @return status - success/failure
     */
    QStatus setListener(ControlPanelListener* listener);

  private:

    /**
     * Private function to handle a Session being joined with this Device
     */
    void handleSessionJoined();

    /**
     * Parse ObjectPath and create a Control Panel unit using the unitName and panel
     * @param objectPath - objectPath to parse
     * @param unitName - unitname will be filled based on the objectPath
     * @param panel - panel will be filled based on the objectPath
     * @return a ControlPanelUnit
     */
    ControlPanelUnit* getControlPanelUnit(qcc::String const& objectPath, qcc::String& unitName, qcc::String& panel);

    /**
     * The Device's BusName
     */
    qcc::String m_DeviceBusName;

    /**
     * SessionHandler for Device
     */
    ControlPanelSessionHandler m_SessionHandler;

    /**
     * Map containing the Devices Units
     */
    std::map<qcc::String, ControlPanelUnit*> m_DeviceUnits;

    /**
     * Listener - can be used to override the default ControlPanelListener of the Device
     */
    ControlPanelListener* m_Listener;

};

} /* namespace services */
} /* namespace ajn */
#endif /* CONTROLPANELDEVICE2_H_ */
