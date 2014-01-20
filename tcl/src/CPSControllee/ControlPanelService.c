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

#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/Widgets/PropertyWidget.h>
#include <alljoyn/controlpanel/Widgets/LabelWidget.h>
#include <alljoyn/controlpanel/Widgets/DialogWidget.h>
#include <alljoyn/controlpanel/Common/HttpControl.h>
#include <alljoyn/controlpanel/Common/ControlMarshalUtil.h>

const uint16_t CPSPort = 1000;

static AJ_Status ReturnErrorMessage(AJ_Message* msg, const char* error)
{
    AJ_Message reply;
    AJ_MarshalErrorMsg(msg, &reply, error);
    return AJ_DeliverMsg(&reply);
}

AJ_Status CpsSendRootUrl(AJ_Message* msg, uint32_t msgId)
{
    AJ_Message reply;
    AJ_MarshalReplyMsg(msg, &reply);

    uint16_t widgetType = 0;
    uint16_t propType = 0;
    uint16_t language = 0;

    HttpControl* control = identifyMsgOrPropId(msgId, &widgetType, &propType, &language);
    if (control == 0) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    if (marshalHttpControlUrl(control, &reply, language)) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    return AJ_DeliverMsg(&reply);
}

AJ_Status CpsGetWidgetProperty(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    uint16_t widgetType = 0;
    uint16_t propType = 0;
    uint16_t language = 0;

    BaseWidget* widget = identifyMsgOrPropId(propId, &widgetType, &propType, &language);
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

AJ_Status CpsGetRootProperty(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    uint8_t found = identifyRootMsgOrPropId(propId);
    if (!found) {
        return status;
    }
    return MarshalVersionRootProperties(replyMsg);
}

AJ_Status CpsGetAllRootProperties(AJ_Message* msg, uint32_t msgId)
{
    AJ_Message reply;
    AJ_Status status = AJ_ERR_UNEXPECTED;

    AJ_MarshalReplyMsg(msg, &reply);

    uint8_t found = identifyRootMsgOrPropId(msgId);
    if (!found) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    status = MarshalAllRootProperties(&reply);
    if (status) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    return AJ_DeliverMsg(&reply);
}

AJ_Status CpsGetAllWidgetProperties(AJ_Message* msg, uint32_t msgId)
{
    AJ_Message reply;
    AJ_Status status = AJ_ERR_UNEXPECTED;

    AJ_MarshalReplyMsg(msg, &reply);

    uint16_t widgetType = 0;
    uint16_t propType = 0;
    uint16_t language = 0;

    BaseWidget* widget = identifyMsgOrPropId(msgId, &widgetType, &propType, &language);
    if (widget == 0) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    status = widget->marshalAllProp(widget, &reply, language);
    if (status) {
        return ReturnErrorMessage(msg, AJ_ErrServiceUnknown);
    }
    return AJ_DeliverMsg(&reply);
}

AJ_Status CpsSendPropertyChangedSignal(AJ_BusAttachment* bus, uint32_t propSignal, uint32_t sessionId)
{
    AJ_Status status;
    AJ_Message msg;

    AJ_Printf("Sending Property Changed Signal.\n");

    uint8_t isProperty = FALSE;

    void* widget = identifyMsgOrPropIdForSignal(propSignal, &isProperty);
    if (widget == 0) {
        return AJ_ERR_UNEXPECTED;
    }

    status = AJ_MarshalSignal(bus, &msg, propSignal, NULL, sessionId, 0, 0);
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

AJ_Status CpsSendDismissSignal(AJ_BusAttachment* bus, uint32_t propSignal, uint32_t sessionId)
{
    AJ_Status status;
    AJ_Message msg;

    AJ_Printf("Sending Dismiss Signal.\n");

    uint8_t found = identifyRootMsgOrPropId(propSignal);
    if (!found) {
        return AJ_ERR_UNEXPECTED;
    }

    status = AJ_MarshalSignal(bus, &msg, propSignal, NULL, sessionId, 0, 0);
    if (status != AJ_OK) {
        return status;
    }

    return AJ_DeliverMsg(&msg);
}
