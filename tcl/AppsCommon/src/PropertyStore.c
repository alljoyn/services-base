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
#include <errno.h>
#include <PropertyStoreOEMProvisioning.h>
#include <alljoyn/about/AboutOEMProvisioning.h>
#ifdef ONBOARDING_SERVICE
#include <alljoyn/onboarding/OnboardingOEMProvisioning.h>
#endif
#include <alljoyn/services_common/PropertyStore.h>
#include <alljoyn/services_common/Services_Common.h>
#include <aj_sasl.h>
#include <aj_guid.h>
#include <aj_nvram.h>
#include <aj_creds.h>
#include <aj_config.h>

const AJAPPS_PropertyStoreEntry ajapps_propertyStoreProperties[AJSVC_PROPERTY_STORE_NUMBER_OF_KEYS] =
{
//  { "Key Name            ", W, A, M, I .. . . ., P },
    { "DeviceId",             0, 1, 0, 1, 0, 0, 0, 1 },
    { "AppId",                0, 1, 0, 1, 0, 0, 0, 1 },
    { "DeviceName",           1, 1, 0, 1, 0, 0, 0, 1 },
// Add other persisted keys above this line
    { "DefaultLanguage",      1, 1, 0, 0, 0, 0, 0, 1 },
    { "Passcode",             1, 0, 0, 0, 0, 0, 0, 0 },
    { "RealmName",            1, 0, 0, 0, 0, 0, 0, 0 },
// Add other configurable keys above this line
    { "AppName",              0, 1, 0, 0, 0, 0, 0, 1 },
    { "Description",          0, 0, 1, 0, 0, 0, 0, 1 },
    { "Manufacturer",         0, 1, 1, 0, 0, 0, 0, 1 },
    { "ModelNumber",          0, 1, 0, 0, 0, 0, 0, 1 },
    { "DateOfManufacture",    0, 0, 0, 0, 0, 0, 0, 1 },
    { "SoftwareVersion",      0, 0, 0, 0, 0, 0, 0, 1 },
    { "AJSoftwareVersion",    0, 0, 0, 0, 0, 0, 0, 1 },
    { "HardwareVersion",      0, 0, 0, 0, 0, 0, 0, 1 },
    { "SupportUrl",           0, 0, 1, 0, 0, 0, 0, 1 },
#if defined CONFIG_SERVICE
    { "MaxLength",            0, 0, 1, 0, 0, 0, 0, 1 },
#endif
// Add other about keys above this line
};

static const char* defaultLanguagesKeyName = { "SupportedLanguages" };

uint8_t PropertyStore_GetMaxValueLength(AJSVC_PropertyStoreFieldIndices fieldIndex)
{
    switch (fieldIndex) {
        case AJSVC_PropertyStoreDeviceName:
            return DEVICE_NAME_VALUE_LENGTH;

        case AJSVC_PropertyStoreDefaultLanguage:
            return LANG_VALUE_LENGTH;

        case AJSVC_PropertyStorePasscode:
            return PASSWORD_VALUE_LENGTH;

        default:
            return KEY_VALUE_LENGTH;
    }
}

const char* PropertyStore_GetFieldNameForIndex(AJSVC_PropertyStoreFieldIndices fieldIndex)
{
    return ajapps_propertyStoreProperties[fieldIndex].keyName;
}

AJSVC_PropertyStoreFieldIndices PropertyStore_GetIndexOfField(const char* fieldName)
{
    AJSVC_PropertyStoreFieldIndices fieldIndex = 0;
    for (; fieldIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_KEYS; fieldIndex++) {
        if (!strcmp(ajapps_propertyStoreProperties[fieldIndex].keyName, fieldName)) {
            return fieldIndex;
        }
    }
    return AJSVC_PROPERTY_STORE_ERROR_FIELD_INDEX;
}

static int8_t GetLanguageIndexForProperty(int8_t langIndex, AJSVC_PropertyStoreFieldIndices fieldIndex)
{
    if (ajapps_propertyStoreProperties[fieldIndex].mode2MultiLng) {
        return langIndex;
    }
    return AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX;
}

