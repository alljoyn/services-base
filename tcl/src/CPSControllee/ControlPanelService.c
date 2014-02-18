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
 * The corresponding flag dbgAJCPS is defined in services_common.h and implemented
 * in services_common.c.
 */
#define AJ_MODULE AJCPS
#include <aj_debug.h>

#include <alljoyn.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/Widgets/PropertyWidget.h>
#include <alljoyn/controlpanel/Widgets/LabelWidget.h>
#include <alljoyn/controlpanel/Widgets/DialogWidget.h>
#include <alljoyn/controlpanel/Common/HttpControl.h>
#include <alljoyn/controlpanel/Common/ControlMarshalUtil.h>
#include <aj_config.h>

const uint16_t AJCPS_Port = 1000;
uint32_t currentSessionId = 0;

static AJSVC_MessageProcessor appGeneratedMessageProcessor = NULL;
static AJCPS_IdentifyMsgOrPropId appIdentifyMsgOrPropId = NULL;
static AJCPS_IdentifyMsgOrPropIdForSignal appIdentifyMsgOrPropIdForSignal = NULL;
static AJCPS_IdentifyRootMsgOrPropId appIdentifyRootMsgOrPropId = NULL;

AJ_Status AJCPS_Start(AJSVC_MessageProcessor generatedMessageProcessor, AJCPS_IdentifyMsgOrPropId identifyMsgOrPropId, AJCPS_IdentifyMsgOrPropIdForSignal identifyMsgOrPropIdForSignal, AJCPS_IdentifyRootMsgOrPropId identifyRootMsgOrPropId)
{
    AJ_Status status = AJ_OK;

    appGeneratedMessageProcessor = generatedMessageProcessor;
    appIdentifyMsgOrPropId = identifyMsgOrPropId;
    appIdentifyMsgOrPropIdForSignal = identifyMsgOrPropIdForSignal;
    appIdentifyRootMsgOrPropId = identifyRootMsgOrPropId;
    if (appGeneratedMessageProcessor == NULL || appIdentifyMsgOrPropId == NULL || appIdentifyMsgOrPropIdForSignal == NULL || appIdentifyRootMsgOrPropId == NULL) {
        AJ_ErrPrintf(("AJCPS_Start(): One of the required callbacks is NULL!\n"));
        status = AJ_ERR_INVALID;
    }

    return status;
}

uint32_t AJCPS_GetCurrentSessionId()
{
    return currentSessionId;
}

static AJ_Status ReturnErrorMessage(AJ_Message* msg, const char* error)
{
    AJ_Message reply;
    AJ_MarshalErrorMsg(msg, &reply, error);
    return AJ_DeliverMsg(&reply);
}

AJ_Status AJCPS_SendRootUrl(AJ_Message* msg, uint32_t msgId)
{
    AJ_Message reply;
    AJ_MarshalReplyMsg(msg, &reply);

    uint16_t widgetType = 0;
    uint16_t propType = 0;
    uint16_t language = 0;

    HttpControl* control = (appIdentifyMsgOrPropId)(msgId, &widgetType, &propType, &language);
    if (control == 0) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    if (marshalHttpControlUrl(control, &reply, language)) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    return AJ_DeliverMsg(&reply);
}

AJ_Status AJCPS_GetWidgetProperty(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    uint16_t widgetType = 0;
    uint16_t propType = 0;
    uint16_t language = 0;

    BaseWidget* widget = (appIdentifyMsgOrPropId)(propId, &widgetType, &propType, &language);
    if (widget == 0) {
        return status;
    }

    switch (propType) {
    case PROPERTY_TYPE_VERSION:
        if (widget->marshalVersion) {
            return widget->marshalVersion(widget, replyMsg, language);
        }
        break;

    case PROPERTY_TYPE_STATES:
        if (widget->marshalStates) {
            return widget->marshalStates(widget, replyMsg, language);
        }
        break;

    case PROPERTY_TYPE_OPTPARAMS:
        if (widget->marshalOptParam) {
            return widget->marshalOptParam(widget, replyMsg, language);
        }
        break;

    case PROPERTY_TYPE_VALUE:
        return marshalPropertyValue(((PropertyWidget*)widget), replyMsg, language);

    case PROPERTY_TYPE_LABEL:
        return marshalLabelLabel(((LabelWidget*)widget), replyMsg, language);

    case PROPERTY_TYPE_MESSAGE:
        return marshalDialogMessage(((DialogWidget*)widget), replyMsg, language);

    case PROPERTY_TYPE_NUM_ACTIONS:
        return marshalDialogNumActions(((DialogWidget*)widget), replyMsg, language);
    }
    return status;
}

