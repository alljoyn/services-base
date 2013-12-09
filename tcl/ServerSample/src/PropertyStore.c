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

#include <alljoyn.h>
#include <errno.h>
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

static property_store_config_entry_t theConfigVar[NUMBER_OF_CONFIG_KEYS];

static char MachineID[MACHINE_ID_LENGTH + 1] = { 0 };
#ifdef CONFIG_SERVICE
static char DefaultLanguageVar[LANG_VALUE_LENGTH + 1] = { 0 };
static char DeviceNameVar[DEVICE_NAME_VALUE_LENGTH + 1] = { 0 };
static char PasscodeVar[PASSWORD_VALUE_LENGTH + 1] = { 0 };
static char RealmNameVar[KEY_VALUE_LENGTH + 1] = { 0 };
#endif

static const char* defaultLanguagesKeyName = { "SupportedLanguages" };

const char* PropertyStore_GetFieldNameForIndex(enum_field_indecies_t index)
{
    return theAboutConfigVar[index].keyName;
}

enum_field_indecies_t PropertyStore_GetIndexOfField(const char* fieldName)
{
    enum_field_indecies_t fieldIndex = 0;
    for (; fieldIndex < NUMBER_OF_KEYS; fieldIndex++) {
        if (!strcmp(theAboutConfigVar[fieldIndex].keyName, fieldName)) {
            return fieldIndex;
        }
    }
    return ERROR_FIELD_INDEX;
}

static enum_lang_indecies_t GetLanguageIndexForProperty(enum_lang_indecies_t langIndex, enum_field_indecies_t fieldIndex)
{
    if (theAboutConfigVar[fieldIndex].mode2MultiLng) {
        return langIndex;
    }
    return NO_LANGUAGE_INDEX;
}

const char* PropertyStore_GetValueForLang(enum_field_indecies_t fieldIndex, enum_lang_indecies_t langIndex)
{
    if (fieldIndex <= ERROR_FIELD_INDEX || fieldIndex >= NUMBER_OF_KEYS || langIndex <= ERROR_LANGUAGE_INDEX || langIndex >= NUMBER_OF_LANGUAGES) {
        return NULL;
    }

    langIndex = GetLanguageIndexForProperty(langIndex, fieldIndex);
    if (fieldIndex < NUMBER_OF_CONFIG_KEYS && (theAboutConfigVar[fieldIndex].mode0Write || theAboutConfigVar[fieldIndex].mode3Init) && theConfigVar[fieldIndex].value[langIndex][0]) {
        AJ_Printf("Has key [%s] value [%s]\n", theAboutConfigVar[fieldIndex].keyName, theConfigVar[fieldIndex].value[langIndex]);
        return theConfigVar[fieldIndex].value[langIndex];
    } else {
        AJ_Printf("Has key [%s] defaultValue [%s]\n", theAboutConfigVar[fieldIndex].keyName, theAboutConfigVar[fieldIndex].value[langIndex]);
        return theAboutConfigVar[fieldIndex].value[langIndex];
    }
}

const char* PropertyStore_GetValue(enum_field_indecies_t fieldIndex)
{
    return PropertyStore_GetValueForLang(fieldIndex, NO_LANGUAGE_INDEX);
}

enum_lang_indecies_t PropertyStore_GetLanguageIndex(const char* const language)
{
    if (language != NULL) {
        enum_lang_indecies_t langIndex = NO_LANGUAGE_INDEX;
        if (language[0] == '\0') { // Check for empty language, if yes then search for current default language index
#ifdef CONFIG_SERVICE
            if (theConfigVar[DefaultLanguage].value[NO_LANGUAGE_INDEX][0]) {
                for (; langIndex < NUMBER_OF_LANGUAGES; langIndex++) {
                    if (!strcmp(theConfigVar[DefaultLanguage].value[NO_LANGUAGE_INDEX], theDefaultLanguages[langIndex])) {
                        return langIndex;
                    }
                }
            } else {
#endif
            for (; langIndex < NUMBER_OF_LANGUAGES; langIndex++) {
                if (!strcmp(theAboutConfigVar[DefaultLanguage].value[NO_LANGUAGE_INDEX], theDefaultLanguages[langIndex])) {
                    return langIndex;
                }
            }
#ifdef CONFIG_SERVICE
        }
#endif
        } else {
            for (; langIndex < NUMBER_OF_LANGUAGES; langIndex++) {
                if (!strcmp(language, theDefaultLanguages[langIndex])) {
                    return langIndex;
                }
            }
        }
    }
    return ERROR_LANGUAGE_INDEX;
}

