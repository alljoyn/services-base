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
#include <alljoyn/onboarding/OnboardingControllerAPI.h>
#include <aj_nvram.h>
#include <aj_wifi_ctrl.h>
#include <alljoyn/services_common/PropertyStore.h>
#include <alljoyn/about/AboutOEMProvisioning.h>
#include <alljoyn/onboarding/OnboardingOEMProvisioning.h>

OBState obState = NOT_CONFIGURED;
OBError obLastError;
AJ_Time obLastScan;
OBScanInfo obScanInfos[OBS_MAX_SCAN_INFOS];
uint8_t obScanInfoCount = 0;
AJ_Time obRetryTimer;
uint8_t bTimerActivated = FALSE;
uint8_t bFirstStart = TRUE;

/* SoftAP state variable for AJTCL */
extern uint8_t IsSoftAP;

static char obSoftAPssid[SSID_MAX_LENGTH + 1] = { 0 };

static const char* OB_GetSoftAPSSID()
{
    if (obSoftAPssid[0] == '\0') {
        const char* deviceId = PropertyStore_GetValue(AJSVC_PropertyStoreDeviceID);
        size_t deviceIdLen = strlen(deviceId);
        char manufacture[DEVICE_MANUFACTURE_NAME_LEN + 1] = { 0 };
        size_t manufacureLen = min(strlen(deviceManufactureName), DEVICE_MANUFACTURE_NAME_LEN);
        char product[DEVICE_PRODUCT_NAME_LEN + 1] = { 0 };
        size_t productLen = min(strlen(deviceProductName), DEVICE_PRODUCT_NAME_LEN);
        size_t serialIdLen = min(deviceIdLen, DEVICE_SERIAL_ID_LEN);
        char serialId[DEVICE_SERIAL_ID_LEN + 1] = { 0 };
        memcpy(manufacture, deviceManufactureName, manufacureLen);
        manufacture[manufacureLen] = '\0';
        memcpy(product, deviceProductName, productLen);
        product[productLen] = '\0';
        memcpy(serialId, deviceId + (deviceIdLen - serialIdLen), serialIdLen);
        serialId[serialIdLen] = '\0';
#ifdef _WIN32
        _snprintf(obSoftAPssid, SSID_MAX_LENGTH + 1, "AJ_%s_%s_%s", manufacture, product, serialId);
#else
        snprintf(obSoftAPssid, SSID_MAX_LENGTH + 1, "AJ_%s_%s_%s", manufacture, product, serialId);
#endif
    }
    return obSoftAPssid;
}

#ifndef AJ_CASE
#define AJ_CASE(_status) case _status: return # _status
#endif

static const char* AJ_WiFiConnectStateText(AJ_WiFiConnectState state)
{
#ifdef NDEBUG
    /* Expectation is that thin client status codes will NOT go beyond 255 */
    static char code[4];

#ifdef _WIN32
    _snprintf(code, sizeof(code), "%03u", state);
#else
    snprintf(code, sizeof(code), "%03u", state);
#endif

    return code;
#else
    switch (state) {
        AJ_CASE(AJ_WIFI_IDLE);
        AJ_CASE(AJ_WIFI_CONNECTING);
        AJ_CASE(AJ_WIFI_CONNECT_OK);
        AJ_CASE(AJ_WIFI_SOFT_AP_INIT);
        AJ_CASE(AJ_WIFI_SOFT_AP_UP);
        AJ_CASE(AJ_WIFI_STATION_OK);
        AJ_CASE(AJ_WIFI_CONNECT_FAILED);
        AJ_CASE(AJ_WIFI_AUTH_FAILED);
        AJ_CASE(AJ_WIFI_DISCONNECTING);

    default:
        return "<unknown>";
    }
#endif
}

