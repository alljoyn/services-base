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

#include <alljoyn.h>
#include <alljoyn/config/ConfigService.h>

/*
 * Message identifiers for the method calls this service implements
 */

#define CONFIG_OBJECT_INDEX                                     NUM_PRE_CONFIG_OBJECTS

#define CONFIG_GET_PROP                                         AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 0, AJ_PROP_GET)
#define CONFIG_SET_PROP                                         AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 0, AJ_PROP_SET)

#define CONFIG_VERSION_PROP                                     AJ_APP_PROPERTY_ID(CONFIG_OBJECT_INDEX, 1, 0)
#define CONFIG_FACTORY_RESET                                    AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 1)
#define CONFIG_RESTART                                          AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 2)
#define CONFIG_GET_CONFIG_CONFIGURATIONS                        AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 3)
#define CONFIG_UPDATE_CONFIGURATIONS                            AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 4)
#define CONFIG_RESET_CONFIGURATIONS                             AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 5)
#define CONFIG_SET_PASSCODE                                     AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 6)

static AJCFG_FactoryReset appFactoryReset = NULL;
static AJCFG_Restart appRestart = NULL;
static AJCFG_SetPasscode appSetPasscode = NULL;
static AJCFG_IsValueValid appIsValueValid = NULL;

AJ_Status AJCFG_Start(AJCFG_FactoryReset factoryReset, AJCFG_Restart restart, AJCFG_SetPasscode setPasscode, AJCFG_IsValueValid isValueValid)
{
    AJ_Status status = AJ_OK;

    appFactoryReset = factoryReset;
    appRestart = restart;
    appSetPasscode = setPasscode;
    appIsValueValid = isValueValid;

    return status;
}

static const char* const AJSVC_ConfigInterface[] = {
    "$org.alljoyn.Config",
    "@Version>q",
    "?FactoryReset",
    "?Restart",
    "?GetConfigurations <s >a{sv}",
    "?UpdateConfigurations <s <a{sv}",
    "?ResetConfigurations <s <as",
    "?SetPasscode <s <ay",
    NULL
};

static const uint16_t AJSVC_ConfigVersion = 1;

const AJ_InterfaceDescription AJSVC_ConfigInterfaces[] = {
    AJ_PropertiesIface,
    AJSVC_ConfigInterface,
    NULL
};

AJ_Status AJCFG_PropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    if (propId == CONFIG_VERSION_PROP) {
        return AJ_MarshalArgs(replyMsg, "q", AJSVC_ConfigVersion);
    } else {
        return AJ_ERR_UNEXPECTED;
    }
}

AJ_Status AJCFG_PropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    return AJ_ERR_UNEXPECTED;
}

AJ_Status AJCFG_FactoryResetHandler(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;

    if (appFactoryReset) {
        status = (appFactoryReset)();
    }

    return status;
}

AJ_Status AJCFG_RestartHandler(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;

    if (appRestart) {
        status = (appRestart)();
    }

    return status;
}

AJ_Status AJCFG_GetConfigurationsHandler(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;
    AJ_Message reply;
    char* language;
    int8_t langIndex = AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX;

    AJ_Printf("GetConfigurations()\n");

    AJSVC_PropertyStoreCategoryFilter filter;
    memset(&filter, 0, sizeof(AJSVC_PropertyStoreCategoryFilter));
    filter.bit1Config = TRUE;
    status = AJ_UnmarshalArgs(msg, "s", &language);
    if (status != AJ_OK) {
        return status;
    }
    if (AJSVC_IsLanguageSupported(msg, &reply, language, &langIndex)) {
        status = AJ_MarshalReplyMsg(msg, &reply);
        if (status != AJ_OK) {
            return status;
        }
        status = AJSVC_PropertyStore_ReadAll(&reply, filter, langIndex);
        if (status != AJ_OK) {
            return status;
        }
    }
    status = AJ_DeliverMsg(&reply);
    if (status != AJ_OK) {
        return status;
    }

    return status;
}

