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

#include <alljoyn.h>
#include <aj_debug.h>
#include <aj_config.h>
#include <aj_creds.h>
#include <aj_nvram.h>
#include <aj_link_timeout.h>
#include <alljoyn/services_common/ServicesCommon.h>
#include <alljoyn/services_common/ServicesHandlers.h>
#include "PropertyStoreOEMProvisioning.h"

/*
 * Logger definition
 */
#ifndef NDEBUG
#ifndef ER_DEBUG_AJSVCAPP
#define ER_DEBUG_AJSVCAPP 0
#endif
AJ_EXPORT uint8_t dbgAJSVCAPP = ER_DEBUG_AJSVCAPP;
#endif

/*
 * Define timeout/pause values. Values are in milli seconds.
 * The following may be tuned according to platform requirements such as battery usage.
 */
#define AJAPP_CONNECT_TIMEOUT     AJ_CONNECT_TIMEOUT
#define AJAPP_CONNECT_PAUSE       (1000 * 2) // Override AJ_CONNECT_PAUSE to be more responsive
#define AJAPP_SLEEP_TIME          (1000 * 2) // A little pause to let things settle
#define AJAPP_UNMARSHAL_TIMEOUT   (1000 * 1) // Override AJ_UNMARSHAL_TIMEOUT to be more responsive

/**
 * Application wide globals
 */

#define ROUTER_NAME "org.alljoyn.BusNode"
static uint8_t isBusConnected = FALSE;
static AJ_BusAttachment busAttachment;

/**
 * Application wide callbacks
 */

static uint32_t PasswordCallback(uint8_t* buffer, uint32_t bufLen)
{
    AJ_Status status = AJ_OK;
    const char* hexPassword = "303030303030";
    size_t hexPasswordLen;
    uint32_t len = 0;

    AJ_AlwaysPrintf(("Configured password=%s\n", hexPassword));
    hexPasswordLen = strlen(hexPassword);
    len = hexPasswordLen / 2;
    status = AJ_HexToRaw(hexPassword, hexPasswordLen, buffer, bufLen);
    if (status == AJ_ERR_RESOURCES) {
        len = 0;
    }

    return len;
}

static uint32_t MyBusAuthPwdCB(uint8_t* buf, uint32_t bufLen)
{
    const char* myRouterPwd = "000000";
    strncpy((char*)buf, myRouterPwd, bufLen);
    return (uint32_t)strlen(myRouterPwd);
}

/**
 * Application handlers
 */

typedef enum {
    INIT_START = 0,
    INIT_SERVICES = INIT_START,
    INIT_FINISHED
} enum_init_state_t;

static uint8_t AJRouter_Connect(AJ_BusAttachment* busAttachment, const char* routerName)
{
    AJ_Status status;
    const char* busUniqueName;

    while (TRUE) {
        AJ_InfoPrintf(("Attempting to connect to bus '%s'\n", routerName));
        status = AJ_FindBusAndConnect(busAttachment, routerName, AJAPP_CONNECT_TIMEOUT);
        if (status != AJ_OK) {
            AJ_InfoPrintf(("Failed to connect to bus sleeping for %d seconds\n", AJAPP_CONNECT_PAUSE / 1000));
            AJ_Sleep(AJAPP_CONNECT_PAUSE);
            continue;
        }
        busUniqueName = AJ_GetUniqueName(busAttachment);
        if (busUniqueName == NULL) {
            AJ_ErrPrintf(("Failed to GetUniqueName() from newly connected bus, retrying\n"));
            continue;
        }
        AJ_InfoPrintf(("Connected to router with BusUniqueName=%s\n", busUniqueName));
        break;
    }

    /* Setup password based authentication listener for secured peer to peer connections */
    AJ_BusSetPasswordCallback(busAttachment, PasswordCallback);

    /* Configure timeout for the link to the Router bus */
    AJ_SetBusLinkTimeout(busAttachment, 60);     // 60 seconds

    return TRUE;
}

static enum_init_state_t currentServicesInitializationState = INIT_START;
static enum_init_state_t nextServicesInitializationState = INIT_START;