int8_t OBCAPI_IsWiFiSoftAP()
{
    AJ_WiFiConnectState state = AJ_GetWifiConnectState();
    switch (state) {
    case AJ_WIFI_SOFT_AP_INIT:
    case AJ_WIFI_SOFT_AP_UP:
    case AJ_WIFI_STATION_OK:
        return TRUE;

    case AJ_WIFI_IDLE:
    case AJ_WIFI_CONNECTING:
    case AJ_WIFI_CONNECT_OK:
    case AJ_WIFI_CONNECT_FAILED:
    case AJ_WIFI_AUTH_FAILED:
    case AJ_WIFI_DISCONNECTING:
    default:
        return FALSE;
    }
}

int8_t OBCAPI_IsWiFiClient()
{
    AJ_WiFiConnectState state = AJ_GetWifiConnectState();
    switch (state) {
    case AJ_WIFI_CONNECTING:
    case AJ_WIFI_CONNECT_OK:
    case AJ_WIFI_DISCONNECTING:
    case AJ_WIFI_CONNECT_FAILED:
    case AJ_WIFI_AUTH_FAILED:
        return TRUE;

    case AJ_WIFI_IDLE:
    case AJ_WIFI_SOFT_AP_INIT:
    case AJ_WIFI_SOFT_AP_UP:
    case AJ_WIFI_STATION_OK:
    default:
        return FALSE;
    }
}

static OBAuthType OBM_GetAuthType(AJ_WiFiSecurityType secType, AJ_WiFiCipherType cipherType)
{
    switch (secType) {
    case AJ_WIFI_SECURITY_NONE:
        return Open;

    case AJ_WIFI_SECURITY_WEP:
        return WEP;

    case AJ_WIFI_SECURITY_WPA:
        switch (cipherType) {
        case AJ_WIFI_CIPHER_TKIP:
            return WPA_TKIP;

        case AJ_WIFI_CIPHER_CCMP:
            return WPA_CCMP;

        case AJ_WIFI_CIPHER_NONE:
            return WPA_AUTO;

        default:
            break;
        }
        break;

    case AJ_WIFI_SECURITY_WPA2:
        switch (cipherType) {
        case AJ_WIFI_CIPHER_TKIP:
            return WPA2_TKIP;

        case AJ_WIFI_CIPHER_CCMP:
            return WPA2_CCMP;

        case AJ_WIFI_CIPHER_NONE:
            return WPA2_AUTO;

        default:
            break;
        }
        break;
    }
    return any;
}

static AJ_WiFiSecurityType OBM_GetSecType(OBAuthType authType)
{
    A_UINT32 secType = AJ_WIFI_SECURITY_NONE;

    switch (authType) {
    case Open:
        secType = AJ_WIFI_SECURITY_NONE;
        break;

    case WEP:
        secType = AJ_WIFI_SECURITY_WEP;
        break;

    case WPA_TKIP:
    case WPA_CCMP:
        secType = AJ_WIFI_SECURITY_WPA;
        break;

    case WPA2_TKIP:
    case WPA2_CCMP:
    case WPS:
        secType = AJ_WIFI_SECURITY_WPA2;
        break;

    default:
        break;
    }

    return secType;
}

static AJ_WiFiCipherType OBM_GetCipherType(OBAuthType authType)
{
    A_UINT32 cipherType = AJ_WIFI_CIPHER_NONE;

    switch (authType) {
    case Open:
        cipherType = AJ_WIFI_CIPHER_NONE;
        break;

    case WEP:
        cipherType = AJ_WIFI_CIPHER_WEP;
        break;

    case WPA_TKIP:
    case WPA2_TKIP:
        cipherType = AJ_WIFI_CIPHER_TKIP;
        break;

    case WPS:
    case WPA_CCMP:
    case WPA2_CCMP:
        cipherType = AJ_WIFI_CIPHER_CCMP;
        break;

    default:
        break;
    }

    return cipherType;
}

