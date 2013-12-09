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

#include <alljoyn/controlpanel/Common/ConstraintList.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelProvided.h"

const char rootContainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer";
const char enRootcontainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en";
const char enTempandhumiditycontainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/tempAndHumidityContainer";
const char enCurrenttempstringpropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/tempAndHumidityContainer/CurrentTempStringProperty";
const char enCurrenthumiditystringpropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/tempAndHumidityContainer/CurrentHumidityStringProperty";
const char enControlscontainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/controlsContainer";
const char enAc_modeObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/controlsContainer/ac_mode";
const char enStatusstringpropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/controlsContainer/statusStringProperty";
const char enSet_temperatureObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/set_temperature";
const char enFan_speedObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/fan_speed";

ContainerWidget rootContainer;
ContainerWidget tempAndHumidityContainer;
PropertyWidget CurrentTempStringProperty;
PropertyWidget CurrentHumidityStringProperty;
ContainerWidget controlsContainer;
PropertyWidget ac_mode;
PropertyWidget statusStringProperty;
PropertyWidget set_temperature;
PropertyWidget fan_speed;

/*
 * Static variables used to fill widgets
 */
static const uint16_t rootContainerHints[] = { LAYOUT_HINT_VERTICAL_LINEAR, LAYOUT_HINT_HORIZONTAL_LINEAR };
static const uint16_t tempAndHumidityContainerHints[] = { LAYOUT_HINT_VERTICAL_LINEAR };
static const char* const CurrentTempStringPropertySignature = "s";
static const char* const CurrentTempStringPropertyLabel[] = { "Current Temperature:" };
static const uint16_t CurrentTempStringPropertyHints[] = { PROPERTY_WIDGET_HINT_TEXTVIEW };
static const char* const CurrentHumidityStringPropertySignature = "s";
static const char* const CurrentHumidityStringPropertyLabel[] = { "Current Humidity:" };
static const uint16_t CurrentHumidityStringPropertyHints[] = { PROPERTY_WIDGET_HINT_TEXTVIEW };
static const uint16_t controlsContainerHints[] = { LAYOUT_HINT_HORIZONTAL_LINEAR };
static const char* const ac_modeSignature = "q";
static const char* const ac_modeLabel[] = { "Mode" };
static const uint16_t ac_modeHints[] = { PROPERTY_WIDGET_HINT_SPINNER };
static ConstraintList ac_modeConstraintList[5];
static const uint16_t ac_modeConstraintValue0 = 0;
static const char* const ac_modeDisplay0[] = { "Auto" };
static const uint16_t ac_modeConstraintValue1 = 1;
static const char* const ac_modeDisplay1[] = { "Cool" };
static const uint16_t ac_modeConstraintValue2 = 2;
static const char* const ac_modeDisplay2[] = { "Heat" };
static const uint16_t ac_modeConstraintValue3 = 3;
static const char* const ac_modeDisplay3[] = { "Fan" };
static const uint16_t ac_modeConstraintValue4 = 4;
static const char* const ac_modeDisplay4[] = { "Off" };
static const char* const statusStringPropertySignature = "s";
static const char* const statusStringPropertyLabel[] = { "Status:" };
static const uint16_t statusStringPropertyHints[] = { PROPERTY_WIDGET_HINT_TEXTVIEW };
static const char* const set_temperatureSignature = "q";
static const char* const set_temperatureLabel[] = { "Temperature" };
static const uint16_t set_temperatureHints[] = { PROPERTY_WIDGET_HINT_SLIDER };
static const char* const set_temperatureUnitMeasure[] = { "Degrees" };
static const uint16_t set_temperatureConstraintRangeMin = 50;
static const uint16_t set_temperatureConstraintRangeMax = 90;
static const uint16_t set_temperatureConstraintRangeInc = 1;
static const char* const fan_speedSignature = "q";
static const char* const fan_speedLabel[] = { "Fan Speed" };
static const uint16_t fan_speedHints[] = { PROPERTY_WIDGET_HINT_SPINNER };
static ConstraintList fan_speedConstraintList[3];
static const uint16_t fan_speedConstraintValue0 = 0;
static const char* const fan_speedDisplay0[] = { "Low" };
static const uint16_t fan_speedConstraintValue1 = 1;
static const char* const fan_speedDisplay1[] = { "Medium" };
static const uint16_t fan_speedConstraintValue2 = 2;
static const char* const fan_speedDisplay2[] = { "High" };


