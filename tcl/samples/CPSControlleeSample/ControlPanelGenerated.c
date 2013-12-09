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

#include <alljoyn/controlpanel/Widgets/ActionWidget.h>
#include <alljoyn/controlpanel/Widgets/PropertyWidget.h>
#include <alljoyn/controlpanel/Widgets/ContainerWidget.h>
#include <alljoyn/controlpanel/Widgets/LabelWidget.h>
#include <alljoyn/controlpanel/Widgets/DialogWidget.h>
#include <alljoyn/controlpanel/Common/HttpControl.h>
#include <alljoyn/controlpanel/Common/ConstraintList.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelProvided.h"

const char rootContainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer";
const char enRootcontainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en";
const char de_ATRootcontainerObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT";
const char enCurrenttempObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/CurrentTemp";
const char de_ATCurrenttempObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/CurrentTemp";
const char enHeatpropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/heatProperty";
const char de_ATHeatpropertyObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/heatProperty";
const char enOvenactionObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/ovenAction";
const char de_ATOvenactionObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/ovenAction";
const char enLightactionObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/lightAction";
const char de_ATLightactionObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/lightAction";
const char enLightconfirmObjectPath[] = "/ControlPanel/MyDevice/rootContainer/en/lightAction/LightConfirm";
const char de_ATLightconfirmObjectPath[] = "/ControlPanel/MyDevice/rootContainer/de_AT/lightAction/LightConfirm";
const char areYouSureObjectPath[] = "/ControlPanel/MyDevice/areYouSure";
const char enAreyousureObjectPath[] = "/ControlPanel/MyDevice/areYouSure/en";
const char de_ATAreyousureObjectPath[] = "/ControlPanel/MyDevice/areYouSure/de_AT";


/*
 * Static variables used to fill rootContainerResponse
 */
static ContainerWidget rootContainer;
static const char* const rootContainerLabel[] = { "My Label of my container", "Container Etikett" };
static const uint16_t rootContainerHints[] = { LAYOUT_HINT_VERTICAL_LINEAR, LAYOUT_HINT_HORIZONTAL_LINEAR };
static LabelWidget CurrentTemp;
static const char* const CurrentTempLabel[] = { "Current Temperature:", "Aktuelle Temperatur:" };
static const uint16_t CurrentTempHints[] = { LABEL_HINT_TEXTLABEL };
static PropertyWidget heatProperty;
static const char* const heatPropertySignature = "q";
static const char* const heatPropertyLabel[] = { "Oven Temperature", "Ofentemperatur" };
static const uint16_t heatPropertyHints[] = { PROPERTY_WIDGET_HINT_SPINNER };
static const char* const heatPropertyUnitMeasure[] = { "Degrees", "Grad" };
static ConstraintList heatPropertyConstraintList[3];
static const uint16_t heatPropertyConstraintValue0 = 175;
static const char* const heatPropertyDisplay0[] = { "Regular", "Normal" };
static const uint16_t heatPropertyConstraintValue1 = 200;
static const char* const heatPropertyDisplay1[] = { "Hot", "Heiss" };
static const uint16_t heatPropertyConstraintValue2 = 225;
static const char* const heatPropertyDisplay2[] = { "Very Hot", "Sehr Heiss" };
static ActionWidget ovenAction;
static const char* const ovenActionLabel[] = { "Start Oven", "Ofen started" };
static const uint16_t ovenActionHints[] = { ACTION_WIDGET_HINT_ACTIONBUTTON };
static ActionWidget lightAction;
static const char* const lightActionLabel[] = { "Turn on oven light", "Ofenlicht anschalten" };
static const uint16_t lightActionHints[] = { ACTION_WIDGET_HINT_ACTIONBUTTON };
static DialogWidget LightConfirm;
static const char* const LightConfirmMessage[] = { "Are you sure you want to turn on the light", "Are you sure you want to turn on the light" };
static const char* const LightConfirmLabel[] = { "Are you sure?", "Sind sie sicher?" };
static const uint16_t LightConfirmHints[] = { DIALOG_HINT_ALERTDIALOG };
static const char* const LightConfirmLabelaction1[] = { "Yes", "Ja" };
static const char* const LightConfirmLabelaction2[] = { "No", "Nein" };
static const char* const LightConfirmLabelaction3[] = { "Cancel", "Abrechen" };
static DialogWidget areYouSure;
static const char* const areYouSureMessage[] = { "Are you sure?", "Sind sie sicher?" };
static const char* const areYouSureLabel[] = { "Are you sure?", "Sind sie sicher?" };
static const char* const areYouSureLabelaction1[] = { "Yes", "Ja" };


