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
#include <aj_nvram.h>
#include <aj_link_timeout.h>
#include <aj_wifi_ctrl.h>

static const char* const OnboardingInterface[] = {
    "$org.alljoyn.Onboarding",
    "@Version>q",
    "@State>n",
    "@LastError>(ns)",
    "?ConfigureWiFi <ssn >n",
    "?Connect",
    "?Offboard",
    "?GetScanInfo >q >a(sn)",
    "!ConnectionResult >(ns)",
    NULL
};

static const uint16_t OnboardingVersion = 1;

const AJ_InterfaceDescription OnboardingInterfaces[] = {
    AJ_PropertiesIface,
    OnboardingInterface,
    NULL
};

/*
 * Modify these variables to change the service's behavior
 */
/*
   //will be used in future versions
   #define CONNECTION_RESULT_TTL 0
   static uint16_t obsSessionId = 0;
 */

/* SoftAP state variable for AJTCL */
extern uint8_t IsSoftAP;

/*
 * Handles a property GET request so marshals the property value to return
 */
AJ_Status OnboardingPropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    if (propId == OBS_VERSION_PROP) {
        uint16_t q = OnboardingVersion;
        return AJ_MarshalArgs(replyMsg, "q", q);
    } else if (propId == OBS_STATE_PROP) {
        return AJ_MarshalArgs(replyMsg, "n", obState);
    } else if (propId == OBS_LASTERROR_PROP) {
        AJ_Status status = AJ_OK;
        AJ_Arg structure;
        status = AJ_MarshalContainer(replyMsg, &structure, AJ_ARG_STRUCT);
        status = AJ_MarshalArgs(replyMsg, "n", obLastError.code);
        status = AJ_MarshalArgs(replyMsg, "s", obLastError.message);
        status = AJ_MarshalCloseContainer(replyMsg, &structure);
        return status;
    } else {
        return AJ_ERR_UNEXPECTED;
    }
}

/*
 * Handles a property SET request and returns an unexpected error as all properties are declared as readonly
 */
AJ_Status OnboardingPropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    return AJ_ERR_UNEXPECTED;
}

AJ_Status OBS_ConfigureWiFi(AJ_Message*msg)
{
    AJ_Status status = AJ_OK;
    AJ_Message reply;

    // Set provided network configuration
    AJ_Printf("ConfigureWiFi()\n");

    OBInfo newInfo;
    char* ssid;
    char* pc;
    if (status = AJ_UnmarshalArgs(msg, "ssn", &ssid, &pc, &newInfo.authType) != AJ_OK)
        return status;
    if ((int8_t)newInfo.authType >= MAX_OF_WIFI_AUTH_TYPE || (int8_t)newInfo.authType <= MIN_OF_WIFI_AUTH_TYPE) {
        AJ_Printf("Unknown authentication type %d\n", newInfo.authType);
        if (status = AJ_MarshalErrorMsg(msg, &reply, InvalidValue) != AJ_OK)
            return status;
        if (status = AJ_DeliverMsg(&reply) != AJ_OK)
            return status;
        return status;
    }
    size_t ssidLen = min(strlen(ssid), SSID_MAX_LENGTH);
    strncpy(newInfo.ssid, ssid, SSID_MAX_LENGTH);
    newInfo.ssid[ssidLen] = '\0';
    size_t pcLen = min(strlen(pc), PASSCODE_MAX_LENGTH);
    strncpy(newInfo.pc, pc, PASSCODE_MAX_LENGTH);
    newInfo.pc[pcLen] = '\0';

    AJ_Printf("Got new info for %s with passcode=%s and auth=%d\n", newInfo.ssid, newInfo.pc, newInfo.authType);
    int16_t retVal = 1;
    if (status = AJ_MarshalReplyMsg(msg, &reply) != AJ_OK)
        return status;
    if (status = AJ_MarshalArgs(&reply, "n", retVal) != AJ_OK)
        return status;
    if (status = AJ_DeliverMsg(&reply) != AJ_OK)
        return status;

    newInfo.state = CONFIGURED_NOT_VALIDATED;
    status = OBS_WriteInfo(&newInfo);

    if (status == AJ_OK) {
        // Change state to CONFIGURED
        obState = newInfo.state;
    }

    return status;
}

