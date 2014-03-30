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
 * The corresponding flag dbgAJSVC is defined in ServicesCommon.h and implemented
 * in ServicesCommon.c.
 */
#define AJ_MODULE AJSVC
#include <aj_debug.h>

#include <alljoyn.h>

#include <alljoyn/services_common/ServicesCommon.h>
#include <alljoyn/services_common/PropertyStore.h>
#include <alljoyn/services_common/ServicesHandlers.h>
#ifdef ONBOARDING_SERVICE
#include <alljoyn/onboarding/OnboardingManager.h>
#endif

#include <aj_config.h>
#include <aj_link_timeout.h>

AJ_Status AJSVC_ConnectedHandler(AJ_BusAttachment* busAttachment)
{
    AJ_Status status = AJ_OK;

#ifdef CONFIG_SERVICE

    status = AJCFG_ConnectedHandler(busAttachment);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
#endif
#ifdef ONBOARDING_SERVICE

    status = AJOBS_ConnectedHandler(busAttachment);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
#endif
#ifdef NOTIFICATION_SERVICE_PRODUCER

    status = AJNS_Producer_ConnectedHandler(busAttachment);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
#endif
#ifdef CONTROLPANEL_SERVICE

    status = AJCPS_ConnectedHandler(busAttachment);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
#endif
#ifdef NOTIFICATION_SERVICE_CONSUMER

    status = AJNS_Consumer_ConnectedHandler(busAttachment);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
#endif
    return status;

ErrorExit:

    AJ_AlwaysPrintf(("Service ConnectedHandler returned an error %s\n", (AJ_StatusText(status))));
    return status;
}

uint8_t AJSVC_CheckSessionAccepted(uint16_t port, uint32_t sessionId, char* joiner)
{
    uint8_t session_accepted = FALSE;

#ifdef NOTIFICATION_SERVICE_PRODUCER
    session_accepted |= AJNS_Producer_CheckSessionAccepted(port, sessionId, joiner);
#endif

#ifdef CONTROLPANEL_SERVICE
    session_accepted |= AJCPS_CheckSessionAccepted(port, sessionId, joiner);
#endif

    return session_accepted;
}

static AJSVC_ServiceStatus SessionJoinedHandler(AJ_BusAttachment* busAttachment, uint32_t sessionId, uint32_t replySerialNum)
{
    AJSVC_ServiceStatus serviceStatus = AJSVC_SERVICE_STATUS_NOT_HANDLED;

#ifdef NOTIFICATION_SERVICE_CONSUMER
    if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
        serviceStatus = AJNS_Consumer_SessionJoinedHandler(busAttachment, sessionId, replySerialNum);
    }
#endif

    return serviceStatus;
}

static AJSVC_ServiceStatus SessionRejectedHandler(AJ_BusAttachment* busAttachment, uint32_t sessionId, uint32_t replySerialNum, uint32_t replyCode)
{
    AJSVC_ServiceStatus serviceStatus = AJSVC_SERVICE_STATUS_NOT_HANDLED;

#ifdef NOTIFICATION_SERVICE_CONSUMER
    if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
        serviceStatus = AJNS_Consumer_SessionRejectedHandler(busAttachment, replySerialNum, replyCode);
    }
#endif

    return serviceStatus;
}

static AJSVC_ServiceStatus SessionLostHandler(AJ_BusAttachment* busAttachment, uint32_t sessionId, uint32_t reason)
{
    AJSVC_ServiceStatus serviceStatus = AJSVC_SERVICE_STATUS_NOT_HANDLED;

#ifdef NOTIFICATION_SERVICE_CONSUMER
    if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
        serviceStatus = AJNS_Consumer_SessionLostHandler(busAttachment, sessionId, reason);
    }
#endif

    return serviceStatus;
}

