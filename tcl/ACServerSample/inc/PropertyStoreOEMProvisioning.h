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

#ifndef _PROPERTYSTOREOEMPROVISIONING_H_
#define _PROPERTYSTOREOEMPROVISIONING_H_

/** @defgroup PropertyStoreOEMProvisioning
 *
 *  @{
 */

#include <alljoyn.h>
#include <aj_auth.h>
#include <aj_config.h>

/**
 * field indecies
 */
typedef enum PropertyStoreFieldIndecies {
    ERROR_FIELD_INDEX = -1,
    //Start of keys
    DeviceID,
    AppID,
    DeviceName,
    NUMBER_OF_PERSISTED_KEYS,
    //End of persisted keys that are not remotely configurable
#ifndef CONFIG_SERVICE
    NUMBER_OF_CONFIG_KEYS = NUMBER_OF_PERSISTED_KEYS,
    //End of configurable keys
#endif
    DefaultLanguage = NUMBER_OF_PERSISTED_KEYS,
    Passcode,
    RealmName,
#ifdef CONFIG_SERVICE
    NUMBER_OF_CONFIG_KEYS,
    //End of configurable keys
    AppName = NUMBER_OF_CONFIG_KEYS,
#else
    AppName,
#endif
    Description,
    Manufacturer,
    ModelNumber,
    DateOfManufacture,
    SoftwareVersion,
    AJSoftwareVersion,
    HardwareVersion,
    SupportUrl,
#ifdef CONFIG_SERVICE
    MaxLength,
#endif
    NUMBER_OF_KEYS,
    //End of About keys
} enum_field_indecies_t;

/**
 * language indecies
 */
typedef enum PropertyStoreLangIndecies {
    ERROR_LANGUAGE_INDEX = -1,
    //Start of language keys
    NO_LANGUAGE_INDEX,
    LANG_1 = NO_LANGUAGE_INDEX,
    NUMBER_OF_LANGUAGES
    //End of language keys
} enum_lang_indecies_t;

extern const char* theDefaultLanguages[NUMBER_OF_LANGUAGES];

#define KEY_NAME_LENGTH  20
#define LANG_VALUE_LENGTH 7
#define KEY_VALUE_LENGTH 10
#define MACHINE_ID_LENGTH 32
#define DEVICE_NAME_VALUE_LENGTH 32
#define PASSWORD_VALUE_LENGTH AJ_ADHOC_LEN

/**
 * property container for non-default parameters
 */
typedef struct ConfigVar {
    char* value[NUMBER_OF_LANGUAGES];
    size_t size;
} property_store_config_entry_t;

/**
 * property structure
 */
typedef struct AboutConfigVar {
    char keyName[KEY_NAME_LENGTH + 1];

    // msb=public/private; bit number 3 - initialise once; bit number 2 - multi-language value; bit number 1 - announce; bit number 0 - Read/Write
    uint8_t mode0Write : 1;
    uint8_t mode1Announce : 1;
    uint8_t mode2MultiLng : 1;
    uint8_t mode3Init : 1;
    uint8_t mode4 : 1;
    uint8_t mode5 : 1;
    uint8_t mode6 : 1;
    uint8_t mode7Public : 1;

    char* value[NUMBER_OF_LANGUAGES];
} property_store_entry_t;

/**
 * properties array variable
 */
extern const property_store_entry_t theAboutConfigVar[NUMBER_OF_KEYS];

#define AJ_PROPERTIES_NV_ID_BEGIN (AJ_NVRAM_ID_CREDS_MAX + 1)
#define AJ_PROPERTIES_NV_ID_END   (AJ_PROPERTIES_NV_ID_BEGIN + (int)NUMBER_OF_CONFIG_KEYS * (int)NUMBER_OF_LANGUAGES - 1)

/**
 * structure defines for the purpose of the parameter
 */
typedef struct CategoryFilter {
    uint8_t bit0About : 1;
#ifdef CONFIG_SERVICE
    uint8_t bit1Config : 1;
#endif
    uint8_t bit2Announce : 1;
} property_store_filter_t;

/** @} */
 #endif //_PROPERTYSTOREOEMPROVISIONING_H_