void WidgetsInit()
{
    initializeContainerWidget(&rootContainer);
    rootContainer.base.numLanguages = 2;
    setBaseEnabled(&rootContainer.base, TRUE);

    rootContainer.base.optParams.bgColor = 0x200;
    rootContainer.base.optParams.label = rootContainerLabel;
    rootContainer.base.optParams.hints = rootContainerHints;
    rootContainer.base.optParams.numHints = 2;

    initializeLabelWidget(&CurrentTemp);
    CurrentTemp.base.numLanguages = 2;
    setBaseEnabled(&CurrentTemp.base, TRUE);
    CurrentTemp.label = CurrentTempLabel;

    CurrentTemp.base.optParams.bgColor = 0x98765;
    CurrentTemp.base.optParams.hints = CurrentTempHints;
    CurrentTemp.base.optParams.numHints = 1;

    initializePropertyWidget(&heatProperty);
    heatProperty.base.numLanguages = 2;
    setBaseEnabled(&heatProperty.base, TRUE);
    setBaseWritable(&heatProperty.base, TRUE);

    heatProperty.signature = heatPropertySignature;
    heatProperty.propertyType = SINGLE_VALUE_PROPERTY;
    heatProperty.getValue = &getuint16Var;

    heatProperty.base.optParams.bgColor = 0x500;
    heatProperty.base.optParams.label = heatPropertyLabel;
    heatProperty.base.optParams.hints = heatPropertyHints;
    heatProperty.base.optParams.numHints = 1;
    heatProperty.optParams.unitOfMeasure = heatPropertyUnitMeasure;

    heatProperty.optParams.numConstraints = 3;
    heatProperty.optParams.constraintList = heatPropertyConstraintList;
    heatProperty.optParams.constraintList[0].value = &heatPropertyConstraintValue0;
    heatProperty.optParams.constraintList[0].display = heatPropertyDisplay0;
    heatProperty.optParams.constraintList[1].value = &heatPropertyConstraintValue1;
    heatProperty.optParams.constraintList[1].display = heatPropertyDisplay1;
    heatProperty.optParams.constraintList[2].value = &heatPropertyConstraintValue2;
    heatProperty.optParams.constraintList[2].display = heatPropertyDisplay2;

    initializeActionWidget(&ovenAction);
    ovenAction.base.numLanguages = 2;
    setBaseEnabled(&ovenAction.base, TRUE);

    ovenAction.base.optParams.bgColor = 0x400;
    ovenAction.base.optParams.label = ovenActionLabel;
    ovenAction.base.optParams.hints = ovenActionHints;
    ovenAction.base.optParams.numHints = 1;

    initializeActionWidget(&lightAction);
    lightAction.base.numLanguages = 2;
    setBaseEnabled(&lightAction.base, TRUE);

    lightAction.base.optParams.bgColor = 0x400;
    lightAction.base.optParams.label = lightActionLabel;
    lightAction.base.optParams.hints = lightActionHints;
    lightAction.base.optParams.numHints = 1;
    initializeDialogWidget(&LightConfirm);
    LightConfirm.base.numLanguages = 2;
    setBaseEnabled(&LightConfirm.base, TRUE);
    LightConfirm.message = LightConfirmMessage;
    LightConfirm.numActions = 3;

    LightConfirm.base.optParams.bgColor = 0x789;
    LightConfirm.base.optParams.label = LightConfirmLabel;
    LightConfirm.base.optParams.hints = LightConfirmHints;
    LightConfirm.base.optParams.numHints = 1;
    LightConfirm.optParams.labelAction1 = LightConfirmLabelaction1;
    LightConfirm.optParams.labelAction2 = LightConfirmLabelaction2;
    LightConfirm.optParams.labelAction3 = LightConfirmLabelaction3;
    initializeDialogWidget(&areYouSure);
    areYouSure.base.numLanguages = 2;
    setBaseEnabled(&areYouSure.base, TRUE);
    areYouSure.message = areYouSureMessage;
    areYouSure.numActions = 1;

    areYouSure.base.optParams.bgColor = 0x789;
    areYouSure.base.optParams.label = areYouSureLabel;
    areYouSure.optParams.labelAction1 = areYouSureLabelaction1;

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

    case DE_AT_ROOTCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = MYLANGUAGES_DE_AT;
        return &rootContainer;

    case DE_AT_ROOTCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_DE_AT;
        return &rootContainer;

    case DE_AT_ROOTCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_DE_AT;
        return &rootContainer;

    case DE_AT_ROOTCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_DE_AT;
        return &rootContainer;

    case EN_CURRENTTEMP_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_LABEL;
        *language = MYLANGUAGES_EN;
        return &CurrentTemp;

    case EN_CURRENTTEMP_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &CurrentTemp;

    case EN_CURRENTTEMP_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &CurrentTemp;

    case EN_CURRENTTEMP_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &CurrentTemp;

    case EN_CURRENTTEMP_LABEL_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_LABEL;
        *language = MYLANGUAGES_EN;
        return &CurrentTemp;

    case DE_AT_CURRENTTEMP_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_LABEL;
        *language = MYLANGUAGES_DE_AT;
        return &CurrentTemp;

    case DE_AT_CURRENTTEMP_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_DE_AT;
        return &CurrentTemp;

    case DE_AT_CURRENTTEMP_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_DE_AT;
        return &CurrentTemp;

    case DE_AT_CURRENTTEMP_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_DE_AT;
        return &CurrentTemp;

    case DE_AT_CURRENTTEMP_LABEL_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_LABEL;
        *language = MYLANGUAGES_DE_AT;
        return &CurrentTemp;

    case EN_HEATPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYLANGUAGES_EN;
        return &heatProperty;

    case EN_HEATPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &heatProperty;

    case EN_HEATPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &heatProperty;

    case EN_HEATPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &heatProperty;

    case EN_HEATPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYLANGUAGES_EN;
        return &heatProperty;

    case DE_AT_HEATPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = MYLANGUAGES_DE_AT;
        return &heatProperty;

    case DE_AT_HEATPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_DE_AT;
        return &heatProperty;

    case DE_AT_HEATPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_DE_AT;
        return &heatProperty;

    case DE_AT_HEATPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_DE_AT;
        return &heatProperty;

    case DE_AT_HEATPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = MYLANGUAGES_DE_AT;
        return &heatProperty;

    case EN_OVENACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = MYLANGUAGES_EN;
        return &ovenAction;

    case EN_OVENACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &ovenAction;

    case EN_OVENACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &ovenAction;

    case EN_OVENACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &ovenAction;

    case DE_AT_OVENACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = MYLANGUAGES_DE_AT;
        return &ovenAction;

    case DE_AT_OVENACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_DE_AT;
        return &ovenAction;

    case DE_AT_OVENACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_DE_AT;
        return &ovenAction;

    case DE_AT_OVENACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_DE_AT;
        return &ovenAction;

    case EN_LIGHTACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = MYLANGUAGES_EN;
        return &lightAction;

    case EN_LIGHTACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &lightAction;

    case EN_LIGHTACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &lightAction;

    case EN_LIGHTACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &lightAction;

    case DE_AT_LIGHTACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = MYLANGUAGES_DE_AT;
        return &lightAction;

    case DE_AT_LIGHTACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_DE_AT;
        return &lightAction;

    case DE_AT_LIGHTACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_DE_AT;
        return &lightAction;

    case DE_AT_LIGHTACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_DE_AT;
        return &lightAction;

    case EN_LIGHTCONFIRM_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = MYLANGUAGES_EN;
        return &LightConfirm;

    case EN_LIGHTCONFIRM_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &LightConfirm;

    case EN_LIGHTCONFIRM_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &LightConfirm;

    case EN_LIGHTCONFIRM_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &LightConfirm;

    case DE_AT_LIGHTCONFIRM_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = MYLANGUAGES_DE_AT;
        return &LightConfirm;

    case DE_AT_LIGHTCONFIRM_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_DE_AT;
        return &LightConfirm;

    case DE_AT_LIGHTCONFIRM_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_DE_AT;
        return &LightConfirm;

    case DE_AT_LIGHTCONFIRM_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_DE_AT;
        return &LightConfirm;

    case EN_AREYOUSURE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = MYLANGUAGES_EN;
        return &areYouSure;

    case EN_AREYOUSURE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_EN;
        return &areYouSure;

    case EN_AREYOUSURE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_EN;
        return &areYouSure;

    case EN_AREYOUSURE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_EN;
        return &areYouSure;

    case DE_AT_AREYOUSURE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = MYLANGUAGES_DE_AT;
        return &areYouSure;

    case DE_AT_AREYOUSURE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = MYLANGUAGES_DE_AT;
        return &areYouSure;

    case DE_AT_AREYOUSURE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = MYLANGUAGES_DE_AT;
        return &areYouSure;

    case DE_AT_AREYOUSURE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = MYLANGUAGES_DE_AT;
        return &areYouSure;

    default:
        return FALSE;
    }
}

