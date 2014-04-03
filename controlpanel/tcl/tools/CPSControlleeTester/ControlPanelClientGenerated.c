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

#include <alljoyn.h>
#include <alljoyn/controlpanel/Common/ConstraintList.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelClientGenerated.h"

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


/*
 * Static variables used to fill widgets
 */


void WidgetsInit()
{

    return;
}

void* IdentifyMsgOrPropId(uint32_t identifier, uint16_t* widgetType, uint16_t* propType, uint16_t* language)
{
    switch (identifier) {
    default:
        return FALSE;
    }
}

void* IdentifyMsgOrPropIdForSignal(uint32_t identifier, uint8_t* isProperty)
{
    switch (identifier) {
    default:
        return FALSE;
    }
}

uint8_t IdentifyRootMsgOrPropId(uint32_t identifier)
{
    switch (identifier) {
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
    testsToRun[0].msgId = EN_MYDEVICE_ROOTCONTAINER_GET_ALL_VALUES;
    testsToRun[0].numParams = 0;
    testsToRun[1].msgId = DE_AT_MYDEVICE_ROOTCONTAINER_GET_ALL_VALUES;
    testsToRun[1].numParams = 0;
    testsToRun[2].msgId = EN_MYDEVICE_CURRENTTEMP_GET_ALL_VALUES;
    testsToRun[2].numParams = 0;
    testsToRun[3].msgId = DE_AT_MYDEVICE_CURRENTTEMP_GET_ALL_VALUES;
    testsToRun[3].numParams = 0;
    testsToRun[4].msgId = EN_MYDEVICE_HEATPROPERTY_GET_ALL_VALUES;
    testsToRun[4].numParams = 0;
    testsToRun[5].msgId = DE_AT_MYDEVICE_HEATPROPERTY_GET_ALL_VALUES;
    testsToRun[5].numParams = 0;
    testsToRun[6].msgId = EN_MYDEVICE_OVENACTION_GET_ALL_VALUES;
    testsToRun[6].numParams = 0;
    testsToRun[7].msgId = EN_MYDEVICE_OVENACTION_EXEC;
    testsToRun[7].numParams = 0;
    testsToRun[8].msgId = DE_AT_MYDEVICE_OVENACTION_GET_ALL_VALUES;
    testsToRun[8].numParams = 0;
    testsToRun[9].msgId = DE_AT_MYDEVICE_OVENACTION_EXEC;
    testsToRun[9].numParams = 0;
    testsToRun[10].msgId = EN_MYDEVICE_LIGHTACTION_GET_ALL_VALUES;
    testsToRun[10].numParams = 0;
    testsToRun[11].msgId = EN_MYDEVICE_LIGHTACTION_EXEC;
    testsToRun[11].numParams = 0;
    testsToRun[12].msgId = DE_AT_MYDEVICE_LIGHTACTION_GET_ALL_VALUES;
    testsToRun[12].numParams = 0;
    testsToRun[13].msgId = DE_AT_MYDEVICE_LIGHTACTION_EXEC;
    testsToRun[13].numParams = 0;
    testsToRun[14].msgId = EN_MYDEVICE_LIGHTCONFIRM_GET_ALL_VALUES;
    testsToRun[14].numParams = 0;
    testsToRun[15].msgId = EN_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION1;
    testsToRun[15].numParams = 0;
    testsToRun[16].msgId = EN_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION2;
    testsToRun[16].numParams = 0;
    testsToRun[17].msgId = EN_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION3;
    testsToRun[17].numParams = 0;
    testsToRun[18].msgId = DE_AT_MYDEVICE_LIGHTCONFIRM_GET_ALL_VALUES;
    testsToRun[18].numParams = 0;
    testsToRun[19].msgId = DE_AT_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION1;
    testsToRun[19].numParams = 0;
    testsToRun[20].msgId = DE_AT_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION2;
    testsToRun[20].numParams = 0;
    testsToRun[21].msgId = DE_AT_MYDEVICE_LIGHTCONFIRM_EXEC_ACTION3;
    testsToRun[21].numParams = 0;
    testsToRun[22].msgId = EN_MYDEVICE_AREYOUSURE_GET_ALL_VALUES;
    testsToRun[22].numParams = 0;
    testsToRun[23].msgId = EN_MYDEVICE_AREYOUSURE_EXEC_ACTION1;
    testsToRun[23].numParams = 0;
    testsToRun[24].msgId = EN_MYDEVICE_AREYOUSURE_EXEC_ACTION2;
    testsToRun[24].numParams = 0;
    testsToRun[25].msgId = EN_MYDEVICE_AREYOUSURE_EXEC_ACTION3;
    testsToRun[25].numParams = 0;
    testsToRun[26].msgId = DE_AT_MYDEVICE_AREYOUSURE_GET_ALL_VALUES;
    testsToRun[26].numParams = 0;
    testsToRun[27].msgId = DE_AT_MYDEVICE_AREYOUSURE_EXEC_ACTION1;
    testsToRun[27].numParams = 0;
    testsToRun[28].msgId = DE_AT_MYDEVICE_AREYOUSURE_EXEC_ACTION2;
    testsToRun[28].numParams = 0;
    testsToRun[29].msgId = DE_AT_MYDEVICE_AREYOUSURE_EXEC_ACTION3;
    testsToRun[29].numParams = 0;

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