AJ_Status AJCPS_GetRootProperty(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    uint8_t found = (appIdentifyRootMsgOrPropId)(propId);
    if (!found) {
        return status;
    }
    return MarshalVersionRootProperties(replyMsg);
}

AJ_Status AJCPS_GetAllRootProperties(AJ_Message* msg, uint32_t msgId)
{
    AJ_Message reply;
    AJ_Status status = AJ_ERR_UNEXPECTED;

    AJ_MarshalReplyMsg(msg, &reply);

    uint8_t found = (appIdentifyRootMsgOrPropId)(msgId);
    if (!found) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    status = MarshalAllRootProperties(&reply);
    if (status) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    return AJ_DeliverMsg(&reply);
}

AJ_Status AJCPS_GetAllWidgetProperties(AJ_Message* msg, uint32_t msgId)
{
    AJ_Message reply;
    AJ_Status status = AJ_ERR_UNEXPECTED;

    AJ_MarshalReplyMsg(msg, &reply);

    uint16_t widgetType = 0;
    uint16_t propType = 0;
    uint16_t language = 0;

    BaseWidget* widget = (appIdentifyMsgOrPropId)(msgId, &widgetType, &propType, &language);
    if (widget == 0) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    status = widget->marshalAllProp(widget, &reply, language);
    if (status) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    return AJ_DeliverMsg(&reply);
}

AJ_Status AJCPS_SendPropertyChangedSignal(AJ_BusAttachment* busAttachment, uint32_t propSignal, uint32_t sessionId)
{
    AJ_Status status;
    AJ_Message msg;

    AJ_InfoPrintf(("Sending Property Changed Signal.\n"));

    uint8_t isProperty = FALSE;

    void* widget = (appIdentifyMsgOrPropIdForSignal)(propSignal, &isProperty);
    if (widget == 0) {
        return AJ_ERR_UNEXPECTED;
    }

    status = AJ_MarshalSignal(busAttachment, &msg, propSignal, NULL, sessionId, 0, 0);
    if (status != AJ_OK) {
        return status;
    }

    if (isProperty) {
        status = marshalPropertyValue(widget, &msg, 0);
        if (status != AJ_OK) {
            return status;
        }
    }

    return AJ_DeliverMsg(&msg);
}

AJ_Status AJCPS_SendDismissSignal(AJ_BusAttachment* busAttachment, uint32_t propSignal, uint32_t sessionId)
{
    AJ_Status status;
    AJ_Message msg;

    AJ_InfoPrintf(("Sending Dismiss Signal.\n"));

    uint8_t found = (appIdentifyRootMsgOrPropId)(propSignal);
    if (!found) {
        return AJ_ERR_UNEXPECTED;
    }

    status = AJ_MarshalSignal(busAttachment, &msg, propSignal, NULL, sessionId, 0, 0);
    if (status != AJ_OK) {
        return status;
    }

    return AJ_DeliverMsg(&msg);
}

AJ_Status AJCPS_ConnectedHandler(AJ_BusAttachment* busAttachment)
{
    AJ_SessionOpts sessionOpts = {
        AJ_SESSION_TRAFFIC_MESSAGES,
        AJ_SESSION_PROXIMITY_ANY,
        AJ_TRANSPORT_ANY,
        TRUE
    };
    AJ_Status status;

    status = AJ_BusBindSessionPort(busAttachment, AJCPS_Port, &sessionOpts, 0);
    if (status != AJ_OK) {
        AJ_ErrPrintf(("Failed to send bind session port message\n"));
    }

    uint8_t serviceStarted = FALSE;
    while (!serviceStarted && (status == AJ_OK)) {
        AJ_Message msg;

        status = AJ_UnmarshalMsg(busAttachment, &msg, AJ_UNMARSHAL_TIMEOUT);
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
        AJ_ErrPrintf(("AllJoyn disconnect bus status=%d\n", status));
        status = AJ_ERR_READ;
    }
    return status;
}

uint8_t AJCPS_CheckSessionAccepted(uint16_t port, uint32_t sessionId, char* joiner)
{
    if (port != AJCPS_Port) {
        return FALSE;
    }
    currentSessionId = sessionId;
    return TRUE;
}

AJSVC_ServiceStatus AJCPS_MessageProcessor(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJ_Status* msgStatus)
{
    return (appGeneratedMessageProcessor)(busAttachment, msg, msgStatus);
}

AJ_Status AJCPS_DisconnectHandler(AJ_BusAttachment* busAttachment)
{
    AJ_Status status = AJ_OK;
//    status = AJ_BusUnbindSession(busAttachment, AJCPS_Port);
//    if (status != AJ_OK) {
//        AJ_ErrPrintf(("Failed to send unbind session port=%d\n", AJCPS_Port));
//    }
    return status;
}