void* identifyMsgOrPropIdForSignal(uint32_t identifier, uint8_t* isProperty)
{
    switch (identifier) {
    case EN_ROOTCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &rootContainer;

    case DE_AT_ROOTCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &rootContainer;

    case EN_CURRENTTEMP_SIGNAL_PROPERTIES_CHANGED:
        return &CurrentTemp;

    case DE_AT_CURRENTTEMP_SIGNAL_PROPERTIES_CHANGED:
        return &CurrentTemp;

    case EN_HEATPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &heatProperty;

    case EN_HEATPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &heatProperty;

    case DE_AT_HEATPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &heatProperty;

    case DE_AT_HEATPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &heatProperty;

    case EN_OVENACTION_SIGNAL_PROPERTIES_CHANGED:
        return &ovenAction;

    case DE_AT_OVENACTION_SIGNAL_PROPERTIES_CHANGED:
        return &ovenAction;

    case EN_LIGHTACTION_SIGNAL_PROPERTIES_CHANGED:
        return &lightAction;

    case DE_AT_LIGHTACTION_SIGNAL_PROPERTIES_CHANGED:
        return &lightAction;

    case EN_LIGHTCONFIRM_SIGNAL_PROPERTIES_CHANGED:
        return &LightConfirm;

    case DE_AT_LIGHTCONFIRM_SIGNAL_PROPERTIES_CHANGED:
        return &LightConfirm;

    case EN_AREYOUSURE_SIGNAL_PROPERTIES_CHANGED:
        return &areYouSure;

    case DE_AT_AREYOUSURE_SIGNAL_PROPERTIES_CHANGED:
        return &areYouSure;

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

    case NOTIFICATION_ACTION_AREYOUSURE_VERSION_PROPERTY:
    case NOTIFICATION_ACTION_AREYOUSURE_GET_ALL_VALUES:
    case NOTIFICATION_ACTION_AREYOUSURE_SIGNAL_DISMISS:
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
    case EN_HEATPROPERTY_VALUE_PROPERTY:
    case DE_AT_HEATPROPERTY_VALUE_PROPERTY:
    {
        uint16_t newValue;
        if ((status = unmarshalPropertyValue(&heatProperty, replyMsg, &newValue, ((SetValueContext*)context)->sender)))
            return status;
        setuint16Var(newValue);
        ((SetValueContext*)context)->numSignals = 2;
        ((SetValueContext*)context)->signals[0] = EN_HEATPROPERTY_SIGNAL_VALUE_CHANGED;
        ((SetValueContext*)context)->signals[1] = DE_AT_HEATPROPERTY_SIGNAL_VALUE_CHANGED;
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
    case EN_OVENACTION_EXEC:
    case DE_AT_OVENACTION_EXEC:
    {
        AJ_Printf("Starting the Oven. Execute was called\n");
    }
    break;

    case EN_LIGHTCONFIRM_EXEC_ACTION1:
    case DE_AT_LIGHTCONFIRM_EXEC_ACTION1:
    {
        AJ_Printf("Execute Action1 was called\n");
    }
    break;

    case EN_LIGHTCONFIRM_EXEC_ACTION2:
    case DE_AT_LIGHTCONFIRM_EXEC_ACTION2:
    {
        AJ_Printf("Execute Action2 was called\n");
    }
    break;

    case EN_LIGHTCONFIRM_EXEC_ACTION3:
    case DE_AT_LIGHTCONFIRM_EXEC_ACTION3:
    {
        AJ_Printf("Execute Action3 was called\n");
    }
    break;

    case EN_AREYOUSURE_EXEC_ACTION1:
    case DE_AT_AREYOUSURE_EXEC_ACTION1:
    {
        AJ_Printf("Execute Action1 was called\n"); addDismissSignal(context, NOTIFICATION_ACTION_AREYOUSURE_SIGNAL_DISMISS);
    }
    break;

    case EN_AREYOUSURE_EXEC_ACTION2:
    case DE_AT_AREYOUSURE_EXEC_ACTION2:
    {
        AJ_MarshalErrorMsg(msg, &reply, AJ_ErrServiceUnknown);
    }
    break;

    case EN_AREYOUSURE_EXEC_ACTION3:
    case DE_AT_AREYOUSURE_EXEC_ACTION3:
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
