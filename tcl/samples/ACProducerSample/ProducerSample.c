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

#include <NotificationProducerSample.h>
#include <alljoyn/notification/NotificationProducer.h>

#include "ControlPanelProvided.h"

#ifdef QCC_OS_LINUX
#include <producer_sample_util.h>
#else
#define GetNotificationFromUser(x) do { } while (0)
#define Producer_SetupEnv(x) do { } while (0)
#define PossiblyDeleteNotification(x) do { } while (0)
#define FreeNotification() do { } while (0)
#endif

#define NUM_CUSTOMS 0
#define NUM_TEXTS   1
#define NUM_RICH_AUDIO 0
#define MESSAGES_INTERVAL 16

/**
 * Static non consts - sample application specirfic
 */
const static char* langEng  = "en";
//const static char* langSpan = "es_SP";
const static char* helloEng = "Hello AJL World";
//const static char* helloSpan = "Hola AJL Munda";
//const static char* onKey = "On";
//const static char* offKey = "Off";
//const static char* HelloVal = "Hello";
//const static char* GoodbyeVal = "Goodbye";
//const static char* Audio1URL = "http://www.getAudio1.org";
//const static char* Audio2URL = "http://www.getAudio2.org";
//const static char* Icon1URL = "http://www.getIcon1.org";
//const static char* controlPanelServiceObjectPath = "/ControlPanel/MyDevice/areYouSure";
//const static char* richIconObjectPath = "/icon/MyDevice";
//const static char* richAudioObjectPath = "/audio/MyDevice";
//static int8_t inputMode;
//static uint16_t isMessageTime = 0;
static notification notificationContent;
struct keyValue textToSend[NUM_TEXTS]; //, customAttributesToSend[NUM_CUSTOMS], richAudioUrls[NUM_RICH_AUDIO];


/**
 * Initial the Notifications that will be used during this sample app
 */
static void InitNotification()
{
    notificationContent.messageType = INFO;

    notificationContent.numCustomAttributes = NUM_CUSTOMS;
//    customAttributesToSend[0].key   = onKey;
//    customAttributesToSend[0].value = HelloVal;
//    customAttributesToSend[1].key   = offKey;
//    customAttributesToSend[1].value = GoodbyeVal;
//    notificationContent.customAttributes = customAttributesToSend;

    notificationContent.numTexts = NUM_TEXTS;
    textToSend[0].key   = langEng;
    textToSend[0].value = helloEng;
//    textToSend[1].key   = langSpan;
//    textToSend[1].value = helloSpan;
    notificationContent.texts = textToSend;

    notificationContent.numAudioUrls = NUM_RICH_AUDIO;
//    richAudioUrls[0].key   = langEng;
//    richAudioUrls[0].value = Audio1URL;
//    richAudioUrls[1].key   = langSpan;
//    richAudioUrls[1].value = Audio2URL;
//    notificationContent.richAudioUrls = richAudioUrls;

//    notificationContent.richIconUrl = Icon1URL;
//    notificationContent.richIconObjectPath = richIconObjectPath;
//    notificationContent.richAudioObjectPath = richAudioObjectPath;
//    notificationContent.controlPanelServiceObjectPath = controlPanelServiceObjectPath;

    notificationContent.ttl = 20000; // mandatory seconds
}

/**
 * Initialize service
 */
void Producer_Init()
{
    Producer_SetupEnv(&inputMode);
    InitNotification();
}

AJ_Status Producer_ConnectedHandler(AJ_BusAttachment* bus)
{
    //Service function stub
    return AJ_OK;
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

        ProducerSetNotification(&notificationContent);
        AJ_Printf("About to send Notification ==> %s \n", textToSend[0].value);
        ProducerSendNotifications();
    }
}

Service_Status Producer_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status*msgStatus)
{
    Service_Status service_Status = SERVICE_STATUS_NOT_HANDLED;

    switch (msg->msgId) {
    case EMERGENCY_NOTIFICATION_GET_PROPERTY:
    case WARNING_NOTIFICATION_GET_PROPERTY:
    case INFO_NOTIFICATION_GET_PROPERTY:
        *msgStatus = AJ_BusPropGet(msg, ProducerPropGetHandler, NULL);
        service_Status = SERVICE_STATUS_HANDLED;
        break;

    case EMERGENCY_NOTIFICATION_SET_PROPERTY:
    case WARNING_NOTIFICATION_SET_PROPERTY:
    case INFO_NOTIFICATION_SET_PROPERTY:
        *msgStatus = AJ_BusPropSet(msg, ProducerPropSetHandler, NULL);
        service_Status = SERVICE_STATUS_HANDLED;
        break;

    default:
        break;
    }
    return service_Status;
}

void Producer_Finish(AJ_BusAttachment* bus)
{
    //Service function stub
    return;
}
