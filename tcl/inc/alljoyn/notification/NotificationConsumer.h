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

#include <alljoyn/services_common/Services_Common.h>
#include <alljoyn/notification/NotificationCommon.h>

/* Allowed limits on message content */
#define NUMALLOWEDCUSTOMATTRIBUTES 10
#define NUMALLOWEDTEXTS         10
#define NUMALLOWEDRICHNOTS      10

/* Notification ProxyObject bus registration */
#define NOTIFICATION_PROXYOBJECT_INDEX 0 + NUM_PRE_NOTIFICATION_CONSUMER_PROXYOBJECTS
#define INTERFACE_GET_PROPERTY_PROXY                          AJ_PRX_MESSAGE_ID(NOTIFICATION_PROXYOBJECT_INDEX, 0, AJ_PROP_GET)
#define INTERFACE_SET_PROPERTY_PROXY                          AJ_PRX_MESSAGE_ID(NOTIFICATION_PROXYOBJECT_INDEX, 0, AJ_PROP_SET)

#define NOTIFICATION_SIGNAL                                   AJ_PRX_MESSAGE_ID(NOTIFICATION_PROXYOBJECT_INDEX, 1, 0)
#define GET_NOTIFICATION_VERSION_PROPERTY_PROXY               AJ_PRX_PROPERTY_ID(NOTIFICATION_PROXYOBJECT_INDEX, 1, 1)

#define SUPERAGENT_SIGNAL                                     AJ_PRX_MESSAGE_ID(NOTIFICATION_PROXYOBJECT_INDEX, 2, 0)
#define GET_SUPERAGENT_VERSION_PROPERTY_PROXY                 AJ_PRX_PROPERTY_ID(NOTIFICATION_PROXYOBJECT_INDEX, 2, 1)

/* Producer ProxyObject bus registration */
#define NOTIFICATION_PRODUCER_PROXYOBJECT_INDEX 1 + NUM_PRE_NOTIFICATION_CONSUMER_PROXYOBJECTS
#define NOTIFICATION_PRODUCER_GET_PROPERTY_PROXY              AJ_PRX_MESSAGE_ID(NOTIFICATION_PRODUCER_PROXYOBJECT_INDEX, 0, AJ_PROP_GET)
#define NOTIFICATION_PRODUCER_SET_PROPERTY_PROXY              AJ_PRX_MESSAGE_ID(NOTIFICATION_PRODUCER_PROXYOBJECT_INDEX, 0, AJ_PROP_SET)

#define NOTIFICATION_PRODUCER_ACKNOWLEDGE_PROXY               AJ_PRX_MESSAGE_ID(NOTIFICATION_PRODUCER_PROXYOBJECT_INDEX, 1, 0)
#define NOTIFICATION_PRODUCER_DISMISS_PROXY                   AJ_PRX_MESSAGE_ID(NOTIFICATION_PRODUCER_PROXYOBJECT_INDEX, 1, 1)
#define GET_NOTIFICATION_PRODUCER_VERSION_PROPERTY_PROXY      AJ_PRX_PROPERTY_ID(NOTIFICATION_PRODUCER_PROXYOBJECT_INDEX, 1, 2)

/* Dismisser ProxyObject bus registration */
#define NOTIFICATION_DISMISSER_PROXYOBJECT_INDEX 2 + NUM_PRE_NOTIFICATION_CONSUMER_PROXYOBJECTS
#define NOTIFICATION_DISMISSER_GET_PROPERTY_PROXY             AJ_PRX_MESSAGE_ID(NOTIFICATION_DISMISSER_PROXYOBJECT_INDEX, 0, AJ_PROP_GET)
#define NOTIFICATION_DISMISSER_SET_PROPERTY_PROXY             AJ_PRX_MESSAGE_ID(NOTIFICATION_DISMISSER_PROXYOBJECT_INDEX, 0, AJ_PROP_SET)

#define NOTIFICATION_DISMISSER_DISMISS_RECEIVED               AJ_PRX_MESSAGE_ID(NOTIFICATION_DISMISSER_PROXYOBJECT_INDEX, 1, 0)
#define GET_NOTIFICATION_DISMISSER_VERSION_PROPERTY_PROXY     AJ_PRX_PROPERTY_ID(NOTIFICATION_PRODUCER_PROXYOBJECT_INDEX, 1, 1)

