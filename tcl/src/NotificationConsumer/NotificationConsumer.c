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

#include <alljoyn/notification/NotificationConsumer.h>

/**
 * Static constants.
 */
static const char SuperagentInterfaceName[]  = "org.alljoyn.Notification.Superagent";
static const char notificationMatch[] = "interface='org.alljoyn.Notification'";
static const char superAgentMatch[] = "interface='org.alljoyn.Notification.Superagent'";
static const char superAgentFilterMatch[] = "interface='org.alljoyn.Notification.Superagent',sender='";
static const char dismisserMatch[] = "interface='org.alljoyn.Notification.Dismisser'";

static const char* SuperagentInterface[] = {
    SuperagentInterfaceName,
    NotificationSignalName,
    NotificationPropertyVersion,
    NULL
};

/**
 * A NULL terminated collection of all interfaces.
 */
const AJ_InterfaceDescription SuperagentInterfaces[] = {
    AJ_PropertiesIface,
    SuperagentInterface,
    NULL
};

const AJ_InterfaceDescription AllInterfaces[] = {
    AJ_PropertiesIface,
    NotificationInterface,
    SuperagentInterface,
    NULL
};

const AJ_Object AllProxyObject          = { "*",   AllInterfaces };
const AJ_Object SuperAgentProxyObject   = { "*",   SuperagentInterfaces };
const AJ_Object NotificationProxyObject = { "*",   NotificationInterfaces };

static char currentSuperAgentBusName[16] = { '\0' };

struct keyValue textsRecd[NUMALLOWEDTEXTS], customAttributesRecd[NUMALLOWEDCUSTOMATTRIBUTES], richAudiosRecd[NUMALLOWEDRICHNOTS];

