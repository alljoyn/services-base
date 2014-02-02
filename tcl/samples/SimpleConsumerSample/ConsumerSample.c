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
#define Consumer_SetupEnv(x, y) do { } while (0)
#define Consumer_GetActionFromUser(x) do { } while (0)
#endif

static uint8_t inputMode = FALSE;
static uint8_t superAgentMode = TRUE;
static uint8_t nextAction = CONSUMER_ACTION_NOTHING;
static uint8_t processingAction = FALSE;

static AJNS_Consumer_NotificationReference savedNotification;

static uint32_t producerSessionId = 0;
static uint32_t lastSessionRequestSerialNum = 0;

void Consumer_Init()
{
    Consumer_SetupEnv(&inputMode, &superAgentMode);
    AJ_Printf("Init(): Set Consumer to detect SuperAgent option is turned %s\n", superAgentMode ? "ON" : "off");
    ProxyObjects[NOTIFICATION_PROXYOBJECT_INDEX] = superAgentMode ? AllProxyObject : NotificationProxyObject;

    memset(&savedNotification, 0, sizeof(AJNS_Consumer_NotificationReference));
}

AJ_Status Consumer_ConnectedHandler(AJ_BusAttachment* busAttachment)
{
    return ConsumerSetSignalRules(busAttachment, superAgentMode, 0);
}

static void OnActionFinished()
{
    processingAction = FALSE;
    savedNotification.version = 0;
    producerSessionId = 0;
    lastSessionRequestSerialNum = 0;
    AJ_Printf("Action finished!\n");
}

static Service_Status Consumer_HandleSessionJoined(AJ_BusAttachment* busAttachment, uint32_t sessionId)
{
    AJ_Status status;
    Service_Status serviceStatus = SERVICE_STATUS_NOT_HANDLED;

    producerSessionId = sessionId;
    switch (nextAction) {
    case CONSUMER_ACTION_ACKNOWLEDGE:
        status = ConsumerAcknowledgeNotification(busAttachment, savedNotification.version, savedNotification.notificationId, savedNotification.originalSenderName, producerSessionId);
        AJ_Printf("HandleSessionJoined(): ConsumerAcknowledgeNotification returned status %s\n", AJ_StatusText(status));
        if (status == AJ_OK) {
            serviceStatus = SERVICE_STATUS_HANDLED;
        } else {
            OnActionFinished();
        }
        if (!inputMode) { // default behaviour is to dimiss next notification
            nextAction = CONSUMER_ACTION_DISMISS;
        }
        break;

    case CONSUMER_ACTION_DISMISS:
        status = ConsumerDismissNotification(busAttachment, savedNotification.version, savedNotification.notificationId, savedNotification.appId, savedNotification.originalSenderName, producerSessionId);
        AJ_Printf("HandleSessionJoined(): ConsumerDismissNotification returned status %s\n", AJ_StatusText(status));
        if (status == AJ_OK) {
            serviceStatus = SERVICE_STATUS_HANDLED;
        } else {
            OnActionFinished();
        }
        if (!inputMode) { // default behaviour is to do no op on next notification
            nextAction = CONSUMER_ACTION_NOTHING;
        }
        break;

    default:
        break;
    }

    return serviceStatus;
}

static Service_Status Consumer_HandleSessionRejected(AJ_BusAttachment* busAttachment)
{
    AJ_Status status;
    Service_Status serviceStatus = SERVICE_STATUS_NOT_HANDLED;

    producerSessionId = 0;
    switch (nextAction) {
    case CONSUMER_ACTION_ACKNOWLEDGE:
        serviceStatus = SERVICE_STATUS_HANDLED;
        OnActionFinished();
        if (!inputMode) { // default behaviour is to dimiss next notification
            nextAction = CONSUMER_ACTION_DISMISS;
        }
        break;

    case CONSUMER_ACTION_DISMISS:
        // We've failed to create a session with the originating Producer but we can tell all other Consumers to dismiss an already received notification
        status = ConsumerDismissNotification(busAttachment, savedNotification.version, savedNotification.notificationId, savedNotification.appId, savedNotification.originalSenderName, producerSessionId);
        AJ_Printf("HandleSessionRejected(): ConsumerDismissNotification returned status %s\n", AJ_StatusText(status));
        serviceStatus = SERVICE_STATUS_HANDLED;
        OnActionFinished();
        if (!inputMode) { // default behaviour is to do no op on next notification
            nextAction = CONSUMER_ACTION_NOTHING;
        }
        break;

    default:
        break;
    }

    return serviceStatus;
}

