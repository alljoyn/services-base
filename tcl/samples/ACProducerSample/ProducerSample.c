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

#include <NotificationProducerSample.h>
#include <alljoyn/notification/NotificationProducer.h>

#include "ControlPanelProvided.h"

#define NUM_CUSTOMS 0
#define NUM_TEXTS   1
#define NUM_RICH_AUDIO 0
#define MESSAGES_INTERVAL 16

/**
 * Static non consts - sample application specirfic
 */
const static char* langEng  = "en";
const static char* helloEng = "Hello AJL World";
static NotificationContent_t notificationContent;
struct keyValue textToSend[NUM_TEXTS];

/**
 * Initial the Notifications that will be used during this sample app
 */
static void InitNotification()
{
    notificationContent.numCustomAttributes = NUM_CUSTOMS;

    notificationContent.numTexts = NUM_TEXTS;
    textToSend[0].key   = langEng;
    textToSend[0].value = helloEng;
    notificationContent.texts = textToSend;

    notificationContent.numAudioUrls = NUM_RICH_AUDIO;

}

/**
 * Initialize service
 */
void Producer_Init()
{
    InitNotification();
}

AJ_Status Producer_ConnectedHandler(AJ_BusAttachment* bus)
{
    AJ_SessionOpts sessionOpts = {
        AJ_SESSION_TRAFFIC_MESSAGES,
        AJ_SESSION_PROXIMITY_ANY,
        AJ_TRANSPORT_ANY,
        FALSE
    };

    AJ_Status status;
    AJ_Time timer = { 0, 0 };
    AJ_InitTimer(&timer);

    status = AJ_BusBindSessionPort(bus, NotificationProducerPort, &sessionOpts);
    if (status != AJ_OK) {
        AJ_Printf("Failed to send bind session port message\n");
    }

    uint8_t serviceStarted = FALSE;
    while (!serviceStarted && (status == AJ_OK)) {
        AJ_Message msg;

        AJ_GetElapsedTime(&timer, TRUE);

        status = AJ_UnmarshalMsg(bus, &msg, AJAPP_UNMARSHAL_TIMEOUT);

        /*
         * TODO This is a temporary hack to work around buggy select implementations
         */
        if (status == AJ_ERR_TIMEOUT) {
            uint32_t elapsed = AJ_GetElapsedTime(&timer, TRUE);
            if (elapsed < AJAPP_UNMARSHAL_TIMEOUT) {
                AJ_Printf("Spurious timeout error (elapsed=%d < AJAPP_UNMARSHAL_TIMEOUT=%d) - continuing\n", elapsed, AJAPP_UNMARSHAL_TIMEOUT);
                status = AJ_OK;
                continue;
            }
        }

        if (status != AJ_OK) {
            break;
        }

        switch (msg.msgId) {
        case AJ_REPLY_ID(AJ_METHOD_BIND_SESSION_PORT):
            if (msg.hdr->msgType == AJ_MSG_ERROR) {
                status = AJ_ERR_FAILURE;
            } else {
                serviceStarted = TRUE;
            }
            break;

        default:
            /*
             * Pass to the built-in bus message handlers
             */
            status = AJ_BusHandleBusMessage(&msg);
            break;
        }
        AJ_CloseMsg(&msg);
    }

    if (status != AJ_OK) {
        AJ_Printf("AllJoyn disconnect bus status=%d\n", status);
        status = AJ_ERR_READ;
    }
    return status;
}

uint8_t Producer_CheckSessionAccepted(uint16_t port, uint32_t sessionId, char* joiner)
{
    if (port != NotificationProducerPort) {
        return FALSE;
    }
    AJ_Printf("Producer: Accepted session on port %u from %s\n", port, joiner);
    return TRUE;
}

/**
 * Meant to simulate scenario where sometimes Notifications are set when
 * Send Notification is called and sometimes not.
 * Sets the notification every MESSAGES_INTERVAL time
 */
void Producer_DoWork(AJ_BusAttachment* bus)
{
    if (isThereANotificationToSend() > 0) {
        notificationContent.numTexts = NUM_TEXTS;
        textToSend[0].key   = langEng;
        textToSend[0].value = getNotificationString();

        ProducerSetNotification(&notificationContent, NOTIFICATION_MESSAGE_TYPE_INFO, 20000);
        AJ_Printf("About to send Notification ==> %s \n", textToSend[0].value);
        ProducerSendNotifications();
    }
}

Service_Status Producer_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status* msgStatus)
{
    Service_Status service_Status = SERVICE_STATUS_NOT_HANDLED;

    switch (msg->msgId) {
    case EMERGENCY_NOTIFICATION_GET_PROPERTY:
    case WARNING_NOTIFICATION_GET_PROPERTY:
    case INFO_NOTIFICATION_GET_PROPERTY:
    case NOTIFICATION_PRODUCER_GET_PROPERTY:
        *msgStatus = AJ_BusPropGet(msg, ProducerPropGetHandler, NULL);
        service_Status = SERVICE_STATUS_HANDLED;
        break;

    case EMERGENCY_NOTIFICATION_SET_PROPERTY:
    case WARNING_NOTIFICATION_SET_PROPERTY:
    case INFO_NOTIFICATION_SET_PROPERTY:
    case NOTIFICATION_PRODUCER_SET_PROPERTY:
        *msgStatus = AJ_BusPropSet(msg, ProducerPropSetHandler, NULL);
        service_Status = SERVICE_STATUS_HANDLED;
        break;

    case NOTIFICATION_PRODUCER_ACKNOWLEDGE:
        *msgStatus = ProducerAcknowledgeMsg(msg);
        service_Status = SERVICE_STATUS_HANDLED;
        break;

    case NOTIFICATION_PRODUCER_DISMISS:
        *msgStatus = ProducerDismissMsg(msg);
        service_Status = SERVICE_STATUS_HANDLED;
        break;

    default:
        break;
    }
    return service_Status;
}

void Producer_Finish(AJ_BusAttachment* bus)
{
    return;
}
