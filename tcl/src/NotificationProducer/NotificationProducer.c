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
static uint8_t isMessageSet   = FALSE;
static int8_t lastMessageType;
static uint32_t nextNotificationId = 0;
static AJ_Message msg;

typedef struct _NotificationMessageTracking {
    uint32_t msgId;
    uint32_t msgSerialNum;
} NotificationMessageTracking_t;

static NotificationMessageTracking_t LastSentNotifications[NUM_MESSAGE_TYPES] = { { 0, 0 }, { 0, 0 }, { 0, 0 } };

/**
 * Static constants.
 */
const char NotificationObjectPathEmergency[]   = "/emergency";
const char NotificationObjectPathWarning[]     = "/warning";
const char NotificationObjectPathInfo[]        = "/info";

/**
 * Set Notification - see notes in h file
 */
AJ_Status ProducerSetNotification(NotificationContent_t* notificationContent, uint16_t messageType, uint16_t ttl)
{
    AJ_Printf("In SetNotification\n");
    AJ_Status status;

    const char* deviceId = PropertyStore_GetValue(DeviceID);
    const char* deviceName = PropertyStore_GetValue(DeviceName);
    const char* appId = PropertyStore_GetValue(AppID);
    const char* appName = PropertyStore_GetValue(AppName);
    const char* originalSenderName = AJ_GetUniqueName(&busAttachment);

    if ((deviceId == 0) || (deviceName == 0) ||
        (appId == 0) || (appName == 0) ||
        (originalSenderName == 0)) {
        AJ_Printf("DeviceId/DeviceName/AppId/AppName/OriginalSender can not be NULL\n");
        return AJ_ERR_DISALLOWED;
    }

    if ((strlen(deviceId) == 0) || (strlen(deviceName) == 0) ||
        (strlen(appId) == 0) || (strlen(appName) == 0) ||
        (strlen(originalSenderName) == 0)) {
        AJ_Printf("DeviceId/DeviceName/AppId/AppName/OriginalSender can not be empty\n");
        return AJ_ERR_DISALLOWED;
    }

    if (messageType >= NUM_MESSAGE_TYPES) {
        AJ_Printf("Could not Set Notification - MessageType is not valid\n");
        return AJ_ERR_DISALLOWED;
    }

    if ((ttl < NOTIFICATION_TTL_MIN) || (ttl > NOTIFICATION_TTL_MAX)) {      //ttl is mandatory and must be in range
        AJ_Printf("TTL '%d' is not a valid TTL value\n", ttl);
        return AJ_ERR_DISALLOWED;
    }

    status = AJ_MarshalSignal(&busAttachment, &msg, AJ_APP_MESSAGE_ID(messageType + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 1, 0), NULL, 0, ALLJOYN_FLAG_SESSIONLESS, ttl);
    if (status != AJ_OK) {
        AJ_Printf("Could not Marshal Signal\n");
        return status;
    }

    do {
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
        CHECK(AJ_MarshalArgs(&msg, "q", NotificationVersion));

        ///////////////////    MessgeId    /////////////////////
        if (!nextNotificationId)
            AJ_RandBytes((uint8_t*)&nextNotificationId, 4);
        CHECK(AJ_MarshalArgs(&msg, "i", nextNotificationId));

        ///////////////////    MessageType   ////////////////////////////

        CHECK(AJ_MarshalArgs(&msg, "q", messageType));

        ///////////////////    DeviceId   ////////////////////////////

        CHECK(AJ_MarshalArgs(&msg, "s", deviceId));

        ///////////////////    DeviceName   ////////////////////////////

        CHECK(AJ_MarshalArgs(&msg, "s", deviceName));

        ///////////////////    AppId   ////////////////////////////

        CHECK(Common_MarshalAppId(&msg, appId));

        ///////////////////    AppName   ////////////////////////////

        CHECK(AJ_MarshalArgs(&msg, "s", appName));

        ///////////////////    Attributes   ////////////////////////////

        CHECK(AJ_MarshalContainer(&msg, &attrbtArray, AJ_ARG_ARRAY));

        if (notificationContent->richIconUrl != 0) {
            AJ_MarshalContainer(&msg, &dictArg, AJ_ARG_DICT_ENTRY);
            AJ_MarshalArgs(&msg, "i", RICH_CONTENT_ICON_URL_ATTRIBUTE_KEY);
            AJ_MarshalVariant(&msg, "s");
            AJ_MarshalArgs(&msg, "s", notificationContent->richIconUrl);
            AJ_MarshalCloseContainer(&msg, &dictArg);
        }
        int8_t indx;
        if (notificationContent->numAudioUrls > 0) {
            CHECK(AJ_MarshalContainer(&msg, &richAudioArray, AJ_ARG_DICT_ENTRY));

            AJ_MarshalArgs(&msg, "i", RICH_CONTENT_AUDIO_URL_ATTRIBUTE_KEY);
            AJ_MarshalVariant(&msg, "a(ss)");

            CHECK(AJ_MarshalContainer(&msg, &richAudioAttrArray, AJ_ARG_ARRAY));

            for (indx = 0; indx < notificationContent->numAudioUrls; indx++) {
                if ((strlen(notificationContent->richAudioUrls[indx].key) == 0) || (strlen(notificationContent->richAudioUrls[indx].value) == 0)) {
                    AJ_Printf("Rich Audio Language/Url can not be empty\n");
                    AJ_MarshalCloseContainer(&msg, &richAudioArray);
                    status = AJ_ERR_DISALLOWED;
                    break;
                }
                CHECK(AJ_MarshalContainer(&msg, &audioStructArg, AJ_ARG_STRUCT));
                CHECK(AJ_MarshalArgs(&msg, "ss", notificationContent->richAudioUrls[indx].key, notificationContent->richAudioUrls[indx].value));
                CHECK(AJ_MarshalCloseContainer(&msg, &audioStructArg));
            }
            if (status)
                break;

            CHECK(AJ_MarshalCloseContainer(&msg, &richAudioAttrArray));

            CHECK(AJ_MarshalCloseContainer(&msg, &richAudioArray));
        }
        if (status)
            break;

        if (notificationContent->richIconObjectPath != 0) {
            AJ_MarshalContainer(&msg, &dictArg, AJ_ARG_DICT_ENTRY);
            AJ_MarshalArgs(&msg, "i", RICH_CONTENT_ICON_OBJECT_PATH_ATTRIBUTE_KEY);
            AJ_MarshalVariant(&msg, "s");
            AJ_MarshalArgs(&msg, "s", notificationContent->richIconObjectPath);
            AJ_MarshalCloseContainer(&msg, &dictArg);
        }

        if (notificationContent->richAudioObjectPath != 0) {
            AJ_MarshalContainer(&msg, &dictArg, AJ_ARG_DICT_ENTRY);
            AJ_MarshalArgs(&msg, "i", RICH_CONTENT_AUDIO_OBJECT_PATH_ATTRIBUTE_KEY);
            AJ_MarshalVariant(&msg, "s");
            AJ_MarshalArgs(&msg, "s", notificationContent->richAudioObjectPath);
            AJ_MarshalCloseContainer(&msg, &dictArg);
        }

        if (notificationContent->controlPanelServiceObjectPath != 0) {
            AJ_MarshalContainer(&msg, &dictArg, AJ_ARG_DICT_ENTRY);
            AJ_MarshalArgs(&msg, "i", CONTROLPANELSERVICE_OBJECT_PATH_ATTRIBUTE_KEY);
            AJ_MarshalVariant(&msg, "s");
            AJ_MarshalArgs(&msg, "s", notificationContent->controlPanelServiceObjectPath);
            AJ_MarshalCloseContainer(&msg, &dictArg);
        }

        AJ_MarshalContainer(&msg, &dictArg, AJ_ARG_DICT_ENTRY);
        AJ_MarshalArgs(&msg, "i", ORIGINAL_SENDER_NAME_ATTRIBUTE_KEY);
        AJ_MarshalVariant(&msg, "s");
        AJ_MarshalArgs(&msg, "s", originalSenderName);
        AJ_MarshalCloseContainer(&msg, &dictArg);

        CHECK(AJ_MarshalCloseContainer(&msg, &attrbtArray));

        ///////////////////    Custom Attributes   ///////////////////

        CHECK(AJ_MarshalContainer(&msg, &customAttributeArray, AJ_ARG_ARRAY));

        for (indx = 0; indx < notificationContent->numCustomAttributes; indx++) {
            CHECK(AJ_MarshalContainer(&msg, &customAttributeDictArg, AJ_ARG_DICT_ENTRY));
            CHECK(AJ_MarshalArgs(&msg, "ss", notificationContent->customAttributes[indx].key, notificationContent->customAttributes[indx].value));
            CHECK(AJ_MarshalCloseContainer(&msg, &customAttributeDictArg));
        }
        if (status)
            break;

        CHECK(AJ_MarshalCloseContainer(&msg, &customAttributeArray));

        ///////////////////   Notifications   ////////////////////////////

        CHECK(AJ_MarshalContainer(&msg, &notTextArray, AJ_ARG_ARRAY));

        for (indx = 0; indx < notificationContent->numTexts; indx++) {
            if ((strlen(notificationContent->texts[indx].key) == 0) || (strlen(notificationContent->texts[indx].value) == 0)) {
                AJ_Printf("Language/Text can not be empty\n");
                AJ_MarshalCloseContainer(&msg, &notTextArray);
                status = AJ_ERR_DISALLOWED;
                break;
            }
            CHECK(AJ_MarshalContainer(&msg, &structArg, AJ_ARG_STRUCT));
            CHECK(AJ_MarshalArgs(&msg, "ss", notificationContent->texts[indx].key, notificationContent->texts[indx].value));
            CHECK(AJ_MarshalCloseContainer(&msg, &structArg));
        }
        if (status)
            break;

        CHECK(AJ_MarshalCloseContainer(&msg, &notTextArray));

        isMessageSet = TRUE;
        lastMessageType = messageType;

        return AJ_OK;
    } while (0);

    AJ_Printf("SetNotification failed: '%s'\n", AJ_StatusText(status));
    isMessageSet = FALSE;
    return status;
}

