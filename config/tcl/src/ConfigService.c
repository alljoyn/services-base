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
 * The corresponding flag dbgAJCFG is defined in ServicesCommon.h and implemented
 * in ServicesCommon.c.
 */
#define AJ_MODULE AJCFG
#include <aj_debug.h>

#include <alljoyn.h>
#include <alljoyn/config/ConfigService.h>
#include <alljoyn/services_common/PropertyStore.h>

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

    AJ_InfoPrintf(("Handling GetConfigurations request\n"));

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
    if (strcmp(AJSVC_PropertyStore_GetFieldName(AJSVC_PROPERTY_STORE_DEFAULT_LANGUAGE), key) == 0) { // Check that if language was updated that it is supported
        if (AJSVC_IsLanguageSupported(msg, reply, value, NULL)) {
            return TRUE;
        }
    } else if (strcmp(AJSVC_PropertyStore_GetFieldName(AJSVC_PROPERTY_STORE_DEVICE_NAME), key) == 0) { // Check that if device name was updated
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
    uint8_t errorReply = FALSE;

    AJ_InfoPrintf(("Handling UpdateConfigurations request\n"));

    status = AJ_UnmarshalArgs(msg, "s", &language);
    if (status != AJ_OK) {
        goto Exit;
    }
    AJ_InfoPrintf(("Lang=%s\n", language));
    errorReply = !AJSVC_IsLanguageSupported(msg, &reply, language, &langIndex);
    if (!errorReply) {
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
            AJ_InfoPrintf(("key=%s value=%s\n", key, value));
            if (IsValueValid(msg, &reply, key, value)) {
                status = AJSVC_PropertyStore_Update(key, langIndex, value);
                if (status == AJ_OK) {
                    numOfUpdatedItems++;
                } else if (status == AJ_ERR_INVALID) {
                    if (!errorReply) {
                        AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_INVALID_VALUE);
                        errorReply = TRUE;
                    }
                } else if (status == AJ_ERR_FAILURE) {
                    if (!errorReply) {
                        AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_UPDATE_NOT_ALLOWED);
                        errorReply = TRUE;
                    }
                }
            } else {
                errorReply = TRUE;
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
    if (!errorReply) {
        status = AJ_MarshalReplyMsg(msg, &reply);
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
        AJ_AboutSetShouldAnnounce();
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
    uint8_t errorReply = FALSE;

    AJ_InfoPrintf(("Handling ResetConfigurations request\n"));

    status = AJ_UnmarshalArgs(msg, "s", &language);
    if (status != AJ_OK) {
        goto Exit;
    }
    AJ_InfoPrintf(("Lang=%s\n", language));
    errorReply = !AJSVC_IsLanguageSupported(msg, &reply, language, &langIndex);
    if (!errorReply) {
        status = AJ_UnmarshalContainer(msg, &array, AJ_ARG_ARRAY);
        if (status != AJ_OK) {
            goto Exit;
        }
        while (1) {
            status = AJ_UnmarshalArgs(msg, "s", &key);
            if (status != AJ_OK) {
                break;
            }
            AJ_InfoPrintf(("Key=%s\n", key));
            status = AJSVC_PropertyStore_Reset(key, langIndex);
            if (status == AJ_OK) {
                numOfDeletedItems++;
            } else if (status == AJ_ERR_INVALID) {
                if (!errorReply) {
                    AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_INVALID_VALUE);
                    errorReply = TRUE;
                }
            } else if (status == AJ_ERR_FAILURE) {
                if (!errorReply) {
                    AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_UPDATE_NOT_ALLOWED);
                    errorReply = TRUE;
                }
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
    if (!errorReply) {
        status = AJ_MarshalReplyMsg(msg, &reply);
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
        AJ_AboutSetShouldAnnounce();
    }

    return status;
}

AJ_Status AJCFG_SetPasscodeHandler(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;
    char* daemonRealm;
    AJ_Arg newPasscode;
    AJ_Message reply;
    uint8_t forceRouterDisconnect = FALSE;
    uint8_t errorReply = FALSE;

    AJ_InfoPrintf(("Handling SetPasscode request\n"));

    status = AJ_UnmarshalArgs(msg, "s", &daemonRealm);
    if (status != AJ_OK) {
        return status;
    }
    AJ_InfoPrintf(("Realm=%s\n", daemonRealm));
    status = AJ_UnmarshalArg(msg, &newPasscode);
    if (status != AJ_OK) {
        return status;
    }
    AJ_InfoPrintf(("Passcode=%d bytes long\n", newPasscode.len));
    if (newPasscode.len > 0) { // Check passcode is not empty
        if (appSetPasscode) {
            status = (appSetPasscode)(daemonRealm, (const uint8_t*)newPasscode.val.v_string, (uint8_t)newPasscode.len);
            if (status == AJ_ERR_RESOURCES) { // Check passcode is too long to persist
                status = AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_MAX_SIZE_EXCEEDED);
                if (status != AJ_OK) {
                    return status;
                }
                errorReply = TRUE;
            }
            forceRouterDisconnect = (status == AJ_ERR_READ);
        }
    } else {
        AJ_ErrPrintf(("Error - newPasscode cannot be empty!\n"));
        status = AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_INVALID_VALUE);
        if (status != AJ_OK) {
            return status;
        }
        errorReply = TRUE;
    }
    if (!errorReply) {
        status = AJ_MarshalReplyMsg(msg, &reply);
        if (status != AJ_OK) {
            return status;
        }
    }
    status = AJ_DeliverMsg(&reply);

    if (forceRouterDisconnect) {
        return AJ_ERR_READ;
    }
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