void WidgetsInit()
{
    initializeContainerWidget(&rootContainer);
    rootContainer.base.numLanguages = 1;
    setBaseEnabled(&rootContainer.base, TRUE);

    rootContainer.base.optParams.bgColor = 0x1e90ff;
    rootContainer.base.optParams.hints = rootContainerHints;
    rootContainer.base.optParams.numHints = 2;

    initializeContainerWidget(&tempAndHumidityContainer);
    tempAndHumidityContainer.base.numLanguages = 1;
    setBaseEnabled(&tempAndHumidityContainer.base, TRUE);

    tempAndHumidityContainer.base.optParams.bgColor = 0x200;
    tempAndHumidityContainer.base.optParams.hints = tempAndHumidityContainerHints;
    tempAndHumidityContainer.base.optParams.numHints = 1;

    initializePropertyWidget(&CurrentTempStringProperty);
    CurrentTempStringProperty.base.numLanguages = 1;
    setBaseEnabled(&CurrentTempStringProperty.base, TRUE);
    setBaseWritable(&CurrentTempStringProperty.base, FALSE);

    CurrentTempStringProperty.signature = CurrentTempStringPropertySignature;
    CurrentTempStringProperty.propertyType = SINGLE_VALUE_PROPERTY;
    CurrentTempStringProperty.getValue = &getCurrentTemperatureString;

    CurrentTempStringProperty.base.optParams.bgColor = 0x500;
    CurrentTempStringProperty.base.optParams.label = CurrentTempStringPropertyLabel;
    CurrentTempStringProperty.base.optParams.hints = CurrentTempStringPropertyHints;
    CurrentTempStringProperty.base.optParams.numHints = 1;

    initializePropertyWidget(&CurrentHumidityStringProperty);
    CurrentHumidityStringProperty.base.numLanguages = 1;
    setBaseEnabled(&CurrentHumidityStringProperty.base, TRUE);
    setBaseWritable(&CurrentHumidityStringProperty.base, FALSE);

    CurrentHumidityStringProperty.signature = CurrentHumidityStringPropertySignature;
    CurrentHumidityStringProperty.propertyType = SINGLE_VALUE_PROPERTY;
    CurrentHumidityStringProperty.getValue = &getCurrentHumidityString;

    CurrentHumidityStringProperty.base.optParams.bgColor = 0x500;
    CurrentHumidityStringProperty.base.optParams.label = CurrentHumidityStringPropertyLabel;
    CurrentHumidityStringProperty.base.optParams.hints = CurrentHumidityStringPropertyHints;
    CurrentHumidityStringProperty.base.optParams.numHints = 1;

    initializeContainerWidget(&controlsContainer);
    controlsContainer.base.numLanguages = 1;
    setBaseEnabled(&controlsContainer.base, TRUE);

    controlsContainer.base.optParams.bgColor = 0x200;
    controlsContainer.base.optParams.hints = controlsContainerHints;
    controlsContainer.base.optParams.numHints = 1;

    initializePropertyWidget(&ac_mode);
    ac_mode.base.numLanguages = 1;
    setBaseEnabled(&ac_mode.base, TRUE);
    setBaseWritable(&ac_mode.base, TRUE);

    ac_mode.signature = ac_modeSignature;
    ac_mode.propertyType = SINGLE_VALUE_PROPERTY;
    ac_mode.getValue = &getCurrentMode;

    ac_mode.base.optParams.bgColor = 0xffd700;
    ac_mode.base.optParams.label = ac_modeLabel;
    ac_mode.base.optParams.hints = ac_modeHints;
    ac_mode.base.optParams.numHints = 1;

    ac_mode.optParams.numConstraints = 5;
    ac_mode.optParams.constraintList = ac_modeConstraintList;
    ac_mode.optParams.constraintList[0].value = &ac_modeConstraintValue0;
    ac_mode.optParams.constraintList[0].display = ac_modeDisplay0;
    ac_mode.optParams.constraintList[1].value = &ac_modeConstraintValue1;
    ac_mode.optParams.constraintList[1].display = ac_modeDisplay1;
    ac_mode.optParams.constraintList[2].value = &ac_modeConstraintValue2;
    ac_mode.optParams.constraintList[2].display = ac_modeDisplay2;
    ac_mode.optParams.constraintList[3].value = &ac_modeConstraintValue3;
    ac_mode.optParams.constraintList[3].display = ac_modeDisplay3;
    ac_mode.optParams.constraintList[4].value = &ac_modeConstraintValue4;
    ac_mode.optParams.constraintList[4].display = ac_modeDisplay4;

    initializePropertyWidget(&statusStringProperty);
    statusStringProperty.base.numLanguages = 1;
    setBaseEnabled(&statusStringProperty.base, TRUE);
    setBaseWritable(&statusStringProperty.base, FALSE);

    statusStringProperty.signature = statusStringPropertySignature;
    statusStringProperty.propertyType = SINGLE_VALUE_PROPERTY;
    statusStringProperty.getValue = &getStatusString;

    statusStringProperty.base.optParams.bgColor = 0x500;
    statusStringProperty.base.optParams.label = statusStringPropertyLabel;
    statusStringProperty.base.optParams.hints = statusStringPropertyHints;
    statusStringProperty.base.optParams.numHints = 1;

    initializePropertyWidget(&set_temperature);
    set_temperature.base.numLanguages = 1;
    setBaseEnabled(&set_temperature.base, FALSE);
    setBaseWritable(&set_temperature.base, TRUE);

    set_temperature.signature = set_temperatureSignature;
    set_temperature.propertyType = SINGLE_VALUE_PROPERTY;
    set_temperature.getValue = &getTargetTemperature;

    set_temperature.base.optParams.bgColor = 0x008000;
    set_temperature.base.optParams.label = set_temperatureLabel;
    set_temperature.base.optParams.hints = set_temperatureHints;
    set_temperature.base.optParams.numHints = 1;
    set_temperature.optParams.unitOfMeasure = set_temperatureUnitMeasure;

    set_temperature.optParams.constraintRangeDefined = TRUE;
    set_temperature.optParams.constraintRange.minValue = &set_temperatureConstraintRangeMin;
    set_temperature.optParams.constraintRange.maxValue = &set_temperatureConstraintRangeMax;
    set_temperature.optParams.constraintRange.increment = &set_temperatureConstraintRangeInc;

    initializePropertyWidget(&fan_speed);
    fan_speed.base.numLanguages = 1;
    setBaseEnabled(&fan_speed.base, FALSE);
    setBaseWritable(&fan_speed.base, TRUE);

    fan_speed.signature = fan_speedSignature;
    fan_speed.propertyType = SINGLE_VALUE_PROPERTY;
    fan_speed.getValue = &getFanSpeed;

    fan_speed.base.optParams.bgColor = 0xff69b4;
    fan_speed.base.optParams.label = fan_speedLabel;
    fan_speed.base.optParams.hints = fan_speedHints;
    fan_speed.base.optParams.numHints = 1;

    fan_speed.optParams.numConstraints = 3;
    fan_speed.optParams.constraintList = fan_speedConstraintList;
    fan_speed.optParams.constraintList[0].value = &fan_speedConstraintValue0;
    fan_speed.optParams.constraintList[0].display = fan_speedDisplay0;
    fan_speed.optParams.constraintList[1].value = &fan_speedConstraintValue1;
    fan_speed.optParams.constraintList[1].display = fan_speedDisplay1;
    fan_speed.optParams.constraintList[2].value = &fan_speedConstraintValue2;
    fan_speed.optParams.constraintList[2].display = fan_speedDisplay2;

    return;
}