AJSVC_ServiceStatus AJSVC_MessageProcessorAndDispatcher(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJ_Status* status)
{
    AJSVC_ServiceStatus serviceStatus = AJSVC_SERVICE_STATUS_NOT_HANDLED;

    if (msg->msgId == AJ_REPLY_ID(AJ_METHOD_JOIN_SESSION)) {     // Process all incoming replies to join a session and pass session state change to all services
        uint32_t replyCode = 0;
        uint32_t sessionId = 0;
        uint8_t sessionJoined = FALSE;
        uint32_t joinSessionReplySerialNum = msg->replySerial;
        if (msg->hdr->msgType == AJ_MSG_ERROR) {
            AJ_AlwaysPrintf(("JoinSessionReply: AJ_METHOD_JOIN_SESSION: AJ_ERR_FAILURE\n"));
            *status = AJ_ERR_FAILURE;
        } else {
            *status = AJ_UnmarshalArgs(msg, "uu", &replyCode, &sessionId);
            if (*status != AJ_OK) {
                AJ_AlwaysPrintf(("JoinSessionReply: failed to unmarshal\n"));
            } else {
                if (replyCode == AJ_JOINSESSION_REPLY_SUCCESS) {
                    AJ_AlwaysPrintf(("JoinSessionReply: AJ_JOINSESSION_REPLY_SUCCESS with sessionId=%u and replySerial=%u\n", sessionId, joinSessionReplySerialNum));
                    sessionJoined = TRUE;
                } else {
                    AJ_AlwaysPrintf(("JoinSessionReply: AJ_ERR_FAILURE\n"));
                    *status = AJ_ERR_FAILURE;
                }
            }
        }
        if (sessionJoined) {
            serviceStatus = SessionJoinedHandler(busAttachment, sessionId, joinSessionReplySerialNum);
        } else {
            serviceStatus = SessionRejectedHandler(busAttachment, sessionId, joinSessionReplySerialNum, replyCode);
        }
        if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
            AJ_ResetArgs(msg);
        }
    } else if (msg->msgId == AJ_SIGNAL_SESSION_LOST || msg->msgId == AJ_SIGNAL_SESSION_LOST_WITH_REASON) {     // Process all incoming LeaveSession replies and lost session signals and pass session state change to all services
        uint32_t sessionId = 0;
        uint32_t reason = 0;
        if (msg->msgId == AJ_SIGNAL_SESSION_LOST_WITH_REASON) {
            *status = AJ_UnmarshalArgs(msg, "uu", &sessionId, &reason);
        } else {
            *status = AJ_UnmarshalArgs(msg, "u", &sessionId);
        }
        if (*status != AJ_OK) {
            AJ_AlwaysPrintf(("JoinSessionReply: failed to marshal\n"));
        } else {
            AJ_AlwaysPrintf(("Session lost: sessionId = %u, reason = %u\n", sessionId, reason));
            serviceStatus = SessionLostHandler(busAttachment, sessionId, reason);
            if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
                AJ_ResetArgs(msg);
            }
        }
    } else {
#ifdef CONFIG_SERVICE
        if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
            serviceStatus = AJCFG_MessageProcessor(busAttachment, msg, status);
        }
#endif
#ifdef ONBOARDING_SERVICE
        if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
            serviceStatus = AJOBS_MessageProcessor(busAttachment, msg, status);
        }
#endif
#ifdef NOTIFICATION_SERVICE_PRODUCER
        if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
            serviceStatus = AJNS_Producer_MessageProcessor(busAttachment, msg, status);
        }
#endif
#ifdef NOTIFICATION_SERVICE_CONSUMER
        if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
            serviceStatus = AJNS_Consumer_MessageProcessor(busAttachment, msg, status);
        }
#endif
#ifdef CONTROLPANEL_SERVICE
        if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
            serviceStatus = AJCPS_MessageProcessor(busAttachment, msg, status);
        }
#endif
    }
    return serviceStatus;
}


AJ_Status AJSVC_DisconnectHandler(AJ_BusAttachment* busAttachment)
{
    AJ_Status status = AJ_OK;

#ifdef CONFIG_SERVICE
    AJCFG_DisconnectHandler(busAttachment);
#endif
#ifdef ONBOARDING_SERVICE
    AJOBS_DisconnectHandler(busAttachment);
#endif
#ifdef NOTIFICATION_SERVICE_CONSUMER
    AJNS_Consumer_DisconnectHandler(busAttachment);
#endif
#ifdef NOTIFICATION_SERVICE_PRODUCER
    AJNS_Producer_DisconnectHandler(busAttachment);
#endif
#ifdef CONTROLPANEL_SERVICE
    AJCPS_DisconnectHandler(busAttachment);
#endif

    return status;
}
