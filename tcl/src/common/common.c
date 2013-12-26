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

#include <alljoyn/notification/common.h>
#include <alljoyn/services_common/Services_Common.h>

const char InterfaceName[]            = "org.alljoyn.Notification";
const char SignalName[]               = "!notify >q >i >q >s >s >ay >s >a{iv} >a{ss} >a(ss)";
const char Version[]                  = "@Version>q";

const uint16_t NotificationVersion = 2;

const uint16_t NOTIFICATION_TTL_MIN   = 30;
const uint16_t NOTIFICATION_TTL_MAX   = 43200;

/**
 * The interface name followed by the method signatures.
 *
 * See also .\inc\aj_introspect.h
 */
const char* NotificationInterface[] = {
    InterfaceName,                           /*The first entry is the interface name.*/
    SignalName,                              /*Signal at index 0 - See above for signature*/
    Version,
    NULL
};

/**
 * A NULL terminated collection of all interfaces.
 */
const AJ_InterfaceDescription NotificationInterfaces[] = {
    AJ_PropertiesIface,
    NotificationInterface,
    NULL
};

const uint16_t NotificationDismisserVersion = 1;

// TODO: Change NotificationDismisserObjectPath to be 'const char []' when AJTCL adds the "DON'T COLLAPSE" flag
#define NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX "/notificationDismisser"
#define NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX_LENGTH 22
#define NOTIFICATION_DISMISSER_OBJECT_PATH_LENGTH (NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX_LENGTH + 1 + 2 * UUID_LENGTH + 1 + 10 + 1) // Prefix of NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX_LENGTH + '/' + AppId in 32 Hex chars + '/' + MsgId in 10 Ascii chars
char NotificationDismisserObjectPath[NOTIFICATION_DISMISSER_OBJECT_PATH_LENGTH] = NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX; // /012345678901234567890123456789012/012345678";

const char* const NotificationDismisserInterface[] = {
    "org.alljoyn.Notification.Dismisser",
    "!Dismiss >i >ay",
    "@Version>q",
    NULL
};

/**
 * A NULL terminated collection of all interfaces.
 */
const AJ_InterfaceDescription NotificationDismisserInterfaces[] = {
    AJ_PropertiesIface,
    NotificationDismisserInterface,
    NULL
};

AJ_Status NotificationSendDismiss(int32_t msgId, const char* appId)
{
    AJ_Status status;
    AJ_Message msg;

    AJ_Printf("In SendDismiss\n");

    if (appId == 0) {
        AJ_Printf("AppId can not be NULL\n");
        return AJ_ERR_DISALLOWED;
    }

    // TODO: Remove setting of temporary Dismisser ObjectPath when AJTCL adds the "DON'T COLLAPSE" flag
    NotificationDismisserObjectPath[snprintf(NotificationDismisserObjectPath, NOTIFICATION_DISMISSER_OBJECT_PATH_LENGTH, "%s/%s/%d", NotificationDismisserObjectPath, appId, msgId)] = '\0';

    status = AJ_MarshalSignal(&busAttachment, &msg, NOTIFICATION_DISMISSER_DISMISS_EMITTER, NULL, 0, ALLJOYN_FLAG_SESSIONLESS, NOTIFICATION_TTL_MAX); // TODO: Add the "DON'T COLLAPSE" flag
    if (status != AJ_OK) {
        AJ_Printf("Could not Marshal Signal\n");
        return status;
    }

    do {
        CHECK(AJ_MarshalArgs(&msg, "i", msgId));
        CHECK(Common_MarshalAppId(&msg, appId));
    } while (0);

    status = AJ_DeliverMsg(&msg);
    if (status != AJ_OK) {
        AJ_Printf("Could not Deliver Message\n");
        return status;
    }

    AJ_CloseMsg(&msg);

    // TODO: Remove resetting of temporary Dismisser ObjectPath when AJTCL adds the "DON'T COLLAPSE" flag
    NotificationDismisserObjectPath[NOTIFICATION_DISMISSER_OBJECT_PATH_PREFIX_LENGTH] = '\0';
    return status;
}

const uint16_t NotificationProducerPort = 1010;
const char NotificationProducerObjectPath[] = "/notificationProducer";

const char* const NotificationProducerInterface[] = {
    "org.alljoyn.Notification.Producer",
    "?Acknowledge <i",
    "?Dismiss <i",
    "@Version>q",
    NULL
};

const uint16_t NotificationProducerVersion = 1;

const AJ_InterfaceDescription NotificationProducerInterfaces[] = {
    AJ_PropertiesIface,
    NotificationProducerInterface,
    NotificationDismisserInterface,
    NULL
};