void* identifyMsgOrPropId(uint32_t identifier, uint16_t* widgetType, uint16_t* propType, uint16_t* language)
{
    switch (identifier) {
    case EN_ROOTCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = MYLANGUAGES_EN;
        return &rootContainer;

    case EN_ROOTCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &rootContainer;

    case EN_ROOTCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &rootContainer;

    case EN_ROOTCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &rootContainer;

    case EN_TEMPANDHUMIDITYCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = MYLANGUAGES_EN;
        return &tempAndHumidityContainer;

    case EN_TEMPANDHUMIDITYCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &tempAndHumidityContainer;

    case EN_TEMPANDHUMIDITYCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &tempAndHumidityContainer;

    case EN_TEMPANDHUMIDITYCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &tempAndHumidityContainer;

    case EN_CURRENTTEMPSTRINGPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYLANGUAGES_EN;
        return &CurrentTempStringProperty;

    case EN_CURRENTTEMPSTRINGPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &CurrentTempStringProperty;

    case EN_CURRENTTEMPSTRINGPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &CurrentTempStringProperty;

    case EN_CURRENTTEMPSTRINGPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &CurrentTempStringProperty;

    case EN_CURRENTTEMPSTRINGPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYLANGUAGES_EN;
        return &CurrentTempStringProperty;

    case EN_CURRENTHUMIDITYSTRINGPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYLANGUAGES_EN;
        return &CurrentHumidityStringProperty;

    case EN_CURRENTHUMIDITYSTRINGPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &CurrentHumidityStringProperty;

    case EN_CURRENTHUMIDITYSTRINGPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &CurrentHumidityStringProperty;

    case EN_CURRENTHUMIDITYSTRINGPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &CurrentHumidityStringProperty;

    case EN_CURRENTHUMIDITYSTRINGPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYLANGUAGES_EN;
        return &CurrentHumidityStringProperty;

    case EN_CONTROLSCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = MYLANGUAGES_EN;
        return &controlsContainer;

    case EN_CONTROLSCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &controlsContainer;

    case EN_CONTROLSCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &controlsContainer;

    case EN_CONTROLSCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &controlsContainer;

    case EN_AC_MODE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYLANGUAGES_EN;
        return &ac_mode;

    case EN_AC_MODE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &ac_mode;

    case EN_AC_MODE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &ac_mode;

    case EN_AC_MODE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &ac_mode;

    case EN_AC_MODE_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYLANGUAGES_EN;
        return &ac_mode;

    case EN_STATUSSTRINGPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYLANGUAGES_EN;
        return &statusStringProperty;

    case EN_STATUSSTRINGPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &statusStringProperty;

    case EN_STATUSSTRINGPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &statusStringProperty;

    case EN_STATUSSTRINGPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &statusStringProperty;

    case EN_STATUSSTRINGPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYLANGUAGES_EN;
        return &statusStringProperty;

    case EN_SET_TEMPERATURE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYLANGUAGES_EN;
        return &set_temperature;

    case EN_SET_TEMPERATURE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &set_temperature;

    case EN_SET_TEMPERATURE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &set_temperature;

    case EN_SET_TEMPERATURE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &set_temperature;

    case EN_SET_TEMPERATURE_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYLANGUAGES_EN;
        return &set_temperature;

    case EN_FAN_SPEED_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYLANGUAGES_EN;
        return &fan_speed;

    case EN_FAN_SPEED_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &fan_speed;

    case EN_FAN_SPEED_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &fan_speed;

    case EN_FAN_SPEED_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &fan_speed;

    case EN_FAN_SPEED_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYLANGUAGES_EN;
        return &fan_speed;

    default:
        return FALSE;
    }
}