Service_Status Consumer_HandleSessionStateChanged(AJ_BusAttachment* busAttachment, uint32_t sessionId, uint8_t sessionJoined, uint32_t replySerialNum)
{
    Service_Status serviceStatus = SERVICE_STATUS_NOT_HANDLED;

    AJ_Printf("Inside HandleSessionStateChanged()\n");
    AJ_Printf("HandleSessionStateChanged(): Got reply that session with id %u was %s\n", sessionId, sessionJoined ? "joined" : "dropped");
    if (lastSessionRequestSerialNum != 0 && lastSessionRequestSerialNum == replySerialNum) { // Check if this is a reply to our request
        AJ_Printf("HandleSessionStateChanged(): Got reply serial number %u that matches last request serial number %u\n", replySerialNum, lastSessionRequestSerialNum);
        if (sessionJoined) { // Successful session join
            serviceStatus = Consumer_HandleSessionJoined(busAttachment, sessionId);
        } else { // Failed session join
            serviceStatus = Consumer_HandleSessionRejected(busAttachment);
        }
    }

    return serviceStatus;
}

Service_Status Consumer_MessageProcessor(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJ_Status* msgStatus)
{
    switch (msg->msgId) {
    case NOTIFICATION_SIGNAL_RECEIVED:
        AJ_Printf("Received Producer signal.\n");
        ConsumerNotifySignalHandler(msg);
        break;

    case SUPERAGENT_SIGNAL:
        AJ_Printf("Received Superagent Signal.\n");
        *msgStatus = ConsumerSetSignalRules(busAttachment, superAgentMode, msg->sender);
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
            *msgStatus = ConsumerSetSignalRules(busAttachment, superAgentMode, NULL);
            if (AJ_OK == *msgStatus) {
                ProxyObjects[NOTIFICATION_PROXYOBJECT_INDEX] = AllProxyObject;
            }
        } else {
            return SERVICE_STATUS_NOT_HANDLED;
        }
        break;

    case AJ_REPLY_ID(NOTIFICATION_PRODUCER_ACKNOWLEDGE_PROXY):
    case AJ_REPLY_ID(NOTIFICATION_PRODUCER_DISMISS_PROXY):
        if (producerSessionId != 0) {
            AJ_Printf("NotificationProducer method replied. Leaving session %u\n", producerSessionId);
            *msgStatus = AJ_BusLeaveSession(busAttachment, producerSessionId);
            if (AJ_OK != *msgStatus) {
                AJ_Printf("Failed to leave session %u\n", producerSessionId);
            }
            OnActionFinished();
        }
        break;

    default:
        return SERVICE_STATUS_NOT_HANDLED;
    }
    return SERVICE_STATUS_HANDLED;
}

static AJ_Status Consumer_CreateSessionWithProducer(AJ_BusAttachment* busAttachment, const char* senderName)
{
    AJ_Status status = AJ_OK;
    AJ_SessionOpts sessionOpts = {
        AJ_SESSION_TRAFFIC_MESSAGES,
        AJ_SESSION_PROXIMITY_ANY,
        AJ_TRANSPORT_ANY,
        FALSE
    };

    AJ_Printf("Inside CreateSessionWithProducer()\n");
    lastSessionRequestSerialNum = busAttachment->serial;
    AJ_Printf("CreateSessionWithProducer(): Joining session with %s on port %u with serial number %u\n", senderName, NotificationProducerPort, lastSessionRequestSerialNum);
    status = AJ_BusJoinSession(busAttachment, senderName, NotificationProducerPort, &sessionOpts);
    AJ_Printf("CreateSessionWithProducer(): AJ_BusJoinSession() returned with status %s\n", AJ_StatusText(status));
    return status;
}

/**
 * Meant to simulate scenario where an Action is set when a new Notification is received.
 */
static void Consumer_DoAction(AJ_BusAttachment* busAttachment)
{
    AJ_Status status;
    if (inputMode) {
        Consumer_GetActionFromUser(&nextAction);
        AJ_Printf("Action received is %u\n", nextAction);
    }
    processingAction = TRUE;
    switch (nextAction) {
    case CONSUMER_ACTION_ACKNOWLEDGE:
        status = Consumer_CreateSessionWithProducer(busAttachment, savedNotification.originalSenderName);
        if (AJ_OK != status) {
            AJ_Printf("DoAction(): session failed to create for Action %u\n", nextAction);
            OnActionFinished();
        } else {
            AJ_Printf("DoAction(): session created for Action %u\n", nextAction);
        }
        break;

    case CONSUMER_ACTION_DISMISS:
        status = Consumer_CreateSessionWithProducer(busAttachment, savedNotification.originalSenderName);
        if (AJ_OK != status) {
            AJ_Printf("DoAction(): session failed to create for Action %u\n", nextAction);
            status = ConsumerDismissNotification(busAttachment, savedNotification.version, savedNotification.notificationId, savedNotification.appId, savedNotification.originalSenderName, producerSessionId);
            if (AJ_OK != status) {
                AJ_Printf("DoAction(): Action Dismiss failed with status %s\n", AJ_StatusText(status));
            }
            if (!inputMode) { // default behaviour is to do no op on next notification
                nextAction = CONSUMER_ACTION_NOTHING;
            }
            OnActionFinished();
        } else {
            AJ_Printf("DoAction(): session created for Action %u\n", nextAction);
        }
        break;

    case CONSUMER_ACTION_NOTHING:
        if (!inputMode) { // default behaviour is to acknowledge next notification
            nextAction = CONSUMER_ACTION_ACKNOWLEDGE;
        }
        OnActionFinished();
        break;
    }
}