AJ_Status OBS_ConnectWiFi(AJ_Message*msg)
{
    AJ_Status status = AJ_OK;

    AJ_Printf("Got connect\n");
    OBInfo obInfo;
    if (status = OBS_ReadInfo(&obInfo) != AJ_OK)
        return status;
    AJ_Printf("ReadInfo status: %s\n", AJ_StatusText(status));
    status = AJ_ERR_RESTART;     // Force disconnect of AJ and services and reconnection of WiFi on restart

    return status;
}

AJ_Status OBS_OffboardWiFi(AJ_Message*msg)
{
    AJ_Status status = AJ_OK;

    AJ_Printf("Offboard()\n");
    if (status = OBCAPI_DoOffboardWiFi() != AJ_OK)
        return status;
    status = AJ_ERR_RESTART;     // Force disconnect of AJ and services and reconnection of WiFi on restart

    return status;
}

AJ_Status OBS_GetScanInfo(AJ_Message*msg)
{
    AJ_Status status = AJ_OK;
    AJ_Message reply;
    AJ_Arg array;
    AJ_Arg structure;

    AJ_Printf("GetScanInfo()\n");

    if (status = AJ_MarshalReplyMsg(msg, &reply) != AJ_OK)
        return status;

    uint32_t elapsed = AJ_GetElapsedTime(&obLastScan, TRUE);
    if (elapsed > 0) {
        elapsed /= 60000;
    }
    if (status = AJ_MarshalArgs(&reply, "q", (uint16_t) elapsed) != AJ_OK)
        return status;
    if (status = AJ_MarshalContainer(&reply, &array, AJ_ARG_ARRAY) != AJ_OK)
        return status;

    int i = 0;
    for (; i < obScanInfoCount; ++i) {
        if (status = AJ_MarshalContainer(&reply, &structure, AJ_ARG_STRUCT) != AJ_OK)
            return status;
        if (status = AJ_MarshalArgs(&reply, "s", obScanInfos[i].ssid) != AJ_OK)
            return status;
        if (status = AJ_MarshalArgs(&reply, "n", obScanInfos[i].authType) != AJ_OK)
            return status;
        if (status = AJ_MarshalCloseContainer(&reply, &structure) != AJ_OK)
            return status;
    }

    if (status = AJ_MarshalCloseContainer(&reply, &array) != AJ_OK)
        return status;
    if (status = AJ_DeliverMsg(&reply) != AJ_OK)
        return status;

    return status;
}

/*
   //will be used in future versions
   AJ_Status OBS_SendConnectionResult(AJ_BusAttachment* bus)
   {
    AJ_Status status = AJ_OK;
    AJ_Printf("ConnectionResult()\n");
    AJ_Message out;
    AJ_Arg structure;
    if (status = AJ_MarshalSignal(bus, &out, OBS_CONNECTION_RESULT, NULL, obsSessionId, AJ_FLAG_GLOBAL_BROADCAST, CONNECTION_RESULT_TTL) != AJ_OK)
        return status;
    if (status = AJ_MarshalContainer(&out, &structure, AJ_ARG_STRUCT) != AJ_OK)
        return status;
    if (status = AJ_MarshalArgs(&out, "n", obLastError.code) != AJ_OK)
        return status;
    if (status = AJ_MarshalArgs(&out, "s", obLastError.message) != AJ_OK)
        return status;
    if (status = AJ_MarshalCloseContainer(&out, &structure) != AJ_OK)
        return status;
    AJ_DeliverMsg(&out);
    AJ_CloseMsg(&out);

    return status;
   }
 */