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

#include "ControlPanelGenerated.h"
#include <alljoyn/controlpanel/LanguageSets.h>
#include <alljoyn/controlpanel/Label.h>
#include "ControlPanelProvided.h"


using namespace ajn;
using namespace services;

bool ControlPanelGenerated::languageSetsDone = false;
ControlPanel* ControlPanelGenerated::rootContainerControlPanel = 0;
Container* ControlPanelGenerated::rootContainer = 0;
Container* ControlPanelGenerated::tempAndHumidityContainer = 0;
CurrentTempStringProperty* ControlPanelGenerated::currentTempStringProperty = 0;
CurrentHumidityStringProperty* ControlPanelGenerated::currentHumidityStringProperty = 0;
Container* ControlPanelGenerated::controlsContainer = 0;
Ac_mode* ControlPanelGenerated::ac_mode = 0;
StatusStringProperty* ControlPanelGenerated::statusStringProperty = 0;
Set_temperature* ControlPanelGenerated::set_temperature = 0;
Fan_speed* ControlPanelGenerated::fan_speed = 0;


#define CHECK(x) if ((status = x) != ER_OK) return status;
#define UNIT_NAME "MyDevice"

void ControlPanelGenerated::PrepareLanguageSets()
{
    if (languageSetsDone)
        return;

    LanguageSet myLanguages("myLanguages");
    myLanguages.addLanguage("en");
    LanguageSets::add(myLanguages.getLanguageSetName(), myLanguages);

    languageSetsDone = true;
}

