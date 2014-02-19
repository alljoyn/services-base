/******************************************************************************
 * Copyright (c) 2013 - 2014, AllSeen Alliance. All rights reserved.
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

/**
 * Per-module definition of the current module for debug logging.  Must be defined
 * prior to first inclusion of aj_debug.h.
 * The corresponding flag dbgAJSVCAPP is defined in the containing sample app.
 */
#define AJ_MODULE AJSVCAPP
#include <aj_debug.h>

#include <alljoyn/controlpanel/Common/ConstraintList.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelProvided.h"

#ifndef NDEBUG
extern AJ_EXPORT uint8_t dbgAJSVCAPP;
#endif

const char MyDeviceRootContainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer";
const char enMyDeviceRootContainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en";
const char enMyDeviceTempAndHumidityContainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/tempAndHumidityContainer";
const char enMyDeviceCurrentTempStringPropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/tempAndHumidityContainer/CurrentTempStringProperty";
const char enMyDeviceCurrentHumidityStringPropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/tempAndHumidityContainer/CurrentHumidityStringProperty";
const char enMyDeviceControlsContainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/controlsContainer";
const char enMyDeviceAc_modeObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/controlsContainer/ac_mode";
const char enMyDeviceStatusStringPropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/controlsContainer/statusStringProperty";
const char enMyDeviceSet_temperatureObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/set_temperature";
const char enMyDeviceFan_speedObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/fan_speed";

ContainerWidget MyDeviceRootContainer;
ContainerWidget MyDeviceTempAndHumidityContainer;
PropertyWidget MyDeviceCurrentTempStringProperty;
PropertyWidget MyDeviceCurrentHumidityStringProperty;
ContainerWidget MyDeviceControlsContainer;
PropertyWidget MyDeviceAc_mode;
PropertyWidget MyDeviceStatusStringProperty;
PropertyWidget MyDeviceSet_temperature;
PropertyWidget MyDeviceFan_speed;

/*
 * Static variables used to fill widgets
 */
static const uint16_t MyDeviceRootContainerHints[] = { LAYOUT_HINT_VERTICAL_LINEAR, LAYOUT_HINT_HORIZONTAL_LINEAR };
static const uint16_t MyDeviceTempAndHumidityContainerHints[] = { LAYOUT_HINT_VERTICAL_LINEAR };
static const char* const MyDeviceCurrentTempStringPropertySignature = "s";
static const char* const MyDeviceCurrentTempStringPropertyLabel[] = { "Current Temperature:" };
static const uint16_t MyDeviceCurrentTempStringPropertyHints[] = { PROPERTY_WIDGET_HINT_TEXTVIEW };
static const char* const MyDeviceCurrentHumidityStringPropertySignature = "s";
static const char* const MyDeviceCurrentHumidityStringPropertyLabel[] = { "Current Humidity:" };
static const uint16_t MyDeviceCurrentHumidityStringPropertyHints[] = { PROPERTY_WIDGET_HINT_TEXTVIEW };
static const uint16_t MyDeviceControlsContainerHints[] = { LAYOUT_HINT_HORIZONTAL_LINEAR };
static const char* const MyDeviceAc_modeSignature = "q";
static const char* const MyDeviceAc_modeLabel[] = { "Mode" };
static const uint16_t MyDeviceAc_modeHints[] = { PROPERTY_WIDGET_HINT_SPINNER };
static ConstraintList MyDeviceAc_modeConstraintList[5];
static const uint16_t MyDeviceAc_modeConstraintValue0 = 0;
static const char* const MyDeviceAc_modeDisplay0[] = { "Auto" };
static const uint16_t MyDeviceAc_modeConstraintValue1 = 1;
static const char* const MyDeviceAc_modeDisplay1[] = { "Cool" };
static const uint16_t MyDeviceAc_modeConstraintValue2 = 2;
static const char* const MyDeviceAc_modeDisplay2[] = { "Heat" };
static const uint16_t MyDeviceAc_modeConstraintValue3 = 3;
static const char* const MyDeviceAc_modeDisplay3[] = { "Fan" };
static const uint16_t MyDeviceAc_modeConstraintValue4 = 4;
static const char* const MyDeviceAc_modeDisplay4[] = { "Off" };
static const char* const MyDeviceStatusStringPropertySignature = "s";
static const char* const MyDeviceStatusStringPropertyLabel[] = { "Status:" };
static const uint16_t MyDeviceStatusStringPropertyHints[] = { PROPERTY_WIDGET_HINT_TEXTVIEW };
static const char* const MyDeviceSet_temperatureSignature = "q";
static const char* const MyDeviceSet_temperatureLabel[] = { "Temperature" };
static const uint16_t MyDeviceSet_temperatureHints[] = { PROPERTY_WIDGET_HINT_SLIDER };
static const char* const MyDeviceSet_temperatureUnitMeasure[] = { "Degrees" };
static const uint16_t MyDeviceSet_temperatureConstraintRangeMin = 50;
static const uint16_t MyDeviceSet_temperatureConstraintRangeMax = 90;
static const uint16_t MyDeviceSet_temperatureConstraintRangeInc = 1;
static const char* const MyDeviceFan_speedSignature = "q";
static const char* const MyDeviceFan_speedLabel[] = { "Fan Speed" };
static const uint16_t MyDeviceFan_speedHints[] = { PROPERTY_WIDGET_HINT_SPINNER };
static ConstraintList MyDeviceFan_speedConstraintList[3];
static const uint16_t MyDeviceFan_speedConstraintValue0 = 0;
static const char* const MyDeviceFan_speedDisplay0[] = { "Low" };
static const uint16_t MyDeviceFan_speedConstraintValue1 = 1;
static const char* const MyDeviceFan_speedDisplay1[] = { "Medium" };
static const uint16_t MyDeviceFan_speedConstraintValue2 = 2;
static const char* const MyDeviceFan_speedDisplay2[] = { "High" };


