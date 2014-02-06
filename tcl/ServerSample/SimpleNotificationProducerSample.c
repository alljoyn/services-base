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
#include <alljoyn/notification/NotificationCommon.h>
#include <alljoyn/notification/NotificationProducer.h>

#ifdef __linux
#include <NotificationProducerSampleUtil.h>
#else
#define MESSAGES_INTERVAL 16
#define Producer_GetNotificationFromUser(...) do { } while (0)
#define Producer_SetupEnv(...) do { } while (0)
#define Producer_PossiblyDeleteNotification(...) do { } while (0)
#define Producer_FreeNotification(...) do { } while (0)
#endif

#define NUM_CUSTOMS 2
#define NUM_TEXTS   2
#define NUM_RICH_AUDIO 2

/**
 * Static non consts - sample application specific
 */
const static char* lang1  = "en";
const static char* lang2 = "de-AT";
const static char* hello1 = "Hello AJL World";
const static char* hello2 = "Hallo AJL Welt";
const static char* onKey = "On";
const static char* offKey = "Off";
const static char* HelloVal = "Hello";
const static char* GoodbyeVal = "Goodbye";
const static char* Audio1URL = "http://www.getAudio1.org";
const static char* Audio2URL = "http://www.getAudio2.org";
const static char* Icon1URL = "http://www.getIcon1.org";
const static char* controlPanelServiceObjectPath = "/ControlPanel/MyDevice/areYouSure";
const static char* richIconObjectPath = "/icon/MyDevice";
const static char* richAudioObjectPath = "/audio/MyDevice";
static uint8_t inputMode = 0;
static uint16_t isMessageTime = 0;
static uint16_t nextMessageTime = MESSAGES_INTERVAL;
AJNS_DictionaryEntry textToSend[NUM_TEXTS], customAttributesToSend[NUM_CUSTOMS], richAudioUrls[NUM_RICH_AUDIO];

/**
 * Initial the Notifications that will be used during this sample app
 */
static AJNS_NotificationContent notificationContent;
static void InitNotification()
{
    notificationContent.numCustomAttributes = NUM_CUSTOMS;
    customAttributesToSend[0].key   = onKey;
    customAttributesToSend[0].value = HelloVal;
    customAttributesToSend[1].key   = offKey;
    customAttributesToSend[1].value = GoodbyeVal;
    notificationContent.customAttributes = customAttributesToSend;

    notificationContent.numTexts = NUM_TEXTS;
    textToSend[0].key   = lang1;
    textToSend[0].value = hello1;
    textToSend[1].key   = lang2;
    textToSend[1].value = hello2;
    notificationContent.texts = textToSend;

    notificationContent.numAudioUrls = NUM_RICH_AUDIO;
    richAudioUrls[0].key   = lang1;
    richAudioUrls[0].value = Audio1URL;
    richAudioUrls[1].key   = lang2;
    richAudioUrls[1].value = Audio2URL;
    notificationContent.richAudioUrls = richAudioUrls;

    notificationContent.richIconUrl = Icon1URL;
    notificationContent.richIconObjectPath = richIconObjectPath;
    notificationContent.richAudioObjectPath = richAudioObjectPath;
    notificationContent.controlPanelServiceObjectPath = controlPanelServiceObjectPath;
}

/**
 * Initialize service
 */
AJ_Status NotificationProducer_Init()
{
    AJ_Status status = AJ_OK;
    Producer_SetupEnv(&inputMode);
    InitNotification();
    return status;
}

/**
 * Meant to simulate scenario where sometimes Notifications are set when
 * Send Notification is called and sometimes not.
 * Sets the notification every MESSAGES_INTERVAL time
 */
static void PossiblySendNotification(AJ_BusAttachment* busAttachment)
{
    uint16_t messageType = AJNS_NOTIFICATION_MESSAGE_TYPE_INFO;
    uint32_t ttl = 20000;
    uint32_t serialNum;

    if (isMessageTime == 0) {
        if (!inputMode) {
            notificationContent.controlPanelServiceObjectPath = ((notificationContent.controlPanelServiceObjectPath == NULL) ? controlPanelServiceObjectPath : NULL); // Toggle notification with action ON/OFF
        } else {
            Producer_GetNotificationFromUser(&notificationContent, &messageType, &ttl, &nextMessageTime);
        }
        AJNS_Producer_SendNotification(busAttachment, &notificationContent, messageType, ttl, &serialNum);
        if (inputMode) {
            Producer_FreeNotification(&notificationContent);
        }
    }

    if (++isMessageTime == nextMessageTime) {
        isMessageTime = 0;
    }
}

void NotificationProducer_DoWork(AJ_BusAttachment* busAttachment)
{
    PossiblySendNotification(busAttachment);
    if (inputMode) {
        Producer_PossiblyDeleteNotification(busAttachment, isMessageTime);
    }
}

AJ_Status NotificationProducer_Finish()
{
    return AJ_OK;
}
