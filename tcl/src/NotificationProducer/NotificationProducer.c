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

#include <alljoyn/notification/common.h>
#include <alljoyn/notification/NotificationProducer.h>
#include <alljoyn/services_common/PropertyStore.h>
#include <aj_crypto.h>
#include <aj_debug.h>

/**
 * Static non consts.
 */
static uint8_t isMessageSet   = FALSE;
static int8_t lastMessageType;
static uint32_t messageId = 0;
static AJ_Message msg;
static uint32_t LastSerialNum[NUM_MESSAGE_TYPES] = { 0, 0, 0 };

/**
 * Static constants.
 */
const char ServicePathEmergency[]   = "/emergency";
const char ServicePathWarning[]     = "/warning";
const char ServicePathInfo[]        = "/info";

static const uint16_t TTL_MIN   = 30;
static const uint16_t TTL_MAX   = 43200;

/**
 * Set Notification - see notes in h file
 */
AJ_Status ProducerSetNotification(notification*notificationContent)
{
    AJ_Printf("In SetNotification\n");
    AJ_Status status;

    const char* deviceId = PropertyStore_GetValue(DeviceID);
    const char* deviceName = PropertyStore_GetValue(DeviceName);
    const char* appId = PropertyStore_GetValue(AppID);
    const char* appName = PropertyStore_GetValue(AppName);

    if ((deviceId == 0) || (deviceName == 0) ||
        (appId == 0) || (appName == 0)) {
        AJ_Printf("DeviceId/DeviceName/AppId/AppName can not be NULL\n");
        return AJ_ERR_DISALLOWED;
    }

    if ((strlen(deviceId) == 0) || (strlen(deviceName) == 0) ||
        (strlen(appId) == 0) || (strlen(appName) == 0)) {
        AJ_Printf("DeviceId/DeviceName/AppId/AppName can not be empty\n");
        return AJ_ERR_DISALLOWED;
    }

    if (notificationContent->messageType >= NUM_MESSAGE_TYPES) {
        AJ_Printf("Could not Set Notification - MessageType is not valid\n");
        return AJ_ERR_DISALLOWED;
    }
    if ((notificationContent->ttl < TTL_MIN) || (notificationContent->ttl > TTL_MAX)) {      //ttl is mandatory and must be in range
        AJ_Printf("TTL '%d' is not a valid TTL value", notificationContent->ttl);
        return AJ_ERR_DISALLOWED;
    }

    status = AJ_MarshalSignal(&busAttachment, &msg, AJ_APP_MESSAGE_ID(notificationContent->messageType + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 1, 0), NULL, 0, ALLJOYN_FLAG_SESSIONLESS, notificationContent->ttl);
    if (status != AJ_OK) {
        AJ_Printf("Could not Marshal Signal\n");
        return status;
    }

    do {
        AJ_Arg attrbtArray;
        AJ_Arg appIdArg;
        AJ_Arg customAttributeArray;
        AJ_Arg notTextArray;
        AJ_Arg richAudioArray;
        AJ_Arg dictArg;
        AJ_Arg customAttributeDictArg;
        AJ_Arg structArg;
        AJ_Arg audioStructArg;
        AJ_Arg richAudioAttrArray;

        ///////////////////       Proto     /////////////////////

        CHECK(AJ_MarshalArgs(&msg, "q", version));

        ///////////////////    NotificationId    /////////////////////
        if (!messageId)
            AJ_RandBytes((uint8_t*)&messageId, 4);
        CHECK(AJ_MarshalArgs(&msg, "i", messageId));

        ///////////////////    MessageType   ////////////////////////////

        CHECK(AJ_MarshalArgs(&msg, "q", notificationContent->messageType));

        ///////////////////    DeviceId   ////////////////////////////

        CHECK(AJ_MarshalArgs(&msg, "s", deviceId));

        ///////////////////    DeviceName   ////////////////////////////

        CHECK(AJ_MarshalArgs(&msg, "s", deviceName));

        ///////////////////    AppId   ////////////////////////////

        uint8_t binAppId[UUID_LENGTH];
        uint32_t sz = strlen(appId);
        if (sz > UUID_LENGTH * 2)
            sz = UUID_LENGTH * 2;
        AJ_HexToRaw(appId, sz, binAppId, UUID_LENGTH);

        AJ_InitArg(&appIdArg, AJ_ARG_BYTE, AJ_ARRAY_FLAG, binAppId, UUID_LENGTH);

        CHECK(AJ_MarshalArg(&msg, &appIdArg));

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

            CHECK(AJ_MarshalCloseContainer(&msg, &richAudioAttrArray));

            if (status)
                break;

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
        lastMessageType = notificationContent->messageType;

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

    AJ_Printf("***************** Message id %d delivered successfully *****************\n", messageId);
    messageId++;
    LastSerialNum[lastMessageType] = serialNum;

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

    if (LastSerialNum[messageType] == 0) {
        AJ_Printf("Could not Delete Message - no message to delete\n");
        return AJ_OK;
    }

    status = AJ_BusCancelSessionless(&busAttachment, LastSerialNum[messageType]);

    if (status != AJ_OK) {
        AJ_Printf("Could not Delete Message\n");
        return status;
    }

    LastSerialNum[messageType] = 0;

    AJ_Printf("***************** Message deleted successfully *****************\n");
    return status;
}

AJ_Status ProducerPropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    switch (propId) {
    case GET_EMERGENCY_NOTIFICATION_VERSION_PROPERTY:
    case GET_WARNING_NOTIFICATION_VERSION_PROPERTY:
    case GET_INFO_NOTIFICATION_VERSION_PROPERTY:
        status = AJ_MarshalArgs(replyMsg, "q", version);
        break;
    }
    return status;
}

AJ_Status ProducerPropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    return AJ_ERR_DISALLOWED;
}