#ifndef NOTIFICATION_SERVICE_PRODUCER
   #define NOTIFICATION_SIGNAL_RECEIVED NOTIFICATION_SIGNAL
#else
   #define NOTIFICATION_SIGNAL_PROD1    AJ_APP_MESSAGE_ID(0 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 1, 0)
   #define NOTIFICATION_SIGNAL_PROD2    AJ_APP_MESSAGE_ID(1 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 1, 0)
   #define NOTIFICATION_SIGNAL_PROD3    AJ_APP_MESSAGE_ID(2 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 1, 0)

   #define NOTIFICATION_SIGNAL_RECEIVED NOTIFICATION_SIGNAL : \
case NOTIFICATION_SIGNAL_PROD1 : \
case NOTIFICATION_SIGNAL_PROD2 : \
case NOTIFICATION_SIGNAL_PROD3
#endif

#define NUM_NOTIFICATION_CONSUMER_PROXYOBJECTS 3

#define NOTIFICATION_CONSUMER_PROXYOBJECTS  \
    { "*",   NotificationInterfaces }, \
    { NotificationProducerObjectPath,   NotificationProducerInterfaces }, \
    { "*",  NotificationDismisserInterfaces },

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
 * ConsumerSetSignalRules, to add the correct filter for the required interface
 * @param bus
 * @param superAgentMode
 * @param senderBusName
 * @return status
 */
extern AJ_Status ConsumerSetSignalRules(AJ_BusAttachment* busAttachment, uint8_t superAgentMode, const char* senderBusName);

/**
 * ConsumerIsSuperAgentLost, checks whether the lost device/app is the SuperAgent
 * @param msg LOST_ADVERTISED_NAME message to process
 * @return Whether the lost device/app is the SuperAgent
 */
extern uint8_t ConsumerIsSuperAgentLost(AJ_Message* msg);

/**
 * ConsumerNotifySignalHandler - receives message, unmarshals it and calls handleNotify
 * @param msg
 * @return status
 */
extern AJ_Status ConsumerNotifySignalHandler(AJ_Message* msg);

/**
 * ConsumerDismissSignalHandler - receives message, unmarshals it and calls handleDismiss
 * @param msg
 * @return status
 */
extern AJ_Status ConsumerDismissSignalHandler(AJ_Message* msg);

/**
 * ConsumerAcknowledgeNotification - send an acknowledment request to the producer of the given message, marshals the methodcall and delivers it
 * @param busAttachment
 * @param version       the message version
 * @param msdId         the message id
 * @param senderName    the bus unique name of the message sender
 * @param sessionId     the session id for the proxy object
 * @return status
 */
extern AJ_Status ConsumerAcknowledgeNotification(AJ_BusAttachment* busAttachment, uint16_t version, int32_t msgId, const char* senderName, uint32_t sessionId);

/**
 * ConsumerDismissNotification - send a dismissal request to the producer of the given message, marshals the methodcall and delivers it
 * @param busAttachment
 * @param version       the message version
 * @param msdId         the message id
 * @param appId         the application id of the message sender application
 * @param senderName    the bus unique name of the message sender
 * @param sessionId     the session id for the proxy object
 * @return status
 */
extern AJ_Status ConsumerDismissNotification(AJ_BusAttachment* busAttachment, uint16_t version, int32_t msgId, const char* appId, const char* senderName, uint32_t sessionId);

/**
 * ConsumerPropGetHandler - handle get property call
 * @param replyMsg
 * @param propId
 * @param context
 * @return the Alljoyn status
 */
extern AJ_Status ConsumerPropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * ConsumerPropSetHandler - handle set property call
 * @param replyMsg
 * @param propId
 * @param context
 * @return the Alljoyn status
 */
extern AJ_Status ConsumerPropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * ApplicationHandleNotify implemented by the application
 * @param notification header and content
 * @return status success/failure
 */
extern AJ_Status ApplicationHandleNotify(AJNS_Notification* notification);

/**
 * ApplicationHandleDismiss implemented by the application
 * @param notificationId the notification id of the notification to dismiss
 * @param appId          the application id of the original sender of the notification to dismiss
 * @return status success/failure
 */
extern AJ_Status ApplicationHandleDismiss(int32_t notificationId, const char* appId);

#endif /* NOTIFICATIONCONSUMER_H_ */


