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

#include <alljoyn/notification/NotificationProducer.h>
#include <alljoyn/services_common/PropertyStore.h>
#include <aj_crypto.h>
#include <aj_debug.h>

/**
 * Static non consts.
 */
static uint32_t notificationId = 0;

typedef struct _AJNS_MessageTracking {
    uint32_t msgId;
    uint32_t msgSerialNum;
} AJNS_MessageTracking;

static AJNS_MessageTracking lastSentNotifications[NUM_MESSAGE_TYPES] = { { 0, 0 }, { 0, 0 }, { 0, 0 } };

/**
 * Static constants.
 */
const char NotificationObjectPathEmergency[]   = "/emergency";
const char NotificationObjectPathWarning[]     = "/warning";
const char NotificationObjectPathInfo[]        = "/info";

/**
 * Marshal Notification
 */
static AJ_Status Producer_MarshalNotificationMsg(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJNS_Notification* notification, uint32_t ttl)
{
    AJ_Status status = AJ_OK;

    if (notification == NULL) {
        AJ_Printf("Nothing to send\n");
        return status;
    }

    status = AJ_MarshalSignal(busAttachment, msg, AJ_APP_MESSAGE_ID(notification->messageType + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 1, 0), NULL, 0, ALLJOYN_FLAG_SESSIONLESS, ttl);
    if (status != AJ_OK) {
        AJ_Printf("Could not Marshal Signal\n");
        return status;
    }

    AJ_Arg attrbtArray;
    AJ_Arg customAttributeArray;
    AJ_Arg notTextArray;
    AJ_Arg richAudioArray;
    AJ_Arg dictArg;
    AJ_Arg customAttributeDictArg;
    AJ_Arg structArg;
    AJ_Arg audioStructArg;
    AJ_Arg richAudioAttrArray;

    ///////////////////       Proto     /////////////////////
    status = AJ_MarshalArgs(msg, "q", notification->version);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    ///////////////////    MessgeId    /////////////////////
    status = AJ_MarshalArgs(msg, "i", notification->notificationId);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    ///////////////////    MessageType   ////////////////////////////
    status = AJ_MarshalArgs(msg, "q", notification->messageType);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    ///////////////////    DeviceId   ////////////////////////////
    status = AJ_MarshalArgs(msg, "s", notification->deviceId);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    ///////////////////    DeviceName   ////////////////////////////
    status = AJ_MarshalArgs(msg, "s", notification->deviceName);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    ///////////////////    AppId   ////////////////////////////
    status = Common_MarshalAppId(msg, notification->appId);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    ///////////////////    AppName   ////////////////////////////
    status = AJ_MarshalArgs(msg, "s", notification->appName);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    ///////////////////    Attributes   ////////////////////////////
    status = AJ_MarshalContainer(msg, &attrbtArray, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    if (notification->content->richIconUrl != 0) {
        status = AJ_MarshalContainer(msg, &dictArg, AJ_ARG_DICT_ENTRY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "i", RICH_CONTENT_ICON_URL_ATTRIBUTE_KEY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalVariant(msg, "s");
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "s", notification->content->richIconUrl);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalCloseContainer(msg, &dictArg);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
    }
    int8_t indx;
    if (notification->content->numAudioUrls > 0) {
        status = AJ_MarshalContainer(msg, &richAudioArray, AJ_ARG_DICT_ENTRY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "i", RICH_CONTENT_AUDIO_URL_ATTRIBUTE_KEY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalVariant(msg, "a(ss)");
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalContainer(msg, &richAudioAttrArray, AJ_ARG_ARRAY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }

        for (indx = 0; indx < notification->content->numAudioUrls; indx++) {
            if ((strlen(notification->content->richAudioUrls[indx].key) == 0) || (strlen(notification->content->richAudioUrls[indx].value) == 0)) {
                AJ_Printf("Rich Audio Language/Url can not be empty\n");
                AJ_MarshalCloseContainer(msg, &richAudioArray);
                status = AJ_ERR_DISALLOWED;
                goto ErrorExit;
            }
            status = AJ_MarshalContainer(msg, &audioStructArg, AJ_ARG_STRUCT);
            if (status != AJ_OK) {
                goto ErrorExit;
            }
            status = AJ_MarshalArgs(msg, "ss", notification->content->richAudioUrls[indx].key, notification->content->richAudioUrls[indx].value);
            if (status != AJ_OK) {
                goto ErrorExit;
            }
            status = AJ_MarshalCloseContainer(msg, &audioStructArg);
            if (status != AJ_OK) {
                goto ErrorExit;
            }
        }

        status = AJ_MarshalCloseContainer(msg, &richAudioAttrArray);
        if (status != AJ_OK) {
            goto ErrorExit;
        }

        status = AJ_MarshalCloseContainer(msg, &richAudioArray);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
    }

    if (notification->content->richIconObjectPath != 0) {
        status = AJ_MarshalContainer(msg, &dictArg, AJ_ARG_DICT_ENTRY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "i", RICH_CONTENT_ICON_OBJECT_PATH_ATTRIBUTE_KEY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalVariant(msg, "s");
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "s", notification->content->richIconObjectPath);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalCloseContainer(msg, &dictArg);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
    }

    if (notification->content->richAudioObjectPath != 0) {
        status = AJ_MarshalContainer(msg, &dictArg, AJ_ARG_DICT_ENTRY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "i", RICH_CONTENT_AUDIO_OBJECT_PATH_ATTRIBUTE_KEY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalVariant(msg, "s");
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "s", notification->content->richAudioObjectPath);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalCloseContainer(msg, &dictArg);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
    }

    if (notification->content->controlPanelServiceObjectPath != 0) {
        status = AJ_MarshalContainer(msg, &dictArg, AJ_ARG_DICT_ENTRY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "i", CONTROLPANELSERVICE_OBJECT_PATH_ATTRIBUTE_KEY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalVariant(msg, "s");
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "s", notification->content->controlPanelServiceObjectPath);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalCloseContainer(msg, &dictArg);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
    }

    if (notification->version > 1) {
        status = AJ_MarshalContainer(msg, &dictArg, AJ_ARG_DICT_ENTRY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "i", ORIGINAL_SENDER_NAME_ATTRIBUTE_KEY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalVariant(msg, "s");
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "s", notification->originalSenderName);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalCloseContainer(msg, &dictArg);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
    }

    status = AJ_MarshalCloseContainer(msg, &attrbtArray);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    ///////////////////    Custom Attributes   ///////////////////
    status = AJ_MarshalContainer(msg, &customAttributeArray, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    for (indx = 0; indx < notification->content->numCustomAttributes; indx++) {
        status = AJ_MarshalContainer(msg, &customAttributeDictArg, AJ_ARG_DICT_ENTRY);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "ss", notification->content->customAttributes[indx].key, notification->content->customAttributes[indx].value);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalCloseContainer(msg, &customAttributeDictArg);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
    }

    status = AJ_MarshalCloseContainer(msg, &customAttributeArray);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    ///////////////////   Notifications   ////////////////////////////
    status = AJ_MarshalContainer(msg, &notTextArray, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    for (indx = 0; indx < notification->content->numTexts; indx++) {
        if ((strlen(notification->content->texts[indx].key) == 0) || (strlen(notification->content->texts[indx].value) == 0)) {
            AJ_Printf("Language/Text can not be empty\n");
            AJ_MarshalCloseContainer(msg, &notTextArray);
            status = AJ_ERR_DISALLOWED;
            goto ErrorExit;
        }
        status = AJ_MarshalContainer(msg, &structArg, AJ_ARG_STRUCT);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalArgs(msg, "ss", notification->content->texts[indx].key, notification->content->texts[indx].value);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
        status = AJ_MarshalCloseContainer(msg, &structArg);
        if (status != AJ_OK) {
            goto ErrorExit;
        }
    }

    status = AJ_MarshalCloseContainer(msg, &notTextArray);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    return AJ_OK;

ErrorExit:

    AJ_Printf("MarshalNotification failed: '%s'\n", AJ_StatusText(status));
    return status;
}

/**
 * Send notify signal
 */
static AJ_Status Producer_SendNotifySignal(AJ_BusAttachment* busAttachment, AJNS_Notification* notification, uint32_t ttl, uint32_t* messageSerialNumber)
{
    AJ_Status status;
    AJ_Message msg;
    uint32_t serialNum;

    AJ_Printf("In SendNotifySignal\n");
    status = Producer_MarshalNotificationMsg(busAttachment, &msg, notification, ttl);
    if (status != AJ_OK) {
        AJ_Printf("Could not Marshal Message\n");
        return status;
    }
    serialNum = msg.hdr->serialNum;
    status = AJ_DeliverMsg(&msg);
    if (status != AJ_OK) {
        AJ_Printf("Could not Deliver Message\n");
        return status;
    }
    AJ_Printf("***************** Notification id %d delivered successfully with serial number %u *****************\n", notification->notificationId, serialNum);
    if (messageSerialNumber != NULL) {
        *messageSerialNumber = serialNum;
    }

    AJ_CloseMsg(&msg);

    return status;
}

/**
 * Send Notification - see notes in h file
 */
AJ_Status ProducerSendNotification(AJ_BusAttachment* busAttachment, AJNS_NotificationContent* content, uint16_t messageType, uint32_t ttl, uint32_t* messageSerialNumber)
{
    AJ_Status status;
    AJNS_Notification notification;
    uint32_t serialNumber;

    AJ_Printf("In SendNotification\n");

    notification.version = NotificationVersion;
    if (messageType >= NUM_MESSAGE_TYPES) {
        AJ_Printf("Could not Set Notification - MessageType is not valid\n");
        return AJ_ERR_DISALLOWED;
    }
    notification.messageType = messageType;

    if ((ttl < NOTIFICATION_TTL_MIN) || (ttl > NOTIFICATION_TTL_MAX)) {      //ttl is mandatory and must be in range
        AJ_Printf("TTL '%u' is not a valid TTL value\n", ttl);
        return AJ_ERR_DISALLOWED;
    }

    notification.deviceId = PropertyStore_GetValue(DeviceID);
    notification.deviceName = PropertyStore_GetValue(DeviceName);
    notification.appId = PropertyStore_GetValue(AppID);
    notification.appName = PropertyStore_GetValue(AppName);

    if ((notification.deviceId == 0) || (notification.deviceName == 0) ||
        (notification.appId == 0) || (notification.appName == 0)) {
        AJ_Printf("DeviceId/DeviceName/AppId/AppName can not be NULL\n");
        return AJ_ERR_DISALLOWED;
    }

    if ((strlen(notification.deviceId) == 0) || (strlen(notification.deviceName) == 0) ||
        (strlen(notification.appId) == 0) || (strlen(notification.appName) == 0)) {
        AJ_Printf("DeviceId/DeviceName/AppId/AppName can not be empty\n");
        return AJ_ERR_DISALLOWED;
    }

    if (notification.version > 1) {
        notification.originalSenderName = AJ_GetUniqueName(busAttachment);

        if (notification.originalSenderName == 0) {
            AJ_Printf("OriginalSender can not be NULL\n");
            return AJ_ERR_DISALLOWED;
        }

        if (strlen(notification.originalSenderName) == 0) {
            AJ_Printf("OriginalSender can not be empty\n");
            return AJ_ERR_DISALLOWED;
        }
    } else {
        notification.originalSenderName = NULL;
    }

    if (!notificationId) {
        AJ_Printf("Generating random number for notification id\n");
        AJ_RandBytes((uint8_t*)&notificationId, 4);
    }

    notification.notificationId = notificationId;
    notification.content = content;

    status = Producer_SendNotifySignal(busAttachment, &notification, ttl, &serialNumber);

    if (status == AJ_OK) {
        lastSentNotifications[messageType].msgId = notificationId++;
        lastSentNotifications[messageType].msgSerialNum = serialNumber;
        if (messageSerialNumber != NULL) {
            *messageSerialNumber = serialNumber;
        }
    }

    return status;
}

AJ_Status ProducerDeleteLastMsg(AJ_BusAttachment* busAttachment, uint16_t messageType)
{
    AJ_Printf("In DeleteLastMsg\n");
    AJ_Status status;

    if (messageType >= NUM_MESSAGE_TYPES) {
        AJ_Printf("Could not delete Notification - MessageType is not valid\n");
        return AJ_ERR_DISALLOWED;
    }

    uint32_t lastSentSerialNumber = lastSentNotifications[messageType].msgSerialNum;
    if (lastSentSerialNumber == 0) {
        AJ_Printf("Could not Delete Message - no message to delete\n");
        return AJ_OK;
    }

    status = AJ_BusCancelSessionless(busAttachment, lastSentSerialNumber);

    if (status != AJ_OK) {
        AJ_Printf("Could not Delete Message\n");
        return status;
    }

    lastSentNotifications[messageType].msgId = 0;
    lastSentNotifications[messageType].msgSerialNum = 0;

    AJ_Printf("***************** Message deleted successfully *****************\n");
    return status;
}

static AJ_Status ProducerCancelMessage(AJ_BusAttachment* busAttachment, int32_t msgId)
{
    AJ_Status status;
    uint16_t messageType = 0;

    AJ_Printf("In CancelMessage\n");

    if (msgId == 0) {
        AJ_Printf("Could not cancel Message - no message to cancel\n");
        return AJ_OK;
    }
    for (; messageType < NUM_MESSAGE_TYPES; messageType++) {
        if (lastSentNotifications[messageType].msgId == msgId) {
            break;
        }
    }
    if (messageType >= NUM_MESSAGE_TYPES) {
        AJ_Printf("Could not find matching Message serial number - no message to cancel\n");
        return AJ_OK;
    }

    status = AJ_BusCancelSessionless(busAttachment, lastSentNotifications[messageType].msgSerialNum);

    if (status != AJ_OK) {
        AJ_Printf("Failed to send cancelation\n");
        return status;
    }

    lastSentNotifications[messageType].msgId = 0;
    lastSentNotifications[messageType].msgSerialNum = 0;

    return status;
}

AJ_Status ProducerDismissMsg(AJ_BusAttachment* busAttachment, AJ_Message* msg)
{
    AJ_Printf("In DismissMsg\n");
    AJ_Status status;
    int32_t msgId;

    status = AJ_UnmarshalArgs(msg, "i", &msgId);
    if (status != AJ_OK) {
        AJ_Printf("Could not unmarshal message\n");
        return status;
    }

    const char* appId = PropertyStore_GetValue(AppID);
    status = NotificationSendDismiss(busAttachment, msgId, appId);
    if (status != AJ_OK) {
        return status;
    }

    status = ProducerCancelMessage(busAttachment, msgId);
    if (status != AJ_OK) {
        return status;
    }

    AJ_Printf("***************** Message dismissed successfully *****************\n");
    return status;
}

AJ_Status ProducerPropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    switch (propId) {
    case GET_EMERGENCY_NOTIFICATION_VERSION_PROPERTY:
    case GET_WARNING_NOTIFICATION_VERSION_PROPERTY:
    case GET_INFO_NOTIFICATION_VERSION_PROPERTY:
        status = AJ_MarshalArgs(replyMsg, "q", NotificationVersion);
        break;

    case GET_NOTIFICATION_PRODUCER_VERSION_PROPERTY:
        status = AJ_MarshalArgs(replyMsg, "q", NotificationProducerVersion);
        break;

    case GET_NOTIFICATION_DISMISSER_VERSION_PROPERTY:
        status = AJ_MarshalArgs(replyMsg, "q", NotificationDismisserVersion);
        break;
    }
    return status;
}

AJ_Status ProducerPropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    return AJ_ERR_DISALLOWED;
}