void WidgetsInit()
{
    initializeContainerWidget(&MyDeviceRootContainer);
    MyDeviceRootContainer.base.numLanguages = 1;
    setBaseEnabled(&MyDeviceRootContainer.base, TRUE);

    MyDeviceRootContainer.base.optParams.bgColor = 0x1e90ff;
    MyDeviceRootContainer.base.optParams.hints = MyDeviceRootContainerHints;
    MyDeviceRootContainer.base.optParams.numHints = 2;

    initializeContainerWidget(&MyDeviceTempAndHumidityContainer);
    MyDeviceTempAndHumidityContainer.base.numLanguages = 1;
    setBaseEnabled(&MyDeviceTempAndHumidityContainer.base, TRUE);

    MyDeviceTempAndHumidityContainer.base.optParams.bgColor = 0x200;
    MyDeviceTempAndHumidityContainer.base.optParams.hints = MyDeviceTempAndHumidityContainerHints;
    MyDeviceTempAndHumidityContainer.base.optParams.numHints = 1;

    initializePropertyWidget(&MyDeviceCurrentTempStringProperty);
    MyDeviceCurrentTempStringProperty.base.numLanguages = 1;
    setBaseEnabled(&MyDeviceCurrentTempStringProperty.base, TRUE);
    setBaseWritable(&MyDeviceCurrentTempStringProperty.base, FALSE);

    MyDeviceCurrentTempStringProperty.signature = MyDeviceCurrentTempStringPropertySignature;
    MyDeviceCurrentTempStringProperty.propertyType = SINGLE_VALUE_PROPERTY;
    MyDeviceCurrentTempStringProperty.getValue = &getCurrentTemperatureString;

    MyDeviceCurrentTempStringProperty.base.optParams.bgColor = 0x500;
    MyDeviceCurrentTempStringProperty.base.optParams.label = MyDeviceCurrentTempStringPropertyLabel;
    MyDeviceCurrentTempStringProperty.base.optParams.hints = MyDeviceCurrentTempStringPropertyHints;
    MyDeviceCurrentTempStringProperty.base.optParams.numHints = 1;

    initializePropertyWidget(&MyDeviceCurrentHumidityStringProperty);
    MyDeviceCurrentHumidityStringProperty.base.numLanguages = 1;
    setBaseEnabled(&MyDeviceCurrentHumidityStringProperty.base, TRUE);
    setBaseWritable(&MyDeviceCurrentHumidityStringProperty.base, FALSE);

    MyDeviceCurrentHumidityStringProperty.signature = MyDeviceCurrentHumidityStringPropertySignature;
    MyDeviceCurrentHumidityStringProperty.propertyType = SINGLE_VALUE_PROPERTY;
    MyDeviceCurrentHumidityStringProperty.getValue = &getCurrentHumidityString;

    MyDeviceCurrentHumidityStringProperty.base.optParams.bgColor = 0x500;
    MyDeviceCurrentHumidityStringProperty.base.optParams.label = MyDeviceCurrentHumidityStringPropertyLabel;
    MyDeviceCurrentHumidityStringProperty.base.optParams.hints = MyDeviceCurrentHumidityStringPropertyHints;
    MyDeviceCurrentHumidityStringProperty.base.optParams.numHints = 1;

    initializeContainerWidget(&MyDeviceControlsContainer);
    MyDeviceControlsContainer.base.numLanguages = 1;
    setBaseEnabled(&MyDeviceControlsContainer.base, TRUE);

    MyDeviceControlsContainer.base.optParams.bgColor = 0x200;
    MyDeviceControlsContainer.base.optParams.hints = MyDeviceControlsContainerHints;
    MyDeviceControlsContainer.base.optParams.numHints = 1;

    initializePropertyWidget(&MyDeviceAc_mode);
    MyDeviceAc_mode.base.numLanguages = 1;
    setBaseEnabled(&MyDeviceAc_mode.base, TRUE);
    setBaseWritable(&MyDeviceAc_mode.base, TRUE);

    MyDeviceAc_mode.signature = MyDeviceAc_modeSignature;
    MyDeviceAc_mode.propertyType = SINGLE_VALUE_PROPERTY;
    MyDeviceAc_mode.getValue = &getCurrentMode;

    MyDeviceAc_mode.base.optParams.bgColor = 0xffd700;
    MyDeviceAc_mode.base.optParams.label = MyDeviceAc_modeLabel;
    MyDeviceAc_mode.base.optParams.hints = MyDeviceAc_modeHints;
    MyDeviceAc_mode.base.optParams.numHints = 1;

    MyDeviceAc_mode.optParams.numConstraints = 5;
    MyDeviceAc_mode.optParams.constraintList = MyDeviceAc_modeConstraintList;
    MyDeviceAc_mode.optParams.constraintList[0].value = &MyDeviceAc_modeConstraintValue0;
    MyDeviceAc_mode.optParams.constraintList[0].display = MyDeviceAc_modeDisplay0;
    MyDeviceAc_mode.optParams.constraintList[1].value = &MyDeviceAc_modeConstraintValue1;
    MyDeviceAc_mode.optParams.constraintList[1].display = MyDeviceAc_modeDisplay1;
    MyDeviceAc_mode.optParams.constraintList[2].value = &MyDeviceAc_modeConstraintValue2;
    MyDeviceAc_mode.optParams.constraintList[2].display = MyDeviceAc_modeDisplay2;
    MyDeviceAc_mode.optParams.constraintList[3].value = &MyDeviceAc_modeConstraintValue3;
    MyDeviceAc_mode.optParams.constraintList[3].display = MyDeviceAc_modeDisplay3;
    MyDeviceAc_mode.optParams.constraintList[4].value = &MyDeviceAc_modeConstraintValue4;
    MyDeviceAc_mode.optParams.constraintList[4].display = MyDeviceAc_modeDisplay4;

    initializePropertyWidget(&MyDeviceStatusStringProperty);
    MyDeviceStatusStringProperty.base.numLanguages = 1;
    setBaseEnabled(&MyDeviceStatusStringProperty.base, TRUE);
    setBaseWritable(&MyDeviceStatusStringProperty.base, FALSE);

    MyDeviceStatusStringProperty.signature = MyDeviceStatusStringPropertySignature;
    MyDeviceStatusStringProperty.propertyType = SINGLE_VALUE_PROPERTY;
    MyDeviceStatusStringProperty.getValue = &getStatusString;

    MyDeviceStatusStringProperty.base.optParams.bgColor = 0x500;
    MyDeviceStatusStringProperty.base.optParams.label = MyDeviceStatusStringPropertyLabel;
    MyDeviceStatusStringProperty.base.optParams.hints = MyDeviceStatusStringPropertyHints;
    MyDeviceStatusStringProperty.base.optParams.numHints = 1;

    initializePropertyWidget(&MyDeviceSet_temperature);
    MyDeviceSet_temperature.base.numLanguages = 1;
    setBaseEnabled(&MyDeviceSet_temperature.base, FALSE);
    setBaseWritable(&MyDeviceSet_temperature.base, TRUE);

    MyDeviceSet_temperature.signature = MyDeviceSet_temperatureSignature;
    MyDeviceSet_temperature.propertyType = SINGLE_VALUE_PROPERTY;
    MyDeviceSet_temperature.getValue = &getTargetTemperature;

    MyDeviceSet_temperature.base.optParams.bgColor = 0x008000;
    MyDeviceSet_temperature.base.optParams.label = MyDeviceSet_temperatureLabel;
    MyDeviceSet_temperature.base.optParams.hints = MyDeviceSet_temperatureHints;
    MyDeviceSet_temperature.base.optParams.numHints = 1;
    MyDeviceSet_temperature.optParams.unitOfMeasure = MyDeviceSet_temperatureUnitMeasure;

    MyDeviceSet_temperature.optParams.constraintRangeDefined = TRUE;
    MyDeviceSet_temperature.optParams.constraintRange.minValue = &MyDeviceSet_temperatureConstraintRangeMin;
    MyDeviceSet_temperature.optParams.constraintRange.maxValue = &MyDeviceSet_temperatureConstraintRangeMax;
    MyDeviceSet_temperature.optParams.constraintRange.increment = &MyDeviceSet_temperatureConstraintRangeInc;

    initializePropertyWidget(&MyDeviceFan_speed);
    MyDeviceFan_speed.base.numLanguages = 1;
    setBaseEnabled(&MyDeviceFan_speed.base, FALSE);
    setBaseWritable(&MyDeviceFan_speed.base, TRUE);

    MyDeviceFan_speed.signature = MyDeviceFan_speedSignature;
    MyDeviceFan_speed.propertyType = SINGLE_VALUE_PROPERTY;
    MyDeviceFan_speed.getValue = &getFanSpeed;

    MyDeviceFan_speed.base.optParams.bgColor = 0xff69b4;
    MyDeviceFan_speed.base.optParams.label = MyDeviceFan_speedLabel;
    MyDeviceFan_speed.base.optParams.hints = MyDeviceFan_speedHints;
    MyDeviceFan_speed.base.optParams.numHints = 1;

    MyDeviceFan_speed.optParams.numConstraints = 3;
    MyDeviceFan_speed.optParams.constraintList = MyDeviceFan_speedConstraintList;
    MyDeviceFan_speed.optParams.constraintList[0].value = &MyDeviceFan_speedConstraintValue0;
    MyDeviceFan_speed.optParams.constraintList[0].display = MyDeviceFan_speedDisplay0;
    MyDeviceFan_speed.optParams.constraintList[1].value = &MyDeviceFan_speedConstraintValue1;
    MyDeviceFan_speed.optParams.constraintList[1].display = MyDeviceFan_speedDisplay1;
    MyDeviceFan_speed.optParams.constraintList[2].value = &MyDeviceFan_speedConstraintValue2;
    MyDeviceFan_speed.optParams.constraintList[2].display = MyDeviceFan_speedDisplay2;

    return;
}

