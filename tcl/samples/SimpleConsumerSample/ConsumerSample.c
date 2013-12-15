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

#include <NotificationConsumerSample.h>
#include <alljoyn/notification/NotificationConsumer.h>

#ifdef QCC_OS_LINUX
#include <consumer_sample_util.h>
#else
#define Consumer_SetupEnv(x) do { } while (0)
#endif

static int8_t superAgentMode = 1;

void Consumer_Init()
{
    Consumer_SetupEnv(&superAgentMode);
    ProxyObjects[NUM_PRE_NOTIFICATION_CONSUMER_OBJECTS] = superAgentMode ? AllAppObject : NotificationAppObject;
}

AJ_Status Consumer_ConnectedHandler(AJ_BusAttachment* bus)
{
    return ConsumerSetSignalRules(bus, superAgentMode, 0);
}

Service_Status Consumer_NotifySessionAccepted(uint32_t sessionId, const char*senderBusName)
{
    return SERVICE_STATUS_NOT_HANDLED;
}

Service_Status Consumer_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status*msgStatus)
{
    switch (msg->msgId) {
    case NOTIFICATION_SIGNAL_RECEIVED:
        AJ_Printf("Received Producer signal.\n");
        ConsumerHandleNotifyMsgObj(msg);
        break;

    case SUPERAGENT_SIGNAL:
        AJ_Printf("Received Superagent Signal.\n");
        *msgStatus = ConsumerSetSignalRules(bus, superAgentMode, msg->sender);
        if (AJ_OK == *msgStatus) {
            ProxyObjects[NUM_PRE_NOTIFICATION_CONSUMER_OBJECTS] = SuperAgentObject;
        }
        ConsumerHandleNotifyMsgObj(msg);
        break;

    case INTERFACE_GET_PROPERTY:
        *msgStatus = AJ_BusPropGet(msg, ConsumerPropGetHandler, NULL);
        break;

    case INTERFACE_SET_PROPERTY:
        *msgStatus = AJ_BusPropSet(msg, ConsumerPropSetHandler, NULL);
        break;

    default:
        return SERVICE_STATUS_NOT_HANDLED;
    }
    return SERVICE_STATUS_HANDLED;
}

void Consumer_IdleConnectedHandler(AJ_BusAttachment* bus)
{
    //Service function stub
    return;
}

void Consumer_Finish(AJ_BusAttachment* bus)
{
    //Service function stub
    return;
}

AJ_Status HandleNotify(uint16_t protoVer,
                       uint32_t messageId,
                       notification*notificationContent,
                       const char* deviceId,
                       const char* deviceName,
                       const char* appId,
                       const char* appName)
{
    AJ_Printf("******************** Begin New Message Received ********************\n");
    AJ_Printf("Message Id: %d\nDevice Id: %s\nDevice Name: %s\nApp Id: %s\nApp Name: %s\nMessage Type: %d\n",
              messageId, deviceId, deviceName, appId, appName, notificationContent->messageType);

    int8_t indx;

    for (indx = 0; indx < notificationContent->numTexts; indx++) {
        AJ_Printf("Language: %s  Message: %s.\n", notificationContent->texts[indx].key, notificationContent->texts[indx].value);
    }

    AJ_Printf("Other parameters included:\n");
    for (indx = 0; indx < notificationContent->numCustomAttributes; indx++) {
        AJ_Printf("Custom Attribute Key: %s  Custom Attribute Value: %s\n", notificationContent->customAttributes[indx].key, notificationContent->customAttributes[indx].value);
    }

    if (notificationContent->richIconUrl != 0 && strlen(notificationContent->richIconUrl) > 0) {
        AJ_Printf("Rich Content Icon Url: %s\n", notificationContent->richIconUrl);
    }

    if (notificationContent->numAudioUrls) {
        AJ_Printf("******************** Begin Rich Audio Content ********************\n");
        for (indx = 0; indx < notificationContent->numAudioUrls; indx++) {
            AJ_Printf("Language: %s  Audio URL %s\n", notificationContent->richAudioUrls[indx].key, notificationContent->richAudioUrls[indx].value);
        }
        AJ_Printf("******************** End Rich Audio Content ********************\n");
    }

    if (notificationContent->richIconObjectPath != 0 && strlen(notificationContent->richIconObjectPath) > 0) {
        AJ_Printf("Rich Content Icon Object Path: %s\n", notificationContent->richIconObjectPath);
    }

    if (notificationContent->richAudioObjectPath != 0 && strlen(notificationContent->richAudioObjectPath) > 0) {
        AJ_Printf("Rich Content Audio Object Path: %s\n", notificationContent->richAudioObjectPath);
    }

    if (notificationContent->controlPanelServiceObjectPath != 0 && strlen(notificationContent->controlPanelServiceObjectPath) > 0) {
        AJ_Printf("ControlPanelService object path: %s\n", notificationContent->controlPanelServiceObjectPath);
    }

    AJ_Printf("******************** End New Message Received ********************\n");

    return AJ_OK;
}
