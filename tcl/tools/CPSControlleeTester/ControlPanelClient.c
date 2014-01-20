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

#include <stdlib.h>

#include "ControlPanelClient.h"
#include "alljoyn.h"

#define CPSC_CONNECT_TIMEOUT     (1000 * 60)
#define CPSC_CONNECT_PAUSE       (1000 * 10)
#define CPSC_UNMARSHAL_TIMEOUT   (1000 * 5)
#define CPSC_SLEEP_TIME          (1000 * 2)

#ifndef COMBINED_SERVICES
AJ_BusAttachment busAttachment;
uint8_t isBusConnected = FALSE;
static char busAddress[] = "org.alljoyn.BusNode";
uint32_t sessionId = 0;

static const char* const AboutInterface[] = {
    "org.alljoyn.About",
    "!Announce >q >q >a(oas) >a{sv}",
    NULL
};

const AJ_InterfaceDescription AboutInterfaces[] = {
    AboutInterface,
    NULL
};

AJ_Object AppObjects[] =
{
    CONTROLPANEL_APPOBJECTS
    { "/About", AboutInterfaces },
    { NULL }
};

#define CONTROL_ANNOUNCE_SIGNAL_RECEIVED  AJ_PRX_MESSAGE_ID(NUM_CONTROLPANEL_OBJECTS, 0, 0)

uint8_t isControlPanelAnnounce(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;
    AJ_Arg array1, array2, struct1;
    uint16_t uint1, uint2;
    char* buffer;

    status = AJ_UnmarshalArgs(msg, "q", &uint1);
    if (status != AJ_OK) {
        return FLASE;
    }
    status = AJ_UnmarshalArgs(msg, "q", &uint2);
    if (status != AJ_OK) {
        return FLASE;
    }
    status = AJ_UnmarshalContainer(msg, &array1, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        return FLASE;
    }
    do {
        status = AJ_UnmarshalContainer(msg, &struct1, AJ_ARG_STRUCT);
        if (status != AJ_OK) {
            return FLASE;
        }
        status = AJ_UnmarshalArgs(msg, "o", &buffer);
        if (status != AJ_OK) {
            return FLASE;
        }

        status = AJ_UnmarshalContainer(msg, &array2, AJ_ARG_ARRAY);
        if (status != AJ_OK) {
            return FLASE;
        }
        do {
            if (status = AJ_UnmarshalArgs(msg, "s", &buffer) != AJ_OK) {
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
        AJ_Printf("Attempting to connect to bus '%s'\n", busAddress);

        status = AJ_Connect(bus, busAddress, timeout);
        if (status != AJ_OK) {
            AJ_Printf("Failed to connect to bus '%s', sleeping for %d seconds...\n", busAddress, CPSC_CONNECT_PAUSE / 1000);
            AJ_Sleep(CPSC_CONNECT_PAUSE);
            continue;
        }

        AJ_Printf("Connected successfully\n");
        isBusConnected = TRUE;

        status = AJ_BusSetSignalRule(bus, CPSAnnounceMatch, AJ_BUS_SIGNAL_ALLOW);
        if (status != AJ_OK) {
            AJ_Printf("Could not set Announcement Interface AddMatch\n");
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
        AJ_Printf("Finished running all the tests\n");
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

    AJ_Printf("MakeMethodCall() test number %i resulted in a status of %s.\n", lastTestRun + 1, AJ_StatusText(status));
}

Service_Status CPS_NotifySessionAccepted(uint32_t sessionId, const char* sender)
{
    char dot = '.';
    uint16_t i;
    for (i = 0; i < strlen(sender); i++) {
        if (*(sender + i) == announceSender[i]) {
            if (*(sender + i) == dot) {
                CPSsessionId = sessionId;
                AJ_Printf("Control Panel Service Connected Successfully to announce.\n");
                return SERVICE_STATUS_HANDLED;
            }
            continue;
        }
        break;
    }
    AJ_Printf("Could not connect.\n");
    CPSsessionId = 0;
    return SERVICE_STATUS_NOT_HANDLED;
}

Service_Status CPS_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status*msgStatus)
{
    Service_Status service_Status = SERVICE_STATUS_HANDLED;
    switch (msg->msgId) {
    case CONTROL_ANNOUNCE_SIGNAL_RECEIVED:
        AJ_Printf("Received Announce Signal from %s.\n", msg->sender);
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
        service_Status = SERVICE_STATUS_NOT_HANDLED;
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
                    AJ_Printf("Could not connect session.\n");
                } else {
                    uint32_t replyCode;

                    AJ_UnmarshalArgs(&msg, "uu", &replyCode, &sessionId);
                    if (replyCode == AJ_JOINSESSION_REPLY_SUCCESS) {
                        CPS_NotifySessionAccepted(sessionId, msg.sender);
                    }
                }
            } else {
                if (CPS_MessageProcessor(&busAttachment, &msg, &status) == SERVICE_STATUS_NOT_HANDLED) {
                    //pass to build in
                    status = AJ_BusHandleBusMessage(&msg);
                }
            }
        }

        /* Messages MUST be discarded to free resources. */
        AJ_CloseMsg(&msg);

        if (status == AJ_ERR_READ) {
            AJ_Printf("AllJoyn disconnect.\n");
            AJ_Disconnect(&busAttachment);
            isBusConnected = FALSE;

            /* Sleep a little while before trying to reconnect. */
            AJ_Sleep(CPSC_SLEEP_TIME);
        }
    }
    AJ_Printf("Control Panel Sample exiting with status 0x%04x.\n", status);
    return status;
}


#ifdef AJ_MAIN
int main()
{
    return AJ_Main();
}
#endif
#endif