AJ_Status ConsumerSetSignalRules(AJ_BusAttachment* bus, uint8_t superAgentMode, const char* senderBusName)
{
    AJ_Status status = AJ_OK;

    AJ_Printf("In SetSignalRules()\n");
    AJ_Printf("Adding Dismisser interface match.\n");
    status = AJ_BusSetSignalRuleFlags(bus, dismisserMatch, AJ_BUS_SIGNAL_ALLOW, AJ_FLAG_NO_REPLY_EXPECTED);
    if (status != AJ_OK) {
        AJ_Printf("Could not set Dismisser Interface AddMatch\n");
        return status;
    }

    if (senderBusName == NULL) {
        AJ_Printf("Adding Notification interface match.\n");

        status = AJ_BusSetSignalRuleFlags(bus, notificationMatch, AJ_BUS_SIGNAL_ALLOW, AJ_FLAG_NO_REPLY_EXPECTED);
        if (status != AJ_OK) {
            AJ_Printf("Could not set Notification Interface AddMatch\n");
            return status;
        }

        if (currentSuperAgentBusName[0]) {
            AJ_Printf("Removing Superagent interface matched for specific sender bus name %s.\n", currentSuperAgentBusName);

            char senderMatch[76];
            size_t availableLen = sizeof(senderMatch);
            availableLen -= strlen(strncpy(senderMatch, superAgentFilterMatch, availableLen));
            availableLen -= strlen(strncat(senderMatch, currentSuperAgentBusName, availableLen));
            availableLen -= strlen(strncat(senderMatch, "'", availableLen));

            status = AJ_BusSetSignalRuleFlags(bus, senderMatch, AJ_BUS_SIGNAL_DENY, AJ_FLAG_NO_REPLY_EXPECTED);
            if (status != AJ_OK) {
                AJ_Printf("Could not remove SuperAgent specific match\n");
                return status;
            }

            status = AJ_BusFindAdvertisedName(bus, currentSuperAgentBusName, AJ_BUS_STOP_FINDING);
            if (status != AJ_OK) {
                AJ_Printf("Could not unregister to find advertised name of lost SuperAgent\n");
                return status;
            }

            currentSuperAgentBusName[0] = '\0'; // Clear current SuperAgent BusUniqueName
        }

        if (superAgentMode) {
            AJ_Printf("Adding Superagent interface match.\n");
            status = AJ_BusSetSignalRuleFlags(bus, superAgentMatch, AJ_BUS_SIGNAL_ALLOW, AJ_FLAG_NO_REPLY_EXPECTED);
            if (status != AJ_OK) {
                AJ_Printf("Could not set Notification Interface AddMatch\n");
                return status;
            }
        }
    } else {
        AJ_Printf("Running SetSignalRules with sender bus name.\n");

        AJ_Printf("Removing Notification interface match.\n");
        status = AJ_BusSetSignalRuleFlags(bus, notificationMatch, AJ_BUS_SIGNAL_DENY, AJ_FLAG_NO_REPLY_EXPECTED);
        if (status != AJ_OK) {
            AJ_Printf("Could not remove Notification Interface match\n");
            return status;
        }

        char senderMatch[76];
        size_t availableLen = sizeof(senderMatch);
        availableLen -= strlen(strncpy(senderMatch, superAgentFilterMatch, availableLen));
        availableLen -= strlen(strncat(senderMatch, senderBusName, availableLen));
        availableLen -= strlen(strncat(senderMatch, "'", availableLen));

        AJ_Printf("Adding Superagent interface matched for specific sender bus name %s.\n", senderBusName);

        status = AJ_BusSetSignalRuleFlags(bus, senderMatch, AJ_BUS_SIGNAL_ALLOW, AJ_FLAG_NO_REPLY_EXPECTED);
        if (status != AJ_OK) {
            AJ_Printf("Could not add SuperAgent specific match\n");
            return status;
        }

        status = AJ_BusFindAdvertisedName(bus, senderBusName, AJ_BUS_START_FINDING);
        if (status != AJ_OK) {
            AJ_Printf("Could not register to find advertised name of SuperAgent\n");
            return status;
        }

        strncpy(currentSuperAgentBusName, senderBusName, 16); // Save current SuperAgent BusUniqueName

        AJ_Printf("Removing Superagent interface match.\n");
        status = AJ_BusSetSignalRuleFlags(bus, superAgentMatch, AJ_BUS_SIGNAL_DENY, AJ_FLAG_NO_REPLY_EXPECTED);
        if (status != AJ_OK) {
            AJ_Printf("Could not remove SuperAgent Interface match\n");
            return status;
        }
    }

    return status;
}

uint8_t ConsumerIsSuperAgentLost(AJ_Message* msg)
{
    if (msg->msgId == AJ_SIGNAL_LOST_ADV_NAME) {
        AJ_Arg arg;
        AJ_UnmarshalArg(msg, &arg); // <arg name="name" type="s" direction="out"/>
        AJ_SkipArg(msg);            // <arg name="transport" type="q" direction="out"/>
        AJ_SkipArg(msg);            // <arg name="prefix" type="s" direction="out"/>
        AJ_ResetArgs(msg);          // Reset to allow others to re-unmarshal message
        AJ_Printf("LostAdvertisedName(%s)\n", arg.val.v_string);
        return (strcmp(arg.val.v_string, currentSuperAgentBusName) == 0);
    }
    return FALSE;
}

