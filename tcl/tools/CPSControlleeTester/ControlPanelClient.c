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

#include <stdlib.h>

#include "ControlPanelClient.h"
#include "alljoyn.h"
#include <PropertyStoreOEMProvisioning.h>

#ifndef NDEBUG
AJ_EXPORT uint8_t dbgAJSVCAPP = 1;
#endif

#define CPSC_CONNECT_TIMEOUT     (1000 * 60)
#define CPSC_CONNECT_PAUSE       (1000 * 10)
#define CPSC_UNMARSHAL_TIMEOUT   (1000 * 5)
#define CPSC_SLEEP_TIME          (1000 * 2)

#ifndef COMBINED_SERVICES
AJ_BusAttachment busAttachment;
uint8_t isBusConnected = FALSE;
static char busAddress[] = "org.alljoyn.BusNode";
uint32_t sessionId = 0;

AJ_Object AppObjects[] =
{
    ABOUT_APPOBJECTS
    CONTROLPANEL_APPOBJECTS
    { NULL }
};

const AJ_Object AnnounceObjects[] = {
    { NULL, NULL }
};

#define CONTROL_ANNOUNCE_SIGNAL_RECEIVED  AJ_PRX_MESSAGE_ID(0, 1, 3)

const char* deviceManufactureName = "COMPANY";
const char* deviceProductName = "GENERIC BOARD";

static const char DEFAULT_LANGUAGE[] = "en";
static const char* DEFAULT_LANGUAGES[] = { DEFAULT_LANGUAGE };
static const char SUPPORTED_LANG2[] = "de-AT";
static const char* SUPPORTED_LANGUAGES[] = { DEFAULT_LANGUAGE, SUPPORTED_LANG2 };
const char** propertyStoreDefaultLanguages = SUPPORTED_LANGUAGES;
const uint8_t AJSVC_PROPERTY_STORE_NUMBER_OF_LANGUAGES = sizeof(SUPPORTED_LANGUAGES) / sizeof(char*);

/**
 * property array of structure with defaults
 */
static const char* DEFAULT_PASSCODES[] = { "000000" };
static const char* DEFAULT_APP_NAMES[] = { "Controler" };
static const char* DEFAULT_DEVICE_NAMES[] = { "CPSTESTER" };
static const char DEFAULT_DESCRIPTION_LANG1[] = "AC IOE device";
static const char DEFAULT_DESCRIPTION_LANG2[] = "Mein erstes IOE Geraet";
static const char* DEFAULT_DESCRIPTIONS[] = { DEFAULT_DESCRIPTION_LANG1, DEFAULT_DESCRIPTION_LANG2 };
static const char DEFAULT_MANUFACTURER_LANG1[] = "Company A(EN)";
static const char DEFAULT_MANUFACTURER_LANG2[] = "Firma A(DE-AT)";
static const char* DEFAULT_MANUFACTURERS[] = { DEFAULT_MANUFACTURER_LANG1, DEFAULT_MANUFACTURER_LANG2 };
static const char* DEFAULT_DEVICE_MODELS[] = { "0.0.1" };
static const char* DEFAULT_DATE_OF_MANUFACTURES[] = { "2014-02-01" };
static const char* DEFAULT_SOFTWARE_VERSIONS[] = { "0.0.1" };
static const char* DEFAULT_HARDWARE_VERSIONS[] = { "0.0.1" };
static const char DEFAULT_SUPPORT_URL_LANG1[] = "www.company_a.com";
static const char DEFAULT_SUPPORT_URL_LANG2[] = "www.company_a.com/de-AT";
static const char* DEFAULT_SUPPORT_URLS[] = { DEFAULT_SUPPORT_URL_LANG1, DEFAULT_SUPPORT_URL_LANG2 };

