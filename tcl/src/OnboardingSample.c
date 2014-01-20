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

#include <alljoyn/onboarding/OnboardingService.h>
#include <alljoyn/onboarding/OnboardingSample.h>
#include <aj_link_timeout.h>

void Onboarding_Init()
{
    if (OBCAPI_IsWiFiClient()) {  //In case hard coded connection has been set - skip onboarding
        return;
    }
    //status = OBS_ClearInfo(); // This should be commented. Used for debugging.
}

void Onboarding_Finish(AJ_BusAttachment* bus)
{
}

AJ_Status Onboarding_ConnectedHandler(AJ_BusAttachment* bus)
{
    AJ_Status status = AJ_OK;
    return status;
}

AJ_Status Onboarding_IdleDisconnectedHandler(AJ_BusAttachment* bus)
{
    AJ_Status status;

    OBInfo obInfo;
    status = OBS_ReadInfo(&obInfo);
    AJ_Printf("ReadInfo status: %s\n", AJ_StatusText(status));

    status = OBCAPI_StartSoftAPIfNeededOrConnect(&obInfo);

    return status;
}

void Onboarding_IdleConnectedHandler(AJ_BusAttachment* bus)
{
}

void Onboarding_SwitchToRetry()
{
    AJ_Status status = AJ_OK;

    OBInfo obInfo;
    status = OBS_ReadInfo(&obInfo);
    if (status != AJ_OK) {
        return;
    }
    obInfo.state = CONFIGURED_RETRY;
    status = OBS_WriteInfo(&obInfo);
    if (status != AJ_OK) {
        return;
    }

    AJ_Printf("SwitchToRetry status: %s\n", AJ_StatusText(status));
}

Service_Status Onboarding_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status*msgStatus)
{
    Service_Status service_Status = SERVICE_STATUS_HANDLED;

    if (*msgStatus == AJ_OK) {
        switch (msg->msgId) {

        case OBS_GET_PROP:
            *msgStatus = AJ_BusPropGet(msg, OnboardingPropGetHandler, NULL);
            break;

        case OBS_SET_PROP:
            *msgStatus = AJ_BusPropSet(msg, OnboardingPropSetHandler, NULL);
            break;

        case OBS_CONFIGURE_WIFI:
            *msgStatus = OBS_ConfigureWiFi(msg);
            break;

        case OBS_CONNECT:
            *msgStatus = OBS_ConnectWiFi(msg);
            break;

        case OBS_OFFBOARD:
            *msgStatus = OBS_OffboardWiFi(msg);
            break;

        case OBS_GET_SCAN_INFO:
            *msgStatus = OBS_GetScanInfo(msg);
            break;

        default:
            service_Status = SERVICE_STATUS_NOT_HANDLED;
            break;
        }
    } else {
        service_Status = SERVICE_STATUS_NOT_HANDLED;
    }

    return service_Status;
}

AJ_Status Onboarding_DisconnectedHandler(AJ_BusAttachment* bus)
{
    AJ_Status status = AJ_OK;
    status = OBCAPI_GotoIdleWiFi(TRUE);
    return status;
}