uint8_t PropertyStore_SetValueForLang(enum_field_indecies_t fieldIndex, enum_lang_indecies_t langIndex, const char* value)
{
    if (fieldIndex <= ERROR_FIELD_INDEX || fieldIndex >= NUMBER_OF_CONFIG_KEYS || langIndex <= ERROR_LANGUAGE_INDEX || langIndex >= NUMBER_OF_LANGUAGES) {
        return FALSE;
    }

    langIndex = GetLanguageIndexForProperty(langIndex, fieldIndex);
    AJ_Printf("Set key [%s] defaultValue [%s]\n", theAboutConfigVar[fieldIndex].keyName, value);
    size_t var_size = theConfigVar[fieldIndex].size;
    memset(theConfigVar[fieldIndex].value[langIndex], 0, var_size);
    strncpy(theConfigVar[fieldIndex].value[langIndex], value, var_size - 1);

    return TRUE;
}

uint8_t PropertyStore_SetValue(enum_field_indecies_t fieldIndex, const char* value)
{
    return PropertyStore_SetValueForLang(fieldIndex, NO_LANGUAGE_INDEX, value);
}

enum_lang_indecies_t PropertyStore_GetCurrentDefaultLanguageIndex()
{
    const char* currentDefaultLanguage = PropertyStore_GetValue(DefaultLanguage);
    enum_lang_indecies_t currentDefaultLanguageIndex = PropertyStore_GetLanguageIndex(currentDefaultLanguage);
    if (currentDefaultLanguageIndex == ERROR_LANGUAGE_INDEX) {
        currentDefaultLanguageIndex = NO_LANGUAGE_INDEX;
        AJ_Printf("Failed to find default language %s defaulting to %s", (currentDefaultLanguage != NULL ? currentDefaultLanguage : "NULL"), theDefaultLanguages[currentDefaultLanguageIndex]);
    }
    return currentDefaultLanguageIndex;
}

#ifdef CONFIG_SERVICE
static void ClearPropertiesInRAM()
{
    memset(MachineID, 0, sizeof(MachineID));
    memset(DefaultLanguageVar, 0, sizeof(DefaultLanguageVar));
    memset(DeviceNameVar, 0, sizeof(DeviceNameVar));
    memset(PasscodeVar, 0, sizeof(PasscodeVar));
    memset(RealmNameVar, 0, sizeof(RealmNameVar));
}
#endif

static void InitMandatoryPropertiesInRAM()
{
    AJ_GUID theAJ_GUID;
    AJ_Status status = AJ_GetLocalGUID(&theAJ_GUID);
    if (status == AJ_OK) {
        AJ_GUID_ToString(&theAJ_GUID, MachineID, sizeof(MachineID));
    }
    const char* currentDeviceNameValue = PropertyStore_GetValue(DeviceName);
    if (currentDeviceNameValue != NULL && currentDeviceNameValue[0] == '\0') {
        char deviceName[DEVICE_NAME_VALUE_LENGTH + 1] = { 0 };
#ifdef ONBOARDING_SERVICE
        size_t serialIdLen = DEVICE_SERIAL_ID_LEN;
#else
        size_t serialIdLen = 7;
#endif
        size_t machineIdLen = strlen(MachineID);
#ifdef _WIN32
        _snprintf(deviceName, DEVICE_NAME_VALUE_LENGTH + 1, "%s %s %s", deviceManufactureName, deviceProductName, &MachineID[machineIdLen - min(serialIdLen, machineIdLen)]);
#else
        snprintf(deviceName, DEVICE_NAME_VALUE_LENGTH + 1, "%s %s %s", deviceManufactureName, deviceProductName, &MachineID[machineIdLen - min(serialIdLen, machineIdLen)]);
#endif
        PropertyStore_SetValue(DeviceName, deviceName);
    }
}