static uint8_t IsValueValid(AJ_Message* msg, AJ_Message* reply, const char* key, const char* value)
{
    if (strcmp(AJSVC_PropertyStore_GetFieldNameForIndex(AJSVC_PROPERTY_STORE_DEFAULT_LANGUAGE), key) == 0) { // Check that if language was updated that it is supported
        if (AJSVC_IsLanguageSupported(msg, reply, value, NULL)) {
            return TRUE;
        }
    } else if (strcmp(AJSVC_PropertyStore_GetFieldNameForIndex(AJSVC_PROPERTY_STORE_DEVICE_NAME), key) == 0) { // Check that if device name was updated
        if (strlen(value) <= AJSVC_PropertyStore_GetMaxValueLength(AJSVC_PROPERTY_STORE_DEVICE_NAME)) {        // that it does not exceed maximum length
            if (strlen(value) > 0) {                                               // that it is not empty
                return TRUE;
            } else {
                AJ_MarshalErrorMsg(msg, reply, AJSVC_ERROR_INVALID_VALUE);
            }
        } else {
            AJ_MarshalErrorMsg(msg, reply, AJSVC_ERROR_MAX_SIZE_EXCEEDED);
        }
    } else {
        if (appIsValueValid == NULL || (appIsValueValid)(key, value)) {
            return TRUE;
        }
        AJ_MarshalErrorMsg(msg, reply, AJSVC_ERROR_INVALID_VALUE);
    }
    return FALSE;
}

AJ_Status AJCFG_UpdateConfigurationsHandler(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;
    AJ_Arg array;
    AJ_Arg dict;
    AJ_Message reply;
    char* key;
    char* sig;
    char* value;
    char* language;
    int8_t langIndex = AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX;
    uint8_t numOfUpdatedItems = 0;

    AJ_Printf("UpdateConfigurations()\n");

    status = AJ_UnmarshalArgs(msg, "s", &language);
    if (status != AJ_OK) {
        goto Exit;
    }
    AJ_Printf("Lang=%s\n", language);
    if (AJSVC_IsLanguageSupported(msg, &reply, language, &langIndex)) {
        status = AJ_MarshalReplyMsg(msg, &reply);
        if (status != AJ_OK) {
            goto Exit;
        }
        status = AJ_UnmarshalContainer(msg, &array, AJ_ARG_ARRAY);
        if (status != AJ_OK) {
            goto Exit;
        }
        while (1) {
            status = AJ_UnmarshalContainer(msg, &dict, AJ_ARG_DICT_ENTRY);
            if (status != AJ_OK) {
                break;
            }
            status = AJ_UnmarshalArgs(msg, "s", &key);
            if (status != AJ_OK) {
                break;
            }
            status = AJ_UnmarshalVariant(msg, (const char**)&sig);
            if (status != AJ_OK) {
                break;
            }
            status = AJ_UnmarshalArgs(msg, sig, &value);
            if (status != AJ_OK) {
                break;
            }
            AJ_Printf("key=%s value=%s\n", key, value);
            if (IsValueValid(msg, &reply, key, value)) {
                if (AJSVC_PropertyStore_Update(key, langIndex, value) == AJ_OK) {
                    numOfUpdatedItems++;
                } else {
                    AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_UPDATE_NOT_ALLOWED);
                }
            }
            status = AJ_UnmarshalCloseContainer(msg, &dict);
            if (status != AJ_OK) {
                break;
            }
        }
        if (status != AJ_OK && status != AJ_ERR_NO_MORE) {
            goto Exit;
        }
        status = AJ_UnmarshalCloseContainer(msg, &array);
        if (status != AJ_OK) {
            goto Exit;
        }
    }
    status = AJ_DeliverMsg(&reply);
    if (status != AJ_OK) {
        goto Exit;
    }

Exit:

    if (numOfUpdatedItems) {
        AJSVC_PropertyStore_SaveAll();
        AJ_About_SetShouldAnnounce(TRUE);
    }

    return status;
}

