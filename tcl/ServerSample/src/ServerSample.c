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

#include "Services_Handlers.h"

#include <aj_link_timeout.h>

#include <alljoyn/services_common/Services_Common.h>

// Application wide globals
char daemonName[] = "org.alljoyn.BusNode";
uint8_t isBusConnected = FALSE;
uint8_t isRebootRequired = FALSE;
AJ_BusAttachment busAttachment;
const uint16_t App_ServicePort = 900;

AJ_Object AppObjects[] = {
    IOE_SERVICES_APPOBJECTS
    { NULL, NULL }
};

AJ_Object ProxyObjects[] = {
    IOE_SERVICES_PROXYOBJECTS
    { NULL, NULL }
};

const AJ_Object AnnounceObjects[] = {
    IOE_SERVICES_ANNOUNCEOBJECTS
    { NULL, NULL }
};

static uint32_t MyBusAuthPwdCB(uint8_t* buf, uint32_t bufLen)
{
    const char* myPwd = "000000";
    strncpy((char*)buf, myPwd, bufLen);
    return (uint32_t)strlen(myPwd);
}

int AJ_Main(void)
{
    static AJ_Status status = AJ_OK;
    static uint8_t isUnmarshalingSuccessful = FALSE;
    Service_Status service_Status;

    AJ_Initialize();
    Service_Init();
    AJ_RegisterObjects(AppObjects, ProxyObjects);
    SetBusAuthPwdCallback(MyBusAuthPwdCB);

    while (TRUE) {
        AJ_Message msg;
        status = AJ_OK;

        if (!isBusConnected) {
            isBusConnected = Daemon_Connect(daemonName);
            if (isBusConnected) {
                status = Service_ConnectedHandler();
            }
        }

        if (isBusConnected) {
            if (status == AJ_OK) {
                status = Application_ConnectedHandler();
            }
        }

        if (status == AJ_OK) {
            status = AJ_UnmarshalMsg(&busAttachment, &msg, AJ_UNMARSHAL_TIMEOUT);
            isUnmarshalingSuccessful = (status == AJ_OK);

            if (status == AJ_ERR_TIMEOUT) {
                if (AJ_ERR_LINK_TIMEOUT == AJ_BusLinkStateProc(&busAttachment)) {
                    status = AJ_ERR_READ;             // something's not right. force disconnect
                } else {                              // nothing on bus, do our own thing
                    Service_IdleConnectedHandler();
                    continue;
                }
            }

            if (isUnmarshalingSuccessful) {

                service_Status = Service_MessageProcessor(&msg, &status);
                if (service_Status == SERVICE_STATUS_NOT_HANDLED) {
                    //Pass to the built-in bus message handlers
                    status = AJ_BusHandleBusMessage(&msg);
                }
                AJ_NotifyLinkActive();
            }

            //Unmarshaled messages must be closed to free resources
            AJ_CloseMsg(&msg);
        }

        if (status == AJ_ERR_READ || status == AJ_ERR_RESTART) {
            if (isBusConnected) {
                Application_DisconnectHandler(status == AJ_ERR_RESTART);
                Service_DisconnectHandler();
                isBusConnected = !Daemon_Disconnect(status == AJ_ERR_RESTART);
                if (status == AJ_ERR_RESTART && isRebootRequired == TRUE) {
                    isRebootRequired = FALSE;
                    AJ_Reboot();
                }
                // Sleep a little while before trying to reconnect
                AJ_Sleep(AJ_SLEEP_TIME);
            }
        }
    }     // while (TRUE)

    return 0;
}

#ifdef AJ_MAIN
int main()
{
    return AJ_Main();
}
#endif