static AJ_Status AJApp_ConnectedHandler(AJ_BusAttachment* busAttachment)
{
    AJ_Status status = AJ_OK;

    if (AJ_GetUniqueName(busAttachment)) {
        if (currentServicesInitializationState == nextServicesInitializationState) {
            switch (currentServicesInitializationState) {
            case INIT_SERVICES:
                status = AJSVC_ConnectedHandler(busAttachment);
                if (status != AJ_OK) {
                    goto ErrorExit;
                }
                currentServicesInitializationState = nextServicesInitializationState = INIT_FINISHED;
                break;

            case INIT_FINISHED:
            default:
                break;
            }
        }
    }
    return status;

ErrorExit:

    AJ_ErrPrintf(("Application ConnectedHandler returned an error %s\n", (AJ_StatusText(status))));
    return status;
}

static AJSVC_ServiceStatus AJApp_MessageProcessor(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJ_Status* status)
{
    AJSVC_ServiceStatus serviceStatus = AJSVC_SERVICE_STATUS_HANDLED;

    switch (currentServicesInitializationState) {
    default:
        serviceStatus = AJSVC_MessageProcessorAndDispatcher(busAttachment, msg, status);
        break;
    }

    return serviceStatus;
}

static AJ_Status AJApp_DisconnectHandler(AJ_BusAttachment* busAttachment, uint8_t restart)
{
    AJ_Status status = AJ_OK;

    currentServicesInitializationState = nextServicesInitializationState = INIT_START;

    status = AJSVC_DisconnectHandler(busAttachment);
    return status;
}

static uint8_t AJRouter_Disconnect(AJ_BusAttachment* busAttachment, uint8_t disconnectWiFi)
{
    AJ_InfoPrintf(("AllJoyn disconnect\n"));
    AJ_Sleep(AJAPP_SLEEP_TIME); // Sleep a little to let any pending requests to router to be sent
    AJ_Disconnect(busAttachment);
    AJ_Sleep(AJAPP_SLEEP_TIME); // Sleep a little while before trying to reconnect

    return TRUE;
}

/**
 * Services Provisioning
 */

AJ_Object ProxyObjects[] = {
    IOE_SERVICES_PROXYOBJECTS
    { NULL, NULL }
};

/**
 * PropertyStore stub implementation for About feature
 */
AJ_Status AJSVC_PropertyStore_ReadAll(AJ_Message* reply, AJSVC_PropertyStoreCategoryFilter filter, int8_t langIndex) {
    return AJ_OK;
}
int8_t AJSVC_PropertyStore_GetLanguageIndex(const char* const language) {
    return AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX;
}
int8_t AJSVC_PropertyStore_GetCurrentDefaultLanguageIndex() {
    return AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX;
}

/**
 * Notification Consumer Provisioning
 */

static AJ_Status ApplicationHandleNotify(AJNS_Notification* notification)
{
    int8_t indx;

    AJ_AlwaysPrintf(("******************** Begin New Message Received ********************\n"));

    if (notification == 0) {
        AJ_AlwaysPrintf(("Notification is NULL\n"));
        return AJ_OK;
    }

    AJ_AlwaysPrintf(("Message Id: %d\nVersion: %u\nDevice Id: %s\nDevice Name: %s\nApp Id: %s\nApp Name: %s\nMessage Type: %d\n",
                     notification->notificationId, notification->version, notification->deviceId, notification->deviceName, notification->appId, notification->appName, notification->messageType));

    if (notification->originalSenderName != 0 && strlen(notification->originalSenderName) > 0) {
        AJ_AlwaysPrintf(("OriginalSender bus unique name: %s\n", notification->originalSenderName));
    }

    for (indx = 0; indx < notification->content->numTexts; indx++) {
        AJ_AlwaysPrintf(("Language: %s  Message: %s.\n", notification->content->texts[indx].key, notification->content->texts[indx].value));
    }

    AJ_AlwaysPrintf(("Other parameters included:\n"));
    for (indx = 0; indx < notification->content->numCustomAttributes; indx++) {
        AJ_AlwaysPrintf(("Custom Attribute Key: %s  Custom Attribute Value: %s\n", notification->content->customAttributes[indx].key, notification->content->customAttributes[indx].value));
    }

    if (notification->content->richIconUrl != 0 && strlen(notification->content->richIconUrl) > 0) {
        AJ_AlwaysPrintf(("Rich Content Icon Url: %s\n", notification->content->richIconUrl));
    }

    if (notification->content->numAudioUrls) {
        AJ_AlwaysPrintf(("******************** Begin Rich Audio Content ********************\n"));
        for (indx = 0; indx < notification->content->numAudioUrls; indx++) {
            AJ_AlwaysPrintf(("Language: %s  Audio URL %s\n", notification->content->richAudioUrls[indx].key, notification->content->richAudioUrls[indx].value));
        }
        AJ_AlwaysPrintf(("******************** End Rich Audio Content ********************\n"));
    }

    if (notification->content->richIconObjectPath != 0 && strlen(notification->content->richIconObjectPath) > 0) {
        AJ_AlwaysPrintf(("Rich Content Icon Object Path: %s\n", notification->content->richIconObjectPath));
    }

    if (notification->content->richAudioObjectPath != 0 && strlen(notification->content->richAudioObjectPath) > 0) {
        AJ_AlwaysPrintf(("Rich Content Audio Object Path: %s\n", notification->content->richAudioObjectPath));
    }

    if (notification->content->controlPanelServiceObjectPath != 0 && strlen(notification->content->controlPanelServiceObjectPath) > 0) {
        AJ_AlwaysPrintf(("ControlPanelService object path: %s\n", notification->content->controlPanelServiceObjectPath));
    }

    AJ_AlwaysPrintf(("******************** End New Message Received ********************\n"));

    return AJ_OK;
}

