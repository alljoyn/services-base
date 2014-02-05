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

#include <alljoyn/about/AboutService.h>
#include <alljoyn/config/ConfigService.h>

static const char* const ConfigInterface[] = {
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

static const uint16_t ConfigVersion = 1;

const AJ_InterfaceDescription ConfigInterfaces[] = {
    AJ_PropertiesIface,
    ConfigInterface,
    NULL
};

AJ_Status ConfigPropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    if (propId == CONFIG_VERSION_PROP) {
        uint16_t q = ConfigVersion;
        return AJ_MarshalArgs(replyMsg, "q", q);
    } else {
        return AJ_ERR_UNEXPECTED;
    }
}

AJ_Status ConfigPropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    return AJ_ERR_UNEXPECTED;
}

AJ_Status ConfigFactoryReset(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;

    status = App_FactoryReset();

    return status;
}

AJ_Status ConfigRestart(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;

    status = App_Restart();

    return status;
}

AJ_Status ConfigGetConfigurations(AJ_Message* msg)
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
    if (Common_IsLanguageSupported(msg, &reply, language, &langIndex)) {
        status = AJ_MarshalReplyMsg(msg, &reply);
        if (status != AJ_OK) {
            return status;
        }
        status = PropertyStore_ReadAll(&reply, filter, langIndex);
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

static uint8_t Config_IsValueValid(AJ_Message* msg, AJ_Message* reply, const char* key, const char* value)
{
    if (strcmp(PropertyStore_GetFieldNameForIndex(AJSVC_PropertyStoreDefaultLanguage), key) == 0) { // Check that if language was updated that it is supported
        if (Common_IsLanguageSupported(msg, reply, value, NULL)) {
            return TRUE;
        }
    } else if (strcmp(PropertyStore_GetFieldNameForIndex(AJSVC_PropertyStoreDeviceName), key) == 0) { // Check that if device name was updated
        if (strlen(value) <= PropertyStore_GetMaxValueLength(AJSVC_PropertyStoreDeviceName)) {        // that it does not exceed maximum length
            if (strlen(value) > 0) {                                               // that it is not empty
                return TRUE;
            } else {
                AJ_MarshalErrorMsg(msg, reply, AJSVC_ERROR_INVALID_VALUE);
            }
        } else {
            AJ_MarshalErrorMsg(msg, reply, AJSVC_ERROR_MAX_SIZE_EXCEEDED);
        }
    } else {
        if (App_IsValueValid(key, value)) {
            return TRUE;
        }
        AJ_MarshalErrorMsg(msg, reply, AJSVC_ERROR_INVALID_VALUE);
    }
    return FALSE;
}

AJ_Status ConfigUpdateConfigurations(AJ_Message* msg)
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
    if (Common_IsLanguageSupported(msg, &reply, language, &langIndex)) {
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
            if (Config_IsValueValid(msg, &reply, key, value)) {
                if (PropertyStore_Update(key, langIndex, value) == AJ_OK) {
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
        PropertyStore_SaveAll();
        SetShouldAnnounce(TRUE);
    }

    return status;
}

AJ_Status ConfigResetConfigurations(AJ_Message* msg)
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
    if (Common_IsLanguageSupported(msg, &reply, language, &langIndex)) {
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
            if (PropertyStore_Reset(key, langIndex) == AJ_OK) {
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
        PropertyStore_SaveAll();
        SetShouldAnnounce(TRUE);
    }

    return status;
}

AJ_Status ConfigSetPasscode(AJ_Message* msg)
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
    if (newPasscode.typeId == AJ_ARG_BYTE) {
        if (newPasscode.len <= PropertyStore_GetMaxValueLength(AJSVC_PropertyStorePasscode)) { // Check passcode does not exceed limit
            if (newPasscode.len > 0) { // Check passcode is not empty
                AJ_Printf("newStringPasscode=%s\n", newPasscode.val.v_string);
                status = AJ_MarshalReplyMsg(msg, &reply);
                if (status != AJ_OK) {
                    return status;
                }
                status = AJ_DeliverMsg(&reply);
                if (status != AJ_OK) {
                    return status;
                }
                status = App_SetPasscode(daemonRealm, newPasscode.val.v_string);
                if (status != AJ_OK) {
                    return status;
                }
            } else {
                AJ_Printf("Error - newPasscode cannot be empty!\n");
                AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_INVALID_VALUE);
                status = AJ_DeliverMsg(&reply);
                if (status != AJ_OK) {
                    return status;
                }
            }
        } else {
            AJ_Printf("Error - newPasscode length %d > %d!\n", newPasscode.len, PropertyStore_GetMaxValueLength(AJSVC_PropertyStorePasscode));
            AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_MAX_SIZE_EXCEEDED);
            status = AJ_DeliverMsg(&reply);
            if (status != AJ_OK) {
                return status;
            }
        }
    } else {
        AJ_Printf("Error - newPasscode is not an 'ay' rather type '%c'!\n", newPasscode.typeId);
        AJ_MarshalErrorMsg(msg, &reply, AJSVC_ERROR_INVALID_VALUE);
        status = AJ_DeliverMsg(&reply);
        if (status != AJ_OK) {
            return status;
        }
    }

    return status;
}