const char* PropertyStore_GetValueForLang(AJSVC_PropertyStoreFieldIndices fieldIndex, int8_t langIndex)
{
    if ((int8_t)fieldIndex <= (int8_t)AJSVC_PROPERTY_STORE_ERROR_FIELD_INDEX || (int8_t)fieldIndex >= (int8_t)AJSVC_PROPERTY_STORE_NUMBER_OF_KEYS) {
        return NULL;
    }
    langIndex = GetLanguageIndexForProperty(langIndex, fieldIndex);
    if (langIndex <= AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX || langIndex >= AJSVC_PROPERTY_STORE_NUMBER_OF_LANGUAGES) {
        return NULL;
    }
    if (fieldIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS &&
        (ajapps_propertyStoreProperties[fieldIndex].mode0Write || ajapps_propertyStoreProperties[fieldIndex].mode3Init) &&
        ajapps_propertyStoreRuntimeValues[fieldIndex].value != NULL &&
        (ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex]) != NULL &&
        (ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex])[0] != '\0') {
        AJ_Printf("Has key [%s] value [%s]\n", ajapps_propertyStoreProperties[fieldIndex].keyName, ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex]);
        return ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex];
    } else if (ajapps_propertyStoreDefaultValues[fieldIndex] != NULL &&
         (ajapps_propertyStoreDefaultValues[fieldIndex])[langIndex] != NULL) {
        AJ_Printf("Has key [%s] defaultValue [%s]\n", ajapps_propertyStoreProperties[fieldIndex].keyName, (ajapps_propertyStoreDefaultValues[fieldIndex])[langIndex]);
        return (ajapps_propertyStoreDefaultValues[fieldIndex])[langIndex];
    }

    return NULL;
}

const char* PropertyStore_GetValue(AJSVC_PropertyStoreFieldIndices fieldIndex)
{
    return PropertyStore_GetValueForLang(fieldIndex, AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX);
}

int8_t PropertyStore_GetLanguageIndex(const char* const language)
{
    if (language != NULL) {
        uint8_t langIndex = AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX;
        for (; langIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_LANGUAGES; langIndex++) {
            if (language[0] == '\0') { // Check for empty language, if yes then search for current default language index
#ifdef CONFIG_SERVICE
                if (ajapps_propertyStoreRuntimeValues[AJSVC_PropertyStoreDefaultLanguage].value[AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX]) {
                    if (!strcmp(ajapps_propertyStoreRuntimeValues[AJSVC_PropertyStoreDefaultLanguage].value[AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX], ajsvc_propertyStoreDefaultLanguages[langIndex])) {
                        return (int8_t)langIndex;
                    }
                } else {
#endif
                if (!strcmp((ajapps_propertyStoreDefaultValues[AJSVC_PropertyStoreDefaultLanguage])[AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX], ajsvc_propertyStoreDefaultLanguages[langIndex])) {
                    return (int8_t)langIndex;
                }
#ifdef CONFIG_SERVICE
                }
#endif
            } else {
                if (!strcmp(language, ajsvc_propertyStoreDefaultLanguages[langIndex])) {
                    return (int8_t)langIndex;
                }
            }
        }
    }
    return AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX;
}

uint8_t PropertyStore_SetValueForLang(AJSVC_PropertyStoreFieldIndices fieldIndex, int8_t langIndex, const char* value)
{
    if ((int8_t)fieldIndex <= (int8_t)AJSVC_PROPERTY_STORE_ERROR_FIELD_INDEX || (int8_t)fieldIndex >= (int8_t)AJSVC_PROPERTY_STORE_NUMBER_OF_KEYS) {
        return FALSE;
    }
    langIndex = GetLanguageIndexForProperty(langIndex, fieldIndex);
    if (langIndex <= AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX || langIndex >= AJSVC_PROPERTY_STORE_NUMBER_OF_LANGUAGES) {
        return FALSE;
    }
    AJ_Printf("Set key [%s] defaultValue [%s]\n", ajapps_propertyStoreProperties[fieldIndex].keyName, value);
    size_t var_size = ajapps_propertyStoreRuntimeValues[fieldIndex].size;
    memset((ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex]), 0, var_size);
    strncpy((ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex]), value, var_size - 1);

    return TRUE;
}

uint8_t PropertyStore_SetValue(AJSVC_PropertyStoreFieldIndices fieldIndex, const char* value)
{
    return PropertyStore_SetValueForLang(fieldIndex, AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX, value);
}

int8_t PropertyStore_GetCurrentDefaultLanguageIndex()
{
    const char* currentDefaultLanguage = PropertyStore_GetValue(AJSVC_PropertyStoreDefaultLanguage);
    int8_t currentDefaultLanguageIndex = PropertyStore_GetLanguageIndex(currentDefaultLanguage);
    if (currentDefaultLanguageIndex == AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX) {
        currentDefaultLanguageIndex = AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX;
        AJ_Printf("Failed to find default language %s defaulting to %s", (currentDefaultLanguage != NULL ? currentDefaultLanguage : "NULL"), ajsvc_propertyStoreDefaultLanguages[AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX]);
    }
    return currentDefaultLanguageIndex;
}

