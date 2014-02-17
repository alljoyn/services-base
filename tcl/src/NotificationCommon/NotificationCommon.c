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
 * The corresponding flag dbgAJNS is defined in services_common.h and implemented
 * in services_common.c.
 */
#define AJ_MODULE AJNS
#include <aj_debug.h>

#include <alljoyn/notification/NotificationCommon.h>
#include <alljoyn/services_common/Services_Common.h>

const char AJNS_NotificationInterfaceName[]   = "org.alljoyn.Notification";
const char AJNS_NotificationSignalName[]      = "!notify >q >i >q >s >s >ay >s >a{iv} >a{ss} >a(ss)";
const char AJNS_NotificationPropertyVersion[] = "@Version>q";

const uint16_t AJNS_NotificationVersion = 2;

const uint16_t AJNS_NOTIFICATION_TTL_MIN   = 30;
const uint16_t AJNS_NOTIFICATION_TTL_MAX   = 43200;

/**
 * The interface name followed by the method signatures.
 *
 * See also .\inc\aj_introspect.h
 */
const char* AJNS_NotificationInterface[] = {
    AJNS_NotificationInterfaceName,               /*The first entry is the interface name.*/
    AJNS_NotificationSignalName,                  /*Signal at index 0 - See above for signature*/
    AJNS_NotificationPropertyVersion,
    NULL
};

/**
 * A NULL terminated collection of all interfaces.
 */
const AJ_InterfaceDescription AJNS_NotificationInterfaces[] = {
    AJ_PropertiesIface,
    AJNS_NotificationInterface,
    NULL
};

const uint16_t AJNS_NotificationDismisserVersion = 1;

// TODO: Change NotificationDismisserObjectPath to be 'const char []' when AJTCL adds the "DON'T COLLAPSE" flag
#define NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX "/notificationDismisser"
#define NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX_LENGTH 22
#define NOTIFICATION_DISMISSER_OBJECT_PATH_LENGTH (NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX_LENGTH + 1 + 2 * UUID_LENGTH + 1 + 10 + 1) // Prefix of NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX_LENGTH + '/' + AppId in 32 Hex chars + '/' + MsgId in 10 Ascii chars
char AJNS_NotificationDismisserObjectPath[NOTIFICATION_DISMISSER_OBJECT_PATH_LENGTH] = NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX; // /012345678901234567890123456789012/012345678";

const char* const AJNS_NotificationDismisserInterface[] = {
    "org.alljoyn.Notification.Dismisser",
    "!Dismiss >i >ay",
    "@Version>q",
    NULL
};

/**
 * A NULL terminated collection of all interfaces.
 */
const AJ_InterfaceDescription AJNS_NotificationDismisserInterfaces[] = {
    AJ_PropertiesIface,
    AJNS_NotificationDismisserInterface,
    NULL
};

AJ_Status AJNS_SendDismissSignal(AJ_BusAttachment* busAttachment, int32_t msgId, const char* appId)
{
    AJ_Status status;
    AJ_Message msg;

    AJ_InfoPrintf(("In SendDismiss\n"));

    if (appId == 0) {
        AJ_ErrPrintf(("AppId can not be NULL\n"));
        return AJ_ERR_DISALLOWED;
    }

    // TODO: Remove setting of temporary Dismisser ObjectPath when AJTCL adds the "DON'T COLLAPSE" flag
    AJNS_NotificationDismisserObjectPath[snprintf(AJNS_NotificationDismisserObjectPath, NOTIFICATION_DISMISSER_OBJECT_PATH_LENGTH, "%s/%s/%d", AJNS_NotificationDismisserObjectPath, appId, msgId)] = '\0';

    status = AJ_MarshalSignal(busAttachment, &msg, NOTIFICATION_DISMISSER_DISMISS_EMITTER, NULL, 0, ALLJOYN_FLAG_SESSIONLESS, AJNS_NOTIFICATION_TTL_MAX); // TODO: Add the "DON'T COLLAPSE" flag
    if (status != AJ_OK) {
        AJ_ErrPrintf(("Could not Marshal Signal\n"));
        return status;
    }

    status = AJ_MarshalArgs(&msg, "i", msgId);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
    status = AJSVC_MarshalAppId(&msg, appId);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
    status = AJ_DeliverMsg(&msg);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
    status = AJ_CloseMsg(&msg);
    if (status != AJ_OK) {
        goto ErrorExit;
    }

    // TODO: Remove resetting of temporary Dismisser ObjectPath when AJTCL adds the "DON'T COLLAPSE" flag
    AJNS_NotificationDismisserObjectPath[NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX_LENGTH] = '\0';
    return status;

ErrorExit:

    AJ_ErrPrintf(("Could not Deliver Message\n"));
    return status;
}

const uint16_t AJNS_NotificationProducerPort = 1010;
const char AJNS_NotificationProducerObjectPath[] = "/notificationProducer";

const char* const AJNS_NotificationProducerInterface[] = {
    "org.alljoyn.Notification.Producer",
    "?Dismiss <i",
    "@Version>q",
    NULL
};

const uint16_t AJNS_NotificationProducerVersion = 1;

const AJ_InterfaceDescription AJNS_NotificationProducerInterfaces[] = {
    AJ_PropertiesIface,
    AJNS_NotificationProducerInterface,
    AJNS_NotificationDismisserInterface,
    NULL
};

