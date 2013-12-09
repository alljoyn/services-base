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

#ifndef CONTROLPANELGENERATED_H_
#define CONTROLPANELGENERATED_H_

#include <alljoyn/controlpanel/ControlPanelControllee.h>
#include <alljoyn/controlpanel/ControlPanelHints.h>
#include <alljoyn/controlpanel/Container.h>
#include <alljoyn/controlpanel/Label.h>
#include <alljoyn/controlpanel/ActionWithDialog.h>
#include "../generated/CurrentTempStringProperty.h"
#include "../generated/CurrentHumidityStringProperty.h"
#include "../generated/Ac_mode.h"
#include "../generated/StatusStringProperty.h"
#include "../generated/Set_temperature.h"
#include "../generated/Fan_speed.h"


/**
 * Generated Class - do not change code
 */
class ControlPanelGenerated {
  public:

    static QStatus PrepareWidgets(ajn::services::ControlPanelControllee*& controlPanelControllee);

    static void PrepareLanguageSets();

    static void Shutdown();

    static bool languageSetsDone;

    //private:

    static ajn::services::ControlPanel* rootContainerControlPanel;
    static ajn::services::Container* rootContainer;
    static ajn::services::Container* tempAndHumidityContainer;
    static ajn::services::CurrentTempStringProperty* currentTempStringProperty;
    static ajn::services::CurrentHumidityStringProperty* currentHumidityStringProperty;
    static ajn::services::Container* controlsContainer;
    static ajn::services::Ac_mode* ac_mode;
    static ajn::services::StatusStringProperty* statusStringProperty;
    static ajn::services::Set_temperature* set_temperature;
    static ajn::services::Fan_speed* fan_speed;

};

#endif /* CONTROLPANELGENERATED_H_ */