void* IdentifyMsgOrPropId(uint32_t identifier, uint16_t* widgetType, uint16_t* propType, uint16_t* language)
{
    switch (identifier) {
    case EN_MYDEVICE_ROOTCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceRootContainer;

    case EN_MYDEVICE_ROOTCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceRootContainer;

    case EN_MYDEVICE_ROOTCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceRootContainer;

    case EN_MYDEVICE_ROOTCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceRootContainer;

    case EN_MYDEVICE_TEMPANDHUMIDITYCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceTempAndHumidityContainer;

    case EN_MYDEVICE_TEMPANDHUMIDITYCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceTempAndHumidityContainer;

    case EN_MYDEVICE_TEMPANDHUMIDITYCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceTempAndHumidityContainer;

    case EN_MYDEVICE_TEMPANDHUMIDITYCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceTempAndHumidityContainer;

    case EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTempStringProperty;

    case EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTempStringProperty;

    case EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTempStringProperty;

    case EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTempStringProperty;

    case EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTempStringProperty;

    case EN_MYDEVICE_CURRENTHUMIDITYSTRINGPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentHumidityStringProperty;

    case EN_MYDEVICE_CURRENTHUMIDITYSTRINGPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentHumidityStringProperty;

    case EN_MYDEVICE_CURRENTHUMIDITYSTRINGPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentHumidityStringProperty;

    case EN_MYDEVICE_CURRENTHUMIDITYSTRINGPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentHumidityStringProperty;

    case EN_MYDEVICE_CURRENTHUMIDITYSTRINGPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentHumidityStringProperty;

    case EN_MYDEVICE_CONTROLSCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceControlsContainer;

    case EN_MYDEVICE_CONTROLSCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceControlsContainer;

    case EN_MYDEVICE_CONTROLSCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceControlsContainer;

    case EN_MYDEVICE_CONTROLSCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceControlsContainer;

    case EN_MYDEVICE_AC_MODE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceAc_mode;

    case EN_MYDEVICE_AC_MODE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceAc_mode;

    case EN_MYDEVICE_AC_MODE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceAc_mode;

    case EN_MYDEVICE_AC_MODE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceAc_mode;

    case EN_MYDEVICE_AC_MODE_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceAc_mode;

    case EN_MYDEVICE_STATUSSTRINGPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceStatusStringProperty;

    case EN_MYDEVICE_STATUSSTRINGPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceStatusStringProperty;

    case EN_MYDEVICE_STATUSSTRINGPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceStatusStringProperty;

    case EN_MYDEVICE_STATUSSTRINGPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceStatusStringProperty;

    case EN_MYDEVICE_STATUSSTRINGPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceStatusStringProperty;

    case EN_MYDEVICE_SET_TEMPERATURE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceSet_temperature;

    case EN_MYDEVICE_SET_TEMPERATURE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceSet_temperature;

    case EN_MYDEVICE_SET_TEMPERATURE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceSet_temperature;

    case EN_MYDEVICE_SET_TEMPERATURE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceSet_temperature;

    case EN_MYDEVICE_SET_TEMPERATURE_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceSet_temperature;

    case EN_MYDEVICE_FAN_SPEED_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceFan_speed;

    case EN_MYDEVICE_FAN_SPEED_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceFan_speed;

    case EN_MYDEVICE_FAN_SPEED_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceFan_speed;

    case EN_MYDEVICE_FAN_SPEED_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceFan_speed;

    case EN_MYDEVICE_FAN_SPEED_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceFan_speed;

    default:
        return FALSE;
    }
}