void PropertyStore_Init()
{
    memset(theConfigVar, 0, sizeof(theConfigVar));
    theConfigVar[DeviceID].size = sizeof(MachineID);
    theConfigVar[DeviceID].value[NO_LANGUAGE_INDEX] = MachineID;
    theConfigVar[AppID].size = sizeof(MachineID);
    theConfigVar[AppID].value[NO_LANGUAGE_INDEX] = MachineID;
#ifdef CONFIG_SERVICE
    theConfigVar[DefaultLanguage].size = sizeof(DefaultLanguageVar);
    theConfigVar[DefaultLanguage].value[NO_LANGUAGE_INDEX] = DefaultLanguageVar;
    theConfigVar[DeviceName].size = sizeof(DeviceNameVar);
    theConfigVar[DeviceName].value[NO_LANGUAGE_INDEX] = DeviceNameVar;
    theConfigVar[Passcode].size = sizeof(PasscodeVar);
    theConfigVar[Passcode].value[NO_LANGUAGE_INDEX] = PasscodeVar;
    theConfigVar[RealmName].size = sizeof(RealmNameVar);
    theConfigVar[RealmName].value[NO_LANGUAGE_INDEX] = RealmNameVar;
#endif
    //App_FactoryReset();//This line should be disabled

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

    enum_lang_indecies_t langIndex = NO_LANGUAGE_INDEX;
    for (langIndex = 0; langIndex < NUMBER_OF_LANGUAGES; langIndex++) {
        enum_field_indecies_t fieldIndex = 0;
        for (; fieldIndex < NUMBER_OF_CONFIG_KEYS; fieldIndex++) {
            void* buf = theConfigVar[fieldIndex].value[langIndex];
            if (buf) {
                size_t size = theConfigVar[fieldIndex].size;
                uint16_t entry = (int)fieldIndex + (int)langIndex * (int)NUMBER_OF_CONFIG_KEYS;
                status = PropertyStore_ReadConfig(AJ_PROPERTIES_NV_ID_BEGIN + entry, buf, size);
                AJ_Printf("nvram read fieldIndex=%d langIndex=%d entry=%d val=%s size=%lu status=%u\n", (int)fieldIndex, (int)langIndex, (int)entry, theConfigVar[fieldIndex].value[langIndex], size, status);
            }
        }
    }

    return status;
}

AJ_Status PropertyStore_SaveAll()
{
    AJ_Status status = AJ_OK;

    enum_lang_indecies_t langIndex = NO_LANGUAGE_INDEX;
    for (; langIndex < NUMBER_OF_LANGUAGES; langIndex++) {
        enum_field_indecies_t fieldIndex = 0;
        for (; fieldIndex < NUMBER_OF_CONFIG_KEYS; fieldIndex++) {
            void* buf = theConfigVar[fieldIndex].value[langIndex];
            if (buf) {
                size_t size = theConfigVar[fieldIndex].size;
                uint16_t entry = (int)fieldIndex + (int)langIndex * (int)NUMBER_OF_CONFIG_KEYS;
                status = PropertyStore_WriteConfig(AJ_PROPERTIES_NV_ID_BEGIN + entry, buf, size, "w");
                AJ_Printf("nvram write fieldIndex=%d langIndex=%d entry=%d val=%s size=%lu status=%u\n", (int)fieldIndex, (int)langIndex, (int)entry, theConfigVar[fieldIndex].value[langIndex], size, status);
            }
        }
    }
    SetShouldAnnounce(TRUE); // Set flag for sending an updated Announcement

    return status;
}

static uint8_t UpdateFieldInRAM(enum_field_indecies_t fieldIndex, enum_lang_indecies_t langIndex, const char* fieldValue)
{
    uint8_t ret = FALSE;

    if (theAboutConfigVar[fieldIndex].mode0Write && theAboutConfigVar[fieldIndex].mode7Public) {
        ret = PropertyStore_SetValueForLang(fieldIndex, langIndex, fieldValue);
    } else {
        AJ_Printf("UpdateFieldInRAM ERROR - field %s has read only attribute or is private\n", theAboutConfigVar[fieldIndex].keyName);
    }

    return ret;
}

static uint8_t DeleteFieldFromRAM(enum_field_indecies_t fieldIndex, enum_lang_indecies_t langIndex)
{
    return UpdateFieldInRAM(fieldIndex, langIndex, "");
}
#endif