void Consumer_IdleConnectedHandler(AJ_BusAttachment* busAttachment)
{
    if (savedNotification.version > 1 && !processingAction) {
        Consumer_DoAction(busAttachment);
    }
    return;
}

void Consumer_Finish(AJ_BusAttachment* busAttachment)
{
    return;
}

AJ_Status ApplicationHandleNotify(AJNS_Notification* notification)
{
    AJ_Printf("******************** Begin New Message Received ********************\n");

    if (notification == 0) {
        AJ_Printf("Notification is NULL\n");
        return AJ_OK;
    }

    AJ_Printf("Message Id: %d\nVersion: %u\nDevice Id: %s\nDevice Name: %s\nApp Id: %s\nApp Name: %s\nMessage Type: %d\n",
              notification->notificationId, notification->version, notification->deviceId, notification->deviceName, notification->appId, notification->appName, notification->messageType);
    int8_t indx;

    if (notification->originalSenderName != 0 && strlen(notification->originalSenderName) > 0) {
        AJ_Printf("OriginalSender bus unique name: %s\n", notification->originalSenderName);
    }

    for (indx = 0; indx < notification->content->numTexts; indx++) {
        AJ_Printf("Language: %s  Message: %s.\n", notification->content->texts[indx].key, notification->content->texts[indx].value);
    }

    AJ_Printf("Other parameters included:\n");
    for (indx = 0; indx < notification->content->numCustomAttributes; indx++) {
        AJ_Printf("Custom Attribute Key: %s  Custom Attribute Value: %s\n", notification->content->customAttributes[indx].key, notification->content->customAttributes[indx].value);
    }

    if (notification->content->richIconUrl != 0 && strlen(notification->content->richIconUrl) > 0) {
        AJ_Printf("Rich Content Icon Url: %s\n", notification->content->richIconUrl);
    }

    if (notification->content->numAudioUrls) {
        AJ_Printf("******************** Begin Rich Audio Content ********************\n");
        for (indx = 0; indx < notification->content->numAudioUrls; indx++) {
            AJ_Printf("Language: %s  Audio URL %s\n", notification->content->richAudioUrls[indx].key, notification->content->richAudioUrls[indx].value);
        }
        AJ_Printf("******************** End Rich Audio Content ********************\n");
    }

    if (notification->content->richIconObjectPath != 0 && strlen(notification->content->richIconObjectPath) > 0) {
        AJ_Printf("Rich Content Icon Object Path: %s\n", notification->content->richIconObjectPath);
    }

    if (notification->content->richAudioObjectPath != 0 && strlen(notification->content->richAudioObjectPath) > 0) {
        AJ_Printf("Rich Content Audio Object Path: %s\n", notification->content->richAudioObjectPath);
    }

    if (notification->content->controlPanelServiceObjectPath != 0 && strlen(notification->content->controlPanelServiceObjectPath) > 0) {
        AJ_Printf("ControlPanelService object path: %s\n", notification->content->controlPanelServiceObjectPath);
    }

    // Check if received notification is from a producer that supports acknowledge and dismiss methods
    if (notification->version >= NotificationVersion && notification->originalSenderName != 0 && strlen(notification->originalSenderName) > 0 && processingAction == FALSE) {
        // Save notification reference so that it can be later acknowledged or dimissed
        savedNotification.version = notification->version;
        savedNotification.notificationId = notification->notificationId;
        strncpy(savedNotification.appId, notification->appId, sizeof(savedNotification.appId) - 1);
        strncpy(savedNotification.originalSenderName, notification->originalSenderName, sizeof(savedNotification.originalSenderName) - 1);
    }
    AJ_Printf("******************** End New Message Received ********************\n");

    return AJ_OK;
}

AJ_Status ApplicationHandleDismiss(int32_t notificationId, const char* appId)
{
    AJ_Printf("******************** Begin New Dismiss Received ********************\n");
    AJ_Printf("Notification Id: %d\nApp Id: %s\n", notificationId, appId);
    if (savedNotification.version > 0 && !strcmp(appId, savedNotification.appId) && notificationId == savedNotification.notificationId) {
        AJ_Printf("Notification dimissed: Version %u sent from OriginalSender %s\n", savedNotification.version, savedNotification.originalSenderName);
        if (processingAction == FALSE) {
            savedNotification.version = 0;
        }
    }
    AJ_Printf("******************** End New Dismiss Received ********************\n");

    return AJ_OK;
}