void* IdentifyMsgOrPropIdForSignal(uint32_t identifier, uint8_t* isProperty)
{
    switch (identifier) {
    case EN_MYDEVICE_ROOTCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceRootContainer;

    case EN_MYDEVICE_TEMPANDHUMIDITYCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceTempAndHumidityContainer;

    case EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceCurrentTempStringProperty;

    case EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &MyDeviceCurrentTempStringProperty;

    case EN_MYDEVICE_CURRENTHUMIDITYSTRINGPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceCurrentHumidityStringProperty;

    case EN_MYDEVICE_CURRENTHUMIDITYSTRINGPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &MyDeviceCurrentHumidityStringProperty;

    case EN_MYDEVICE_CONTROLSCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceControlsContainer;

    case EN_MYDEVICE_AC_MODE_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceAc_mode;

    case EN_MYDEVICE_AC_MODE_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &MyDeviceAc_mode;

    case EN_MYDEVICE_STATUSSTRINGPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceStatusStringProperty;

    case EN_MYDEVICE_STATUSSTRINGPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &MyDeviceStatusStringProperty;

    case EN_MYDEVICE_SET_TEMPERATURE_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceSet_temperature;

    case EN_MYDEVICE_SET_TEMPERATURE_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &MyDeviceSet_temperature;

    case EN_MYDEVICE_FAN_SPEED_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceFan_speed;

    case EN_MYDEVICE_FAN_SPEED_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &MyDeviceFan_speed;

    default:
        return FALSE;
    }
}

