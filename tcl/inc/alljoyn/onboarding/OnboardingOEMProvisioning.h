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

#ifndef _ONBOARDINGOEMPROVISIONING_H_
#define _ONBOARDINGOEMPROVISIONING_H_

/** @defgroup OnboardingOEMProvisioning
 *
 *  @{
 */

#include <alljoyn.h>

/*
 * Modify these variables to change the service's behavior
 */
#define AJ_OBS_OBINFO_NV_ID (AJ_PROPERTIES_NV_ID_END + 1)
#define SSID_MAX_LENGTH 32
#define PASSCODE_MAX_LENGTH 128 //64 characters, hexified
#define ERROR_MESSAGE_LEN 32

/**
 * Wait time for clients to connect to SoftAP station (ms)
 */
extern const uint32_t OBS_WAIT_FOR_SOFTAP_CONNECTION; // 600000;
/**
 * retry parameters after failed connection of already validated configuration.
 */
extern const uint8_t OBS_MAX_RETRIES; // 2;
/**
 * Wait time between retries (ms)
 */
extern const uint32_t OBS_WAIT_BETWEEN_RETRIES; // 180000;
/**
 * maximum number of scan info results.
 */
#define OBS_MAX_SCAN_INFOS (5)

/**
 * authentication types
 */
typedef enum {
    MIN_OF_WIFI_AUTH_TYPE = -4,
    WPA2_AUTO = -3,
    WPA_AUTO = -2,
    any = -1,
    Open = 0,
    WEP = 1,
    WPA_TKIP = 2,
    WPA_CCMP = 3,
    WPA2_TKIP = 4,
    WPA2_CCMP = 5,
    WPS = 6,
    MAX_OF_WIFI_AUTH_TYPE = 7
} OBAuthType;

/**
 * onboarding states
 */
typedef enum {
    NOT_CONFIGURED = 0,
    CONFIGURED_NOT_VALIDATED = 1,
    CONFIGURED_VALIDATING = 2,
    CONFIGURED_VALIDATED = 3,
    CONFIGURED_ERROR = 4,
    CONFIGURED_RETRY = 5,
} OBState;

/**
 * last error
 */
typedef enum {
    OBLastError_Validated = 0,
    OBLastError_Unreachable = 1,
    OBLastError_Unsupported_protocol = 2,
    OBLastError_Unauthorized = 3,
    OBLastError_Error_message = 4,
} OBLastError;

/**
 * on boarding information
 */
typedef struct OBInfo {
    char ssid[SSID_MAX_LENGTH + 1];
    char pc[PASSCODE_MAX_LENGTH + 1];
    OBAuthType authType;
    OBState state;
} OBInfo;

/**
 * on boarding scan information
 */
typedef struct OBScanInfo {
    char ssid[SSID_MAX_LENGTH + 1];
    OBAuthType authType;
} OBScanInfo;

/**
 * on boarding error
 */
typedef struct OBError {
    int16_t code;
    char message[ERROR_MESSAGE_LEN + 1];
} OBError;

//Soft AP definitions
/**
 * Length of manufacture name section in the SoftAPSSID
 */
#define DEVICE_MANUFACTURE_NAME_LEN 7
/**
 * Length of product name section in the SoftAPSSID
 */
#define DEVICE_PRODUCT_NAME_LEN 13
/**
 * Length of serial id section in the SoftAPSSID
 */
#define DEVICE_SERIAL_ID_LEN 7

/**
 * Whether the SSID is hidden
 */
extern const uint8_t OBS_SoftAPIsHidden; // FALSE;

/**
 * SoftAP passpharse. If NULL means OPEN network otherwise assumes WPA2 up to 63 characters.
 */
extern const char* OBS_SoftAPPassphrase; // NULL;

/**
 * Onboarding state variable.
 */
extern OBState obState;

/**
 * Onboarding last error variable.
 */
extern OBError obLastError;

/**
 * Onboarding last scan time.
 */
extern AJ_Time obLastScan;

/**
 * Onboarding scan infos variable.
 */
extern OBScanInfo obScanInfos[OBS_MAX_SCAN_INFOS];

/**
 * Onboarding scan infos count variable.
 */
extern uint8_t obScanInfoCount;

/**
 * Onboarding retry timer.
 */
extern AJ_Time obRetryTimer;

/**
 * Onboarding timer activated state.
 */
extern uint8_t bTimerActivated;

/**
 * read the onboarding information
 * @return status
 */
AJ_Status OBS_ReadInfo(OBInfo* info);

/**
 * write the onboarding information
 * @return status
 */
AJ_Status OBS_WriteInfo(OBInfo* info);

/**
 * clear onboarding information
 * @return status
 */
AJ_Status OBS_ClearInfo();

/** @} */
 #endif /* _ONBOARDINGOEMPROVISIONING_H_ */
