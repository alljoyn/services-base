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

#include <alljoyn/notification/NotificationConsumer.h>

/**
 * Static constants.
 */
static const char SuperagentInterfaceName[]  = "org.alljoyn.Notification.Superagent";
static const char notificationMatch[] = "interface='org.alljoyn.Notification'";
static const char superAgentMatch[] = "interface='org.alljoyn.Notification.Superagent'";
static const char superAgentFilterMatch[] = "interface='org.alljoyn.Notification.Superagent',sender='";

static const char* SuperagentInterface[] = {
    SuperagentInterfaceName,
    SignalName,
    Version,
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

const AJ_Object AllAppObject          = { "*",   AllInterfaces };
const AJ_Object SuperAgentObject      = { "*",   SuperagentInterfaces };
const AJ_Object NotificationAppObject = { "*",   NotificationInterfaces };

struct keyValue textsRecd[NUMALLOWEDTEXTS], customAttributesRecd[NUMALLOWEDCUSTOMATTRIBUTES], richAudiosRecd[NUMALLOWEDRICHNOTS];

AJ_Status ConsumerSetSignalRules(AJ_BusAttachment* bus, int8_t superAgentMode, const char* senderBusName)
{
    AJ_Status status;

    AJ_Printf("Running SetSignalRules.\n");
    if (senderBusName == 0) {
        AJ_Printf("Running SetSignalRules - Adding Notification interface.\n");

        status = BusSetSignalRule(bus, notificationMatch, AJ_BUS_SIGNAL_ALLOW);
        if (status != AJ_OK) {
            AJ_Printf("Could not set Notification Interface AddMatch\n");
            return status;
        }

        if (superAgentMode) {
            AJ_Printf("Running SetSignalRules - Adding Superagent interface.\n");
            status = BusSetSignalRule(bus, superAgentMatch, AJ_BUS_SIGNAL_ALLOW);
            if (status != AJ_OK) {
                AJ_Printf("Could not set Notification Interface AddMatch\n");
                return status;
            }
        }
    } else {
        AJ_Printf("Running SetSignalRules with sender bus name.\n");

        AJ_Printf("Removing Notification match.\n");
        status = BusSetSignalRule(bus, notificationMatch, AJ_BUS_SIGNAL_DENY);
        if (status != AJ_OK) {
            AJ_Printf("Could not remove Notification Interface match\n");
            return status;
        }

        char senderMatch[100];
        size_t availableLen = sizeof(senderMatch);
        availableLen -= strlen(strncpy(senderMatch, superAgentFilterMatch, availableLen));
        availableLen -= strlen(strncat(senderMatch, senderBusName, availableLen));
        availableLen -= strlen(strncat(senderMatch, "'", availableLen));

        AJ_Printf("Re-adding superagent match with sender bus name %s.\n", senderBusName);

        status = BusSetSignalRule(bus, senderMatch, AJ_BUS_SIGNAL_ALLOW);
        if (status != AJ_OK) {
            AJ_Printf("Could not add SuperAgent specific match\n");
            return status;
        }

        AJ_Printf("Removing Superagent match.\n");
        status = BusSetSignalRule(bus, superAgentMatch, AJ_BUS_SIGNAL_DENY);
        if (status != AJ_OK) {
            AJ_Printf("Could not remove SuperAgent Interface match\n");
            return status;
        }
    }

    return status;
}

AJ_Status ConsumerHandleNotifyMsgObj(AJ_Message* msg)
{
    AJ_Status status;
    uint16_t protoVer;
    notification notificationContent;

    uint32_t messageId = 0;
    char* deviceId = NULL;
    char* deviceName = NULL;
    char appId[UUID_LENGTH * 2 + 1];
    char* appName = NULL;

    AJ_Arg attrbtArray;
    AJ_Arg appIdArray;
    AJ_Arg customAttributeArray;
    AJ_Arg notTextArray;
    AJ_Arg richAudioArray;

    notificationContent.numCustomAttributes = 0;
    notificationContent.numTexts = 0;
    notificationContent.numAudioUrls = 0;
    notificationContent.richIconUrl = "";
    notificationContent.richIconObjectPath = "";
    notificationContent.richAudioObjectPath = "";
    notificationContent.controlPanelServiceObjectPath = "";


    do {
        CHECK(AJ_UnmarshalArgs(msg, "q", &protoVer));

        CHECK(AJ_UnmarshalArgs(msg, "i", &messageId));

        CHECK(AJ_UnmarshalArgs(msg, "q", &notificationContent.messageType));

        CHECK(AJ_UnmarshalArgs(msg, "s", &deviceId));

        CHECK(AJ_UnmarshalArgs(msg, "s", &deviceName));

        CHECK(AJ_UnmarshalArg(msg, &appIdArray));

        size_t appIdLen = ((appIdArray.len > UUID_LENGTH) ? UUID_LENGTH : appIdArray.len) * 2 + 1;
        CHECK(AJ_RawToHex(appIdArray.val.v_byte, appIdArray.len, appId, appIdLen, FALSE));

        CHECK(AJ_UnmarshalArgs(msg, "s", &appName));

        CHECK(AJ_UnmarshalContainer(msg, &attrbtArray, AJ_ARG_ARRAY));

        while (1) {
            AJ_Arg dictArg;
            int32_t attrbtKey;
            const char* variantSig;

            status = AJ_UnmarshalContainer(msg, &dictArg, AJ_ARG_DICT_ENTRY);
            if (status == AJ_ERR_NO_MORE) {
                AJ_UnmarshalCloseContainer(msg, &attrbtArray);
                break;
            } else if (status) {
                break;
            }

            status = AJ_UnmarshalArgs(msg, "i", &attrbtKey);
            switch (attrbtKey) {
            case RICH_CONTENT_ICON_URL_ATTRIBUTE_KEY:
            {
                CHECK(AJ_UnmarshalVariant(msg, &variantSig));
                CHECK(AJ_UnmarshalArgs(msg, "s", &notificationContent.richIconUrl));
            }
            break;

            case RICH_CONTENT_ICON_OBJECT_PATH_ATTRIBUTE_KEY:
            {
                CHECK(AJ_UnmarshalVariant(msg, &variantSig));
                CHECK(AJ_UnmarshalArgs(msg, "s", &notificationContent.richIconObjectPath));
            }
            break;

            case RICH_CONTENT_AUDIO_OBJECT_PATH_ATTRIBUTE_KEY:
            {
                CHECK(AJ_UnmarshalVariant(msg, &variantSig));
                CHECK(AJ_UnmarshalArgs(msg, "s", &notificationContent.richAudioObjectPath));
            }
            break;

            case CONTROLPANELSERVICE_OBJECT_PATH_ATTRIBUTE_KEY:
            {
                CHECK(AJ_UnmarshalVariant(msg, &variantSig));
                CHECK(AJ_UnmarshalArgs(msg, "s", &notificationContent.controlPanelServiceObjectPath));
            }
            break;

            case RICH_CONTENT_AUDIO_URL_ATTRIBUTE_KEY:
            {
                CHECK(AJ_UnmarshalVariant(msg, &variantSig));
                CHECK(AJ_UnmarshalContainer(msg, &richAudioArray, AJ_ARG_ARRAY));

                while (1) {
                    AJ_Arg structArg;
                    char* urlLanguage;
                    char* urlText;

                    status = AJ_UnmarshalContainer(msg, &structArg, AJ_ARG_STRUCT);
                    if (status == AJ_ERR_NO_MORE) {
                        AJ_UnmarshalCloseContainer(msg, &richAudioArray);
                        break;
                    } else if (status) {
                        break;
                    }

                    status = AJ_UnmarshalArgs(msg, "ss", &urlLanguage, &urlText);

                    if (notificationContent.numAudioUrls < NUMALLOWEDRICHNOTS) {         // if it doesn't fit we just skip
                        richAudiosRecd[notificationContent.numAudioUrls].key   = urlLanguage;
                        richAudiosRecd[notificationContent.numAudioUrls].value = urlText;
                    }

                    notificationContent.numAudioUrls++;
                    status = AJ_UnmarshalCloseContainer(msg, &structArg);
                }
                notificationContent.richAudioUrls = richAudiosRecd;
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
            status = AJ_UnmarshalCloseContainer(msg, &dictArg);
        }

        if (status && status != AJ_ERR_NO_MORE)
            break;

        CHECK(AJ_UnmarshalContainer(msg, &customAttributeArray, AJ_ARG_ARRAY));

        while (1) {
            AJ_Arg customAttributeDictArg;
            char* customKey;
            char* customVal;

            status = AJ_UnmarshalContainer(msg, &customAttributeDictArg, AJ_ARG_DICT_ENTRY);
            if (status == AJ_ERR_NO_MORE) {
                AJ_UnmarshalCloseContainer(msg, &customAttributeArray);
                break;
            } else if (status) {
                break;
            }

            status = AJ_UnmarshalArgs(msg, "ss", &customKey, &customVal);

            if (notificationContent.numCustomAttributes < NUMALLOWEDCUSTOMATTRIBUTES) { // if it doesn't fit we just skip
                customAttributesRecd[notificationContent.numCustomAttributes].key   = customKey;
                customAttributesRecd[notificationContent.numCustomAttributes].value = customVal;
            }

            notificationContent.numCustomAttributes++;
            status = AJ_UnmarshalCloseContainer(msg, &customAttributeDictArg);
        }
        notificationContent.customAttributes = customAttributesRecd;

        if (status && status != AJ_ERR_NO_MORE)
            break;

        CHECK(AJ_UnmarshalContainer(msg, &notTextArray, AJ_ARG_ARRAY));

        while (1) {
            AJ_Arg structArg;
            char* notificationLanguage;
            char* notificationText;

            status = AJ_UnmarshalContainer(msg, &structArg, AJ_ARG_STRUCT);
            if (status == AJ_ERR_NO_MORE) {
                AJ_UnmarshalCloseContainer(msg, &notTextArray);
                break;
            } else if (status) {
                break;
            }

            status = AJ_UnmarshalArgs(msg, "ss", &notificationLanguage, &notificationText);

            if (notificationContent.numTexts < NUMALLOWEDTEXTS) { // if it doesn't fit we just skip
                textsRecd[notificationContent.numTexts].key   = notificationLanguage;
                textsRecd[notificationContent.numTexts].value = notificationText;
            }

            notificationContent.numTexts++;
            status = AJ_UnmarshalCloseContainer(msg, &structArg);
        }
        notificationContent.texts = textsRecd;
        if (status && status != AJ_ERR_NO_MORE)
            break;

    } while (0);

    if (status && status != AJ_ERR_NO_MORE) {
        AJ_Printf("Handle Notification failed: '%s'\n", AJ_StatusText(status));
    } else {
        status = HandleNotify(protoVer, messageId, &notificationContent, deviceId, deviceName, appId, appName);
    }
    return status;
}

AJ_Status ConsumerPropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    switch (propId) {
    case GET_NOTIFICATION_VERSION_PROPERTY:
    case GET_SUPERAGENT_VERSION_PROPERTY:
        status = AJ_MarshalArgs(replyMsg, "q", version);
        break;
    }
    return status;
}

AJ_Status ConsumerPropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    return AJ_ERR_DISALLOWED;
}