uint8_t IdentifyRootMsgOrPropId(uint32_t identifier)
{
    switch (identifier) {
    case MYDEVICE_ROOT_CONTROLPANEL_ROOTCONTAINER_VERSION_PROPERTY:
    case MYDEVICE_ROOT_CONTROLPANEL_ROOTCONTAINER_GET_ALL_VALUES:
        return TRUE;

    default:
        return FALSE;
    }
}

AJ_Status SetValueProperty(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    const char* variantSig;
    AJ_UnmarshalVariant(replyMsg, &variantSig);

    switch (propId) {
    case EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_VALUE_PROPERTY:
        {
            const char* newValue;
            status = unmarshalPropertyValue(&MyDeviceCurrentTempStringProperty, replyMsg, &newValue, ((SetValueContext*)context)->sender);
            if (status != AJ_OK) {
                return status;
            }
            setCurrentTemperatureString(newValue);
            ((SetValueContext*)context)->numSignals = 1;
            ((SetValueContext*)context)->signals[0] = EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_SIGNAL_VALUE_CHANGED;
        }
        break;

    case EN_MYDEVICE_CURRENTHUMIDITYSTRINGPROPERTY_VALUE_PROPERTY:
        {
            const char* newValue;
            status = unmarshalPropertyValue(&MyDeviceCurrentHumidityStringProperty, replyMsg, &newValue, ((SetValueContext*)context)->sender);
            if (status != AJ_OK) {
                return status;
            }
            setCurrentHumidityString(newValue);
            ((SetValueContext*)context)->numSignals = 1;
            ((SetValueContext*)context)->signals[0] = EN_MYDEVICE_CURRENTHUMIDITYSTRINGPROPERTY_SIGNAL_VALUE_CHANGED;
        }
        break;

    case EN_MYDEVICE_AC_MODE_VALUE_PROPERTY:
        {
            uint16_t newValue;
            status = unmarshalPropertyValue(&MyDeviceAc_mode, replyMsg, &newValue, ((SetValueContext*)context)->sender);
            if (status != AJ_OK) {
                return status;
            }
            setCurrentMode(newValue);
            ((SetValueContext*)context)->numSignals = 1;
            ((SetValueContext*)context)->signals[0] = EN_MYDEVICE_AC_MODE_SIGNAL_VALUE_CHANGED;
        }
        break;

    case EN_MYDEVICE_STATUSSTRINGPROPERTY_VALUE_PROPERTY:
        {
            const char* newValue;
            status = unmarshalPropertyValue(&MyDeviceStatusStringProperty, replyMsg, &newValue, ((SetValueContext*)context)->sender);
            if (status != AJ_OK) {
                return status;
            }
            setStatusString(newValue);
            ((SetValueContext*)context)->numSignals = 1;
            ((SetValueContext*)context)->signals[0] = EN_MYDEVICE_STATUSSTRINGPROPERTY_SIGNAL_VALUE_CHANGED;
        }
        break;

    case EN_MYDEVICE_SET_TEMPERATURE_VALUE_PROPERTY:
        {
            uint16_t newValue;
            status = unmarshalPropertyValue(&MyDeviceSet_temperature, replyMsg, &newValue, ((SetValueContext*)context)->sender);
            if (status != AJ_OK) {
                return status;
            }
            setTargetTemperature(newValue);
            ((SetValueContext*)context)->numSignals = 1;
            ((SetValueContext*)context)->signals[0] = EN_MYDEVICE_SET_TEMPERATURE_SIGNAL_VALUE_CHANGED;
        }
        break;

    case EN_MYDEVICE_FAN_SPEED_VALUE_PROPERTY:
        {
            uint16_t newValue;
            status = unmarshalPropertyValue(&MyDeviceFan_speed, replyMsg, &newValue, ((SetValueContext*)context)->sender);
            if (status != AJ_OK) {
                return status;
            }
            setFanSpeed(newValue);
            ((SetValueContext*)context)->numSignals = 1;
            ((SetValueContext*)context)->signals[0] = EN_MYDEVICE_FAN_SPEED_SIGNAL_VALUE_CHANGED;
        }
        break;


    }

    return status;
}

