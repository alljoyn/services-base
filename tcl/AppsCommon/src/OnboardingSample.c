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
 * The corresponding flag dbgAJSVCAPP is defined in the containing sample app.
 */
#define AJ_MODULE AJSVCAPP
#include <aj_debug.h>

#include <alljoyn.h>
#include "OnboardingSample.h"
#include <alljoyn/onboarding/OnboardingService.h>
#include <alljoyn/onboarding/OnboardingManager.h>
#include "PropertyStoreOEMProvisioning.h"
#include <alljoyn/services_common/PropertyStore.h>
#include <aj_nvram.h>

#ifndef NDEBUG
extern AJ_EXPORT uint8_t dbgAJSVCAPP;
#endif

static const char* appDeviceManufactureName = NULL;
static const char* appDeviceProductName = NULL;

static const char* GenerateSoftAPSSID(char* obSoftAPssid)
{
    if (obSoftAPssid[0] == '\0') {
        const char* deviceId = AJSVC_PropertyStore_GetValue(AJSVC_PROPERTY_STORE_DEVICE_ID);
        size_t deviceIdLen = strlen(deviceId);
        char manufacture[AJOBS_DEVICE_MANUFACTURE_NAME_LEN + 1] = { 0 };
        size_t manufacureLen = min(strlen(deviceManufactureName), AJOBS_DEVICE_MANUFACTURE_NAME_LEN);
        char product[AJOBS_DEVICE_PRODUCT_NAME_LEN + 1] = { 0 };
        size_t productLen = min(strlen(deviceProductName), AJOBS_DEVICE_PRODUCT_NAME_LEN);
        size_t serialIdLen = min(deviceIdLen, AJOBS_DEVICE_SERIAL_ID_LEN);
        char serialId[AJOBS_DEVICE_SERIAL_ID_LEN + 1] = { 0 };
        memcpy(manufacture, deviceManufactureName, manufacureLen);
        manufacture[manufacureLen] = '\0';
        memcpy(product, deviceProductName, productLen);
        product[productLen] = '\0';
        memcpy(serialId, deviceId + (deviceIdLen - serialIdLen), serialIdLen);
        serialId[serialIdLen] = '\0';
#ifdef _WIN32
        _snprintf(obSoftAPssid, AJOBS_SSID_MAX_LENGTH + 1, "AJ_%s_%s_%s", manufacture, product, serialId);
#else
        snprintf(obSoftAPssid, AJOBS_SSID_MAX_LENGTH + 1, "AJ_%s_%s_%s", manufacture, product, serialId);
#endif
    }
    return obSoftAPssid;
}

#define AJ_OBS_OBINFO_NV_ID (AJ_PROPERTIES_NV_ID_END + 1)

AJ_Status OnboardingReadInfo(AJOBS_Info* info)
{
    AJ_Status status = AJ_OK;
    size_t size = sizeof(AJOBS_Info);

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
            status = AJ_ERR_READ;
        } else {
            AJ_AlwaysPrintf(("Read Info values: state=%d, ssid=%s authType=%d pc=%s\n", info->state, info->ssid, info->authType, info->pc));
        }
    }

    return status;
}

AJ_Status OnboardingWriteInfo(AJOBS_Info* info)
{
    AJ_Status status = AJ_OK;
    size_t size = sizeof(AJOBS_Info);

    if (NULL == info) {
        return AJ_ERR_NULL;
    }

    AJ_AlwaysPrintf(("Going to write Info values: state=%d, ssid=%s authType=%d pc=%s\n", info->state, info->ssid, info->authType, info->pc));

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

static AJOBS_Settings obSettings = AJOBS_DEFAULT_SETTINGS;

AJ_Status Onboarding_Init(const char* deviceManufactureName, const char* deviceProductName)
{
    AJ_Status status = AJ_OK;
    appDeviceManufactureName = deviceManufactureName;
    appDeviceProductName = deviceProductName;
    if (appDeviceManufactureName == NULL || appDeviceManufactureName[0] == '\0') {
        AJ_AlwaysPrintf(("Onboarding_Init(): DeviceManufactureName is NULL or empty\n"));
        status = AJ_ERR_INVALID;
        goto Exit;
    } else if (appDeviceProductName == NULL || appDeviceProductName[0] == '\0') {
        AJ_AlwaysPrintf(("Onboarding_Init(): DeviceProductName is NULL or empty\n"));
        status = AJ_ERR_INVALID;
        goto Exit;
    }
    GenerateSoftAPSSID(obSettings.AJOBS_SoftAPSSID);
    status = AJOBS_Start(&obSettings, &OnboardingReadInfo, &OnboardingWriteInfo);

Exit:

    return status;
}

void Onboarding_DoWork(AJ_BusAttachment* bus)
{
}

AJ_Status Onboarding_Finish()
{
    return AJ_OK;
}