QStatus ControlPanelGenerated::PrepareWidgets(ControlPanelControllee*& controlPanelControllee)
{
    if (controlPanelControllee)
        return ER_BAD_ARG_1;

    PrepareLanguageSets();

    QStatus status = ER_OK;
    controlPanelControllee = new ControlPanelControllee(UNIT_NAME);

    rootContainerControlPanel = ControlPanel::createControlPanel(LanguageSets::get("myLanguages"));
    if (!rootContainerControlPanel)
        return ER_FAIL;
    CHECK(controlPanelControllee->addControlPanel(rootContainerControlPanel));

    rootContainer = new Container("rootContainer");
    CHECK(rootContainerControlPanel->setRootWidget(rootContainer));

    rootContainer->setEnabled(true);
    rootContainer->setIsSecured(false);
    rootContainer->setBgColor(0x1e90ff);

    std::vector<uint16_t> rootContainerHintsVec;
    rootContainerHintsVec.push_back(VERTICAL_LINEAR);
    rootContainerHintsVec.push_back(HORIZONTAL_LINEAR);
    rootContainer->setHints(rootContainerHintsVec);

    tempAndHumidityContainer = new Container("tempAndHumidityContainer");
    CHECK(rootContainer->addChildWidget(tempAndHumidityContainer));

    tempAndHumidityContainer->setEnabled(true);
    tempAndHumidityContainer->setIsSecured(false);
    tempAndHumidityContainer->setBgColor(0x200);

    std::vector<uint16_t> tempAndHumidityContainerHintsVec;
    tempAndHumidityContainerHintsVec.push_back(VERTICAL_LINEAR);
    tempAndHumidityContainer->setHints(tempAndHumidityContainerHintsVec);

    currentTempStringProperty = new CurrentTempStringProperty("currentTempStringProperty", STRING_PROPERTY);
    CHECK(tempAndHumidityContainer->addChildWidget(currentTempStringProperty));

    currentTempStringProperty->setEnabled(true);
    currentTempStringProperty->setIsSecured(false);
    currentTempStringProperty->setWritable(false);
    CHECK(currentTempStringProperty->setGetValue(getCurrentTemperatureString));
    currentTempStringProperty->setBgColor(0x500);

    std::vector<qcc::String> currentTempStringPropertylabelVec;
    currentTempStringPropertylabelVec.push_back("Current Temperature:");
    currentTempStringProperty->setLabels(currentTempStringPropertylabelVec);

    std::vector<uint16_t> currentTempStringPropertyHintsVec;
    currentTempStringPropertyHintsVec.push_back(TEXTVIEW);
    currentTempStringProperty->setHints(currentTempStringPropertyHintsVec);

    currentHumidityStringProperty = new CurrentHumidityStringProperty("currentHumidityStringProperty", STRING_PROPERTY);
    CHECK(tempAndHumidityContainer->addChildWidget(currentHumidityStringProperty));

    currentHumidityStringProperty->setEnabled(true);
    currentHumidityStringProperty->setIsSecured(false);
    currentHumidityStringProperty->setWritable(false);
    CHECK(currentHumidityStringProperty->setGetValue(getCurrentHumidityString));
    currentHumidityStringProperty->setBgColor(0x500);

    std::vector<qcc::String> currentHumidityStringPropertylabelVec;
    currentHumidityStringPropertylabelVec.push_back("Current Humidity:");
    currentHumidityStringProperty->setLabels(currentHumidityStringPropertylabelVec);

    std::vector<uint16_t> currentHumidityStringPropertyHintsVec;
    currentHumidityStringPropertyHintsVec.push_back(TEXTVIEW);
    currentHumidityStringProperty->setHints(currentHumidityStringPropertyHintsVec);

    controlsContainer = new Container("controlsContainer");
    CHECK(rootContainer->addChildWidget(controlsContainer));

    controlsContainer->setEnabled(true);
    controlsContainer->setIsSecured(false);
    controlsContainer->setBgColor(0x200);

    std::vector<uint16_t> controlsContainerHintsVec;
    controlsContainerHintsVec.push_back(HORIZONTAL_LINEAR);
    controlsContainer->setHints(controlsContainerHintsVec);

    ac_mode = new Ac_mode("ac_mode", UINT16_PROPERTY);
    CHECK(controlsContainer->addChildWidget(ac_mode));

    ac_mode->setEnabled(true);
    ac_mode->setIsSecured(false);
    ac_mode->setWritable(true);
    CHECK(ac_mode->setGetValue(getCurrentMode));
    ac_mode->setBgColor(0xffd700);

    std::vector<qcc::String> ac_modelabelVec;
    ac_modelabelVec.push_back("Mode");
    ac_mode->setLabels(ac_modelabelVec);

    std::vector<uint16_t> ac_modeHintsVec;
    ac_modeHintsVec.push_back(SPINNER);
    ac_mode->setHints(ac_modeHintsVec);

    std::vector<ConstraintList> ac_modeConstraintListVec(5);

    std::vector<qcc::String> ac_modeDisplay1Vec;
    ac_modeDisplay1Vec.push_back("Auto");
    ac_modeConstraintListVec[0].setDisplays(ac_modeDisplay1Vec);
    ac_modeConstraintListVec[0].setConstraintValue((uint16_t)0);

    std::vector<qcc::String> ac_modeDisplay2Vec;
    ac_modeDisplay2Vec.push_back("Cool");
    ac_modeConstraintListVec[1].setDisplays(ac_modeDisplay2Vec);
    ac_modeConstraintListVec[1].setConstraintValue((uint16_t)1);

    std::vector<qcc::String> ac_modeDisplay3Vec;
    ac_modeDisplay3Vec.push_back("Heat");
    ac_modeConstraintListVec[2].setDisplays(ac_modeDisplay3Vec);
    ac_modeConstraintListVec[2].setConstraintValue((uint16_t)2);

    std::vector<qcc::String> ac_modeDisplay4Vec;
    ac_modeDisplay4Vec.push_back("Fan");
    ac_modeConstraintListVec[3].setDisplays(ac_modeDisplay4Vec);
    ac_modeConstraintListVec[3].setConstraintValue((uint16_t)3);

    std::vector<qcc::String> ac_modeDisplay5Vec;
    ac_modeDisplay5Vec.push_back("Off");
    ac_modeConstraintListVec[4].setDisplays(ac_modeDisplay5Vec);
    ac_modeConstraintListVec[4].setConstraintValue((uint16_t)4);

    ac_mode->setConstraintList(ac_modeConstraintListVec);

    statusStringProperty = new StatusStringProperty("statusStringProperty", STRING_PROPERTY);
    CHECK(controlsContainer->addChildWidget(statusStringProperty));

    statusStringProperty->setEnabled(true);
    statusStringProperty->setIsSecured(false);
    statusStringProperty->setWritable(false);
    CHECK(statusStringProperty->setGetValue(getStatusString));
    statusStringProperty->setBgColor(0x500);

    std::vector<qcc::String> statusStringPropertylabelVec;
    statusStringPropertylabelVec.push_back("Status:");
    statusStringProperty->setLabels(statusStringPropertylabelVec);

    std::vector<uint16_t> statusStringPropertyHintsVec;
    statusStringPropertyHintsVec.push_back(TEXTVIEW);
    statusStringProperty->setHints(statusStringPropertyHintsVec);

    set_temperature = new Set_temperature("set_temperature", UINT16_PROPERTY);
    CHECK(rootContainer->addChildWidget(set_temperature));

    set_temperature->setEnabled(false);
    set_temperature->setIsSecured(false);
    set_temperature->setWritable(true);
    CHECK(set_temperature->setGetValue(getTargetTemperature));
    set_temperature->setBgColor(0x008000);

    std::vector<qcc::String> set_temperaturelabelVec;
    set_temperaturelabelVec.push_back("Temperature");
    set_temperature->setLabels(set_temperaturelabelVec);

    std::vector<uint16_t> set_temperatureHintsVec;
    set_temperatureHintsVec.push_back(SLIDER);
    set_temperature->setHints(set_temperatureHintsVec);

    std::vector<qcc::String> set_temperatureunitMeasureVec;
    set_temperatureunitMeasureVec.push_back("Degrees");
    set_temperature->setUnitOfMeasures(set_temperatureunitMeasureVec);

    ConstraintRange* set_temperatureConstraintRange = new ConstraintRange();
    CHECK(set_temperatureConstraintRange->setConstraintMin((uint16_t)50));
    CHECK(set_temperatureConstraintRange->setConstraintMax((uint16_t)90));
    CHECK(set_temperatureConstraintRange->setConstraintIncrement((uint16_t)1));
    set_temperature->setConstraintRange(set_temperatureConstraintRange);

    fan_speed = new Fan_speed("fan_speed", UINT16_PROPERTY);
    CHECK(rootContainer->addChildWidget(fan_speed));

    fan_speed->setEnabled(false);
    fan_speed->setIsSecured(false);
    fan_speed->setWritable(true);
    CHECK(fan_speed->setGetValue(getFanSpeed));
    fan_speed->setBgColor(0xff69b4);

    std::vector<qcc::String> fan_speedlabelVec;
    fan_speedlabelVec.push_back("Fan Speed");
    fan_speed->setLabels(fan_speedlabelVec);

    std::vector<uint16_t> fan_speedHintsVec;
    fan_speedHintsVec.push_back(SPINNER);
    fan_speed->setHints(fan_speedHintsVec);

    std::vector<ConstraintList> fan_speedConstraintListVec(3);

    std::vector<qcc::String> fan_speedDisplay1Vec;
    fan_speedDisplay1Vec.push_back("Low");
    fan_speedConstraintListVec[0].setDisplays(fan_speedDisplay1Vec);
    fan_speedConstraintListVec[0].setConstraintValue((uint16_t)0);

    std::vector<qcc::String> fan_speedDisplay2Vec;
    fan_speedDisplay2Vec.push_back("Medium");
    fan_speedConstraintListVec[1].setDisplays(fan_speedDisplay2Vec);
    fan_speedConstraintListVec[1].setConstraintValue((uint16_t)1);

    std::vector<qcc::String> fan_speedDisplay3Vec;
    fan_speedDisplay3Vec.push_back("High");
    fan_speedConstraintListVec[2].setDisplays(fan_speedDisplay3Vec);
    fan_speedConstraintListVec[2].setConstraintValue((uint16_t)2);

    fan_speed->setConstraintList(fan_speedConstraintListVec);

    return status;
}

void ControlPanelGenerated::Shutdown()
{
    if (rootContainerControlPanel) {
        delete (rootContainerControlPanel);
        rootContainerControlPanel = 0;
    }
    if (rootContainer) {
        delete (rootContainer);
        rootContainer = 0;
    }
    if (tempAndHumidityContainer) {
        delete (tempAndHumidityContainer);
        tempAndHumidityContainer = 0;
    }
    if (currentTempStringProperty) {
        delete (currentTempStringProperty);
        currentTempStringProperty = 0;
    }
    if (currentHumidityStringProperty) {
        delete (currentHumidityStringProperty);
        currentHumidityStringProperty = 0;
    }
    if (controlsContainer) {
        delete (controlsContainer);
        controlsContainer = 0;
    }
    if (ac_mode) {
        delete (ac_mode);
        ac_mode = 0;
    }
    if (statusStringProperty) {
        delete (statusStringProperty);
        statusStringProperty = 0;
    }
    if (set_temperature) {
        delete (set_temperature);
        set_temperature = 0;
    }
    if (fan_speed) {
        delete (fan_speed);
        fan_speed = 0;
    }

}
