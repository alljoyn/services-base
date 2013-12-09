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

#ifndef CONTROLPANELUNIT_H_
#define CONTROLPANELUNIT_H_

#include <qcc/String.h>
#include <alljoyn/Status.h>
#include <map>
#include <alljoyn/controlpanel/HttpControl.h>

namespace ajn {
namespace services {

class ControlPanelDevice;
class ControlPanel;

/**
 * Class that represents a ControlPanel Unit.
 */
class ControlPanelUnit {
  public:

    /**
     * ControlPanelUnit()
     * @param unitName
     * @param device
     */
    ControlPanelUnit(qcc::String const& unitName, ControlPanelDevice* device);

    /**
     * ~ControlPanelUnit()
     */
    virtual ~ControlPanelUnit();

    /**
     * getDevice()
     * @return ControlPanelDevice*
     */
    ControlPanelDevice* getDevice() const;

    /**
     * getUnitName()
     * @return string
     */
    const qcc::String& getUnitName() const;

    /**
     * add a HttpControl
     * @param objectPath of HTTPControl
     */
    QStatus addHttpControl(qcc::String const& objectPath);

    /**
     * addControlPanel
     * @param objectPath
     * @param panelName
     */
    QStatus addControlPanel(qcc::String const& objectPath, qcc::String const& panelName);

    /**
     * Fill control panels and the HTTPControl
     */
    QStatus registerObjects();

    /**
     * Called when shutting down device
     * @return status - success/failure
     */
    QStatus shutdownUnit();

    /**
     * Get the ControlPanels of the Unit
     * @return controlPanels map
     */
    const std::map<qcc::String, ControlPanel*>& getControlPanels() const;

    /**
     * Get the HttpControl of the Unit
     * @return httpControl
     */
    HttpControl* getHttpControl() const;

  private:

    /**
     * Name of Unit
     */
    qcc::String m_UnitName;

    /**
     * Device Unit belongs to
     */
    ControlPanelDevice* m_Device;

    /**
     * The ControlPanels of this Unit
     */
    std::map<qcc::String, ControlPanel*> m_ControlPanels;

    /**
     * The HTTPControl of the Unit
     */
    HttpControl* m_HttpControl;
};

} /* namespace services */
} /* namespace ajn */
#endif /* CONTROLPANELUNIT_H_ */