/**
 * Set Notification - see notes in h file
 */
AJ_Status ProducerSendNotifications()
{
    AJ_Printf("In SendNotification\n");
    AJ_Status status;

    if (!isMessageSet) {
        AJ_Printf("Could not Send Signal - Message is not set\n");
        return AJ_ERR_DISALLOWED;
    }

    if (!isBusConnected) {
        AJ_Printf("Could not Send Signal - Bus is not connected\n");
        return AJ_ERR_DISALLOWED;
    }

    uint32_t serialNum = msg.hdr->serialNum;
    status = AJ_DeliverMsg(&msg);
    if (status != AJ_OK) {
        AJ_Printf("Could not Deliver Message\n");
        return status;
    }

    AJ_Printf("***************** Notification id %u delivered successfully *****************\n", nextNotificationId);
    LastSentNotifications[lastMessageType].msgId = nextNotificationId;
    LastSentNotifications[lastMessageType].msgSerialNum = serialNum;
    nextNotificationId++;

    AJ_CloseMsg(&msg);
    isMessageSet = FALSE;
    return status;
}

AJ_Status ProducerDeleteLastMsg(uint16_t messageType)
{
    AJ_Printf("In DeleteLastMsg\n");
    AJ_Status status;

    if (messageType >= NUM_MESSAGE_TYPES) {
        AJ_Printf("Could not delete Notification - MessageType is not valid\n");
        return AJ_ERR_DISALLOWED;
    }

    uint32_t lastSentSerialNumber = LastSentNotifications[messageType].msgSerialNum;
    if (lastSentSerialNumber == 0) {
        AJ_Printf("Could not Delete Message - no message to delete\n");
        return AJ_OK;
    }

    status = AJ_BusCancelSessionless(&busAttachment, lastSentSerialNumber);

    if (status != AJ_OK) {
        AJ_Printf("Could not Delete Message\n");
        return status;
    }

    LastSentNotifications[messageType].msgId = 0;
    LastSentNotifications[messageType].msgSerialNum = 0;

    AJ_Printf("***************** Message deleted successfully *****************\n");
    return status;
}