void* identifyMsgOrPropIdForSignal(uint32_t identifier, uint8_t* isProperty)
{
    switch (identifier) {
    case EN_ROOTCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &rootContainer;

    case EN_TEMPANDHUMIDITYCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &tempAndHumidityContainer;

    case EN_CURRENTTEMPSTRINGPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &CurrentTempStringProperty;

    case EN_CURRENTTEMPSTRINGPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &CurrentTempStringProperty;

    case EN_CURRENTHUMIDITYSTRINGPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &CurrentHumidityStringProperty;

    case EN_CURRENTHUMIDITYSTRINGPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &CurrentHumidityStringProperty;

    case EN_CONTROLSCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &controlsContainer;

    case EN_AC_MODE_SIGNAL_PROPERTIES_CHANGED:
        return &ac_mode;

    case EN_AC_MODE_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &ac_mode;

    case EN_STATUSSTRINGPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &statusStringProperty;

    case EN_STATUSSTRINGPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &statusStringProperty;

    case EN_SET_TEMPERATURE_SIGNAL_PROPERTIES_CHANGED:
        return &set_temperature;

    case EN_SET_TEMPERATURE_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &set_temperature;

    case EN_FAN_SPEED_SIGNAL_PROPERTIES_CHANGED:
        return &fan_speed;

    case EN_FAN_SPEED_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &fan_speed;

    default:
        return FALSE;
    }
}

