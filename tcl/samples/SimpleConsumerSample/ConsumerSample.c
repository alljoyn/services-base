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

#include <NotificationConsumerSample.h>
#include <alljoyn/notification/NotificationConsumer.h>

#ifdef __linux
#include <consumer_sample_util.h>
#else
#define Consumer_SetupEnv(x) do { } while (0)
#endif

static uint8_t superAgentMode = 1;

static enum _action {
    NOTHING = 0,
    ACKNOWLEDGE,
    DISMISS
} nextAction = NOTHING;

static struct _NotificationReference {
    uint16_t version;
    int32_t notificationId;
    char appId[UUID_LENGTH * 2 + 1];
    char originalSenderName[16];
} savedNotification;

static uint32_t producerSessionId = 0;
static uint32_t lastSessionRequestSerialNum = 0;

void Consumer_Init()
{
    Consumer_SetupEnv(&superAgentMode);
    AJ_Printf("Init(): Set Consumer to detect SuperAgent option is turned %s\n", superAgentMode ? "ON" : "off");
    ProxyObjects[NOTIFICATION_PROXYOBJECT_INDEX] = superAgentMode ? AllProxyObject : NotificationProxyObject;

    memset(&savedNotification, 0, sizeof(struct _NotificationReference));
}

AJ_Status Consumer_ConnectedHandler(AJ_BusAttachment* bus)
{
    return ConsumerSetSignalRules(bus, superAgentMode, 0);
}

Service_Status Consumer_HandleSessionStateChanged(AJ_BusAttachment* bus, uint32_t sessionId, uint8_t sessionJoined, uint32_t replySerialNum)
{
    Service_Status serviceStatus = SERVICE_STATUS_NOT_HANDLED;
    AJ_Printf("Inside HandleSessionStateChanged()\n");
    AJ_Printf("HandleSessionStateChanged(): Got reply that session with id %u was %s\n", sessionId, sessionJoined ? "joined" : "dropped");
    if (savedNotification.version > 0 && lastSessionRequestSerialNum == replySerialNum) {
        AJ_Printf("HandleSessionStateChanged(): Got reply serial number %u that matches last request serial number %u\n", replySerialNum, lastSessionRequestSerialNum);
        if (sessionJoined && producerSessionId == 0) {
            producerSessionId = sessionId;
        } else if (!sessionJoined && producerSessionId != 0) {
            producerSessionId = 0;
        } else {
            return SERVICE_STATUS_NOT_HANDLED;
        }
        AJ_Status status;
        switch (nextAction) {
        case ACKNOWLEDGE:
            status = ConsumerAcknowledgeNotification(bus, savedNotification.version, savedNotification.notificationId, savedNotification.originalSenderName, producerSessionId);
            AJ_Printf("HandleSessionStateChanged(): ConsumerAcknowledgeNotification returned status %s\n", AJ_StatusText(status));
            if (status == AJ_OK) {
                serviceStatus = SERVICE_STATUS_HANDLED;
            }
            if (producerSessionId != 0) {
                AJ_Printf("HandleSessionStateChanged(): Leaving session %u\n", producerSessionId);
                AJ_BusLeaveSession(bus, producerSessionId);
                producerSessionId = 0;
                lastSessionRequestSerialNum = 0;
            }
            nextAction = DISMISS;
            break;

        case DISMISS:
            status = ConsumerDismissNotification(bus, savedNotification.version, savedNotification.notificationId, savedNotification.appId, savedNotification.originalSenderName, producerSessionId);
            AJ_Printf("HandleSessionStateChanged(): ConsumerDismissNotification returned status %s\n", AJ_StatusText(status));
            if (status == AJ_OK) {
                serviceStatus = SERVICE_STATUS_HANDLED;
            }
            if (producerSessionId != 0) {
                AJ_Printf("HandleSessionStateChanged(): Leaving session %u\n", producerSessionId);
                AJ_BusLeaveSession(bus, producerSessionId);
                producerSessionId = 0;
                lastSessionRequestSerialNum = 0;
            }
            nextAction = NOTHING;
            break;

        default:
            break;
        }
        savedNotification.version = 0;
    }
    return serviceStatus;
}

Service_Status Consumer_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status* msgStatus)
{
    switch (msg->msgId) {
    case NOTIFICATION_SIGNAL_RECEIVED:
        AJ_Printf("Received Producer signal.\n");
        ConsumerNotifySignalHandler(msg);
        break;

    case SUPERAGENT_SIGNAL:
        AJ_Printf("Received Superagent Signal.\n");
        *msgStatus = ConsumerSetSignalRules(bus, superAgentMode, msg->sender);
        if (AJ_OK == *msgStatus) {
            ProxyObjects[NOTIFICATION_PROXYOBJECT_INDEX] = SuperAgentProxyObject;
        }
        ConsumerNotifySignalHandler(msg);
        break;

    case NOTIFICATION_DISMISSER_DISMISS_RECEIVED:
        AJ_Printf("Received Dismisser Dismiss signal.\n");
        ConsumerDismissSignalHandler(msg);
        break;

    case AJ_SIGNAL_LOST_ADV_NAME:
        if (superAgentMode && ConsumerIsSuperAgentLost(msg)) {
            *msgStatus = ConsumerSetSignalRules(bus, superAgentMode, NULL);
            if (AJ_OK == *msgStatus) {
                ProxyObjects[NOTIFICATION_PROXYOBJECT_INDEX] = AllProxyObject;
            }
        } else {
            return SERVICE_STATUS_NOT_HANDLED;
        }
        break;

    default:
        return SERVICE_STATUS_NOT_HANDLED;
    }
    return SERVICE_STATUS_HANDLED;
}

