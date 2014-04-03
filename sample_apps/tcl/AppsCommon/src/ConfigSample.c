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

#include "ConfigSample.h"
#include <alljoyn/config/ConfigService.h>
#include <alljoyn.h>
#include "PropertyStoreOEMProvisioning.h"
#include <alljoyn/services_common/PropertyStore.h>
#include <aj_creds.h>
#ifdef ONBOARDING_SERVICE
    #include <alljoyn/onboarding/OnboardingManager.h>
#endif

#ifndef NDEBUG
extern AJ_EXPORT uint8_t dbgAJSVCAPP;
#endif

static AJ_Status FactoryReset()
{
    AJ_Status status = AJ_OK;

    AJ_AlwaysPrintf(("GOT FACTORY RESET\n"));
    status = AJSVC_PropertyStore_ResetAll();
    if (status != AJ_OK) {
        return status;
    }
    AJ_ClearCredentials();
#ifdef ONBOARDING_SERVICE
    status = AJOBS_ClearInfo();
    if (status != AJ_OK) {
        return status;
    }
#endif // ONBOARDING_SERVICE

    return AJ_ERR_RESTART_APP;     // Force disconnect of AJ and services and reconnection of WiFi on restart of app
}

static AJ_Status Restart()
{
    AJ_AlwaysPrintf(("GOT RESTART REQUEST\n"));
    AJ_AboutSetShouldAnnounce(); // Set flag for sending an updated Announcement
    return AJ_ERR_RESTART_APP; // Force disconnect of AJ and services and reconnection of WiFi on restart of app
}

static AJ_Status SetPasscode(const char* daemonRealm, const uint8_t* newPasscode, uint8_t newPasscodeLen)
{
    AJ_Status status = AJ_OK;
    char newStringPasscode[PASSWORD_VALUE_LENGTH + 1];

    status = AJ_RawToHex(newPasscode, newPasscodeLen, newStringPasscode, sizeof(newStringPasscode), FALSE);
    if (status != AJ_OK) {
        return status;
    }
    if (AJSVC_PropertyStore_SetValue(AJSVC_PROPERTY_STORE_REALM_NAME, daemonRealm) && AJSVC_PropertyStore_SetValue(AJSVC_PROPERTY_STORE_PASSCODE, newStringPasscode)) {

        status = AJSVC_PropertyStore_SaveAll();
        if (status != AJ_OK) {
            return status;
        }
        AJ_ClearCredentials();
        status = AJ_ERR_READ;     //Force disconnect of AJ and services to refresh current sessions
    } else {

        status = AJSVC_PropertyStore_LoadAll();
        if (status != AJ_OK) {
            return status;
        }
    }

    return status;
}

static uint8_t IsValueValid(const char* key, const char* value)
{
    return TRUE;
}

AJ_Status Config_Init()
{
    AJ_Status status = AJCFG_Start(&FactoryReset, &Restart, &SetPasscode, &IsValueValid);
    return status;
}

void Config_DoWork(AJ_BusAttachment* bus)
{
}

AJ_Status Config_Finish(AJ_BusAttachment* busAttachment)
{
    return AJ_OK;
}
