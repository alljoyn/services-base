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
#include <aj_creds.h>

#include <alljoyn/about/AboutOEMProvisioning.h>
#ifdef CONFIG_SERVICE
    #include <alljoyn/config/ConfigOEMProvisioning.h>
#endif
#ifdef ONBOARDING_SERVICE
    #include <alljoyn/onboarding/OnboardingOEMProvisioning.h>
#endif
#include <alljoyn/services_common/PropertyStore.h>
#include <alljoyn/services_common/Services_Common.h>

#define CHECK(x) if ((status = (x)) != AJ_OK) { break; }

const char* deviceManufactureName = "COMPANY";
const char* deviceProductName = "GENERIC BOARD";

const char* theDefaultLanguages[NUMBER_OF_LANGUAGES] = { "en", "de-AT" };

/**
 * property array of structure with defaults
 */
const property_store_entry_t theAboutConfigVar[NUMBER_OF_KEYS] =
{
//  { "Key Name 19 + '\0'  ", W, A, M, I .. . . ., P,       { "Value for lang1 32/64 + '\0'    ", "Value for lang2 32/64 + '\0'    " } },
    { "DeviceId",             0, 1, 0, 1, 0, 0, 0, 1,       { NULL,                               NULL } },
    { "AppId",                0, 1, 0, 1, 0, 0, 0, 1,       { NULL,                               NULL } },
// Add other persisted keys above this line
    { "DefaultLanguage",      1, 1, 0, 0, 0, 0, 0, 1,       { "en",                               NULL } },
    { "DeviceName",           1, 1, 0, 1, 0, 0, 0, 1,       { "",                                 NULL } },
    { "Passcode",             1, 0, 0, 0, 0, 0, 0, 0,       { "000000",                           NULL } },
    { "RealmName",            1, 0, 0, 0, 0, 0, 0, 0,       { "",                                 NULL } },
// Add other configurable keys above this line
#if     defined CONTROLPANEL_SERVICE
    { "AppName",              0, 1, 0, 0, 0, 0, 0, 1,       { "Controlee",                        NULL } },
#elif   defined NOTIFICATION_SERVICE_PRODUCER
    { "AppName",              0, 1, 0, 0, 0, 0, 0, 1,       { "Notifier",                         NULL } },
#elif   defined ONBOARDING_SERVICE
    { "AppName",              0, 1, 0, 0, 0, 0, 0, 1,       { "Onboardee",                        NULL } },
#elif   defined CONFIG_SERVICE
    { "AppName",              0, 1, 0, 0, 0, 0, 0, 1,       { "Configuree",                       NULL } },
#else
    { "AppName",              0, 1, 0, 0, 0, 0, 0, 1,       { "Announcer",                        NULL } },
#endif
    { "Description",          0, 0, 1, 0, 0, 0, 0, 1,       { "My first IOE device",              "Mein erstes IOE Geraet" } },
    { "Manufacturer",         0, 1, 1, 0, 0, 0, 0, 1,       { "Company A(EN)",                    "Firma A(DE-AT)" } },
    { "ModelNumber",          0, 1, 0, 0, 0, 0, 0, 1,       { "0.0.1",                            NULL } },
    { "DateOfManufacture",    0, 0, 0, 0, 0, 0, 0, 1,       { "2013-10-09",                       NULL } },
    { "SoftwareVersion",      0, 0, 0, 0, 0, 0, 0, 1,       { "0.0.1",                            NULL } },
    { "AJSoftwareVersion",    0, 0, 0, 0, 0, 0, 0, 1,       { "3.4.0",                            NULL } },
    { "HardwareVersion",      0, 0, 0, 0, 0, 0, 0, 1,       { "0.0.1",                            NULL } },
    { "SupportUrl",           0, 0, 1, 0, 0, 0, 0, 1,       { "www.company_a.com",                "www.company_a.com/de-AT" } },
#if     defined CONFIG_SERVICE
    { "MaxLength",            0, 0, 1, 0, 0, 0, 0, 1,       { "",                                 NULL } }
#endif
// Add other about keys above this line
};

