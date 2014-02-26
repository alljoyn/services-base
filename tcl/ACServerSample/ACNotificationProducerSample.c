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

#include <alljoyn.h>
#include <NotificationProducerSample.h>
#include <alljoyn/notification/NotificationProducer.h>

#include "ControlPanelProvided.h"

#ifndef NDEBUG
extern AJ_EXPORT uint8_t dbgAJSVCAPP;
#endif

#define NUM_CUSTOMS 0
#define NUM_TEXTS   1
#define NUM_RICH_AUDIO 0
#define MESSAGES_INTERVAL 16

/**
 * Static non consts - sample application specirfic
 */
const static char* langEng  = "en";
const static char* helloEng = "Hello AJL World";
static AJNS_DictionaryEntry textToSend[NUM_TEXTS];
static AJNS_NotificationContent notificationContent;

/**
 * Initial the Notifications that will be used during this sample app
 */
static void InitNotificationContent()
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
AJ_Status NotificationProducer_Init()
{
    InitNotificationContent();
    return AJ_OK;
}

/**
 * Meant to simulate scenario where sometimes Notifications are set when
 * Send Notification is called and sometimes not.
 */
void NotificationProducer_DoWork(AJ_BusAttachment* busAttachment)
{
    if (isThereANotificationToSend() > 0) {
        notificationContent.numTexts = NUM_TEXTS;
        textToSend[0].key   = langEng;
        textToSend[0].value = getNotificationString();

        AJ_AlwaysPrintf(("About to send Notification ==> %s \n", textToSend[0].value));
        AJNS_Producer_SendNotification(busAttachment, &notificationContent, AJNS_NOTIFICATION_MESSAGE_TYPE_INFO, 20000, NULL);
    }
}

AJ_Status NotificationProducer_Finish(AJ_BusAttachment* busAttachment)
{
    return AJ_OK;
}