const char** propertyStoreDefaultValues[AJSVC_PROPERTY_STORE_NUMBER_OF_KEYS] =
{
// "Default Values per language",                    "Key Name"
    NULL,                                           /*DeviceId*/
    NULL,                                           /*AppId*/
    DEFAULT_DEVICE_NAMES,                           /*DeviceName*/
// Add other persisted keys above this line
    DEFAULT_LANGUAGES,                              /*DefaultLanguage*/
    DEFAULT_PASSCODES,                              /*Passcode*/
    NULL,                                           /*RealmName*/
// Add other configurable keys above this line
    DEFAULT_APP_NAMES,                              /*AppName*/
    DEFAULT_DESCRIPTIONS,                           /*Description*/
    DEFAULT_MANUFACTURERS,                          /*Manufacturer*/
    DEFAULT_DEVICE_MODELS,                          /*ModelNumber*/
    DEFAULT_DATE_OF_MANUFACTURES,                   /*DateOfManufacture*/
    DEFAULT_SOFTWARE_VERSIONS,                      /*SoftwareVersion*/
    NULL,                                           /*AJSoftwareVersion*/
#if defined CONFIG_SERVICE
    NULL,                                           /*MaxLength*/
#endif
    DEFAULT_HARDWARE_VERSIONS,                      /*HardwareVersion*/
    DEFAULT_SUPPORT_URLS,                           /*SupportUrl*/
// Add other about keys above this line
};

static char machineIdVar[MACHINE_ID_LENGTH + 1] = { 0 };
static char* machineIdVars[] = { machineIdVar };
static char deviceNameVar[DEVICE_NAME_VALUE_LENGTH + 1] = { 0 };
static char* deviceNameVars[] = { deviceNameVar };
#ifdef CONFIG_SERVICE
static char defaultLanguageVar[LANG_VALUE_LENGTH + 1] = { 0 };
static char* defaultLanguageVars[] = { defaultLanguageVar };
static char passcodeVar[PASSWORD_VALUE_LENGTH + 1] = { 0 };
static char* passcodeVars[] = { passcodeVar };
static char realmNameVar[KEY_VALUE_LENGTH + 1] = { 0 };
static char* realmNameVars[] = { realmNameVar };
#endif