AJ_Status ConsumerNotifySignalHandler(AJ_Message* msg)
{
    AJ_Status status;
    Notification_t notification;

    char appId[UUID_LENGTH * 2 + 1];
    AJ_Arg appIdArray;
    size_t appIdLen;

    AJ_Arg attrbtArray;
    AJ_Arg customAttributeArray;
    AJ_Arg notTextArray;
    AJ_Arg richAudioArray;

    memset(&notification, 0, sizeof(Notification_t));

    AJ_Printf("Received notification signal from sender %s\n", msg->sender);

    if (status = AJ_UnmarshalArgs(msg, "q", &notification.header.version) != AJ_OK)
        goto Exit;


    if (status = AJ_UnmarshalArgs(msg, "i", &notification.header.notificationId) != AJ_OK)
        goto Exit;


    if (status = AJ_UnmarshalArgs(msg, "q", &notification.header.messageType) != AJ_OK)
        goto Exit;


    if (status = AJ_UnmarshalArgs(msg, "s", &notification.header.deviceId) != AJ_OK)
        goto Exit;


    if (status = AJ_UnmarshalArgs(msg, "s", &notification.header.deviceName) != AJ_OK)
        goto Exit;


    if (status = AJ_UnmarshalArg(msg, &appIdArray) != AJ_OK)
        goto Exit;

    appIdLen = ((appIdArray.len > UUID_LENGTH) ? UUID_LENGTH : appIdArray.len) * 2 + 1;

    if (status = AJ_RawToHex(appIdArray.val.v_byte, appIdArray.len, appId, appIdLen, FALSE) != AJ_OK)
        goto Exit;
    notification.header.appId = appId;


    if (status = AJ_UnmarshalArgs(msg, "s", &notification.header.appName) != AJ_OK)
        goto Exit;


    if (status = AJ_UnmarshalContainer(msg, &attrbtArray, AJ_ARG_ARRAY) != AJ_OK)
        goto Exit;

    while (1) {
        AJ_Arg dictArg;
        int32_t attrbtKey;
        const char* variantSig;

        status = AJ_UnmarshalContainer(msg, &dictArg, AJ_ARG_DICT_ENTRY);
        if (status == AJ_ERR_NO_MORE) {
            if (status = AJ_UnmarshalCloseContainer(msg, &attrbtArray) != AJ_OK)
                goto Exit;
            else
                break;
        } else if (status) {
            goto Exit;
        }

        if (status = AJ_UnmarshalArgs(msg, "i", &attrbtKey) != AJ_OK)
            goto Exit;

        switch (attrbtKey) {
        case RICH_CONTENT_ICON_URL_ATTRIBUTE_KEY:
        {
            if (status = AJ_UnmarshalVariant(msg, &variantSig) != AJ_OK)
                goto Exit;
            if (status = AJ_UnmarshalArgs(msg, "s", &notification.content.richIconUrl) != AJ_OK)
                goto Exit;
        }
        break;

        case RICH_CONTENT_ICON_OBJECT_PATH_ATTRIBUTE_KEY:
        {
            if (status = AJ_UnmarshalVariant(msg, &variantSig) != AJ_OK)
                goto Exit;
            if (status = AJ_UnmarshalArgs(msg, "s", &notification.content.richIconObjectPath) != AJ_OK)
                goto Exit;
        }
        break;

        case RICH_CONTENT_AUDIO_OBJECT_PATH_ATTRIBUTE_KEY:
        {
            if (status = AJ_UnmarshalVariant(msg, &variantSig) != AJ_OK)
                goto Exit;
            if (status = AJ_UnmarshalArgs(msg, "s", &notification.content.richAudioObjectPath) != AJ_OK)
                goto Exit;
        }
        break;

        case CONTROLPANELSERVICE_OBJECT_PATH_ATTRIBUTE_KEY:
        {
            if (status = AJ_UnmarshalVariant(msg, &variantSig) != AJ_OK)
                goto Exit;
            if (status = AJ_UnmarshalArgs(msg, "s", &notification.content.controlPanelServiceObjectPath) != AJ_OK)
                goto Exit;
        }
        break;

        case ORIGINAL_SENDER_NAME_ATTRIBUTE_KEY:
        {
            if (status = AJ_UnmarshalVariant(msg, &variantSig) != AJ_OK)
                goto Exit;
            if (status = AJ_UnmarshalArgs(msg, "s", &notification.header.originalSenderName) != AJ_OK)
                goto Exit;
        }
        break;

        case RICH_CONTENT_AUDIO_URL_ATTRIBUTE_KEY:
        {
            if (status = AJ_UnmarshalVariant(msg, &variantSig) != AJ_OK)
                goto Exit;
            if (status = AJ_UnmarshalContainer(msg, &richAudioArray, AJ_ARG_ARRAY) != AJ_OK)
                goto Exit;

            while (1) {
                AJ_Arg structArg;
                char* urlLanguage;
                char* urlText;

                status = AJ_UnmarshalContainer(msg, &structArg, AJ_ARG_STRUCT);
                if (status == AJ_ERR_NO_MORE) {
                    if (status = AJ_UnmarshalCloseContainer(msg, &richAudioArray) != AJ_OK)
                        goto Exit;
                    else
                        break;
                } else if (status) {
                    goto Exit;
                }

                if (status = AJ_UnmarshalArgs(msg, "ss", &urlLanguage, &urlText) != AJ_OK)
                    goto Exit;
                if (notification.content.numAudioUrls < NUMALLOWEDRICHNOTS) {             // if it doesn't fit we just skip
                    richAudiosRecd[notification.content.numAudioUrls].key   = urlLanguage;
                    richAudiosRecd[notification.content.numAudioUrls].value = urlText;
                    notification.content.numAudioUrls++;
                }

                if (status = AJ_UnmarshalCloseContainer(msg, &structArg) != AJ_OK)
                    goto Exit;
            }
            notification.content.richAudioUrls = richAudiosRecd;
        }
        break;

        default:
            AJ_Printf("Unknown argument - skipping\n");
            status = AJ_SkipArg(msg);
            if (status != AJ_OK) {
                AJ_Printf("Error could not skip argument\n");
                return status;
            }
        }
        if (status = AJ_UnmarshalCloseContainer(msg, &dictArg) != AJ_OK)
            goto Exit;
    }

    if (status = AJ_UnmarshalContainer(msg, &customAttributeArray, AJ_ARG_ARRAY) != AJ_OK)
        goto Exit;

    while (1) {
        AJ_Arg customAttributeDictArg;
        char* customKey;
        char* customVal;

        status = AJ_UnmarshalContainer(msg, &customAttributeDictArg, AJ_ARG_DICT_ENTRY);
        if (status == AJ_ERR_NO_MORE) {
            if (status = AJ_UnmarshalCloseContainer(msg, &customAttributeArray) != AJ_OK)
                goto Exit;
            else
                break;
        } else if (status) {
            goto Exit;
        }

        if (status = AJ_UnmarshalArgs(msg, "ss", &customKey, &customVal) != AJ_OK)
            goto Exit;

        if (notification.content.numCustomAttributes < NUMALLOWEDCUSTOMATTRIBUTES) {     // if it doesn't fit we just skip
            customAttributesRecd[notification.content.numCustomAttributes].key   = customKey;
            customAttributesRecd[notification.content.numCustomAttributes].value = customVal;
            notification.content.numCustomAttributes++;
        }

        if (status = AJ_UnmarshalCloseContainer(msg, &customAttributeDictArg) != AJ_OK)
            goto Exit;
    }
    notification.content.customAttributes = customAttributesRecd;

    if (status = AJ_UnmarshalContainer(msg, &notTextArray, AJ_ARG_ARRAY) != AJ_OK)
        goto Exit;

    while (1) {
        AJ_Arg structArg;
        char* notificationLanguage;
        char* notificationText;

        status = AJ_UnmarshalContainer(msg, &structArg, AJ_ARG_STRUCT);
        if (status == AJ_ERR_NO_MORE) {
            if (status = AJ_UnmarshalCloseContainer(msg, &notTextArray) != AJ_OK)
                goto Exit;
            else
                break;
        } else if (status) {
            goto Exit;
        }

        if (status = AJ_UnmarshalArgs(msg, "ss", &notificationLanguage, &notificationText) != AJ_OK)
            goto Exit;

        if (notification.content.numTexts < NUMALLOWEDTEXTS) {     // if it doesn't fit we just skip
            textsRecd[notification.content.numTexts].key   = notificationLanguage;
            textsRecd[notification.content.numTexts].value = notificationText;
            notification.content.numTexts++;
        }

        if (status = AJ_UnmarshalCloseContainer(msg, &structArg) != AJ_OK)
            goto Exit;
    }
    notification.content.texts = textsRecd;

Exit:

    if (status != AJ_OK) {
        AJ_Printf("Handle Notification failed: '%s'\n", AJ_StatusText(status));
    } else {
        status = ApplicationHandleNotify(&notification);
    }
    return status;
}