static AJ_Status Consumer_Init()
{
    AJ_Status status = AJNS_Consumer_Start(1, ProxyObjects, &ApplicationHandleNotify, NULL);
    return status;
}

/**
 * The AllJoyn Message Loop
 */

int AJ_Main(void)
{
    AJ_Status status = AJ_OK;
    uint8_t isUnmarshalingSuccessful = FALSE;
    AJSVC_ServiceStatus serviceStatus;
    AJ_Message msg;

    AJ_Initialize();

    status = Consumer_Init();
    if (status != AJ_OK) {
        goto Exit;
    }

    AJ_RegisterObjects(NULL, ProxyObjects);
    SetBusAuthPwdCallback(MyBusAuthPwdCB);

    while (TRUE) {
        status = AJ_OK;
        serviceStatus = AJSVC_SERVICE_STATUS_NOT_HANDLED;

        if (!isBusConnected) {
            isBusConnected = AJRouter_Connect(&busAttachment, ROUTER_NAME);
            if (!isBusConnected) { // Failed to connect to router?
                continue; // Retry establishing connection to router.
            }
        }

        status = AJApp_ConnectedHandler(&busAttachment);

        if (status == AJ_OK) {
            status = AJ_UnmarshalMsg(&busAttachment, &msg, AJAPP_UNMARSHAL_TIMEOUT);
            isUnmarshalingSuccessful = (status == AJ_OK);

            if (status == AJ_ERR_TIMEOUT) {
                if (AJ_ERR_LINK_TIMEOUT == AJ_BusLinkStateProc(&busAttachment)) {
                    status = AJ_ERR_READ;             // something's not right. force disconnect
                }
            }

            if (isUnmarshalingSuccessful) {
                if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
                    serviceStatus = AJApp_MessageProcessor(&busAttachment, &msg, &status);
                }
                if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
                    //Pass to the built-in bus message handlers
                    status = AJ_BusHandleBusMessage(&msg);
                }
                AJ_NotifyLinkActive();
            }

            //Unmarshaled messages must be closed to free resources
            AJ_CloseMsg(&msg);
        }

        if (status == AJ_ERR_READ || status == AJ_ERR_RESTART || status == AJ_ERR_RESTART_APP) {
            if (isBusConnected) {
                AJApp_DisconnectHandler(&busAttachment, status != AJ_ERR_READ);
                isBusConnected = !AJRouter_Disconnect(&busAttachment, status != AJ_ERR_READ);
                if (status == AJ_ERR_RESTART_APP) {
                    AJ_Reboot();
                }
            }
        }
    }     // while (TRUE)

    return 0;

Exit:

    return (1);
}

#ifdef AJ_MAIN
int main()
{
    return AJ_Main();
}
#endif


