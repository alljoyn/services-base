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

/*! \addtogroup PRODUCER Notification Producer
 *
 * @{
 * \details Functions and variables that assist in writing Notification Producers
 * \example ProducerSample.c
 */

#ifndef NOTIFICATIONPRODUCER_H_
#define NOTIFICATIONPRODUCER_H_

#include <alljoyn/notification/common.h>
#include <alljoyn/services_common/Services_Common.h>

/*!
   \brief Constant for messageType  parameter to \ref SetNotification
 */
#define EMERGENCY 0

/*!
   \brief Constant for messageType  parameter to \ref SetNotification
 */
#define WARNING   1

/*!
   \brief Constant for messageType  parameter to \ref SetNotification
 */
#define INFO      2

/*!
   \brief Constant for about of notification producer objects
 */
#define NUM_NOTIFICATION_PRODUCER_OBJECTS 4

/*!
   \brief Get Property event for Emergency NotificationService object
   \details
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
 */
#define EMERGENCY_NOTIFICATION_GET_PROPERTY AJ_APP_MESSAGE_ID(0 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 0, AJ_PROP_GET)

/*!
   \brief Get Property event for Warning NotificationService object
   \details
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
 */
#define WARNING_NOTIFICATION_GET_PROPERTY   AJ_APP_MESSAGE_ID(1 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 0, AJ_PROP_GET)

/*!
   \brief Get Property event for Info NotificationService object
   \details
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
 */
#define INFO_NOTIFICATION_GET_PROPERTY      AJ_APP_MESSAGE_ID(2 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 0, AJ_PROP_GET)

/*!
   \brief Set Property event for Emergency NotificationService object
   \details
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
 */
#define EMERGENCY_NOTIFICATION_SET_PROPERTY AJ_APP_MESSAGE_ID(0 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 0, AJ_PROP_SET)

/*!
   \brief Set Property event for Warning NotificationService object
   \details
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
 */
#define WARNING_NOTIFICATION_SET_PROPERTY   AJ_APP_MESSAGE_ID(1 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 0, AJ_PROP_SET)

/*!
   \brief Set Property event for Info NotificationService object
   \details
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
 */
#define INFO_NOTIFICATION_SET_PROPERTY      AJ_APP_MESSAGE_ID(2 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 0, AJ_PROP_SET)

/*!
   \brief Get Version event for Emergency NotificationService object
   \details
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
 */
#define GET_EMERGENCY_NOTIFICATION_VERSION_PROPERTY AJ_APP_PROPERTY_ID(0 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 1, 1)

/*!
   \brief Get Version event for Warning NotificationService object
   \details
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
 */
#define GET_WARNING_NOTIFICATION_VERSION_PROPERTY   AJ_APP_PROPERTY_ID(1 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 1, 1)

/*!
   \brief Get Version event for Info NotificationService object
   \details
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
 */
#define GET_INFO_NOTIFICATION_VERSION_PROPERTY      AJ_APP_PROPERTY_ID(2 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS, 1, 1)

/*!
   \brief The object path on which "Emergency" type notifications are sent
 */
extern const char ServicePathEmergency[];

/*!
   \brief The object path on which "warning" type notifications are sent
 */
extern const char ServicePathWarning[];

/*!
   \brief The object path on which "Info" type notifications are sent
 */
extern const char ServicePathInfo[];

/* Producer Object bus registration */
#define NOTIFICATION_PRODUCER_OBJECT_INDEX 3 + NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS
#define NOTIFICATION_PRODUCER_GET_PROPERTY AJ_APP_PROPERTY_ID(NOTIFICATION_PRODUCER_OBJECT_INDEX, 0, AJ_PROP_GET)
#define NOTIFICATION_PRODUCER_SET_PROPERTY AJ_APP_PROPERTY_ID(NOTIFICATION_PRODUCER_OBJECT_INDEX, 0, AJ_PROP_SET)

#define NOTIFICATION_PRODUCER_ACKNOWLEDGE               AJ_APP_MESSAGE_ID(NOTIFICATION_PRODUCER_OBJECT_INDEX, 1, 0)
#define NOTIFICATION_PRODUCER_DISMISS                   AJ_APP_MESSAGE_ID(NOTIFICATION_PRODUCER_OBJECT_INDEX, 1, 1)
#define GET_NOTIFICATION_PRODUCER_VERSION_PROPERTY      AJ_APP_PROPERTY_ID(NOTIFICATION_PRODUCER_OBJECT_INDEX, 1, 2)

/*!
   \brief AllJoyn objects exposed by the NotificationProducer
 */
#define NOTIFICATION_PRODUCER_APPOBJECTS \
    { ServicePathEmergency,             NotificationInterfaces }, \
    { ServicePathWarning,               NotificationInterfaces }, \
    { ServicePathInfo,                  NotificationInterfaces }, \
    { NotificationProducerObjectPath,   NotificationProducerInterfaces },

/*!
   \brief AllJoyn objects announced by the NotificationProducer
 */
#define NOTIFICATION_PRODUCER_ANNOUNCEOBJECTS  \
    { ServicePathEmergency,             NotificationInterfaces }, \
    { ServicePathWarning,               NotificationInterfaces }, \
    { ServicePathInfo,                  NotificationInterfaces }, \
    { NotificationProducerObjectPath,   NotificationProducerInterfaces },

/*!
   \brief Prepare a notification for sending. Call \ref SendNotifications to actually send the notification after calling this function
   \param notificationContent The content of the notification to be set
   \return AJ_Status
 */
extern AJ_Status ProducerSetNotification(NotificationContent_t* notificationContent, uint16_t messageType, uint16_t ttl);

/*!
   \brief Send the notifications previously set with \ref SetNotification
   \return AJ_Status
 */
extern AJ_Status ProducerSendNotifications();

/*!
   \brief Instruct the AllJoyn bus to remove last message of the specified message type from the bus
   \details
   Effectively, this overrides the ttl parameter in the \ref SetNotification function
   \param messageType One of \ref INFO, \ref WARNING, or \ref EMERGENCY
   \return AJ_Status
 */
extern AJ_Status ProducerDeleteLastMsg(uint16_t messageType);

/*!
   \brief Implementation of Acknowledge functionality removing given message from the bus
   \details
   Effectively, this overrides the ttl parameter in the \ref SetNotification function
   \param msg The received Acknowledge request message to process
   \return AJ_Status
 */
extern AJ_Status ProducerAcknowledgeMsg(AJ_Message*msg);

/*!
   \brief Implementation of Dismiss functionality acknowledging and sending a Dismiss SSL to recall received message
   \details
   Effectively, this overrides the ttl parameter in the \ref SetNotification function
   \param msg The received Dismiss request message to process
   \return AJ_Status
 */
extern AJ_Status ProducerDismissMsg(AJ_Message*msg);

/*!
   \brief Implementation of GetProperty functionality for the notification objects
   \details
   Use this function to respond to Get Property events as a callback function passed to AJ_BusPropGet
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
   \param replyMsg The reply message that will be sent
   \param propId The identifier of the property field
   \param context For internal use
   \return AJ_Status
 */
extern AJ_Status ProducerPropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/*!
   \brief Implementation of SetProperty functionality for the notification objects
   \details
   Use this function to respond to Set Property events as a callback function passed to AJ_BusPropSet
   \dontinclude ProducerSample.c
   \skip switch (msg->msgId)
   \until }
   \param replyMsg The reply message that will be sent
   \param propId The identifier of the property field
   \param context For internal use
   \return AJ_Status
 */
extern AJ_Status ProducerPropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

#endif /* NOTIFICATIONPRODUCER_H_ */
