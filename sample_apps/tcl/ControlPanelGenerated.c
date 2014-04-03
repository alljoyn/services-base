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
const char de_ATMyDeviceRootContainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT";
const char enMyDeviceCurrentTempObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/CurrentTemp";
const char de_ATMyDeviceCurrentTempObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/CurrentTemp";
const char enMyDeviceHeatPropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/heatProperty";
const char de_ATMyDeviceHeatPropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/heatProperty";
const char enMyDeviceOvenActionObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/ovenAction";
const char de_ATMyDeviceOvenActionObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/ovenAction";
const char enMyDeviceLightActionObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/lightAction";
const char de_ATMyDeviceLightActionObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/lightAction";
const char enMyDeviceLightConfirmObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/lightAction/LightConfirm";
const char de_ATMyDeviceLightConfirmObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/lightAction/LightConfirm";
const char MyDeviceAreYouSureObjectPath[] = "/ControlPanel/MyDevice/areYouSure";
const char enMyDeviceAreYouSureObjectPath[] = "/ControlPanel/MyDevice/areYouSure/en";
const char de_ATMyDeviceAreYouSureObjectPath[] = "/ControlPanel/MyDevice/areYouSure/de_AT";

ContainerWidget MyDeviceRootContainer;
LabelWidget MyDeviceCurrentTemp;
PropertyWidget MyDeviceHeatProperty;
ActionWidget MyDeviceOvenAction;
ActionWidget MyDeviceLightAction;
DialogWidget MyDeviceLightConfirm;
DialogWidget MyDeviceAreYouSure;

/*
 * Static variables used to fill widgets
 */
static const char* const MyDeviceRootContainerLabel[] = { "My Label of my container", "Container Etikett" };
static const uint16_t MyDeviceRootContainerHints[] = { LAYOUT_HINT_VERTICAL_LINEAR, LAYOUT_HINT_HORIZONTAL_LINEAR };
static const char* const MyDeviceCurrentTempLabel[] = { "Current Temperature:", "Aktuelle Temperatur:" };
static const uint16_t MyDeviceCurrentTempHints[] = { LABEL_HINT_TEXTLABEL };
static const char* const MyDeviceHeatPropertySignature = "q";
static const char* const MyDeviceHeatPropertyLabel[] = { "Oven Temperature", "Ofentemperatur" };
static const uint16_t MyDeviceHeatPropertyHints[] = { PROPERTY_WIDGET_HINT_SPINNER };
static const char* const MyDeviceHeatPropertyUnitMeasure[] = { "Degrees", "Grad" };
static ConstraintList MyDeviceHeatPropertyConstraintList[3];
static const uint16_t MyDeviceHeatPropertyConstraintValue0 = 175;
static const char* const MyDeviceHeatPropertyDisplay0[] = { "Regular", "Normal" };
static const uint16_t MyDeviceHeatPropertyConstraintValue1 = 200;
static const char* const MyDeviceHeatPropertyDisplay1[] = { "Hot", "Heiss" };
static const uint16_t MyDeviceHeatPropertyConstraintValue2 = 225;
static const char* const MyDeviceHeatPropertyDisplay2[] = { "Very Hot", "Sehr Heiss" };
static const char* const MyDeviceOvenActionLabel[] = { "Start Oven", "Ofen started" };
static const uint16_t MyDeviceOvenActionHints[] = { ACTION_WIDGET_HINT_ACTIONBUTTON };
static const char* const MyDeviceLightActionLabel[] = { "Turn on oven light", "Ofenlicht anschalten" };
static const uint16_t MyDeviceLightActionHints[] = { ACTION_WIDGET_HINT_ACTIONBUTTON };
static const char* const MyDeviceLightConfirmMessage[] = { "Are you sure you want to turn on the light", "Are you sure you want to turn on the light" };
static const char* const MyDeviceLightConfirmLabel[] = { "Are you sure?", "Sind sie sicher?" };
static const uint16_t MyDeviceLightConfirmHints[] = { DIALOG_HINT_ALERTDIALOG };
static const char* const MyDeviceLightConfirmLabelaction1[] = { "Yes", "Ja" };
static const char* const MyDeviceLightConfirmLabelaction2[] = { "No", "Nein" };
static const char* const MyDeviceLightConfirmLabelaction3[] = { "Cancel", "Abrechen" };
static const char* const MyDeviceAreYouSureMessage[] = { "Are you sure?", "Sind sie sicher?" };
static const char* const MyDeviceAreYouSureLabel[] = { "Are you sure?", "Sind sie sicher?" };
static const char* const MyDeviceAreYouSureLabelaction1[] = { "Yes", "Ja" };


