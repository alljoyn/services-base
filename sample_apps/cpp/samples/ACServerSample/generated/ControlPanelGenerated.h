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

#ifndef CONTROLPANELGENERATED_H_
#define CONTROLPANELGENERATED_H_

#include <alljoyn/controlpanel/ControlPanelControllee.h>
#include <alljoyn/controlpanel/Container.h>
#include <alljoyn/controlpanel/Label.h>
#include <alljoyn/controlpanel/ActionWithDialog.h>
#include "../generated/MyDeviceCurrentTempStringProperty.h"
#include "../generated/MyDeviceCurrentHumidityStringProperty.h"
#include "../generated/MyDeviceAc_mode.h"
#include "../generated/MyDeviceStatusStringProperty.h"
#include "../generated/MyDeviceSet_temperature.h"
#include "../generated/MyDeviceFan_speed.h"
#include "../generated/MyDeviceTurnFanOn.h"
#include "../generated/MyDeviceTurnFanOff.h"


/**
 * Generated Class - do not change code
 */
class ControlPanelGenerated {
  public:

    static QStatus PrepareWidgets(ajn::services::ControlPanelControllee*& controlPanelControllee);

    static void PrepareLanguageSets();

    static void Shutdown();

    static bool languageSetsDone;

    static ajn::services::ControlPanelControlleeUnit* myDeviceUnit;

    static ajn::services::ControlPanel* myDeviceRootContainerControlPanel;

    static ajn::services::Container* myDeviceRootContainer;

    static ajn::services::Container* myDeviceTempAndHumidityContainer;

    static ajn::services::MyDeviceCurrentTempStringProperty* myDeviceCurrentTempStringProperty;

    static ajn::services::MyDeviceCurrentHumidityStringProperty* myDeviceCurrentHumidityStringProperty;

    static ajn::services::Container* myDeviceControlsContainer;

    static ajn::services::MyDeviceAc_mode* myDeviceAc_mode;

    static ajn::services::MyDeviceStatusStringProperty* myDeviceStatusStringProperty;

    static ajn::services::MyDeviceSet_temperature* myDeviceSet_temperature;

    static ajn::services::MyDeviceFan_speed* myDeviceFan_speed;

    static ajn::services::NotificationAction* myDeviceTurnFanOnNotificationAction;

    static ajn::services::MyDeviceTurnFanOn* myDeviceTurnFanOn;

    static ajn::services::NotificationAction* myDeviceTurnFanOffNotificationAction;

    static ajn::services::MyDeviceTurnFanOff* myDeviceTurnFanOff;


};

#endif /* CONTROLPANELGENERATED_H_ */