AJ_Status OBS_ReadInfo(OBInfo* info)
{
    AJ_Status status = AJ_OK;
    size_t size = sizeof(OBInfo);

    if (NULL == info) {
        return AJ_ERR_NULL;
    }
    memset(info, 0, size);

    if (!AJ_NVRAM_Exist(AJ_OBS_OBINFO_NV_ID)) {
        return AJ_ERR_INVALID;
    }

    AJ_NV_DATASET* nvramHandle = AJ_NVRAM_Open(AJ_OBS_OBINFO_NV_ID, "r", 0);
    if (nvramHandle != NULL) {
        int sizeRead = AJ_NVRAM_Read(info, size, nvramHandle);
        status = AJ_NVRAM_Close(nvramHandle);
        if (sizeRead != sizeRead) {
            status = AJ_ERR_WRITE;
        } else {
            AJ_Printf("Readed Info values: state=%d, ssid=%s authType=%d pc=%s\n", info->state, info->ssid, info->authType, info->pc);
        }
    }

    return status;
}

AJ_Status OBS_WriteInfo(OBInfo* info)
{
    AJ_Status status = AJ_OK;
    size_t size = sizeof(OBInfo);

    if (NULL == info) {
        return AJ_ERR_NULL;
    }

    AJ_Printf("Going to write Info values: state=%d, ssid=%s authType=%d pc=%s\n", info->state, info->ssid, info->authType, info->pc);

    AJ_NV_DATASET* nvramHandle = AJ_NVRAM_Open(AJ_OBS_OBINFO_NV_ID, "w", size);
    if (nvramHandle != NULL) {
        int sizeWritten = AJ_NVRAM_Write(info, size, nvramHandle);
        status = AJ_NVRAM_Close(nvramHandle);
        if (sizeWritten != size) {
            status = AJ_ERR_WRITE;
        }
    }

    return status;
}

static void OBM_GotScanInfo(const char* ssid, const uint8_t bssid[6], uint8_t rssi, OBAuthType authType)
{
    if (obScanInfoCount < OBS_MAX_SCAN_INFOS) {
        strncpy(obScanInfos[obScanInfoCount].ssid, ssid, SSID_MAX_LENGTH);
        obScanInfos[obScanInfoCount].authType = authType;
        ++obScanInfoCount;
    }
}

static void OBM_WiFiScanResult(void* context, const char* ssid, const uint8_t bssid[6], uint8_t rssi, AJ_WiFiSecurityType secType, AJ_WiFiCipherType cipherType)
{
    static const char* const sec[] = { "OPEN", "WEP", "WPA", "WPA2" };
    static const char* const typ[] = { "", ":TKIP", ":CCMP", ":WEP" };
    AJ_Printf("WiFiScanResult found ssid=%s rssi=%d security=%s%s\n", ssid, rssi, sec[secType], typ[cipherType]);
    OBAuthType authType = OBM_GetAuthType(secType, cipherType);
    OBM_GotScanInfo(ssid, bssid, rssi, authType);
}

AJ_Status OBCAPI_DoScanInfo()
{
    AJ_Status status = AJ_OK;

    memset(obScanInfos, 0, sizeof(obScanInfos));
    obScanInfoCount = 0;
    // Scan neighbouring networks and save info -> Call OBM_WiFiScanResult().
    status = AJ_WiFiScan(NULL, OBM_WiFiScanResult, OBS_MAX_SCAN_INFOS);
    if (status == AJ_OK) {
        AJ_InitTimer(&obLastScan);
    }

    return status;
}

AJ_Status OBCAPI_GotoIdleWiFi(uint8_t reset)
{
    AJ_Status status = AJ_OK;
    AJ_WiFiConnectState wifiConnectState = AJ_GetWifiConnectState();

    if (wifiConnectState != AJ_WIFI_IDLE) {
        status = AJ_DisconnectWiFi();
        if (reset) {
            status = AJ_ResetWiFi();
        }
    }
    AJ_Sleep(1000);

    while ((wifiConnectState != AJ_WIFI_IDLE) && (wifiConnectState != AJ_WIFI_CONNECT_FAILED) && (wifiConnectState != AJ_WIFI_DISCONNECTING)) {
        wifiConnectState = AJ_GetWifiConnectState();
        AJ_Printf("WIFI_CONNECT_STATE: %s\n", AJ_WiFiConnectStateText(wifiConnectState));
        AJ_Sleep(500);
    }

    IsSoftAP = FALSE;

    status = OBCAPI_DoScanInfo();
    return status;
}

