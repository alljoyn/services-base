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

#ifndef NOTIFICATIONCONSUMER_H_
#define NOTIFICATIONCONSUMER_H_

#include <alljoyn.h>
#include <alljoyn/services_common/Services_Common.h>
#include <alljoyn/notification/NotificationCommon.h>

/* Allowed limits on message content */
#define NUMALLOWEDCUSTOMATTRIBUTES      10
#define NUMALLOWEDTEXTS                 10
#define NUMALLOWEDRICHNOTS              10

#define NUM_NOTIFICATION_CONSUMER_PROXYOBJECTS 3

#define NOTIFICATION_CONSUMER_PROXYOBJECTS  \
    { "*",   AJNS_NotificationInterfaces }, \
    { AJNS_NotificationProducerObjectPath,   AJNS_NotificationProducerInterfaces }, \
    { "*",  AJNS_NotificationDismisserInterfaces },

extern const AJ_InterfaceDescription SuperagentInterfaces[];
extern const AJ_InterfaceDescription AllInterfaces[];

extern const AJ_Object AllProxyObject;
extern const AJ_Object SuperAgentProxyObject;
extern const AJ_Object NotificationProxyObject;

typedef struct _AJNS_Consumer_NotificationReference {
    uint16_t version;
    int32_t notificationId;
    char appId[UUID_LENGTH * 2 + 1];
    char originalSenderName[16];
} AJNS_Consumer_NotificationReference;

/**
 * OnNotify implemented by the application
 * @param notification header and content
 * @return status success/failure
 */
typedef AJ_Status (*AJNS_Consumer_OnNotify)(AJNS_Notification* notification);

/**
 * OnDismiss implemented by the application
 * @param notificationId the notification id of the notification to dismiss
 * @param appId          the application id of the original sender of the notification to dismiss
 * @return status success/failure
 */
typedef AJ_Status (*AJNS_Consumer_OnDismiss)(int32_t notificationId, const char* appId);

/**
 * Start Notification service framework Consumer passing mode and callbacks
 * @param superAgentMode
 * @param proxyObjects
 * @param onNotify
 * @param isValueValid
 * @return onDismiss
 */
AJ_Status AJNS_Consumer_Start(uint8_t appSuperAgentMode, AJ_Object* proxyObjects, AJNS_Consumer_OnNotify appOnNotify, const AJNS_Consumer_OnDismiss appOnDismiss);

/**
 * Consumer_SetSignalRules, to add the correct filter for the required interface
 * @param bus
 * @param superAgentMode
 * @param senderBusName
 * @return status
 */
AJ_Status AJNS_Consumer_SetSignalRules(AJ_BusAttachment* busAttachment, uint8_t superAgentMode, const char* senderBusName);

/**
 * Consumer_IsSuperAgentLost, checks whether the lost device/app is the SuperAgent
 * @param msg LOST_ADVERTISED_NAME message to process
 * @return Whether the lost device/app is the SuperAgent
 */
uint8_t AJNS_Consumer_IsSuperAgentLost(AJ_Message* msg);

/**
 * Consumer_NotifySignalHandler - receives message, unmarshals it and calls handleNotify
 * @param msg
 * @return status
 */
AJ_Status AJNS_Consumer_NotifySignalHandler(AJ_Message* msg);

/**
 * Consumer_DismissSignalHandler - receives message, unmarshals it and calls handleDismiss
 * @param msg
 * @return status
 */
AJ_Status AJNS_Consumer_DismissSignalHandler(AJ_Message* msg);

/**
 * Consumer_DismissNotification - send a dismissal request to the producer of the given message, marshals the methodcall and delivers it
 * @param busAttachment
 * @param version               the message version
 * @param msdId                 the message id
 * @param appId                 the application id of the message sender application
 * @param senderName            the bus unique name of the message sender
 * @param completedCallback     the callback to call when the Dismiss method reply is received
 * @return status
 */
AJ_Status AJNS_Consumer_DismissNotification(AJ_BusAttachment* busAttachment, uint16_t version, int32_t notificationId, const char* appId, const char* senderName, AJSVC_MethodCallCompleted completedCallback);

/**
 * Consumer_PropGetHandler - handle get property call
 * @param replyMsg
 * @param propId
 * @param context
 * @return the Alljoyn status
 */
AJ_Status AJNS_Consumer_PropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * Consumer_PropSetHandler - handle set property call
 * @param replyMsg
 * @param propId
 * @param context
 * @return the Alljoyn status
 */
AJ_Status AJNS_Consumer_PropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * Function called after service connects
 * @param busAttachment
 * @return the Alljoyn status
 */
AJ_Status AJNS_Consumer_ConnectedHandler(AJ_BusAttachment* busAttachment);

/**
 * Process the message received
 * @param busAttachment
 * @param msg
 * @param msgStatus
 * @return status - was message handled
 */
AJSVC_ServiceStatus AJNS_Consumer_MessageProcessor(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJ_Status* msgStatus);

/**
 * Handle failed session join
 * @param busAttachment
 * @param sessionId
 * @param replySerialNum
 * @return status - was message handled
 */
AJSVC_ServiceStatus AJNS_Consumer_SessionJoinedHandler(AJ_BusAttachment* busAttachment, uint32_t sessionId, uint32_t replySerialNum);

/**
 * Handle successful session join
 * @param busAttachment
 * @param replySerialNum
 * @param replyCode
 * @return status - was message handled
 */
AJSVC_ServiceStatus AJNS_Consumer_SessionRejectedHandler(AJ_BusAttachment* busAttachment, uint32_t replySerialNum, uint32_t replyCode);

/**
 * Handle session loss
 * @param busAttachment
 * @param sessionId
 * @param reason
 * @return status - was message handled
 */
AJSVC_ServiceStatus AJNS_Consumer_SessionLostHandler(AJ_BusAttachment* busAttachment, uint32_t sessionId, uint32_t reason);

/**
 * Function called after service disconnects
 * @param busAttachment
 * @return the Alljoyn status
 */
AJ_Status AJNS_Consumer_DisconnectHandler(AJ_BusAttachment* busAttachment);

#endif /* NOTIFICATIONCONSUMER_H_ */