AJ_Status AJCFG_ResetConfigurationsHandler(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;
    AJ_Arg array;
    AJ_Message reply;
    char* key;
    char* language;
    int8_t langIndex = AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX;
    uint8_t numOfDeletedItems = 0;

    AJ_Printf("ResetConfigurations()\n");

    status = AJ_UnmarshalArgs(msg, "s", &language);
    if (status != AJ_OK) {
        goto Exit;
    }
    AJ_Printf("Lang=%s\n", language);
    if (AJSVC_IsLanguageSupported(msg, &reply, language, &langIndex)) {
        status = AJ_MarshalReplyMsg(msg, &reply);
        if (status != AJ_OK) {
            goto Exit;
        }
        status = AJ_UnmarshalContainer(msg, &array, AJ_ARG_ARRAY);
        if (status != AJ_OK) {
            goto Exit;
        }
        while (1) {
            status = AJ_UnmarshalArgs(msg, "s", &key);
            if (status != AJ_OK) {
                break;
            }
            AJ_Printf("Key=%s\n", key);
            if (AJSVC_PropertyStore_Reset(key, langIndex) == AJ_OK) {
                numOfDeletedItems++;
            } else {
                AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_UPDATE_NOT_ALLOWED);
            }
        }
        if (status != AJ_OK && status != AJ_ERR_NO_MORE) {
            goto Exit;
        }
        status = AJ_UnmarshalCloseContainer(msg, &array);
        if (status != AJ_OK) {
            goto Exit;
        }
    }
    status = AJ_DeliverMsg(&reply);
    if (status != AJ_OK) {
        goto Exit;
    }

Exit:

    if (numOfDeletedItems) {
        AJSVC_PropertyStore_SaveAll();
        AJ_About_SetShouldAnnounce(TRUE);
    }

    return status;
}

AJ_Status AJCFG_SetPasscodeHandler(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;
    char* daemonRealm;
    AJ_Arg newPasscode;
    AJ_Message reply;

    AJ_Printf("SetPasscode()\n");

    status = AJ_UnmarshalArgs(msg, "s", &daemonRealm);
    if (status != AJ_OK) {
        return status;
    }
    AJ_Printf("DaemonRealm=%s\n", daemonRealm);
    status = AJ_UnmarshalArg(msg, &newPasscode);
    if (status != AJ_OK) {
        return status;
    }
    status = AJ_MarshalReplyMsg(msg, &reply);
    if (status != AJ_OK) {
        return status;
    }
    if (newPasscode.typeId == AJ_ARG_BYTE) {
        if (newPasscode.len > 0) { // Check passcode is not empty
            if (appSetPasscode) {
                status = (appSetPasscode)(daemonRealm, newPasscode.val.v_string, newPasscode.len);
                if (status == AJ_ERR_RESOURCES) { // Check passcode is too long to persist
                    AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_MAX_SIZE_EXCEEDED);
                }
            }
        } else {
            AJ_Printf("Error - newPasscode cannot be empty!\n");
            AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_INVALID_VALUE);
        }
    } else {
        AJ_Printf("Error - newPasscode is not an 'ay' rather type '%c'!\n", newPasscode.typeId);
        AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_INVALID_VALUE);
    }
    status = AJ_DeliverMsg(&reply);

    return status;
}

AJ_Status AJCFG_ConnectedHandler(AJ_BusAttachment* busAttachment)
{
    return AJ_OK;
}

AJSVC_ServiceStatus AJCFG_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status* msgStatus)
{
    AJSVC_ServiceStatus serviceStatus = AJSVC_SERVICE_STATUS_HANDLED;

    switch (msg->msgId) {

    case CONFIG_GET_PROP:
        *msgStatus = AJ_BusPropGet(msg, AJCFG_PropGetHandler, NULL);
        break;

    case CONFIG_SET_PROP:
        *msgStatus = AJ_BusPropSet(msg, AJCFG_PropSetHandler, NULL);
        break;

    case CONFIG_FACTORY_RESET:
        *msgStatus = AJCFG_FactoryResetHandler(msg);
        break;

    case CONFIG_RESTART:
        *msgStatus = AJCFG_RestartHandler(msg);
        break;

    case CONFIG_GET_CONFIG_CONFIGURATIONS:
        *msgStatus = AJCFG_GetConfigurationsHandler(msg);
        break;

    case CONFIG_RESET_CONFIGURATIONS:
        *msgStatus = AJCFG_ResetConfigurationsHandler(msg);
        break;

    case CONFIG_UPDATE_CONFIGURATIONS:
        *msgStatus = AJCFG_UpdateConfigurationsHandler(msg);
        break;

    case CONFIG_SET_PASSCODE:
        *msgStatus = AJCFG_SetPasscodeHandler(msg);
        break;

    default:
        serviceStatus = AJSVC_SERVICE_STATUS_NOT_HANDLED;
        break;
    }

    return serviceStatus;
}

AJ_Status AJCFG_DisconnectHandler(AJ_BusAttachment* busAttachment)
{
    return AJ_OK;
}