static AJ_Status OBM_EnableSoftAP(const char* ssid, A_UINT8 hidden, const char* passphrase)
{
    AJ_Status status = AJ_OK;

    status = OBCAPI_GotoIdleWiFi(TRUE);

    AJ_Printf("Configuring soft AP %s\n", ssid);

    status = AJ_EnableSoftAP(ssid, hidden, passphrase);
    if (status != AJ_OK) {
        AJ_Printf("EnableSoftAP error\n");
        return status;
    }

    AJ_WiFiConnectState wifiConnectState = AJ_GetWifiConnectState();
    while (wifiConnectState == AJ_WIFI_SOFT_AP_INIT) {
        AJ_Sleep(100);
        AJ_Printf(".");
        wifiConnectState = AJ_GetWifiConnectState();
    }

    if ((wifiConnectState == AJ_WIFI_SOFT_AP_UP) || (wifiConnectState == AJ_WIFI_STATION_OK)) {
        IsSoftAP = TRUE;
        AJ_Printf("Soft AP Wifi state = %s\n", AJ_WiFiConnectStateText(wifiConnectState));
    } else {
        AJ_Printf("ERROR unexpected soft AP Wifi state = %s\n", AJ_WiFiConnectStateText(wifiConnectState));
    }

    return status;
}

