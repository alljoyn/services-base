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

#ifndef _ONBOARDINGMANAGER_H_
#define _ONBOARDINGMANAGER_H_

/** @defgroup OnboardingManager Onboarding State Management
 *
 *  @{
 */

#include <alljoyn.h>
#include <alljoyn/onboarding/OnboardingService.h>

/**
 * Is there a Wi-Fi connection?
 */
uint8_t AJOBS_IsWiFiConnected();

/*
 * Modify these variables to change the service's behavior
 */
#define AJOBS_PASSCODE_MAX_LENGTH 128 /**< 64 characters, hexified */
#define AJOBS_ERROR_MESSAGE_LEN 32    /**< error message length */

/**
 * Maximum number of scan info results.
 */
#define AJOBS_MAX_SCAN_INFOS 5

/**
 * authentication types
 */
typedef enum _AJOBS_AuthType {
    AJOBS_AUTH_TYPE_MIN_OF_WIFI_AUTH_TYPE = -4,
    AJOBS_AUTH_TYPE_WPA2_AUTO = -3,
    AJOBS_AUTH_TYPE_WPA_AUTO = -2,
    AJOBS_AUTH_TYPE_ANY = -1,
    AJOBS_AUTH_TYPE_OPEN = 0,
    AJOBS_AUTH_TYPE_WEP = 1,
    AJOBS_AUTH_TYPE_WPA_TKIP = 2,
    AJOBS_AUTH_TYPE_WPA_CCMP = 3,
    AJOBS_AUTH_TYPE_WPA2_TKIP = 4,
    AJOBS_AUTH_TYPE_WPA2_CCMP = 5,
    AJOBS_AUTH_TYPE_WPS = 6,
    AJOBS_AUTH_TYPE_MAX_OF_WIFI_AUTH_TYPE = 7
} AJOBS_AuthType;

/**
 * Onboarding states
 */
typedef enum _AJOBS_State {
    AJOBS_STATE_NOT_CONFIGURED = 0,
    AJOBS_STATE_CONFIGURED_NOT_VALIDATED = 1,
    AJOBS_STATE_CONFIGURED_VALIDATING = 2,
    AJOBS_STATE_CONFIGURED_VALIDATED = 3,
    AJOBS_STATE_CONFIGURED_ERROR = 4,
    AJOBS_STATE_CONFIGURED_RETRY = 5,
} AJOBS_State;

/**
 * Onboarding state getter.
 */
AJOBS_State AJOBS_GetState();

/**
 * Onboarding state setter.
 */
void AJOBS_SetState(AJOBS_State state);

/**
 * Last connection error
 */
typedef enum _AJOBS_LastError {
    AJOBS_STATE_LAST_ERROR_VALIDATED = 0,
    AJOBS_STATE_LAST_ERROR_UNREACHABLE = 1,
    AJOBS_STATE_LAST_ERROR_UNSUPPORTED_PROTOCOL = 2,
    AJOBS_STATE_LAST_ERROR_UNAUTHORIZED = 3,
    AJOBS_STATE_LAST_ERROR_ERROR_MESSAGE = 4,
} AJOBS_LastError;

/**
 * Onboarding information
 */
typedef struct _AJOBS_Info {
    char ssid[AJOBS_SSID_MAX_LENGTH + 1];            /**< ssid */
    char pc[AJOBS_PASSCODE_MAX_LENGTH + 1];          /**< pc */
    AJOBS_AuthType authType;                         /**< authType */
    AJOBS_State state;                               /**< state */
} AJOBS_Info;

/**
 * Onboarding info getter.
 */
AJ_Status AJOBS_GetInfo(AJOBS_Info* info);

/**
 * Onboarding info setter.
 */
AJ_Status AJOBS_SetInfo(AJOBS_Info* info);

/**
 * Onboarding last scan time.
 */
const AJ_Time* AJOBS_GetLastScanTime();

/**
 * Onboarding scan information.
 */
typedef struct _AJOBS_ScanInfo {
    char ssid[AJOBS_SSID_MAX_LENGTH + 1];            /**< ssid */
    AJOBS_AuthType authType;                         /**< authType */
} AJOBS_ScanInfo;

