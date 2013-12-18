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

#include <alljoyn/controlpanel/ControlPanelService.h>
#include <ControlPanelProvided.h>
#include <aj_link_timeout.h>
#include <ControlPanelSample.h>

#define UUID_LENGTH 16

/**
 * Static non consts.
 */

uint32_t CPSsessionId = 0;

void CPS_Init()
{
    WidgetsInit();
}

AJ_Status ControlPanel_ConnectedHandler(AJ_BusAttachment* bus)
{
    AJ_SessionOpts sessionOpts = {
        AJ_SESSION_TRAFFIC_MESSAGES,
        AJ_SESSION_PROXIMITY_ANY,
        AJ_TRANSPORT_ANY,
        TRUE
    };

    AJ_Status status;
    AJ_Time timer = { 0, 0 };
    AJ_InitTimer(&timer);

    status = AJ_BusBindSessionPort(bus, CPSPort, &sessionOpts);
    if (status != AJ_OK) {
        AJ_Printf("Failed to send bind session port message\n");
    }

    uint8_t serviceStarted = FALSE;
    while (!serviceStarted && (status == AJ_OK)) {
        AJ_Message msg;

        AJ_GetElapsedTime(&timer, TRUE);

        status = AJ_UnmarshalMsg(bus, &msg, AJ_UNMARSHAL_TIMEOUT);

        /*
         * TODO This is a temporary hack to work around buggy select implementations
         */
        if (status == AJ_ERR_TIMEOUT) {
            uint32_t elapsed = AJ_GetElapsedTime(&timer, TRUE);
            if (elapsed < AJ_UNMARSHAL_TIMEOUT) {
                AJ_Printf("Spurious timeout error (elapsed=%d < AJ_UNMARSHAL_TIMEOUT=%d) - continuing\n", elapsed, AJ_UNMARSHAL_TIMEOUT);
                status = AJ_OK;
                continue;
            }
        }

        if (status != AJ_OK) {
            break;
        }

        switch (msg.msgId) {
        case AJ_REPLY_ID(AJ_METHOD_BIND_SESSION_PORT):
            if (msg.hdr->msgType == AJ_MSG_ERROR) {
                status = AJ_ERR_FAILURE;
            } else {
                serviceStarted = TRUE;
            }
            break;

        default:
            /*
             * Pass to the built-in bus message handlers
             */
            status = AJ_BusHandleBusMessage(&msg);
            break;
        }
        AJ_CloseMsg(&msg);
    }

    if (status != AJ_OK) {
        AJ_Printf("AllJoyn disconnect bus status=%d\n", status);
        status = AJ_ERR_READ;
    }
    return status;
}

uint8_t CPS_CheckSessionAccepted(uint16_t port, uint32_t sessionId, char* joiner)
{
    if (port != CPSPort)
        return FALSE;
    CPSsessionId = sessionId;
    return TRUE;
}

Service_Status CPS_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status*msgStatus)
{
    Service_Status service_Status = SERVICE_STATUS_HANDLED;

    switch (msg->msgId) {
        GET_WIDGET_VALUE_CASES
        *msgStatus = AJ_BusPropGet(msg, CpsGetWidgetProperty, NULL);
        break;

        GET_WIDGET_ALL_VALUE_CASES
        *msgStatus = CpsGetAllWidgetProperties(msg, msg->msgId);
        break;

        GET_ROOT_VALUE_CASES
        *msgStatus = AJ_BusPropGet(msg, CpsGetRootProperty, NULL);
        break;

        GET_ROOT_ALL_VALUE_CASES
        *msgStatus = CpsGetAllRootProperties(msg, msg->msgId);
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
                    *msgStatus = CpsSendPropertyChangedSignal(bus, context.signals[indx], CPSsessionId);
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
                    if (context.signals[indx].signalType == SIGNAL_TYPE_DATA_CHANGED)
                        *msgStatus = CpsSendPropertyChangedSignal(bus, context.signals[indx].signalId, CPSsessionId);
                    else if (context.signals[indx].signalType == SIGNAL_TYPE_DISMISS)
                        *msgStatus = CpsSendDismissSignal(bus, context.signals[indx].signalId, CPSsessionId);
                }
            }
        }
        break;

        GET_URL_CASES
        *msgStatus = CpsSendRootUrl(msg, msg->msgId);
        break;

    case AJ_SIGNAL_SESSION_LOST:
        break;

    default:
        service_Status = SERVICE_STATUS_NOT_HANDLED;
        break;
    }

    return service_Status;
}

void Controllee_DoWork(AJ_BusAttachment*bus)
{
    uint8_t sendUpdates = checkForUpdatesToSend();
    if (sendUpdates > 0) {

        // 0001 == need to update the temperature text field
        // 0010 == need to update the status text field
        // 0100 == need to update the state of temperature selector
        // 1000 == need to update the state of fan speed selector

        if ((sendUpdates & (1 << 0)) != 0) {
            AJ_Printf("##### Sending update signal: temperature string field \n");
            CpsSendPropertyChangedSignal(bus, EN_MYDEVICE_CURRENTTEMPSTRINGPROPERTY_SIGNAL_VALUE_CHANGED, CPSsessionId);
        }
        if ((sendUpdates & (1 << 1)) != 0) {
            AJ_Printf("##### Sending update signal: status string field \n");
            CpsSendPropertyChangedSignal(bus, EN_MYDEVICE_STATUSSTRINGPROPERTY_SIGNAL_VALUE_CHANGED, CPSsessionId);
        }
        if ((sendUpdates & (1 << 2)) != 0) {
            AJ_Printf("##### Sending update signal: temperature selector state \n");
            CpsSendPropertyChangedSignal(bus, EN_MYDEVICE_SET_TEMPERATURE_SIGNAL_PROPERTIES_CHANGED, CPSsessionId);
        }
        if ((sendUpdates & (1 << 3)) != 0) {
            AJ_Printf("##### Sending update signal: fan speed selector state \n");
            CpsSendPropertyChangedSignal(bus, EN_MYDEVICE_FAN_SPEED_SIGNAL_PROPERTIES_CHANGED, CPSsessionId);
        }
    }
    return;
}

void CPS_Finish()
{
    return;
}