uint8_t identifyRootMsgOrPropId(uint32_t identifier)
{
    switch (identifier) {
    case ROOT_CONTROLPANEL_ROOTCONTAINER_VERSION_PROPERTY:
    case ROOT_CONTROLPANEL_ROOTCONTAINER_GET_ALL_VALUES:
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
    case EN_CURRENTTEMPSTRINGPROPERTY_VALUE_PROPERTY:
    {
        const char* newValue;
        if ((status = unmarshalPropertyValue(&CurrentTempStringProperty, replyMsg, &newValue, ((SetValueContext*)context)->sender)))
            return status;
        setCurrentTemperatureString(newValue);
        ((SetValueContext*)context)->numSignals = 1;
        ((SetValueContext*)context)->signals[0] = EN_CURRENTTEMPSTRINGPROPERTY_SIGNAL_VALUE_CHANGED;
    }
    break;

    case EN_CURRENTHUMIDITYSTRINGPROPERTY_VALUE_PROPERTY:
    {
        const char* newValue;
        if ((status = unmarshalPropertyValue(&CurrentHumidityStringProperty, replyMsg, &newValue, ((SetValueContext*)context)->sender)))
            return status;
        setCurrentHumidityString(newValue);
        ((SetValueContext*)context)->numSignals = 1;
        ((SetValueContext*)context)->signals[0] = EN_CURRENTHUMIDITYSTRINGPROPERTY_SIGNAL_VALUE_CHANGED;
    }
    break;

    case EN_AC_MODE_VALUE_PROPERTY:
    {
        uint16_t newValue;
        if ((status = unmarshalPropertyValue(&ac_mode, replyMsg, &newValue, ((SetValueContext*)context)->sender)))
            return status;
        setCurrentMode(newValue);
        ((SetValueContext*)context)->numSignals = 1;
        ((SetValueContext*)context)->signals[0] = EN_AC_MODE_SIGNAL_VALUE_CHANGED;
    }
    break;

    case EN_STATUSSTRINGPROPERTY_VALUE_PROPERTY:
    {
        const char* newValue;
        if ((status = unmarshalPropertyValue(&statusStringProperty, replyMsg, &newValue, ((SetValueContext*)context)->sender)))
            return status;
        setStatusString(newValue);
        ((SetValueContext*)context)->numSignals = 1;
        ((SetValueContext*)context)->signals[0] = EN_STATUSSTRINGPROPERTY_SIGNAL_VALUE_CHANGED;
    }
    break;

    case EN_SET_TEMPERATURE_VALUE_PROPERTY:
    {
        uint16_t newValue;
        if ((status = unmarshalPropertyValue(&set_temperature, replyMsg, &newValue, ((SetValueContext*)context)->sender)))
            return status;
        setTargetTemperature(newValue);
        ((SetValueContext*)context)->numSignals = 1;
        ((SetValueContext*)context)->signals[0] = EN_SET_TEMPERATURE_SIGNAL_VALUE_CHANGED;
    }
    break;

    case EN_FAN_SPEED_VALUE_PROPERTY:
    {
        uint16_t newValue;
        if ((status = unmarshalPropertyValue(&fan_speed, replyMsg, &newValue, ((SetValueContext*)context)->sender)))
            return status;
        setFanSpeed(newValue);
        ((SetValueContext*)context)->numSignals = 1;
        ((SetValueContext*)context)->signals[0] = EN_FAN_SPEED_SIGNAL_VALUE_CHANGED;
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