static AJ_Status OBCAPI_DoConnectWifi(OBInfo* connectInfo)
{
    AJ_Status status = AJ_OK;
    uint8_t retries = 0;

    AJ_Printf("Attempting to connect to %s with passcode=%s and auth=%d\n", connectInfo->ssid, connectInfo->pc, connectInfo->authType);

    AJ_WiFiSecurityType secType = AJ_WIFI_SECURITY_NONE;
    AJ_WiFiCipherType cipherType = AJ_WIFI_CIPHER_NONE;

    OBAuthType fallback = Open;
    OBAuthType fallbackUntil = Open;

    switch (connectInfo->authType) {
    case any:
        if (strlen(connectInfo->pc) == 0) {
            break;
        }
        fallback = WPA2_CCMP;
        fallbackUntil = Open;
        break;

    case WPA_AUTO:
        if (strlen(connectInfo->pc) == 0) {
            break;
        }
        fallback = WPA_CCMP;
        fallbackUntil = WPA_TKIP;
        break;

    case WPA2_AUTO:
        if (strlen(connectInfo->pc) == 0) {
            break;
        }
        fallback = WPA2_CCMP;
        fallbackUntil = WPA2_TKIP;
        break;

    default:
        fallback = connectInfo->authType;
    }

    secType = OBM_GetSecType(fallback);
    cipherType = OBM_GetCipherType(fallback);
    AJ_Printf("Trying to connect with auth=%d (secType=%d, cipherType=%d)\n", fallback, secType, cipherType);

    status = OBCAPI_GotoIdleWiFi(TRUE); // Go into IDLE mode and reset wifi
    while (1) {
        AJ_WiFiConnectState wifiConnectState = AJ_GetWifiConnectState();

        if (connectInfo->state == CONFIGURED_NOT_VALIDATED) {
            connectInfo->state = CONFIGURED_VALIDATING;
        }

        uint8_t raw[(PASSCODE_MAX_LENGTH / 2) + 1];
        char* password = connectInfo->pc;
        if (AJ_WIFI_CIPHER_WEP != cipherType) {
            size_t hexLen = strlen(connectInfo->pc);
            AJ_HexToRaw(connectInfo->pc, hexLen, raw, (PASSCODE_MAX_LENGTH / 2) + 1);
            password = (char*)raw;
            password[hexLen / 2] = '\0';
        }

        status = AJ_ConnectWiFi(connectInfo->ssid, secType, cipherType, password);
        AJ_Printf("%s: AJ_ConnectWifi returned %s\n", __FUNCTION__, AJ_StatusText(status));

        if (status == AJ_OK) {
            // Clear last error
            obLastError.code = OBLastError_Validated;
            obLastError.message[0] = '\0';

            wifiConnectState = AJ_GetWifiConnectState();
            AJ_Printf("WIFI_CONNECT_STATE: %s, connectInfo->state: %u\n", AJ_WiFiConnectStateText(wifiConnectState), connectInfo->state);
            uint32_t counter = 0;
            while (wifiConnectState == AJ_WIFI_CONNECTING || wifiConnectState == AJ_WIFI_DISCONNECTING) {
                AJ_Printf(".");
                AJ_Sleep(500);
                wifiConnectState = AJ_GetWifiConnectState();
                if (++counter >= 60) {
                    break;
                }
            }
            AJ_Printf("\n");

            //wifiConnectState = AJ_WIFI_CONNECT_FAILED;// This line should be disable. Debug only. ###
            switch (wifiConnectState) {
            case AJ_WIFI_CONNECT_OK:
                obState = CONFIGURED_VALIDATED;
                connectInfo->authType = fallback;
                break;

            case AJ_WIFI_CONNECT_FAILED:
            case AJ_WIFI_AUTH_FAILED:
                if (wifiConnectState == AJ_WIFI_CONNECT_FAILED) {
                    obLastError.code = OBLastError_Unreachable;
                    strncpy(obLastError.message, "Network unreachable!", ERROR_MESSAGE_LEN);
                } else { // (AJ_WIFI_AUTH_FAILED)
                    obLastError.code = OBLastError_Unauthorized;
                    strncpy(obLastError.message, "Authorization failed!", ERROR_MESSAGE_LEN);
                }

                if (connectInfo->state == CONFIGURED_VALIDATED || connectInfo->state == CONFIGURED_RETRY) {
                    obState = CONFIGURED_RETRY;
                } else {
                    obState = CONFIGURED_ERROR;
                }
                break;

            default:
                AJ_Printf("Warning - OBS_DoConnectWifi wifiConnectState = %s\n", AJ_WiFiConnectStateText(wifiConnectState));
                obLastError.code = OBLastError_Error_message;
                strncpy(obLastError.message, "Failed to connect! Unexpected error", ERROR_MESSAGE_LEN);
                obState = CONFIGURED_ERROR;
                break;
            }

            if (obState == CONFIGURED_VALIDATED) {
                break;
            }

            if (obState == CONFIGURED_ERROR || obState == CONFIGURED_RETRY) {
                retries++;
                if (retries >= OBS_MAX_RETRIES) {
                    if (obState == CONFIGURED_ERROR && connectInfo->state == CONFIGURED_VALIDATING) {
                        if (connectInfo->authType < 0 && fallback > fallbackUntil) {
                            obLastError.code = OBLastError_Unsupported_protocol;
                            strncpy(obLastError.message, "Unsupported protocol", ERROR_MESSAGE_LEN);
                            fallback--; // Try next authentication protocol
                            secType = OBM_GetSecType(fallback);
                            cipherType = OBM_GetCipherType(fallback);
                            retries = 0;
                            AJ_Printf("Trying to connect with fallback auth=%d (secType=%d, cipherType=%d)\n", fallback, secType, cipherType);
                            status = OBCAPI_GotoIdleWiFi(FALSE); // Go into IDLE mode disconnecting from current connection
                            continue;
                        }
                    }
                    break;
                }
                AJ_Printf("Retry number %d out of %d\n", retries, OBS_MAX_RETRIES);
            }
        } else {
            obLastError.code = OBLastError_Error_message;
            strncpy(obLastError.message, "Driver error", ERROR_MESSAGE_LEN);
            obState = CONFIGURED_ERROR;
            break;
        }
        status = OBCAPI_GotoIdleWiFi(FALSE); // Go into IDLE mode disconnecting from current connection
    }

    connectInfo->state = obState;
    status = OBS_WriteInfo(connectInfo);
    return status;
}

