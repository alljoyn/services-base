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
#include "../generated/MyDeviceHeatProperty.h"
#include "../generated/MyDeviceOvenAction.h"
#include "../generated/MyDeviceLightConfirm.h"
#include "../generated/MyDeviceAreYouSure.h"


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

    static ajn::services::Label* myDeviceCurrentTemp;

    static ajn::services::MyDeviceHeatProperty* myDeviceHeatProperty;

    static ajn::services::MyDeviceOvenAction* myDeviceOvenAction;

    static ajn::services::ActionWithDialog* myDeviceLightAction;

    static ajn::services::MyDeviceLightConfirm* myDeviceLightConfirm;

    static ajn::services::NotificationAction* myDeviceAreYouSureNotificationAction;

    static ajn::services::MyDeviceAreYouSure* myDeviceAreYouSure;


};

#endif /* CONTROLPANELGENERATED_H_ */