void WidgetsInit()
{
    initializeContainerWidget(&MyDeviceRootContainer);
    MyDeviceRootContainer.base.numLanguages = 2;
    setBaseEnabled(&MyDeviceRootContainer.base, TRUE);

    MyDeviceRootContainer.base.optParams.bgColor = 0x200;
    MyDeviceRootContainer.base.optParams.label = MyDeviceRootContainerLabel;
    MyDeviceRootContainer.base.optParams.hints = MyDeviceRootContainerHints;
    MyDeviceRootContainer.base.optParams.numHints = 2;

    initializeLabelWidget(&MyDeviceCurrentTemp);
    MyDeviceCurrentTemp.base.numLanguages = 2;
    setBaseEnabled(&MyDeviceCurrentTemp.base, TRUE);
    MyDeviceCurrentTemp.label = MyDeviceCurrentTempLabel;

    MyDeviceCurrentTemp.base.optParams.bgColor = 0x98765;
    MyDeviceCurrentTemp.base.optParams.hints = MyDeviceCurrentTempHints;
    MyDeviceCurrentTemp.base.optParams.numHints = 1;

    initializePropertyWidget(&MyDeviceHeatProperty);
    MyDeviceHeatProperty.base.numLanguages = 2;
    setBaseEnabled(&MyDeviceHeatProperty.base, TRUE);
    setBaseWritable(&MyDeviceHeatProperty.base, TRUE);

    MyDeviceHeatProperty.signature = MyDeviceHeatPropertySignature;
    MyDeviceHeatProperty.propertyType = SINGLE_VALUE_PROPERTY;
    MyDeviceHeatProperty.getValue = &getuint16Var;

    MyDeviceHeatProperty.base.optParams.bgColor = 0x500;
    MyDeviceHeatProperty.base.optParams.label = MyDeviceHeatPropertyLabel;
    MyDeviceHeatProperty.base.optParams.hints = MyDeviceHeatPropertyHints;
    MyDeviceHeatProperty.base.optParams.numHints = 1;
    MyDeviceHeatProperty.optParams.unitOfMeasure = MyDeviceHeatPropertyUnitMeasure;

    MyDeviceHeatProperty.optParams.numConstraints = 3;
    MyDeviceHeatProperty.optParams.constraintList = MyDeviceHeatPropertyConstraintList;
    MyDeviceHeatProperty.optParams.constraintList[0].value = &MyDeviceHeatPropertyConstraintValue0;
    MyDeviceHeatProperty.optParams.constraintList[0].display = MyDeviceHeatPropertyDisplay0;
    MyDeviceHeatProperty.optParams.constraintList[1].value = &MyDeviceHeatPropertyConstraintValue1;
    MyDeviceHeatProperty.optParams.constraintList[1].display = MyDeviceHeatPropertyDisplay1;
    MyDeviceHeatProperty.optParams.constraintList[2].value = &MyDeviceHeatPropertyConstraintValue2;
    MyDeviceHeatProperty.optParams.constraintList[2].display = MyDeviceHeatPropertyDisplay2;

    initializeActionWidget(&MyDeviceOvenAction);
    MyDeviceOvenAction.base.numLanguages = 2;
    setBaseEnabled(&MyDeviceOvenAction.base, TRUE);

    MyDeviceOvenAction.base.optParams.bgColor = 0x400;
    MyDeviceOvenAction.base.optParams.label = MyDeviceOvenActionLabel;
    MyDeviceOvenAction.base.optParams.hints = MyDeviceOvenActionHints;
    MyDeviceOvenAction.base.optParams.numHints = 1;

    initializeActionWidget(&MyDeviceLightAction);
    MyDeviceLightAction.base.numLanguages = 2;
    setBaseEnabled(&MyDeviceLightAction.base, TRUE);

    MyDeviceLightAction.base.optParams.bgColor = 0x400;
    MyDeviceLightAction.base.optParams.label = MyDeviceLightActionLabel;
    MyDeviceLightAction.base.optParams.hints = MyDeviceLightActionHints;
    MyDeviceLightAction.base.optParams.numHints = 1;
    initializeDialogWidget(&MyDeviceLightConfirm);
    MyDeviceLightConfirm.base.numLanguages = 2;
    setBaseEnabled(&MyDeviceLightConfirm.base, TRUE);
    MyDeviceLightConfirm.message = MyDeviceLightConfirmMessage;
    MyDeviceLightConfirm.numActions = 3;

    MyDeviceLightConfirm.base.optParams.bgColor = 0x789;
    MyDeviceLightConfirm.base.optParams.label = MyDeviceLightConfirmLabel;
    MyDeviceLightConfirm.base.optParams.hints = MyDeviceLightConfirmHints;
    MyDeviceLightConfirm.base.optParams.numHints = 1;
    MyDeviceLightConfirm.optParams.labelAction1 = MyDeviceLightConfirmLabelaction1;
    MyDeviceLightConfirm.optParams.labelAction2 = MyDeviceLightConfirmLabelaction2;
    MyDeviceLightConfirm.optParams.labelAction3 = MyDeviceLightConfirmLabelaction3;
    initializeDialogWidget(&MyDeviceAreYouSure);
    MyDeviceAreYouSure.base.numLanguages = 2;
    setBaseEnabled(&MyDeviceAreYouSure.base, TRUE);
    MyDeviceAreYouSure.message = MyDeviceAreYouSureMessage;
    MyDeviceAreYouSure.numActions = 1;

    MyDeviceAreYouSure.base.optParams.bgColor = 0x789;
    MyDeviceAreYouSure.base.optParams.label = MyDeviceAreYouSureLabel;
    MyDeviceAreYouSure.optParams.labelAction1 = MyDeviceAreYouSureLabelaction1;

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

    case DE_AT_MYDEVICE_ROOTCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceRootContainer;

    case DE_AT_MYDEVICE_ROOTCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceRootContainer;

    case DE_AT_MYDEVICE_ROOTCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceRootContainer;

    case DE_AT_MYDEVICE_ROOTCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceRootContainer;

    case EN_MYDEVICE_CURRENTTEMP_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_LABEL;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTemp;

    case EN_MYDEVICE_CURRENTTEMP_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTemp;

    case EN_MYDEVICE_CURRENTTEMP_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTemp;

    case EN_MYDEVICE_CURRENTTEMP_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTemp;

    case EN_MYDEVICE_CURRENTTEMP_LABEL_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_LABEL;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceCurrentTemp;

    case DE_AT_MYDEVICE_CURRENTTEMP_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_LABEL;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceCurrentTemp;

    case DE_AT_MYDEVICE_CURRENTTEMP_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceCurrentTemp;

    case DE_AT_MYDEVICE_CURRENTTEMP_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceCurrentTemp;

    case DE_AT_MYDEVICE_CURRENTTEMP_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceCurrentTemp;

    case DE_AT_MYDEVICE_CURRENTTEMP_LABEL_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_LABEL;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceCurrentTemp;

    case EN_MYDEVICE_HEATPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceHeatProperty;

    case EN_MYDEVICE_HEATPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceHeatProperty;

    case EN_MYDEVICE_HEATPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceHeatProperty;

    case EN_MYDEVICE_HEATPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceHeatProperty;

    case EN_MYDEVICE_HEATPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceHeatProperty;

    case DE_AT_MYDEVICE_HEATPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceHeatProperty;

    case DE_AT_MYDEVICE_HEATPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceHeatProperty;

    case DE_AT_MYDEVICE_HEATPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceHeatProperty;

    case DE_AT_MYDEVICE_HEATPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceHeatProperty;

    case DE_AT_MYDEVICE_HEATPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceHeatProperty;

    case EN_MYDEVICE_OVENACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceOvenAction;

    case EN_MYDEVICE_OVENACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceOvenAction;

    case EN_MYDEVICE_OVENACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceOvenAction;

    case EN_MYDEVICE_OVENACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceOvenAction;

    case DE_AT_MYDEVICE_OVENACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceOvenAction;

    case DE_AT_MYDEVICE_OVENACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceOvenAction;

    case DE_AT_MYDEVICE_OVENACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceOvenAction;

    case DE_AT_MYDEVICE_OVENACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceOvenAction;

    case EN_MYDEVICE_LIGHTACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceLightAction;

    case EN_MYDEVICE_LIGHTACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceLightAction;

    case EN_MYDEVICE_LIGHTACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceLightAction;

    case EN_MYDEVICE_LIGHTACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceLightAction;

    case DE_AT_MYDEVICE_LIGHTACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceLightAction;

    case DE_AT_MYDEVICE_LIGHTACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceLightAction;

    case DE_AT_MYDEVICE_LIGHTACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceLightAction;

    case DE_AT_MYDEVICE_LIGHTACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceLightAction;

    case EN_MYDEVICE_LIGHTCONFIRM_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceLightConfirm;

    case EN_MYDEVICE_LIGHTCONFIRM_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceLightConfirm;

    case EN_MYDEVICE_LIGHTCONFIRM_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceLightConfirm;

    case EN_MYDEVICE_LIGHTCONFIRM_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceLightConfirm;

    case DE_AT_MYDEVICE_LIGHTCONFIRM_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceLightConfirm;

    case DE_AT_MYDEVICE_LIGHTCONFIRM_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceLightConfirm;

    case DE_AT_MYDEVICE_LIGHTCONFIRM_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceLightConfirm;

    case DE_AT_MYDEVICE_LIGHTCONFIRM_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceLightConfirm;

    case EN_MYDEVICE_AREYOUSURE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceAreYouSure;

    case EN_MYDEVICE_AREYOUSURE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceAreYouSure;

    case EN_MYDEVICE_AREYOUSURE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceAreYouSure;

    case EN_MYDEVICE_AREYOUSURE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_EN;
        return &MyDeviceAreYouSure;

    case DE_AT_MYDEVICE_AREYOUSURE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceAreYouSure;

    case DE_AT_MYDEVICE_AREYOUSURE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceAreYouSure;

    case DE_AT_MYDEVICE_AREYOUSURE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceAreYouSure;

    case DE_AT_MYDEVICE_AREYOUSURE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYDEVICE_MYLANGUAGES_DE_AT;
        return &MyDeviceAreYouSure;

    default:
        return FALSE;
    }
}