const char* aboutIconMimetype = { "image/png" };
const uint8_t aboutIconContent[] =
{ 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52 \
    , 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x32, 0x02, 0x03, 0x00, 0x00, 0x00, 0x63, 0x51, 0x60 \
    , 0x22, 0x00, 0x00, 0x00, 0x0c, 0x50, 0x4c, 0x54, 0x45, 0x65, 0x2d, 0x67, 0xeb, 0x00, 0x88, 0xec \
    , 0x06, 0x8d, 0xf1, 0x44, 0xaa, 0x1f, 0x54, 0xd3, 0x5b, 0x00, 0x00, 0x00, 0x01, 0x74, 0x52, 0x4e \
    , 0x53, 0x00, 0x40, 0xe6, 0xd8, 0x66, 0x00, 0x00, 0x00, 0xe8, 0x49, 0x44, 0x41, 0x54, 0x28, 0xcf \
    , 0x95, 0x92, 0x51, 0x0a, 0xc3, 0x20, 0x0c, 0x86, 0xd3, 0xc0, 0x40, 0x84, 0xdd, 0x61, 0xec, 0x49 \
    , 0x72, 0x9f, 0xde, 0x47, 0xfa, 0x34, 0x3c, 0x45, 0x18, 0x85, 0x49, 0x4e, 0xb9, 0x18, 0xab, 0xd6 \
    , 0xb1, 0x97, 0x06, 0xb4, 0x7e, 0xa6, 0x26, 0xbf, 0x89, 0x00, 0x97, 0xec, 0xb6, 0x9e, 0xc9, 0x8b \
    , 0x0e, 0xee, 0x04, 0x40, 0x92, 0x1b, 0x49, 0x04, 0x7a, 0xcb, 0x01, 0x28, 0x20, 0xc4, 0xd4, 0x7c \
    , 0x0f, 0x90, 0x11, 0x04, 0x39, 0xd0, 0x29, 0x24, 0xd3, 0x39, 0x41, 0x0c, 0x53, 0x3e, 0x4c, 0x1b \
    , 0x4b, 0x4f, 0x87, 0x29, 0x65, 0x49, 0x7b, 0x89, 0x01, 0x64, 0x91, 0x44, 0xf6, 0x2a, 0xc4, 0x26 \
    , 0xf1, 0x1f, 0x5d, 0x10, 0xbb, 0xba, 0xe5, 0x77, 0x93, 0x15, 0x4c, 0x40, 0xb5, 0x64, 0xc1, 0x9a \
    , 0x66, 0x37, 0x91, 0x2d, 0x10, 0xda, 0xf5, 0x9e, 0xba, 0xc0, 0xad, 0x39, 0x31, 0xea, 0xc0, 0xfe \
    , 0xab, 0x2b, 0x5b, 0x9d, 0x42, 0x11, 0x3e, 0xd0, 0x68, 0x5c, 0x18, 0x13, 0x74, 0xf2, 0x01, 0x4b \
    , 0x71, 0xea, 0x95, 0x3d, 0x05, 0x56, 0xcc, 0x5a, 0xb9, 0xb2, 0x19, 0x20, 0xfb, 0xa8, 0x5f, 0x3e \
    , 0x0a, 0xcd, 0xc4, 0x07, 0x89, 0xd3, 0x84, 0xcd, 0xb7, 0xa8, 0x8b, 0x4c, 0x4f, 0x39, 0xb7, 0x68 \
    , 0xd6, 0x1a, 0xbc, 0xcc, 0xf7, 0x58, 0x7c, 0xad, 0x43, 0x77, 0x8d, 0xf3, 0xd2, 0x72, 0x0c, 0xd2 \
    , 0x16, 0x0d, 0x95, 0x34, 0x91, 0xfa, 0x46, 0x67, 0x21, 0x45, 0xcb, 0xd0, 0x1a, 0x56, 0xc7, 0x41 \
    , 0x7a, 0xc6, 0xe7, 0x89, 0xe4, 0x3f, 0x81, 0x51, 0xfc, 0x79, 0x3f, 0xc3, 0x96, 0xf5, 0xda, 0x5b \
    , 0x84, 0x2f, 0x85, 0x3b, 0x47, 0x0d, 0xe8, 0x0d, 0xca, 0xd3, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45 \
    , 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82, 0x82 };
const size_t aboutIconContentSize = sizeof(aboutIconContent);
const char* aboutIconUrl = { "https://www.alljoyn.org/sites/all/themes/at_alljoyn/images/img-alljoyn-logo.png" };

#ifdef CONFIG_SERVICE
AJ_Status App_FactoryReset()
{
    AJ_Printf("GOT FACTORY RESET\n");
    AJ_Status status = AJ_OK;

    do {
        CHECK(PropertyStore_ResetAll());
        /*CHECK(*/ AJ_ClearCredentials() /*)*/;
#ifdef ONBOARDING_SERVICE
        CHECK(OBS_ClearInfo());
#endif // ONBOARDING_SERVICE
    } while (0);

    if (status == AJ_OK) {
        isRebootRequired = TRUE;
        return AJ_ERR_RESTART; // Force disconnect of AJ and services and reconnection of WiFi on restart
    }

    return status;
}

AJ_Status App_Restart()
{
    AJ_Printf("GOT RESTART REQUEST\n");
    SetShouldAnnounce(TRUE); // Set flag for sending an updated Announcement
    isRebootRequired = TRUE;
    return AJ_ERR_RESTART; // Force disconnect of AJ and services and reconnection of WiFi on restart
}

AJ_Status App_SetPasscode(const char* daemonRealm, const char* newStringPasscode)
{
    AJ_Status status = AJ_OK;

    do {
        if (PropertyStore_SetValue(RealmName, daemonRealm) && PropertyStore_SetValue(Passcode, newStringPasscode)) {
            CHECK(PropertyStore_SaveAll());
            CHECK(AJ_ClearCredentials());
            status = AJ_ERR_READ; //Force disconnect of AJ and services to refresh current sessions
        } else {
            CHECK(PropertyStore_LoadAll());
        }
    } while (0);

    return status;
}

uint8_t App_IsValueValid(const char* key, const char* value)
{
    return TRUE;
}
#endif // CONFIG_SERVICE

#ifdef ONBOARDING_SERVICE
/**
 * Whether the SSID is hidden
 */
const uint8_t OBS_SoftAPIsHidden = FALSE;
/**
 * SoftAP passpharse. If NULL means OPEN network otherwise assumes WPA2 up to 63 characters.
 */
const char* OBS_SoftAPPassphrase = NULL;
/**
 * retry parameters after failed connection of already validated configuration.
 */
const uint8_t OBS_MAX_RETRIES = 2;
/**
 * Wait time between retries (ms)
 */
const uint32_t OBS_WAIT_BETWEEN_RETRIES  = 180000;
#endif // ONBOARDING_SERVICE