#ifdef CONFIG_SERVICE
static void ClearPropertiesInRAM()
{
    AJSVC_PropertyStoreFieldIndices fieldIndex = 0;
    for (;fieldIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS; fieldIndex++) {
        if (ajapps_propertyStoreRuntimeValues[fieldIndex].value) {
            uint8_t langIndex = AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX;
            for (;langIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_LANGUAGES; langIndex++) {
                if (ajapps_propertyStoreProperties[fieldIndex].mode2MultiLng || langIndex == AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX) {
                    char* buf = ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex];
                    if (buf) {
                        memset(buf, 0, ajapps_propertyStoreRuntimeValues[fieldIndex].size);
                    }
                }
            }
        }
    }
}
#endif

static void InitMandatoryPropertiesInRAM()
{
    char* machineIdValue = ajapps_propertyStoreRuntimeValues[AJSVC_PropertyStoreAppID].value[AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX];
    const char* currentAppIdValue = PropertyStore_GetValue(AJSVC_PropertyStoreAppID);
    if (currentAppIdValue == NULL || currentAppIdValue[0] == '\0') {
        AJ_GUID theAJ_GUID;
        AJ_Status status = AJ_GetLocalGUID(&theAJ_GUID);
        if (status == AJ_OK) {
            AJ_GUID_ToString(&theAJ_GUID, machineIdValue, sizeof(machineIdValue));
        }
    }
    const char* currentDeviceIdValue = PropertyStore_GetValue(AJSVC_PropertyStoreDeviceID);
    if (currentDeviceIdValue == NULL || currentDeviceIdValue[0] == '\0') {
        PropertyStore_SetValue(AJSVC_PropertyStoreDeviceID, machineIdValue);
    }
    const char* currentDeviceNameValue = PropertyStore_GetValue(AJSVC_PropertyStoreDeviceName);
    if (currentDeviceNameValue == NULL || currentDeviceNameValue[0] == '\0') {
        char deviceName[DEVICE_NAME_VALUE_LENGTH + 1] = { 0 };
#ifdef ONBOARDING_SERVICE
        size_t serialIdLen = DEVICE_SERIAL_ID_LEN;
#else
        size_t serialIdLen = 7;
#endif
        size_t machineIdLen = strlen(machineIdValue);
#ifdef _WIN32
        _snprintf(deviceName, DEVICE_NAME_VALUE_LENGTH + 1, "%s %s %s", deviceManufactureName, deviceProductName, &machineIdValue[machineIdLen - min(serialIdLen, machineIdLen)]);
#else
        snprintf(deviceName, DEVICE_NAME_VALUE_LENGTH + 1, "%s %s %s", deviceManufactureName, deviceProductName, &machineIdValue[machineIdLen - min(serialIdLen, machineIdLen)]);
#endif
        PropertyStore_SetValue(AJSVC_PropertyStoreDeviceName, deviceName);
    }
}

void PropertyStore_Init()
{
#ifdef CONFIG_SERVICE
    PropertyStore_LoadAll();
#endif
    InitMandatoryPropertiesInRAM();
}

#ifdef CONFIG_SERVICE
static AJ_Status PropertyStore_ReadConfig(uint16_t index, void* ptr, size_t size)
{
    AJ_Status status = AJ_OK;

    AJ_NV_DATASET* nvramHandle = AJ_NVRAM_Open(index, "r", 0);
    if (nvramHandle != NULL) {
        size_t sizeRead = AJ_NVRAM_Read(ptr, size, nvramHandle);
        status = AJ_NVRAM_Close(nvramHandle);
        if (sizeRead != sizeRead) {
            status = AJ_ERR_WRITE;
        }
    }

    return status;
}

static AJ_Status PropertyStore_WriteConfig(uint16_t index, void* ptr, size_t size, char* mode)
{
    AJ_Status status = AJ_OK;

    AJ_NV_DATASET* nvramHandle = AJ_NVRAM_Open(index, mode, size);
    if (nvramHandle != NULL) {
        size_t sizeWritten = AJ_NVRAM_Write(ptr, size, nvramHandle);
        status = AJ_NVRAM_Close(nvramHandle);
        if (sizeWritten != size) {
            status = AJ_ERR_WRITE;
        }
    }

    return status;
}

