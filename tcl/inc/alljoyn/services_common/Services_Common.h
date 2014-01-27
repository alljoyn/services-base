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

#ifndef _SERVICES_COMMON_H_
#define _SERVICES_COMMON_H_

#include <alljoyn/services_common/PropertyStore.h>
#define CHECK(x) if ((status = (x)) != AJ_OK) { break; }

/**
 * busAttachment that will be used for connecting and sending messages
 * Needs to be defined in app
 */
extern AJ_BusAttachment busAttachment;

/**
 * "boolean" that reflects whether the bus is connected or not
 * Can be set to TRUE or FALSE - should default to FALSE
 */
extern uint8_t isBusConnected;

/**
 * "boolean" that reflects whether at the conclusion of this message loop processing the device will reboot
 * Can be set to TRUE or FALSE - should default to FALSE
 */
extern uint8_t isRebootRequired;

/**
 * UpdateNotAllowed Error Message for services
 */
#define AJSVC_ERROR_UPDATE_NOT_ALLOWED         "org.alljoyn.Error.UpdateNotAllowed"

/**
 * InvalidValue Error Message for services
 */
#define AJSVC_ERROR_INVALID_VALUE               "org.alljoyn.Error.InvalidValue"

/**
 * FeatureNotAvailable Error Message for services
 */
#define AJSVC_ERROR_FEATURE_NOT_AVAILABLE       "org.alljoyn.Error.FeatureNotAvailable"

/**
 * MazSizeExceeded Error Message for services
 */
#define AJSVC_ERROR_MAX_SIZE_EXCEEDED           "org.alljoyn.Error.MaxSizeExceeded"

/**
 * LanguageNotSupported Error Message for services
 */
#define AJSVC_ERROR_LANGUAGE_NOT_SUPPORTED      "org.alljoyn.Error.LanguageNotSupported"

/**
 * returns the language index for the given language name possibly creating an error reply message if erred
 * @param msg
 * @param reply
 * @param language
 * @param langIndex
 * @return success
 */
uint8_t Common_IsLanguageSupported(AJ_Message* msg, AJ_Message* reply, const char* language, enum_lang_indecies_t* langIndex);

/*!
   \brief Length of the AppID field
 */
#define UUID_LENGTH 16

/**
 * returns the language index for the given language name possibly creating an error reply message if erred
 * @param msg   the message to marshal the appId into
 * @param appId the application id to marshal
 * @return status
 */
AJ_Status Common_MarshalAppId(AJ_Message* msg, const char* appId);

// The following is to be included in all services
#include <alljoyn/services_common/Services_Registration.h>

#endif /* _SERVICES_COMMON_H_ */