/**
 * Onboarding scan infos variable getter.
 */
const AJOBS_ScanInfo* AJOBS_GetScanInfos();

/**
 * Onboarding scan infos count variable getter.
 */
uint8_t AJOBS_GetScanInfoCount();

/**
 * Onboarding error
 */
typedef struct _AJOBS_Error {
    int16_t code;                                /**< code */
    char message[AJOBS_ERROR_MESSAGE_LEN + 1];   /**< message */
} AJOBS_Error;

/**
 * Onboarding error getter.
 */
const AJOBS_Error* AJOBS_GetError();

/**
 * Prototype for a function that reads the onboarding information.
 * @return status
 */
typedef AJ_Status (*AJOBS_ReadInfo)(AJOBS_Info* info);

/**
 * Protptype for a function that writes the onboarding information.
 * @return status
 */
typedef AJ_Status (*AJOBS_WriteInfo)(const AJOBS_Info* info);

/**
 * Structure that holds all relevant configuration settings for:
 * a) the SoftAP definition and
 * b) parameter for the recovery/retry algorithm when connection attempts fail
 */
typedef struct _AJOBS_Settings {
    /**
     * Wait time for clients to connect to SoftAP station (ms)
     */
    const uint32_t AJOBS_WAIT_FOR_SOFTAP_CONNECTION; // 600000
    /**
     * Number of retry attempts after failed attempt to connect to an already validated configuration.
     */
    const uint8_t AJOBS_MAX_RETRIES; // 2
    /**
     * Wait time between retries (ms)
     */
    const uint32_t AJOBS_WAIT_BETWEEN_RETRIES; // 180000
    /**
     * SoftAP SSID name.
     **/
    const char AJOBS_SoftAPSSID[AJOBS_SSID_MAX_LENGTH + 1];
    /**
     * Whether the SoftAP SSID is hidden
     */
    const uint8_t AJOBS_SoftAPIsHidden; // FALSE
    /**
     * SoftAP passpharse. If NULL means OPEN network otherwise assumes WPA2 8 to 63 characters long.
     */
    const char* AJOBS_SoftAPPassphrase; // NULL
} AJOBS_Settings;

#define AJOBS_DEFAULT_SETTINGS { 600000, 2, 180000, { 0 }, FALSE, NULL };             /**< jobs default settings */

/**
 * Start Onboarding service framework passing settings and persistence callbacks/
 * @param settings
 * @param readInfo
 * @param writeInfo
 * @return aj_status
 */
AJ_Status AJOBS_Start(const AJOBS_Settings* settings, AJOBS_ReadInfo readInfo, AJOBS_WriteInfo writeInfo);

//Soft AP definitions
/**
 * Length of manufacture name section in the SoftAPSSID
 */
#define AJOBS_DEVICE_MANUFACTURE_NAME_LEN 7
/**
 * Length of product name section in the SoftAPSSID
 */
#define AJOBS_DEVICE_PRODUCT_NAME_LEN 13
/**
 * Length of serial id section in the SoftAPSSID
 */
#define AJOBS_DEVICE_SERIAL_ID_LEN 7

/**
 * Clear onboarding information.
 * @return aj_status
 */
AJ_Status AJOBS_ClearInfo();

/**
 * Establish a Wi-Fi connection either as:
 * a) a SoftAP station with a connected client or
 * b) as a client to the currently configured AP
 * @return aj_status
 */
AJ_Status AJOBS_EstablishWiFi();

/**
 * Switch to retry state toggling between:
 * a) waiting for an Onboarder application to connect to the SoftAP till timeout of AJOBS_WAIT_BETWEEN_RETRIES and
 * b) attempting to reconnect using the current validated configuration
 */
void AJOBS_SwitchToRetry();

/**
 * Disconnect from the current Wi-Fi connection and go into Idle mode.
 * @return aj_status
 */
AJ_Status AJOBS_DisconnectWiFi();

/** @} */ //end of group 'OnboardingManager'
 #endif /* _ONBOARDINGMANAGER_H_ */