AJ_Status PropertyStore_LoadAll()
{
    AJ_Status status = AJ_OK;

    int8_t langIndex = AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX;
    for (; langIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_LANGUAGES; langIndex++) {
        AJSVC_PropertyStoreFieldIndices fieldIndex = 0;
        for (; fieldIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS; fieldIndex++) {
            if (ajapps_propertyStoreRuntimeValues[fieldIndex].value == NULL ||
                (langIndex != AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX && !ajapps_propertyStoreProperties[fieldIndex].mode2MultiLng)) {
                continue;
            }
            void* buf = ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex];
            if (buf) {
                size_t size = ajapps_propertyStoreRuntimeValues[fieldIndex].size;
                uint16_t entry = (int)fieldIndex + (int)langIndex * (int)AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS;
                status = PropertyStore_ReadConfig(AJ_PROPERTIES_NV_ID_BEGIN + entry, buf, size);
                AJ_Printf("nvram read fieldIndex=%d langIndex=%d entry=%d val=%s size=%lu status=%s\n", (int)fieldIndex, (int)langIndex, (int)entry, ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex], size, AJ_StatusText(status));
            }
        }
    }

    return status;
}

AJ_Status PropertyStore_SaveAll()
{
    AJ_Status status = AJ_OK;

    int8_t langIndex = AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX;
    for (; langIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_LANGUAGES; langIndex++) {
        AJSVC_PropertyStoreFieldIndices fieldIndex = 0;
        for (; fieldIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS; fieldIndex++) {
            if (ajapps_propertyStoreRuntimeValues[fieldIndex].value == NULL ||
                (langIndex != AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX && !ajapps_propertyStoreProperties[fieldIndex].mode2MultiLng)) {
                continue;
            }
            void* buf = ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex];
            if (buf) {
                size_t size = ajapps_propertyStoreRuntimeValues[fieldIndex].size;
                uint16_t entry = (int)fieldIndex + (int)langIndex * (int)AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS;
                status = PropertyStore_WriteConfig(AJ_PROPERTIES_NV_ID_BEGIN + entry, buf, size, "w");
                AJ_Printf("nvram write fieldIndex=%d langIndex=%d entry=%d val=%s size=%lu status=%s\n", (int)fieldIndex, (int)langIndex, (int)entry, ajapps_propertyStoreRuntimeValues[fieldIndex].value[langIndex], size, AJ_StatusText(status));
            }
        }
    }
    SetShouldAnnounce(TRUE); // Set flag for sending an updated Announcement

    return status;
}

static uint8_t UpdateFieldInRAM(AJSVC_PropertyStoreFieldIndices fieldIndex, int8_t langIndex, const char* fieldValue)
{
    uint8_t ret = FALSE;

    if (ajapps_propertyStoreProperties[fieldIndex].mode0Write && ajapps_propertyStoreProperties[fieldIndex].mode7Public) {
        ret = PropertyStore_SetValueForLang(fieldIndex, langIndex, fieldValue);
    } else {
        AJ_Printf("UpdateFieldInRAM ERROR - field %s has read only attribute or is private\n", ajapps_propertyStoreProperties[fieldIndex].keyName);
    }

    return ret;
}

static uint8_t DeleteFieldFromRAM(AJSVC_PropertyStoreFieldIndices fieldIndex, int8_t langIndex)
{
    return UpdateFieldInRAM(fieldIndex, langIndex, "");
}
#endif

