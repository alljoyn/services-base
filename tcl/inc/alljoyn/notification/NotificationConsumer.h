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

#ifndef NOTIFICATIONCONSUMER_H_
#define NOTIFICATIONCONSUMER_H_

#include <alljoyn/notification/common.h>
#include <alljoyn/services_common/Services_Common.h>

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

#define NUM_NOTIFICATION_CONSUMER_OBJECTS 1

#define NOTIFICATION_CONSUMER_APPOBJECTS  { "*",   NotificationInterfaces },

#define NOTIFICATION_SIGNAL    AJ_PRX_MESSAGE_ID(0 + NUM_PRE_NOTIFICATION_CONSUMER_OBJECTS, 1, 0)
#define SUPERAGENT_SIGNAL      AJ_PRX_MESSAGE_ID(0 + NUM_PRE_NOTIFICATION_CONSUMER_OBJECTS, 2, 0)

#define INTERFACE_GET_PROPERTY AJ_APP_MESSAGE_ID(0 + NUM_PRE_NOTIFICATION_CONSUMER_OBJECTS, 0, AJ_PROP_GET)
#define INTERFACE_SET_PROPERTY AJ_APP_MESSAGE_ID(0 + NUM_PRE_NOTIFICATION_CONSUMER_OBJECTS, 0, AJ_PROP_SET)

#define GET_NOTIFICATION_VERSION_PROPERTY AJ_APP_PROPERTY_ID(0 + NUM_PRE_NOTIFICATION_CONSUMER_OBJECTS, 1, 1)
#define GET_SUPERAGENT_VERSION_PROPERTY   AJ_APP_PROPERTY_ID(0 + NUM_PRE_NOTIFICATION_CONSUMER_OBJECTS, 2, 1)

#define NUMALLOWEDCUSTOMATTRIBUTES 10
#define NUMALLOWEDTEXTS         10
#define NUMALLOWEDRICHNOTS      10

#define UUID_LENGTH 16

extern const AJ_InterfaceDescription SuperagentInterfaces[];
extern const AJ_InterfaceDescription AllInterfaces[];

extern const AJ_Object AllAppObject;
extern const AJ_Object SuperAgentObject;
extern const AJ_Object NotificationAppObject;

/**
 * ConsumerSetSignalRules, to add the correct filter for the required interface
 * @param bus
 * @param superAgentMode
 * @param senderBusName
 * @return status
 */
extern AJ_Status ConsumerSetSignalRules(AJ_BusAttachment* bus, int8_t superAgentMode, const char* senderBusName);

/**
 * ConsumerHandleNotifyMsgObj - receives message, unmarshals it and calls handleNotify
 * @param msg
 * @return
 */
extern AJ_Status ConsumerHandleNotifyMsgObj(AJ_Message* msg);

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
 * HandleNotify implemented by the application
 * @param protoVer
 * @param messageId
 * @param notificationContent
 * @param deviceId
 * @param deviceName
 * @param appId
 * @param appName
 * @return status success/failure
 */
extern AJ_Status HandleNotify(uint16_t protoVer,
                              uint32_t messageId,
                              notification*notificationContent,
                              const char* deviceId,
                              const char* deviceName,
                              const char* appId,
                              const char* appName);

#endif /* NOTIFICATIONCONSUMER_H_ */