AJ_Status ConsumerDismissSignalHandler(AJ_Message* msg)
{
    AJ_Status status;

    uint32_t notificationId = 0;
    char appId[UUID_LENGTH * 2 + 1];

    AJ_Arg appIdArray;


    if (status = AJ_UnmarshalArgs(msg, "i", &notificationId) != AJ_OK)
        return status;

    if (status = AJ_UnmarshalArg(msg, &appIdArray) != AJ_OK)
        return status;

    size_t appIdLen = ((appIdArray.len > UUID_LENGTH) ? UUID_LENGTH : appIdArray.len) * 2 + 1;

    if (status = AJ_RawToHex(appIdArray.val.v_byte, appIdArray.len, appId, appIdLen, FALSE) != AJ_OK)
        return status;

    ApplicationHandleDismiss(notificationId, appId);

    return status;
}

AJ_Status ConsumerAcknowledgeNotification(AJ_BusAttachment* bus, uint16_t version, int32_t msgId, const char* senderName, uint32_t sessionId)
{
    AJ_Printf("Inside AcknowledgeNotification()\n");
    AJ_Status status = AJ_OK;

    if (version < 2) { // Producer does not support acknowledgement
        return AJ_ERR_INVALID;
    }

    if (status == AJ_OK && sessionId != 0) {
        AJ_Message ackMsg;
        status = AJ_MarshalMethodCall(bus, &ackMsg, NOTIFICATION_PRODUCER_ACKNOWLEDGE_PROXY, senderName, sessionId, AJ_NO_FLAGS, AJ_CALL_TIMEOUT);
        if (status != AJ_OK) {
            AJ_Printf("Could not marshal method call\n");
            return status;
        }
        status = AJ_MarshalArgs(&ackMsg, "i", msgId);
        if (status != AJ_OK) {
            AJ_Printf("Could not marshal arguments\n");
            return status;
        }
        status = AJ_DeliverMsg(&ackMsg);
        if (status != AJ_OK) {
            AJ_Printf("Could not deliver message\n");
            return status;
        }
        AJ_CloseMsg(&ackMsg);
    }
    return status;
}