AJ_Status PropertyStore_ReadAll(AJ_Message* msg, AJSVC_PropertyStoreCategoryFilter filter, int8_t langIndex)
{
    AJ_Status status = AJ_OK;
    AJ_Arg array;
    AJ_Arg array2;
    AJ_Arg dict;

    AJ_Printf("PropertyStore_ReadAll()\n");

    status = AJ_MarshalContainer(msg, &array, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        return status;
    }

    AJSVC_PropertyStoreFieldIndices fieldIndex = 0;
    for (; fieldIndex < AJSVC_PROPERTY_STORE_NUMBER_OF_KEYS; fieldIndex++) {
#ifdef CONFIG_SERVICE
        if (ajapps_propertyStoreProperties[fieldIndex].mode7Public && (filter.bit0About || (filter.bit1Config && ajapps_propertyStoreProperties[fieldIndex].mode0Write) || (filter.bit2Announce && ajapps_propertyStoreProperties[fieldIndex].mode1Announce))) {
#else
        if (ajapps_propertyStoreProperties[fieldIndex].mode7Public && (filter.bit0About || (filter.bit2Announce && ajapps_propertyStoreProperties[fieldIndex].mode1Announce))) {
#endif
            const char* value;
            value = PropertyStore_GetValueForLang(fieldIndex, langIndex);

            if (value == NULL) {     // Non existing values are skipped!
                AJ_Printf("PropertyStore_ReadAll - Failed to get value for fieldIndex=%d langIndex=%d, skipping.\n", (int)fieldIndex, (int)langIndex);
            } else {
                status = AJ_MarshalContainer(msg, &dict, AJ_ARG_DICT_ENTRY);
                if (status != AJ_OK) {
                    return status;
                }
                status = AJ_MarshalArgs(msg, "s", ajapps_propertyStoreProperties[fieldIndex].keyName);
                if (status != AJ_OK) {
                    return status;
                }

                if (fieldIndex == AJSVC_PropertyStoreAppID) {                  //Todo - replace it with generic solution
                    status = AJ_MarshalVariant(msg, "ay");
                    if (status != AJ_OK) {
                        return status;
                    }
                    AJ_Arg arg;
                    uint8_t rawValue[16];
                    status = AJ_HexToRaw(value, 0, rawValue, (size_t)sizeof(rawValue));
                    status = AJ_MarshalArg(msg, AJ_InitArg(&arg, AJ_ARG_BYTE, AJ_ARRAY_FLAG, rawValue, sizeof(rawValue)));
                    if (status != AJ_OK) {
                        return status;
                    }
                }
#ifdef CONFIG_SERVICE
                else if (fieldIndex == AJSVC_PropertyStoreMaxLength) {
                    status = AJ_MarshalVariant(msg, "q");
                    if (status != AJ_OK) {
                        return status;
                    }
                    status = AJ_MarshalArgs(msg, "q", DEVICE_NAME_VALUE_LENGTH);
                    if (status != AJ_OK) {
                        return status;
                    }
                }
#endif
                else if (fieldIndex == AJSVC_PropertyStoreAJSoftwareVersion) {
                    status = AJ_MarshalVariant(msg, "s");
                    if (status != AJ_OK) {
                        return status;
                    }
                    status = AJ_MarshalArgs(msg, "s", AJ_GetVersion());
                    if (status != AJ_OK) {
                        return status;
                    }
                }
                else {
                    status = AJ_MarshalVariant(msg, "s");
                    if (status != AJ_OK) {
                        return status;
                    }
                    status = AJ_MarshalArgs(msg, "s", value);
                    if (status != AJ_OK) {
                        return status;
                    }
                }
                status = AJ_MarshalCloseContainer(msg, &dict);
                if (status != AJ_OK) {
                    return status;
                }
            }
        }
    }

    if (filter.bit0About) {
        // Add supported languages
        status = AJ_MarshalContainer(msg, &dict, AJ_ARG_DICT_ENTRY);
        if (status != AJ_OK) {
            return status;
        }
        status = AJ_MarshalArgs(msg, "s", defaultLanguagesKeyName);
        if (status != AJ_OK) {
            return status;
        }
        status = AJ_MarshalVariant(msg, "as");
        if (status != AJ_OK) {
            return status;
        }
        status = AJ_MarshalContainer(msg, &array2, AJ_ARG_ARRAY);
        if (status != AJ_OK) {
            return status;
        }

        uint8_t index = AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX;
        for (; index < AJSVC_PROPERTY_STORE_NUMBER_OF_LANGUAGES; index++) {
            status = AJ_MarshalArgs(msg, "s", ajsvc_propertyStoreDefaultLanguages[index]);
            if (status != AJ_OK) {
                return status;
            }
        }

        status = AJ_MarshalCloseContainer(msg, &array2);
        if (status != AJ_OK) {
            return status;
        }
        status = AJ_MarshalCloseContainer(msg, &dict);
        if (status != AJ_OK) {
            return status;
        }
    }
    status = AJ_MarshalCloseContainer(msg, &array);
    if (status != AJ_OK) {
        return status;
    }

    return status;
}

#ifdef CONFIG_SERVICE
AJ_Status PropertyStore_Update(const char* key, int8_t langIndex, const char* value)
{
    if (UpdateFieldInRAM(PropertyStore_GetIndexOfField(key), langIndex, value)) {
        return AJ_OK;
    }

    return AJ_ERR_FAILURE;
}

AJ_Status PropertyStore_Reset(const char* key, int8_t langIndex)
{
    if (DeleteFieldFromRAM(PropertyStore_GetIndexOfField(key), langIndex)) {
        InitMandatoryPropertiesInRAM();
        return AJ_OK;
    }

    return AJ_ERR_FAILURE;
}

AJ_Status PropertyStore_ResetAll()
{
    ClearPropertiesInRAM();
    InitMandatoryPropertiesInRAM();
    return PropertyStore_SaveAll();
}
#endif