void* IdentifyMsgOrPropIdForSignal(uint32_t identifier, uint8_t* isProperty)
{
    switch (identifier) {
    case EN_MYDEVICE_ROOTCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceRootContainer;

    case DE_AT_MYDEVICE_ROOTCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceRootContainer;

    case EN_MYDEVICE_CURRENTTEMP_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceCurrentTemp;

    case DE_AT_MYDEVICE_CURRENTTEMP_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceCurrentTemp;

    case EN_MYDEVICE_HEATPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceHeatProperty;

    case EN_MYDEVICE_HEATPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &MyDeviceHeatProperty;

    case DE_AT_MYDEVICE_HEATPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceHeatProperty;

    case DE_AT_MYDEVICE_HEATPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &MyDeviceHeatProperty;

    case EN_MYDEVICE_OVENACTION_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceOvenAction;

    case DE_AT_MYDEVICE_OVENACTION_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceOvenAction;

    case EN_MYDEVICE_LIGHTACTION_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceLightAction;

    case DE_AT_MYDEVICE_LIGHTACTION_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceLightAction;

    case EN_MYDEVICE_LIGHTCONFIRM_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceLightConfirm;

    case DE_AT_MYDEVICE_LIGHTCONFIRM_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceLightConfirm;

    case EN_MYDEVICE_AREYOUSURE_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceAreYouSure;

    case DE_AT_MYDEVICE_AREYOUSURE_SIGNAL_PROPERTIES_CHANGED:
        return &MyDeviceAreYouSure;

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

    case MYDEVICE_NOTIFICATION_ACTION_AREYOUSURE_VERSION_PROPERTY:
    case MYDEVICE_NOTIFICATION_ACTION_AREYOUSURE_GET_ALL_VALUES:
    case MYDEVICE_NOTIFICATION_ACTION_AREYOUSURE_SIGNAL_DISMISS:
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
    case EN_MYDEVICE_HEATPROPERTY_VALUE_PROPERTY:
    case DE_AT_MYDEVICE_HEATPROPERTY_VALUE_PROPERTY:
        {
            uint16_t newValue;
            status = unmarshalPropertyValue(&MyDeviceHeatProperty, replyMsg, &newValue, ((SetValueContext*)context)->sender);
            if (status != AJ_OK) {
                return status;
            }
            setuint16Var(newValue);
            ((SetValueContext*)context)->numSignals = 2;
            ((SetValueContext*)context)->signals[0] = EN_MYDEVICE_HEATPROPERTY_SIGNAL_VALUE_CHANGED;
            ((SetValueContext*)context)->signals[1] = DE_AT_MYDEVICE_HEATPROPERTY_SIGNAL_VALUE_CHANGED;
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
    case EN_MYDEVICE_OVENACTION_EXEC:
    case DE_AT_MYDEVICE_OVENACTION_EXEC:
        {
            AJ_InfoPrintf(("Starting the Oven. Execute was called\n"));
        }
        break;

    case EN_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION1:
    case DE_AT_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION1:
        {
            AJ_InfoPrintf(("Execute Action1 was called\n"));
        }
        break;

    case EN_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION2:
    case DE_AT_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION2:
        {
            AJ_InfoPrintf(("Execute Action2 was called\n"));
        }
        break;

    case EN_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION3:
    case DE_AT_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION3:
        {
            AJ_InfoPrintf(("Execute Action3 was called\n"));
        }
        break;

    case EN_MYDEVICE_AREYOUSURE_EXEC_ACTION1:
    case DE_AT_MYDEVICE_AREYOUSURE_EXEC_ACTION1:
        {
            AJ_InfoPrintf(("Execute Action1 was called\n")); addDismissSignal(context, MYDEVICE_NOTIFICATION_ACTION_AREYOUSURE_SIGNAL_DISMISS);
        }
        break;

    case EN_MYDEVICE_AREYOUSURE_EXEC_ACTION2:
    case DE_AT_MYDEVICE_AREYOUSURE_EXEC_ACTION2:
        {
            AJ_MarshalErrorMsg(msg, &reply, AJ_ErrServiceUnknown);
        }
        break;

    case EN_MYDEVICE_AREYOUSURE_EXEC_ACTION3:
    case DE_AT_MYDEVICE_AREYOUSURE_EXEC_ACTION3:
        {
            AJ_MarshalErrorMsg(msg, &reply, AJ_ErrServiceUnknown);
        }
        break;


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
