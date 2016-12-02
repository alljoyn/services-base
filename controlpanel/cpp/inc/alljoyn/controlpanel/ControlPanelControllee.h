/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#ifndef CONTROLPANELCONTROLLEE_H_
#define CONTROLPANELCONTROLLEE_H_

#include <alljoyn/controlpanel/ControlPanelControlleeUnit.h>

namespace ajn {
namespace services {

/**
 * ControlPanelControllee Class.
 * Allows creation of a controllee to be controller by a controller
 * Made up of ControlPanels and/or NotificationWithActions and possibly
 * a HTTPControl
 */
class ControlPanelControllee {
  public:

    /**
     * Constructor of ControlPanelControllee
     */
    ControlPanelControllee();

    /**
     * Destructor of ControlPanelControllee
     */
    virtual ~ControlPanelControllee();

    /**
     * Add a ControlPanelUnit to the ControlPanelControllee
     * @param unit - unit to add
     * @return status - success/failure
     */
    QStatus addControlPanelUnit(ControlPanelControlleeUnit* unit);

    /**
     * Register the busObjects of the ControlPanelControllee
     * @param bus - the bus to register the Objects with
     * @return status - success/failure
     */
    QStatus registerObjects(BusAttachment* bus);

    /**
     * Unregister the busObjects of the ControlPanelControllee
     * @param bus - the bus to unregister the Objects with
     * @return status - success/failure
     */
    QStatus unregisterObjects(BusAttachment* bus);

    /**
     * Get the Units of the Controllee
     * @return the Controllee Units
     */
    const std::vector<ControlPanelControlleeUnit*>& getControlleeUnits() const;

  private:

    /**
     * Vector containing the Devices Units
     */
    std::vector<ControlPanelControlleeUnit*> m_Units;
};

} /* namespace services */
} /* namespace ajn */
#endif /* CONTROLPANELCONTROLLEE_H_ */