AJ_Status ConsumerDismissNotification(AJ_BusAttachment* bus, uint16_t version, int32_t msgId, const char* appId, const char* senderName, uint32_t sessionId)
{
    AJ_Printf("Inside DismissNotification()\n");
    AJ_Status status = AJ_OK;

    if (version < 2) { // Producer does not support dismissal but other consumers might so send Dismiss signal
        status = NotificationSendDismiss(msgId, appId);
        return status;
    }

    if (status == AJ_OK && sessionId != 0) {
        AJ_Message dismissMsg;
        status = AJ_MarshalMethodCall(bus, &dismissMsg, NOTIFICATION_PRODUCER_DISMISS_PROXY, senderName, sessionId, AJ_NO_FLAGS, AJ_CALL_TIMEOUT);
        if (status != AJ_OK) {
            AJ_Printf("Could not marshal method call\n");
            return status;
        }
        status = AJ_MarshalArgs(&dismissMsg, "i", msgId);
        if (status != AJ_OK) {
            AJ_Printf("Could not marshal arguments\n");
            return status;
        }
        status = AJ_DeliverMsg(&dismissMsg);
        if (status != AJ_OK) {
            AJ_Printf("Could not deliver message\n");
            return status;
        }
        AJ_CloseMsg(&dismissMsg);
    } else if (status != AJ_ERR_READ) { // Failed to perform in-session dismissal against producer so send Dismiss signal
        status = NotificationSendDismiss(msgId, appId);
    }

    return status;
}

