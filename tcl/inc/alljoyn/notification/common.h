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

#ifndef COMMON_H_
#define COMMON_H_

#include <alljoyn.h>

/**
 * Definitions
 */
#define RICH_CONTENT_ICON_URL_ATTRIBUTE_KEY             0
#define RICH_CONTENT_AUDIO_URL_ATTRIBUTE_KEY            1
#define RICH_CONTENT_ICON_OBJECT_PATH_ATTRIBUTE_KEY     2
#define RICH_CONTENT_AUDIO_OBJECT_PATH_ATTRIBUTE_KEY    3
#define CONTROLPANELSERVICE_OBJECT_PATH_ATTRIBUTE_KEY   4
#define ORIGINAL_SENDER_NAME_ATTRIBUTE_KEY              5

#define NUM_MESSAGE_TYPES 3

/**
 * Generic structure for key=value
 */
struct keyValue {
    const char* key;
    const char* value;
};

/*!
   \brief struct that holds the notification header
 */
typedef struct _NotificationHeader {
    uint16_t version;                           /**< version of notification */
    uint16_t messageType;                       /**< messageType One of \ref INFO, \ref WARNING, or \ref EMERGENCY */
    int32_t notificationId;                     /**< notification message id */
    const char* originalSenderName;             /**< originalSenderName The AllJoyn bus unique name of the originating producer application */
    const char* deviceId;                       /**< device id of originating producer application */
    const char* deviceName;                     /**< device name of originating producer application */
    const char* appId;                          /**< application id of originating producer application */
    const char* appName;                        /**< application name of originating producer application */
} NotificationHeader_t;

/*!
   \brief struct that holds the notification content
 */
typedef struct _NotificationContent {
    int8_t numCustomAttributes;                 /**< numCustomAttributes number of custom Attributs */
    struct keyValue* customAttributes;           /**< customAttributes Custom attributs */
    int8_t numTexts;                            /**< numTexts number of versions of the notification text */
    struct keyValue* texts;                      /**< texts The text of the notification, one entry per language */
    int8_t numAudioUrls;                        /**< numAudioUrls The number of audio URLs sent */
    struct keyValue* richAudioUrls;              /**< richAudioUrls The audio URLs, one per language */
    const char* richIconUrl;                    /**< richIconUrl A URL for an icon to be displayed along with the notification */
    const char* richIconObjectPath;             /**< richIconObjectPath The AllJoyn object path of an accompanying icons object */
    const char* richAudioObjectPath;            /**< richAudioObjectPath The AllJoyn object path of an accompanying audio object */
    const char* controlPanelServiceObjectPath;  /**< controlPanelServiceObjectPath The AllJoyn object path of an accompanying Control Panel Service object */
    const char* originalSenderName;             /**< originalSenderName The AllJoyn bus unique name of the originating producer application */
} NotificationContent_t;

/*!
   \brief struct that holds the notification (header + content)
 */
typedef struct _Notification {
    NotificationHeader_t header;
    NotificationContent_t content;
} Notification_t;

/**
 * Notification interface name
 */
extern const char InterfaceName[];
/**
 * Notification interface signal
 */
extern const char SignalName[];
/**
 * Notification interface version property
 */
extern const char Version[];
/**
 * Notification interface version property value
 */
extern const uint16_t NotificationVersion;

/**
 * Notification interface name followed by the method signatures.
 *
 * See also .\inc\aj_introspect.h
 */
extern const char* NotificationInterface[];

/**
 * A NULL terminated collection of all interfaces.
 */
extern const AJ_InterfaceDescription NotificationInterfaces[];

/*!
   \brief Minimal time in seconds for the notification signal to live
 */
extern const uint16_t NOTIFICATION_TTL_MIN;

/*!
   \brief Maximal time in seconds for the notification signal to live
 */
extern const uint16_t NOTIFICATION_TTL_MAX;

/**
 * Notification Dismisser object for the Dismiss signal emitter
 */
/*
 * Notification Dismisser interface name followed by the method signatures.
 *
 * See also .\inc\aj_introspect.h
 */
extern const char* const NotificationDismisserInterface[];
/*
 * Notification Dismisser interface version property value
 */
extern const uint16_t NotificationDismisserVersion;
/*
 * Notification Dismisser interface signal emitter
 */
extern AJ_Status NotificationSendDismiss(int32_t msgId, const char* appId);
/**
 * A NULL terminated collection of all NotificationDismisser interfaces.
 */
extern const AJ_InterfaceDescription NotificationDismisserInterfaces[];
/*
 * Notification Dismisser signal emitter object path
 */
extern char NotificationDismisserObjectPath[];

/**
 * Dismisser Object description
 */
#define NUM_NOTIFICATION_COMMON_OBJECTS 1
#define NOTIFICATION_COMMON_APPOBJECTS \
    { NotificationDismisserObjectPath,   NotificationDismisserInterfaces },

#define NOTIFICATION_DISMISSER_OBJECT_INDEX 0 + NUM_PRE_NOTIFICATION_COMMON_OBJECTS
#define NOTIFICATION_DISMISSER_GET_PROPERTY             AJ_APP_PROPERTY_ID(NOTIFICATION_DISMISSER_OBJECT_INDEX, 0, AJ_PROP_GET)
#define NOTIFICATION_DISMISSER_SET_PROPERTY             AJ_APP_PROPERTY_ID(NOTIFICATION_DISMISSER_OBJECT_INDEX, 0, AJ_PROP_SET)

#define NOTIFICATION_DISMISSER_DISMISS_EMITTER          AJ_APP_MESSAGE_ID(NOTIFICATION_DISMISSER_OBJECT_INDEX, 1, 0)
#define GET_NOTIFICATION_DISMISSER_VERSION_PROPERTY     AJ_APP_PROPERTY_ID(NOTIFICATION_DISMISSER_OBJECT_INDEX, 1, 1)

/**
 * Notification Producer interface name followed by the method signatures.
 *
 * See also .\inc\aj_introspect.h
 */
extern const char* const NotificationProducerInterface[];
/**
 * Notification Producer interface version property value
 */
extern const uint16_t NotificationProducerVersion;
/**
 * A NULL terminated collection of all NotificationProducer interfaces.
 */
extern const AJ_InterfaceDescription NotificationProducerInterfaces[];
/**
 * The Notification service Producer object path
 */
extern const char NotificationProducerObjectPath[];
/**
 * The Notification service Producer port
 */
extern const uint16_t NotificationProducerPort;
#endif /* COMMON_H_ */
