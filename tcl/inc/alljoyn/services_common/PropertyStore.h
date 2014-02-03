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

#ifndef _PROPERTY_STORE_H_
#define _PROPERTY_STORE_H_

#include <alljoyn.h>

/** @defgroup PropertyStore
 *
 *  @{
 */

/**
 * field indices
 */
typedef enum _AJSVC_PropertyStoreFieldIndices {
    AJSVC_PROPERTY_STORE_ERROR_FIELD_INDEX = -1,
    //Start of keys
    AJSVC_PropertyStoreDeviceID,
    AJSVC_PropertyStoreAppID,
    AJSVC_PropertyStoreDeviceName,
    AJSVC_PROPERTY_STORE_NUMBER_OF_PERSISTED_KEYS,
    //End of persisted keys that are not remotely configurable
#ifndef CONFIG_SERVICE
    AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS = AJSVC_PROPERTY_STORE_NUMBER_OF_PERSISTED_KEYS,
    //End of configurable keys
#endif
    AJSVC_PropertyStoreDefaultLanguage = AJSVC_PROPERTY_STORE_NUMBER_OF_PERSISTED_KEYS,
    AJSVC_PropertyStorePasscode,
    AJSVC_PropertyStoreRealmName,
#ifdef CONFIG_SERVICE
    AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS,
    //End of configurable keys
    AJSVC_PropertyStoreAppName = AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS,
#else
    AJSVC_PropertyStoreAppName,
#endif
    AJSVC_PropertyStoreDescription,
    AJSVC_PropertyStoreManufacturer,
    AJSVC_PropertyStoreModelNumber,
    AJSVC_PropertyStoreDateOfManufacture,
    AJSVC_PropertyStoreSoftwareVersion,
    AJSVC_PropertyStoreAJSoftwareVersion,
    AJSVC_PropertyStoreHardwareVersion,
    AJSVC_PropertyStoreSupportUrl,
#ifdef CONFIG_SERVICE
    AJSVC_PropertyStoreMaxLength,
#endif
    AJSVC_PROPERTY_STORE_NUMBER_OF_KEYS,
    //End of About keys
} AJSVC_PropertyStoreFieldIndices;

/**
 * get maximux value length for given key
 * @param reply
 * @param fieldIndex
 * @param langIndex
 * @return status
 */
uint8_t PropertyStore_GetMaxValueLength(AJSVC_PropertyStoreFieldIndices fieldIndex);

/**
 * language indices
 */
#define AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX   -1
#define AJSVC_PROPERTY_STORE_NO_LANGUAGE_INDEX       0
extern const uint_8 AJSVC_PROPERTY_STORE_NUMBER_OF_LANGUAGES;
extern const char** ajsvc_propertyStoreDefaultLanguages;

/**
 * bitfield that defines the category to filter the properties
 */
typedef struct _AJSVC_PropertyStoreCategoryFilter {
    uint8_t bit0About : 1;
#ifdef CONFIG_SERVICE
    uint8_t bit1Config : 1;
#endif
    uint8_t bit2Announce : 1;
} AJSVC_PropertyStoreCategoryFilter;

/**
 * read all properties
 * @param reply
 * @param filter
 * @param langIndex
 * @return status
 */
AJ_Status PropertyStore_ReadAll(AJ_Message* reply, AJSVC_PropertyStoreCategoryFilter filter, int8_t langIndex);

/**
 * update given property
 * @param key
 * @param langIndex
 * @param value
 * @return status
 */
AJ_Status PropertyStore_Update(const char* key, int8_t langIndex, const char* value);

/**
 * reset given property back to default
 * @param key
 * @param langIndex
 * @return status
 */
AJ_Status PropertyStore_Reset(const char* key, int8_t langIndex);

/**
 * reset all properties back to defaults
 * @return status
 */
AJ_Status PropertyStore_ResetAll();

/**
 * initialize the store
 */
void PropertyStore_Init();

/**
 * get field name for given field index
 * @param fieldIndex
 * @return fieldName
 */
const char* PropertyStore_GetFieldNameForIndex(AJSVC_PropertyStoreFieldIndices fieldIndex);

/**
 * get field index for given field name
 * @param fieldName
 * @return fieldIndex
 */
AJSVC_PropertyStoreFieldIndices PropertyStore_GetIndexOfField(const char* fieldName);

/**
 * get value for given field index for given language index
 * @param fieldIndex
 * @param langIndex
 * @return value
 */
const char* PropertyStore_GetValueForLang(AJSVC_PropertyStoreFieldIndices fieldIndex, int8_t langIndex);

/**
 * get value for field index for default language
 * @param fieldIndex
 * @return value
 */
const char* PropertyStore_GetValue(AJSVC_PropertyStoreFieldIndices fieldIndex);

/**
 * set value for given field index for given language index
 * @param fieldIndex
 * @param langIndex
 * @param value
 * @return success
 */
uint8_t PropertyStore_SetValueForLang(AJSVC_PropertyStoreFieldIndices fieldIndex, int8_t langIndex, const char* value);

/**
 * set value for given field index for the default language
 * @param fieldIndex
 * @param value
 * @return success
 */
uint8_t PropertyStore_SetValue(AJSVC_PropertyStoreFieldIndices fieldIndex, const char* value);

/**
 * get default language index among all languages indexes
 * @return langIndex
 */
int8_t PropertyStore_GetCurrentDefaultLanguageIndex();

/**
 * returns the language index for the given language name
 * @param language
 * @return langIndex
 */
int8_t PropertyStore_GetLanguageIndex(const char* const language);

/**
 * load all persisted values
 */
AJ_Status PropertyStore_LoadAll();

/**
 * save all persisted values
 */
AJ_Status PropertyStore_SaveAll();

/**
 * clear persisted values and load all factory defaults
 */
void PropertyStore_FactoryReset();

/** @} */
 #endif /* _PROPERTY_STORE_H_ */
