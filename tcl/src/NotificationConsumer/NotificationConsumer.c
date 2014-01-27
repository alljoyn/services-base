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

AJNS_DictionaryEntry textsRecd[NUMALLOWEDTEXTS], customAttributesRecd[NUMALLOWEDCUSTOMATTRIBUTES], richAudiosRecd[NUMALLOWEDRICHNOTS];

AJ_Status ConsumerSetSignalRules(AJ_BusAttachment* busAttachment, uint8_t superAgentMode, const char* senderBusName)
{
    AJ_Status status = AJ_OK;

    AJ_Printf("In SetSignalRules()\n");
    AJ_Printf("Adding Dismisser interface match.\n");
    status = AJ_BusSetSignalRuleFlags(busAttachment, dismisserMatch, AJ_BUS_SIGNAL_ALLOW, AJ_FLAG_NO_REPLY_EXPECTED);
    if (status != AJ_OK) {
        AJ_Printf("Could not set Dismisser Interface AddMatch\n");
        return status;
    }

    if (senderBusName == NULL) {
        AJ_Printf("Adding Notification interface match.\n");

        status = AJ_BusSetSignalRuleFlags(busAttachment, notificationMatch, AJ_BUS_SIGNAL_ALLOW, AJ_FLAG_NO_REPLY_EXPECTED);
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

            status = AJ_BusSetSignalRuleFlags(busAttachment, senderMatch, AJ_BUS_SIGNAL_DENY, AJ_FLAG_NO_REPLY_EXPECTED);
            if (status != AJ_OK) {
                AJ_Printf("Could not remove SuperAgent specific match\n");
                return status;
            }

            status = AJ_BusFindAdvertisedName(busAttachment, currentSuperAgentBusName, AJ_BUS_STOP_FINDING);
            if (status != AJ_OK) {
                AJ_Printf("Could not unregister to find advertised name of lost SuperAgent\n");
                return status;
            }

            currentSuperAgentBusName[0] = '\0'; // Clear current SuperAgent BusUniqueName
        }

        if (superAgentMode) {
            AJ_Printf("Adding Superagent interface match.\n");
            status = AJ_BusSetSignalRuleFlags(busAttachment, superAgentMatch, AJ_BUS_SIGNAL_ALLOW, AJ_FLAG_NO_REPLY_EXPECTED);
            if (status != AJ_OK) {
                AJ_Printf("Could not set Notification Interface AddMatch\n");
                return status;
            }
        }
    } else {
        AJ_Printf("Running SetSignalRules with sender bus name.\n");

        AJ_Printf("Removing Notification interface match.\n");
        status = AJ_BusSetSignalRuleFlags(busAttachment, notificationMatch, AJ_BUS_SIGNAL_DENY, AJ_FLAG_NO_REPLY_EXPECTED);
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

        status = AJ_BusSetSignalRuleFlags(busAttachment, senderMatch, AJ_BUS_SIGNAL_ALLOW, AJ_FLAG_NO_REPLY_EXPECTED);
        if (status != AJ_OK) {
            AJ_Printf("Could not add SuperAgent specific match\n");
            return status;
        }

        status = AJ_BusFindAdvertisedName(busAttachment, senderBusName, AJ_BUS_START_FINDING);
        if (status != AJ_OK) {
            AJ_Printf("Could not register to find advertised name of SuperAgent\n");
            return status;
        }

        strncpy(currentSuperAgentBusName, senderBusName, 16); // Save current SuperAgent BusUniqueName

        AJ_Printf("Removing Superagent interface match.\n");
        status = AJ_BusSetSignalRuleFlags(busAttachment, superAgentMatch, AJ_BUS_SIGNAL_DENY, AJ_FLAG_NO_REPLY_EXPECTED);
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
    AJNS_Notification notification;
    AJNS_NotificationContent content;

    char appId[UUID_LENGTH * 2 + 1];
    size_t appIdLen;
    AJ_Arg appIdArray;
    AJ_Arg attrbtArray;
    AJ_Arg customAttributeArray;
    AJ_Arg notTextArray;
    AJ_Arg richAudioArray;

    memset(&notification, 0, sizeof(AJNS_Notification));
    memset(&content, 0, sizeof(AJNS_NotificationContent));
    notification.content = &content;

    AJ_Printf("Received notification signal from sender %s\n", msg->sender);

    status = AJ_UnmarshalArgs(msg, "q", &notification.version);
    if (status != AJ_OK) {
        goto Exit;
    }

    status = AJ_UnmarshalArgs(msg, "i", &notification.notificationId);
    if (status != AJ_OK) {
        goto Exit;
    }

    status = AJ_UnmarshalArgs(msg, "q", &notification.messageType);
    if (status != AJ_OK) {
        goto Exit;
    }

    status = AJ_UnmarshalArgs(msg, "s", &notification.deviceId);
    if (status != AJ_OK) {
        goto Exit;
    }

    status = AJ_UnmarshalArgs(msg, "s", &notification.deviceName);
    if (status != AJ_OK) {
        goto Exit;
    }

    status = AJ_UnmarshalArg(msg, &appIdArray);
    if (status != AJ_OK) {
        goto Exit;
    }

    appIdLen = ((appIdArray.len > UUID_LENGTH) ? UUID_LENGTH : appIdArray.len) * 2 + 1;

    status = AJ_RawToHex(appIdArray.val.v_byte, appIdArray.len, appId, appIdLen, FALSE);
    if (status != AJ_OK) {
        goto Exit;
    }
    notification.appId = appId;

    status = AJ_UnmarshalArgs(msg, "s", &notification.appName);
    if (status != AJ_OK) {
        goto Exit;
    }

    status = AJ_UnmarshalContainer(msg, &attrbtArray, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        goto Exit;
    }

    while (1) {
        AJ_Arg dictArg;
        int32_t attrbtKey;
        const char* variantSig;

        status = AJ_UnmarshalContainer(msg, &dictArg, AJ_ARG_DICT_ENTRY);
        if (status == AJ_ERR_NO_MORE) {
            status = AJ_UnmarshalCloseContainer(msg, &attrbtArray);
            if (status != AJ_OK) {
                goto Exit;
            } else {
                break;
            }
        } else if (status) {
            goto Exit;
        }

        status = AJ_UnmarshalArgs(msg, "i", &attrbtKey);
        if (status != AJ_OK) {
            goto Exit;
        }

        switch (attrbtKey) {
        case RICH_CONTENT_ICON_URL_ATTRIBUTE_KEY:
            {
                status = AJ_UnmarshalVariant(msg, &variantSig);
                if (status != AJ_OK) {
                    goto Exit;
                }
                status = AJ_UnmarshalArgs(msg, "s", &(notification.content->richIconUrl));
                if (status != AJ_OK) {
                    goto Exit;
                }
            }
            break;

        case RICH_CONTENT_ICON_OBJECT_PATH_ATTRIBUTE_KEY:
            {
                status = AJ_UnmarshalVariant(msg, &variantSig);
                if (status != AJ_OK) {
                    goto Exit;
                }
                status = AJ_UnmarshalArgs(msg, "s", &(notification.content->richIconObjectPath));
                if (status != AJ_OK) {
                    goto Exit;
                }
            }
            break;

        case RICH_CONTENT_AUDIO_OBJECT_PATH_ATTRIBUTE_KEY:
            {
                status = AJ_UnmarshalVariant(msg, &variantSig);
                if (status != AJ_OK) {
                    goto Exit;
                }
                status = AJ_UnmarshalArgs(msg, "s", &(notification.content->richAudioObjectPath));
                if (status != AJ_OK) {
                    goto Exit;
                }
            }
            break;

        case CONTROLPANELSERVICE_OBJECT_PATH_ATTRIBUTE_KEY:
            {
                status = AJ_UnmarshalVariant(msg, &variantSig);
                if (status != AJ_OK) {
                    goto Exit;
                }
                status = AJ_UnmarshalArgs(msg, "s", &(notification.content->controlPanelServiceObjectPath));
                if (status != AJ_OK) {
                    goto Exit;
                }
            }
            break;

        case ORIGINAL_SENDER_NAME_ATTRIBUTE_KEY:
            {
                status = AJ_UnmarshalVariant(msg, &variantSig);
                if (status != AJ_OK) {
                    goto Exit;
                }
                status = AJ_UnmarshalArgs(msg, "s", &notification.originalSenderName);
                if (status != AJ_OK) {
                    goto Exit;
                }
            }
            break;

        case RICH_CONTENT_AUDIO_URL_ATTRIBUTE_KEY:
            {
                status = AJ_UnmarshalVariant(msg, &variantSig);
                if (status != AJ_OK) {
                    goto Exit;
                }
                status = AJ_UnmarshalContainer(msg, &richAudioArray, AJ_ARG_ARRAY);
                if (status != AJ_OK) {
                    goto Exit;
                }

                while (1) {
                    AJ_Arg structArg;
                    char* urlLanguage;
                    char* urlText;

                    status = AJ_UnmarshalContainer(msg, &structArg, AJ_ARG_STRUCT);
                    if (status == AJ_ERR_NO_MORE) {
                        status = AJ_UnmarshalCloseContainer(msg, &richAudioArray);
                        if (status != AJ_OK) {
                            goto Exit;
                        } else {
                            break;
                        }
                    } else if (status) {
                        goto Exit;
                    }

                    status = AJ_UnmarshalArgs(msg, "ss", &urlLanguage, &urlText);
                    if (status != AJ_OK) {
                        goto Exit;
                    }
                    if (notification.content->numAudioUrls < NUMALLOWEDRICHNOTS) {             // if it doesn't fit we just skip
                        richAudiosRecd[notification.content->numAudioUrls].key   = urlLanguage;
                        richAudiosRecd[notification.content->numAudioUrls].value = urlText;
                        notification.content->numAudioUrls++;
                    }

                    status = AJ_UnmarshalCloseContainer(msg, &structArg);
                    if (status != AJ_OK) {
                        goto Exit;
                    }
                }
                notification.content->richAudioUrls = richAudiosRecd;
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
        if (status != AJ_OK) {
            goto Exit;
        }
    }

    status = AJ_UnmarshalContainer(msg, &customAttributeArray, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        goto Exit;
    }

    while (1) {
        AJ_Arg customAttributeDictArg;
        char* customKey;
        char* customVal;

        status = AJ_UnmarshalContainer(msg, &customAttributeDictArg, AJ_ARG_DICT_ENTRY);
        if (status == AJ_ERR_NO_MORE) {
            status = AJ_UnmarshalCloseContainer(msg, &customAttributeArray);
            if (status != AJ_OK) {
                goto Exit;
            } else {
                break;
            }
        } else if (status) {
            goto Exit;
        }

        status = AJ_UnmarshalArgs(msg, "ss", &customKey, &customVal);
        if (status != AJ_OK) {
            goto Exit;
        }

        if (notification.content->numCustomAttributes < NUMALLOWEDCUSTOMATTRIBUTES) {     // if it doesn't fit we just skip
            customAttributesRecd[notification.content->numCustomAttributes].key   = customKey;
            customAttributesRecd[notification.content->numCustomAttributes].value = customVal;
            notification.content->numCustomAttributes++;
        }

        status = AJ_UnmarshalCloseContainer(msg, &customAttributeDictArg);
        if (status != AJ_OK) {
            goto Exit;
        }
    }
    notification.content->customAttributes = customAttributesRecd;

    status = AJ_UnmarshalContainer(msg, &notTextArray, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        goto Exit;
    }

    while (1) {
        AJ_Arg structArg;
        char* notificationLanguage;
        char* notificationText;

        status = AJ_UnmarshalContainer(msg, &structArg, AJ_ARG_STRUCT);
        if (status == AJ_ERR_NO_MORE) {
            status = AJ_UnmarshalCloseContainer(msg, &notTextArray);
            if (status != AJ_OK) {
                goto Exit;
            } else {
                break;
            }
        } else if (status) {
            goto Exit;
        }

        status = AJ_UnmarshalArgs(msg, "ss", &notificationLanguage, &notificationText);
        if (status != AJ_OK) {
            goto Exit;
        }

        if (notification.content->numTexts < NUMALLOWEDTEXTS) {     // if it doesn't fit we just skip
            textsRecd[notification.content->numTexts].key   = notificationLanguage;
            textsRecd[notification.content->numTexts].value = notificationText;
            notification.content->numTexts++;
        }

        status = AJ_UnmarshalCloseContainer(msg, &structArg);
        if (status != AJ_OK) {
            goto Exit;
        }
    }
    notification.content->texts = textsRecd;

Exit:

    if (status != AJ_OK) {
        AJ_Printf("Handle Notification failed: '%s'\n", AJ_StatusText(status));
    } else {
        status = ApplicationHandleNotify(&notification);
    }

    AJ_CloseMsg(msg);

    return status;
}

AJ_Status ConsumerDismissSignalHandler(AJ_Message* msg)
{
    AJ_Status status;

    uint32_t notificationId = 0;
    char appId[UUID_LENGTH * 2 + 1];
    size_t appIdLen;
    AJ_Arg appIdArray;

    status = AJ_UnmarshalArgs(msg, "i", &notificationId);
    if (status != AJ_OK) {
        goto Exit;
    }

    status = AJ_UnmarshalArg(msg, &appIdArray);
    if (status != AJ_OK) {
        goto Exit;
    }

    appIdLen = ((appIdArray.len > UUID_LENGTH) ? UUID_LENGTH : appIdArray.len) * 2 + 1;

    status = AJ_RawToHex(appIdArray.val.v_byte, appIdArray.len, appId, appIdLen, FALSE);
    if (status != AJ_OK) {
        goto Exit;
    }

    ApplicationHandleDismiss(notificationId, appId);

Exit:

    AJ_CloseMsg(msg);

    return status;
}

AJ_Status ConsumerAcknowledgeNotification(AJ_BusAttachment* busAttachment, uint16_t version, int32_t msgId, const char* senderName, uint32_t sessionId)
{
    AJ_Printf("Inside AcknowledgeNotification()\n");
    AJ_Status status = AJ_OK;

    if (version < 2) { // Producer does not support acknowledgement
        return AJ_ERR_INVALID;
    }

    if (status == AJ_OK && sessionId != 0) {
        AJ_Message ackMsg;
        status = AJ_MarshalMethodCall(busAttachment, &ackMsg, NOTIFICATION_PRODUCER_ACKNOWLEDGE_PROXY, senderName, sessionId, AJ_NO_FLAGS, AJ_CALL_TIMEOUT);
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

AJ_Status ConsumerDismissNotification(AJ_BusAttachment* busAttachment, uint16_t version, int32_t msgId, const char* appId, const char* senderName, uint32_t sessionId)
{
    AJ_Printf("Inside DismissNotification()\n");
    AJ_Status status = AJ_OK;

    if (version < 2) { // Producer does not support dismissal but other consumers might so send Dismiss signal
        status = NotificationSendDismiss(busAttachment, msgId, appId);
        return status;
    }

    if (status == AJ_OK && sessionId != 0) {
        AJ_Message dismissMsg;
        status = AJ_MarshalMethodCall(busAttachment, &dismissMsg, NOTIFICATION_PRODUCER_DISMISS_PROXY, senderName, sessionId, AJ_NO_FLAGS, AJ_CALL_TIMEOUT);
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
        status = NotificationSendDismiss(busAttachment, msgId, appId);
    }

    return status;
}