static AJ_Status ProducerCancelMessage(int32_t msgId)
{
    AJ_Status status;
    uint16_t messageType = 0;

    AJ_Printf("In CancelMessage\n");

    if (msgId == 0) {
        AJ_Printf("Could not cancel Message - no message to cancel\n");
        return AJ_OK;
    }
    for (; messageType < NUM_MESSAGE_TYPES; messageType++) {
        if (LastSentNotifications[messageType].msgId == msgId) {
            break;
        }
    }
    if (messageType >= NUM_MESSAGE_TYPES) {
        AJ_Printf("Could not find matching Message serial number - no message to cancel\n");
        return AJ_OK;
    }

    status = AJ_BusCancelSessionless(&busAttachment, LastSentNotifications[messageType].msgSerialNum);

    if (status != AJ_OK) {
        AJ_Printf("Failed to send cancelation\n");
        return status;
    }

    LastSentNotifications[messageType].msgId = 0;
    LastSentNotifications[messageType].msgSerialNum = 0;

    return status;
}

AJ_Status ProducerAcknowledgeMsg(AJ_Message*msg)
{
    AJ_Printf("In AcknowledgeMsg\n");
    AJ_Status status;
    int32_t msgId;

    status = AJ_UnmarshalArgs(msg, "i", &msgId);
    if (status != AJ_OK) {
        AJ_Printf("Could not unmarshal message\n");
        return status;
    }

    status = ProducerCancelMessage(msgId);
    if (status != AJ_OK) {
        return status;
    }

    AJ_Printf("***************** Message acknowledged successfully *****************\n");
    return status;
}

AJ_Status ProducerDismissMsg(AJ_Message* msg)
{
    AJ_Printf("In DismissMsg\n");
    AJ_Status status;
    int32_t msgId;

    status = AJ_UnmarshalArgs(msg, "i", &msgId);
    if (status != AJ_OK) {
        AJ_Printf("Could not unmarshal message\n");
        return status;
    }

    status = ProducerCancelMessage(msgId);
    if (status != AJ_OK) {
        return status;
    }

    const char* appId = PropertyStore_GetValue(AppID);
    status = NotificationSendDismiss(msgId, appId);
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