AJ_Status OBCAPI_StartSoftAPIfNeededOrConnect(OBInfo* obInfo)
{
    AJ_Status status = AJ_OK;

    // Check if just started
    if (bFirstStart) {
        bFirstStart = FALSE;
        if (obInfo->state == CONFIGURED_RETRY) {
            obInfo->state = CONFIGURED_VALIDATED;
        }
    }
    while (1) {
        // Check if require to switch into AP mode.
        if ((obInfo->state == NOT_CONFIGURED || obInfo->state == CONFIGURED_ERROR || obInfo->state == CONFIGURED_RETRY)) {
            // Check if not already switched into AP mode.
            if (!OBCAPI_IsWiFiSoftAP()) {
                const char* softApSSID = OB_GetSoftAPSSID();
                AJ_Printf("Establishing SoftAP with ssid=%s%s auth=%s\n", softApSSID, (OBS_SoftAPIsHidden ? " (hidden)" : ""), OBS_SoftAPPassphrase == NULL ? "OPEN" : OBS_SoftAPPassphrase);
                status = OBM_EnableSoftAP(softApSSID, OBS_SoftAPIsHidden, OBS_SoftAPPassphrase);
                if (AJ_OK == status) {
                    if (obInfo->state == CONFIGURED_RETRY) {
                        AJ_Printf("Retry timer activated\n");
                        AJ_InitTimer(&obRetryTimer);
                        bTimerActivated = TRUE;
                    }
                }
            }
            /*
             * Wait until a remote station connects
             */
            AJ_Printf("Waiting for remote station to connect\n");
            AJ_WiFiConnectState wifiConnectState = AJ_GetWifiConnectState();
            while (wifiConnectState != AJ_WIFI_STATION_OK) {
                //check for timer elapsed for retry
                if (bTimerActivated) {
                    uint32_t elapsed = AJ_GetElapsedTime(&obRetryTimer, TRUE);
                    if (elapsed > OBS_WAIT_BETWEEN_RETRIES) {
                        AJ_Printf("Retry timer elapsed at %ums\n", OBS_WAIT_BETWEEN_RETRIES);
                        obInfo->state = CONFIGURED_VALIDATED;
                        status = OBS_WriteInfo(obInfo);
                        break;
                    }
                }
                AJ_Sleep(100);
                wifiConnectState = AJ_GetWifiConnectState();
                if (wifiConnectState != AJ_WIFI_SOFT_AP_UP) {
                    break;
                }
            }
            bTimerActivated = FALSE;
            if (obInfo->state == CONFIGURED_VALIDATED) {
                continue; // Loop back and connect in client mode
            }
        } else {
            if (!OBCAPI_IsWiFiClient()) {
                // Otherwise connect to given configuration and according to error code returned map to relevant onboarding state and set value for LastError and ConnectionResult.
                status = OBCAPI_DoConnectWifi(obInfo);
                if (obInfo->state == CONFIGURED_ERROR || obInfo->state == CONFIGURED_RETRY) {
                    continue; // Loop back and establish SoftAP mode
                }
            }
        }
        break; // Either connected to (as client) or connected from (as SoftAP) a station
    }
    return status;
}

AJ_Status OBS_ClearInfo()
{
    OBInfo emptyInfo;
    memset(&emptyInfo, 0, sizeof(emptyInfo));

    // Clear state
    obState = emptyInfo.state = NOT_CONFIGURED;

    // Clear last error
    obLastError.code = OBLastError_Validated;
    obLastError.message[0] = '\0';

    return (OBS_WriteInfo(&emptyInfo));
}

AJ_Status OBCAPI_DoOffboardWiFi()
{
    return (OBS_ClearInfo());
}