AJ_Status PropertyStore_ReadAll(AJ_Message* msg, property_store_filter_t filter, enum_lang_indecies_t langIndex)
{
    AJ_Status status = AJ_OK;
    AJ_Arg array;
    AJ_Arg array2;
    AJ_Arg dict;

    AJ_Printf("PropertyStore_ReadAll()\n");

    do {
        CHECK(AJ_MarshalContainer(msg, &array, AJ_ARG_ARRAY));

        enum_field_indecies_t fieldIndex = 0;
        for (; fieldIndex < NUMBER_OF_KEYS; fieldIndex++) {
#ifdef CONFIG_SERVICE
            if (theAboutConfigVar[fieldIndex].mode7Public && (filter.bit0About || (filter.bit1Config && theAboutConfigVar[fieldIndex].mode0Write) || (filter.bit2Announce && theAboutConfigVar[fieldIndex].mode1Announce))) {
#else
            if (theAboutConfigVar[fieldIndex].mode7Public && (filter.bit0About || (filter.bit2Announce && theAboutConfigVar[fieldIndex].mode1Announce))) {
#endif
                const char* value;
                value = PropertyStore_GetValueForLang(fieldIndex, langIndex);

                if (value == NULL) { // Non existing values are skipped!
                    AJ_Printf("PropertyStore_ReadAll - Failed to get value for fieldIndex=%d langIndex=%d, skipping.\n", (int)fieldIndex, (int)langIndex);
                } else {
                    CHECK(AJ_MarshalContainer(msg, &dict, AJ_ARG_DICT_ENTRY));
                    CHECK(AJ_MarshalArgs(msg, "s", theAboutConfigVar[fieldIndex].keyName));

                    if (fieldIndex == AppID) {              //Todo - replace it with generic solution
                        CHECK(AJ_MarshalVariant(msg, "ay"));
                        AJ_Arg arg;
                        uint8_t rawValue[16];
                        status = AJ_HexToRaw(value, 0, rawValue, (size_t)sizeof(rawValue));
                        CHECK(AJ_MarshalArg(msg, AJ_InitArg(&arg, AJ_ARG_BYTE, AJ_ARRAY_FLAG, rawValue, sizeof(rawValue))));
#ifdef CONFIG_SERVICE
                    } else if (fieldIndex == MaxLength) {
                        CHECK(AJ_MarshalVariant(msg, "q"));
                        CHECK(AJ_MarshalArgs(msg, "q", DEVICE_NAME_VALUE_LENGTH));
#endif
                    } else {
                        CHECK(AJ_MarshalVariant(msg, "s"));
                        CHECK(AJ_MarshalArgs(msg, "s", value));
                    }
                    CHECK(AJ_MarshalCloseContainer(msg, &dict));
                }
            }
        }

        if (filter.bit0About) {
            // Add supported languages
            CHECK(AJ_MarshalContainer(msg, &dict, AJ_ARG_DICT_ENTRY));
            CHECK(AJ_MarshalArgs(msg, "s", defaultLanguagesKeyName));
            CHECK(AJ_MarshalVariant(msg, "as"));
            CHECK(AJ_MarshalContainer(msg, &array2, AJ_ARG_ARRAY));

            enum_lang_indecies_t index = NO_LANGUAGE_INDEX;
            for (; index < NUMBER_OF_LANGUAGES; index++) {
                CHECK(AJ_MarshalArgs(msg, "s", theDefaultLanguages[index]));
            }

            CHECK(AJ_MarshalCloseContainer(msg, &array2));
            CHECK(AJ_MarshalCloseContainer(msg, &dict));
        }
        CHECK(AJ_MarshalCloseContainer(msg, &array));
    } while (0);

    return status;
}

#ifdef CONFIG_SERVICE
AJ_Status PropertyStore_Update(const char* key, enum_lang_indecies_t langIndex, const char* value)
{
    if (UpdateFieldInRAM(PropertyStore_GetIndexOfField(key), langIndex, value)) {
        return AJ_OK;
    }

    return AJ_ERR_FAILURE;
}

AJ_Status PropertyStore_Reset(const char* key, enum_lang_indecies_t langIndex)
{
    if (DeleteFieldFromRAM(PropertyStore_GetIndexOfField(key), langIndex)) {
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