PropertyStoreConfigEntry propertyStoreRuntimeValues[AJSVC_PROPERTY_STORE_NUMBER_OF_CONFIG_KEYS] =
{
//  {"Buffers for Values per language", "Buffer Size"},                  "Key Name"
    { machineIdVars,             MACHINE_ID_LENGTH + 1 },               /*DeviceId*/
    { machineIdVars,             MACHINE_ID_LENGTH + 1 },               /*AppId*/
    { deviceNameVars,            DEVICE_NAME_VALUE_LENGTH + 1 },        /*DeviceName*/
// Add other persisted keys above this line
#ifdef CONFIG_SERVICE
    { defaultLanguageVars,       LANG_VALUE_LENGTH + 1 },               /*AppName*/
    { passcodeVars,              PASSWORD_VALUE_LENGTH + 1 },           /*Description*/
    { realmNameVars,             KEY_VALUE_LENGTH + 1 },                /*Manufacturer*/
#endif
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

uint8_t isControlPanelAnnounce(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;
    AJ_Arg array1, array2, struct1;
    uint16_t uint1, uint2;
    char* buffer;

    status = AJ_UnmarshalArgs(msg, "q", &uint1);
    if (status != AJ_OK) {
        return FALSE;
    }

    status = AJ_UnmarshalArgs(msg, "q", &uint2);
    if (status != AJ_OK) {
        return FALSE;
    }
    status = AJ_UnmarshalContainer(msg, &array1, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        return FALSE;
    }
    do {
        status = AJ_UnmarshalContainer(msg, &struct1, AJ_ARG_STRUCT);
        if (status != AJ_OK) {
            return FALSE;
        }
        status = AJ_UnmarshalArgs(msg, "o", &buffer);
        if (status != AJ_OK) {
            return FALSE;
        }

        status = AJ_UnmarshalContainer(msg, &array2, AJ_ARG_ARRAY);
        if (status != AJ_OK) {
            return FALSE;
        }
        do {
            if ((status = AJ_UnmarshalArgs(msg, "s", &buffer)) != AJ_OK) {
                if (status == AJ_ERR_NO_MORE) {
                    break;
                } else {
                    return FALSE;
                }
            }
            if (strcmp(buffer, "org.alljoyn.ControlPanel.ControlPanel") == 0) {
                return TRUE;
            }
        } while (1);
        status = AJ_UnmarshalCloseContainer(msg, &array2);
        if (status != AJ_OK) {
            break;
        }
        status = AJ_UnmarshalCloseContainer(msg, &struct1);
        if (status != AJ_OK) {
            break;
        }
    } while (1);
    return FALSE;
}

static const char PWD[] = "ABCDEFGH";

uint32_t MyBusAuthPwdCB(uint8_t* buf, uint32_t bufLen)
{
    const char* myPwd = "000000";
    strncpy((char*)buf, myPwd, bufLen);
    return (uint32_t)strlen(myPwd);
}

uint32_t PasswordCallback(uint8_t* buffer, uint32_t bufLen)
{
    memcpy(buffer, PWD, sizeof(PWD));
    return sizeof(PWD) - 1;
}
#endif

/**
 * Static consts.
 */
static const uint16_t ServicePort = 1000;
uint32_t CPSsessionId = 0;
/**
 * Static non consts.
 */

int runningTestNum = 0;
int lastTestRun = -1;
static uint16_t numTests = NUMBER_OF_TESTS;

/**
 * Define Externs
 */
static CPSTest testsToRun[NUMBER_OF_TESTS];


char* CPSAnnounceMatch = "sessionless='t',interface='org.alljoyn.ControlPanel.Announcement'";

/**
 * Objects implemented by the application. The first member in the AJ_Object structure is the path.
 * The second is the collection of all interfaces at that path.
 */
void CPS_Init()
{
    TestsInit(testsToRun);
}


AJ_Status CPS_StartService(AJ_BusAttachment* bus, const char* busAddress, uint32_t timeout, uint8_t connected)
{
    AJ_Status status = AJ_OK;
    while (TRUE) {
        AJ_InfoPrintf(("Attempting to connect to bus '%s'\n", busAddress));

        status = AJ_Connect(bus, busAddress, timeout);
        if (status != AJ_OK) {
            AJ_ErrPrintf(("Failed to connect to bus '%s', sleeping for %d seconds...\n", busAddress, CPSC_CONNECT_PAUSE / 1000));
            AJ_Sleep(CPSC_CONNECT_PAUSE);
            continue;
        }

        AJ_InfoPrintf(("Connected successfully\n"));
        isBusConnected = TRUE;

        status = AJ_BusSetSignalRule(bus, CPSAnnounceMatch, AJ_BUS_SIGNAL_ALLOW);
        if (status != AJ_OK) {
            AJ_ErrPrintf(("Could not set Announcement Interface AddMatch\n"));
            return status;
        }
        break;
    }
    ;
    return status;
}

void CPS_IdleConnectedHandler(AJ_BusAttachment*bus)
{
    if (runningTestNum == lastTestRun || CPSsessionId == 0) {
        return;
    }

    if (runningTestNum == numTests) {
        runningTestNum = 0;
        AJ_InfoPrintf(("Finished running all the tests\n"));
        exit(0);
    }

    AJ_Status status;
    AJ_Message msg;

    status = AJ_MarshalMethodCall(&busAttachment, &msg, testsToRun[runningTestNum].msgId,
                                  announceSender, CPSsessionId, 0, CPSC_CONNECT_TIMEOUT);

    uint16_t numParam;
    for (numParam = 0; numParam < testsToRun[runningTestNum].numParams; numParam++) {
        if (status == AJ_OK) {
            status = AJ_MarshalArgs(&msg, "q", testsToRun[runningTestNum].param[numParam]);
        }
    }

    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
        lastTestRun = runningTestNum;
    }

    AJ_InfoPrintf(("MakeMethodCall() test number %i resulted in a status of %s.\n", lastTestRun + 1, AJ_StatusText(status)));
}