AJ_Status ExecuteAction(AJ_Message* msg, uint32_t msgId, ExecuteActionContext* context)
{
    AJ_Message reply;
    AJ_MarshalReplyMsg(msg, &reply);

    switch (msgId) {

    }

    return AJ_DeliverMsg(&reply);
}

void TestsInit(CPSTest* testsToRun)
{

}

AJSVC_ServiceStatus GeneratedMessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status* msgStatus)
{
    AJSVC_ServiceStatus AJSVC_ServiceStatus = AJSVC_SERVICE_STATUS_HANDLED;

    switch (msg->msgId) {
        GET_WIDGET_VALUE_CASES
        *msgStatus = AJ_BusPropGet(msg, AJCPS_GetWidgetProperty, NULL);
        break;

        GET_WIDGET_ALL_VALUE_CASES
        *msgStatus = AJCPS_GetAllWidgetProperties(msg, msg->msgId);
        break;

        GET_ROOT_VALUE_CASES
        *msgStatus = AJ_BusPropGet(msg, AJCPS_GetRootProperty, NULL);
        break;

        GET_ROOT_ALL_VALUE_CASES
        *msgStatus = AJCPS_GetAllRootProperties(msg, msg->msgId);
        break;

        SET_VALUE_CASES
        {
            SetValueContext context;
            context.sender = msg->sender;
            context.numSignals = 0;
            *msgStatus = AJ_BusPropSet(msg, SetValueProperty, &context);
            if (*msgStatus == AJ_OK && context.numSignals != 0) {
                uint16_t indx;
                for (indx = 0; indx < context.numSignals; indx++) {
                    *msgStatus = AJCPS_SendPropertyChangedSignal(bus, context.signals[indx], AJCPS_GetCurrentSessionId());
                }
            }
        }
        break;

        ACTION_CASES
        {
            ExecuteActionContext context;
            context.numSignals = 0;
            *msgStatus = ExecuteAction(msg, msg->msgId, &context);
            if (*msgStatus == AJ_OK && context.numSignals != 0) {
                uint16_t indx;
                for (indx = 0; indx < context.numSignals; indx++) {
                    if (context.signals[indx].signalType == SIGNAL_TYPE_DATA_CHANGED) {
                        *msgStatus = AJCPS_SendPropertyChangedSignal(bus, context.signals[indx].signalId, AJCPS_GetCurrentSessionId());
                    } else if (context.signals[indx].signalType == SIGNAL_TYPE_DISMISS) {
                        *msgStatus = AJCPS_SendDismissSignal(bus, context.signals[indx].signalId, AJCPS_GetCurrentSessionId());
                    }
                }
            }
        }
        break;

        GET_URL_CASES
        *msgStatus = AJCPS_SendRootUrl(msg, msg->msgId);
        break;

    case AJ_SIGNAL_SESSION_LOST:
        break;

    default:
        AJSVC_ServiceStatus = AJSVC_SERVICE_STATUS_NOT_HANDLED;
        break;
    }

    return AJSVC_ServiceStatus;
}
