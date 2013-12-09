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

#define NUM_MESSAGE_TYPES 3

/*!
   \brief struct that holds the notification content
 */
typedef struct {
    uint16_t messageType;                       /**< messageType One of \ref INFO, \ref WARNING, or \ref EMERGENCY */
    uint32_t messageId;                         /**< message id */
    int8_t numCustomAttributes;                 /**< numCustomAttributes number of custom Attributs */
    struct keyValue*customAttributes;           /**< customAttributes Custom attributs */
    int8_t numTexts;                            /**< numTexts number of versions of the notification text */
    struct keyValue*texts;                      /**< texts The text of the notification, one entry per language */
    int8_t numAudioUrls;                        /**< numAudioUrls The number of audio URLs sent */
    struct keyValue*richAudioUrls;              /**< richAudioUrls The audio URLs, one per language */
    const char* richIconUrl;                    /**< richIconUrl A URL for an icon to be displayed along with the notification */
    const char* richIconObjectPath;             /**< richIconObjectPath The AllJoyn object path of an accompanying icons object */
    const char* richAudioObjectPath;            /**< richAudioObjectPath The AllJoyn object path of an accompanying audio object */
    const char* controlPanelServiceObjectPath;  /**< controlPanelServiceObjectPath The AllJoyn object path of an accompanying Control Panel Service object */
    uint16_t ttl;                               /**< ttl TTL (in seconds) for how long this notification will persist on the bus */
}notification;


/**
 * Generic structure for key=value
 */
struct keyValue {
    const char*key;
    const char*value;
};

/**
 * Static non consts.
 */
extern const char InterfaceName[];
extern const char SignalName[];
extern const char Version[];

extern const uint16_t version;

/**
 * The interface name followed by the method signatures.
 *
 * See also .\inc\aj_introspect.h
 */
extern const char*NotificationInterface[];

/**
 * A NULL terminated collection of all interfaces.
 */
extern const AJ_InterfaceDescription NotificationInterfaces[];

#endif /* COMMON_H_ */