static AJ_Status Consumer_CreateSessionWithProducer(AJ_BusAttachment* bus, const char* senderName)
{
    AJ_Status status = AJ_OK;
    AJ_SessionOpts sessionOpts = {
        AJ_SESSION_TRAFFIC_MESSAGES,
        AJ_SESSION_PROXIMITY_ANY,
        AJ_TRANSPORT_ANY,
        FALSE
    };

    AJ_Printf("Inside CreateSessionWithProducer()\n");
    lastSessionRequestSerialNum = bus->serial;
    AJ_Printf("CreateSessionWithProducer(): Joining session with %s on port %u with serial number %u\n", senderName, NotificationProducerPort, lastSessionRequestSerialNum);
    status = AJ_BusJoinSession(bus, senderName, NotificationProducerPort, &sessionOpts);
    AJ_Printf("CreateSessionWithProducer(): AJ_BusJoinSession() returned with status %s\n", AJ_StatusText(status));
    return status;
}

void Consumer_IdleConnectedHandler(AJ_BusAttachment* bus)
{
    if (savedNotification.version > 1 && producerSessionId == 0) {
        switch (nextAction) {
        case ACKNOWLEDGE:
        case DISMISS:
            Consumer_CreateSessionWithProducer(bus, savedNotification.originalSenderName);
            AJ_Printf("IdleConnectedHandler(): session created for Action %u\n", (uint8_t)nextAction);
            break;

        case NOTHING:
            savedNotification.version = 0;
            nextAction = ACKNOWLEDGE;
            break;
        }
    }
    return;
}

void Consumer_Finish(AJ_BusAttachment* bus)
{
    //Service function stub
    return;
}

AJ_Status ApplicationHandleNotify(Notification_t* notification)
{
    AJ_Printf("******************** Begin New Message Received ********************\n");

    if (notification == 0) {
        AJ_Printf("Notification is NULL\n");
        return AJ_OK;
    }

    AJ_Printf("Message Id: %ld\nVersion: %u\nDevice Id: %s\nDevice Name: %s\nApp Id: %s\nApp Name: %s\nMessage Type: %d\n",
              notification->header.notificationId, notification->header.version, notification->header.deviceId, notification->header.deviceName, notification->header.appId, notification->header.appName, notification->header.messageType);
    int8_t indx;

    if (notification->header.originalSenderName != 0 && strlen(notification->header.originalSenderName) > 0) {
        AJ_Printf("OriginalSender bus unique name: %s\n", notification->header.originalSenderName);
    }

    for (indx = 0; indx < notification->content.numTexts; indx++) {
        AJ_Printf("Language: %s  Message: %s.\n", notification->content.texts[indx].key, notification->content.texts[indx].value);
    }

    AJ_Printf("Other parameters included:\n");
    for (indx = 0; indx < notification->content.numCustomAttributes; indx++) {
        AJ_Printf("Custom Attribute Key: %s  Custom Attribute Value: %s\n", notification->content.customAttributes[indx].key, notification->content.customAttributes[indx].value);
    }

    if (notification->content.richIconUrl != 0 && strlen(notification->content.richIconUrl) > 0) {
        AJ_Printf("Rich Content Icon Url: %s\n", notification->content.richIconUrl);
    }

    if (notification->content.numAudioUrls) {
        AJ_Printf("******************** Begin Rich Audio Content ********************\n");
        for (indx = 0; indx < notification->content.numAudioUrls; indx++) {
            AJ_Printf("Language: %s  Audio URL %s\n", notification->content.richAudioUrls[indx].key, notification->content.richAudioUrls[indx].value);
        }
        AJ_Printf("******************** End Rich Audio Content ********************\n");
    }

    if (notification->content.richIconObjectPath != 0 && strlen(notification->content.richIconObjectPath) > 0) {
        AJ_Printf("Rich Content Icon Object Path: %s\n", notification->content.richIconObjectPath);
    }

    if (notification->content.richAudioObjectPath != 0 && strlen(notification->content.richAudioObjectPath) > 0) {
        AJ_Printf("Rich Content Audio Object Path: %s\n", notification->content.richAudioObjectPath);
    }

    if (notification->content.controlPanelServiceObjectPath != 0 && strlen(notification->content.controlPanelServiceObjectPath) > 0) {
        AJ_Printf("ControlPanelService object path: %s\n", notification->content.controlPanelServiceObjectPath);
    }

    savedNotification.version = notification->header.version;
    savedNotification.notificationId = notification->header.notificationId;
    strcpy(savedNotification.appId, notification->header.appId);
    strcpy(savedNotification.originalSenderName, notification->header.originalSenderName);
    AJ_Printf("******************** End New Message Received ********************\n");

    return AJ_OK;
}

AJ_Status ApplicationHandleDismiss(int32_t notificationId, const char* appId)
{
    AJ_Printf("******************** Begin New Dismiss Received ********************\n");
    AJ_Printf("Notification Id: %ld\nApp Id: %s\n", notificationId, appId);
    if (savedNotification.version > 0 && !strcmp(appId, savedNotification.appId) && notificationId == savedNotification.notificationId) {
        AJ_Printf("Notification dimissed: Version %u sent from OriginalSender %s\n", savedNotification.version, savedNotification.originalSenderName);
        savedNotification.version = 0;
    }
    AJ_Printf("******************** End New Dismiss Received ********************\n");

    return AJ_OK;
}
