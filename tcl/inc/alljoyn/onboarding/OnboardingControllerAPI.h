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

#ifndef _ONBOARDINGCONTROLLERAPI_H_
#define _ONBOARDINGCONTROLLERAPI_H_

/** @defgroup Onboarding Controller API
 *
 *  @{
 */
#include <alljoyn/onboarding/OnboardingOEMProvisioning.h>
#include <alljoyn/services_common/PropertyStore.h>
#include <alljoyn/services_common/Services_Common.h>
#include <aj_wifi_ctrl.h>

/**
 * perform a check of the current configuration state and accordingly take the relevant action of
 * either establishing SoftAP mode if not configured
 * or attempt to connect using the current connection info.
 * @param obInfo
 * @return status
 */
AJ_Status OBCAPI_StartSoftAPIfNeededOrConnect(OBInfo* obInfo);

/**
 * disconnect from current mode (SoftAP or client) and go to idle mode.
 * @param reset
 * @return status
 */
AJ_Status OBCAPI_GotoIdleWiFi(uint8_t reset);

/**
 * perform a offboarding clearing the current configuration state and values.
 * @return status
 */
AJ_Status OBCAPI_DoOffboardWiFi();

/**
 * perform a scaninfo.
 * @return status
 */
AJ_Status OBCAPI_DoScanInfo();

/**
 * return whether the Wi-Fi is in SoftAP mode.
 * @return success
 */
int8_t OBCAPI_IsWiFiSoftAP();

/**
 * return whether the Wi-Fi is in client mode.
 * @return success
 */
int8_t OBCAPI_IsWiFiClient();

/** @} */
 #endif // _ONBOARDINGCONTROLLERAPI_H_