AJSVC_ServiceStatus CPS_NotifySessionAccepted(uint32_t sessionId, const char* sender)
{
    char dot = '.';
    uint16_t i;
    for (i = 0; i < strlen(sender); i++) {
        if (*(sender + i) == announceSender[i]) {
            if (*(sender + i) == dot) {
                CPSsessionId = sessionId;
                AJ_InfoPrintf(("Control Panel Service Connected Successfully to announce.\n"));
                return AJSVC_SERVICE_STATUS_HANDLED;
            }
            continue;
        }
        break;
    }
    AJ_ErrPrintf(("Could not connect.\n"));
    CPSsessionId = 0;
    return AJSVC_SERVICE_STATUS_NOT_HANDLED;
}

AJSVC_ServiceStatus CPS_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status*msgStatus)
{
    AJSVC_ServiceStatus service_Status = AJSVC_SERVICE_STATUS_HANDLED;
    switch (msg->msgId) {
    case CONTROL_ANNOUNCE_SIGNAL_RECEIVED:
        AJ_InfoPrintf(("Received Announce Signal from %s.\n", msg->sender));
        if (CPSsessionId == 0) {
            if (!isControlPanelAnnounce(msg)) {
                break;
            }
            AJ_SessionOpts sessionOpts = {
                AJ_SESSION_TRAFFIC_MESSAGES,
                AJ_SESSION_PROXIMITY_ANY,
                AJ_TRANSPORT_ANY,
                TRUE
            };
            strcpy(announceSender, msg->sender);

            AJ_BusJoinSession(bus, msg->sender, ServicePort, &sessionOpts);
        }
        break;

        ALL_REPLY_CASES
            runningTestNum++;
        CPS_IdleConnectedHandler(bus);
        break;

    case AJ_SIGNAL_SESSION_LOST:
        CPSsessionId = 0;
        break;

    default:
        service_Status = AJSVC_SERVICE_STATUS_NOT_HANDLED;
        break;
    }
    return service_Status;
}

void CPS_Finish()
{
    return;
}

#ifndef COMBINED_SERVICES
int AJ_Main(void)
{

    AJ_Status status = AJ_OK;

    /* Required: Need to initialize once before starting */
    AJ_Initialize();

    /* Required: Register the AppObjects before starting */
    AJ_RegisterObjects(NULL, AppObjects);

    CPS_Init();

    SetBusAuthPwdCallback(MyBusAuthPwdCB);

    while (TRUE) {

        /* Aljoyn related section */
        AJ_Message msg;

        if (!isBusConnected) {
            status = CPS_StartService(&busAttachment, busAddress, CPSC_CONNECT_TIMEOUT, isBusConnected);
        }

        status = AJ_UnmarshalMsg(&busAttachment, &msg, CPSC_UNMARSHAL_TIMEOUT);

        if (AJ_ERR_TIMEOUT == status) { // nothing on bus, do our own thing
            CPS_IdleConnectedHandler(&busAttachment);
            continue;
        }

        if (AJ_OK == status) {
            if  (msg.msgId == AJ_REPLY_ID(AJ_METHOD_JOIN_SESSION)) {
                if (msg.hdr->msgType == AJ_MSG_ERROR) {
                    AJ_ErrPrintf(("Could not connect session.\n"));
                } else {
                    uint32_t replyCode;

                    AJ_UnmarshalArgs(&msg, "uu", &replyCode, &sessionId);
                    if (replyCode == AJ_JOINSESSION_REPLY_SUCCESS) {
                        CPS_NotifySessionAccepted(sessionId, msg.sender);
                    }
                }
            } else {
                if (CPS_MessageProcessor(&busAttachment, &msg, &status) == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
                    //pass to build in
                    status = AJ_BusHandleBusMessage(&msg);
                }
            }
        }

        /* Messages MUST be discarded to free resources. */
        AJ_CloseMsg(&msg);

        if (status == AJ_ERR_READ) {
            AJ_ErrPrintf(("AllJoyn disconnect.\n"));
            AJ_Disconnect(&busAttachment);
            isBusConnected = FALSE;

            /* Sleep a little while before trying to reconnect. */
            AJ_Sleep(CPSC_SLEEP_TIME);
        }
    }
    AJ_InfoPrintf(("Control Panel Sample exiting with status 0x%04x.\n", status));
    return status;
}


#ifdef AJ_MAIN
int main()
{
    return AJ_Main();
}
#endif
#endif

