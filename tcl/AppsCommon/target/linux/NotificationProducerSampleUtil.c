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

// _GNU_SOURCE for asprintf (see man asprintf)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <aj_target.h>
#include <aj_status.h>
#include <aj_debug.h>
#include <alljoyn/notification/NotificationProducer.h>
#include <NotificationProducerSampleUtil.h>

#define BUF_SIZE 500
#define MAX_MSG 5

AJNS_DictionaryEntry richAudioUrls[MAX_MSG];
AJNS_DictionaryEntry texts[MAX_MSG];
AJNS_DictionaryEntry customAttributes[MAX_MSG];

static char* NS_GetLine(char* str, size_t num, void* fp)
{
    char* p = fgets(str, num, fp);

    if (p != NULL) {
        size_t last = strlen(str) - 1;
        if (str[last] == '\n') {
            str[last] = '\0';
        } else {
            while (1) {
                char tmp[num + 1];
                char* t = fgets(tmp, num, fp);
                if (t == NULL) {
                    return t;
                }
                size_t end = strlen(tmp) - 1;
                if (tmp[end] == '\n') {
                    break;
                }
            }
        }
    }
    return p;
}

/**
 * A function that allows us to enter QA InputMode
 */
void Producer_SetupEnv(uint8_t* inputMode)
{
    char* value = getenv("INPUT_MODE");
    if (value) {
        *inputMode = (uint8_t)atoi(value);
    }
}

void Producer_GetNotificationFromUser(AJNS_NotificationContent* notificationContent, uint16_t* messageType, uint32_t* ttl, uint16_t* nextMessageTime)
{
    char buf[BUF_SIZE];
    uint8_t i;

    notificationContent->numCustomAttributes = 0;
    notificationContent->numTexts = 1;
    notificationContent->numAudioUrls = 0;

    uint8_t defaultMessageType = *messageType;
    AJ_Printf("Please enter the messageType 0=EMERGENCY, 1=WARNING, 2=INFO.\n");
    AJ_Printf("Empty string or invalid input will default to %u\n", defaultMessageType);
    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            *messageType = (uint8_t)atoi(buf);
            char stringType[10];
            sprintf(stringType, "%u", *messageType);
            if (!(strcmp(buf, stringType) == 0)) {             //they do not match, it is not int
                AJ_Printf("Message Type is not an integer value. Defaulting to %u\n", defaultMessageType);
                *messageType = defaultMessageType;
            } else if (*messageType != AJNS_NOTIFICATION_MESSAGE_TYPE_EMERGENCY && *messageType != AJNS_NOTIFICATION_MESSAGE_TYPE_WARNING && *messageType != AJNS_NOTIFICATION_MESSAGE_TYPE_INFO) {
                AJ_Printf("Message Type is not one of 0, 1 or 2. Defaulting to %u\n", defaultMessageType);
                *messageType = defaultMessageType;
            }
        }
    }

    uint8_t defaultNumTexts = notificationContent->numTexts = 1;
    AJ_Printf("Please enter the number of languages you wish to send the message in. Maximum %d\n", MAX_MSG);
    AJ_Printf("Empty string or invalid data will default to %u\n", defaultNumTexts);

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            notificationContent->numTexts = atoi(buf);
            if (notificationContent->numTexts < 1 || notificationContent->numTexts > MAX_MSG) {
                notificationContent->numTexts = defaultNumTexts;
            }
        }
    }

    for (i = 0; i < notificationContent->numTexts; i++) {
        char lang[BUF_SIZE] = "en";
        char msg[BUF_SIZE] = "Hello World";

        AJ_Printf("Please enter the message's language %u. Max 499 characters, Empty string acceptable\n", i + 1);

        if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
            if (strlen(buf)) {
                strcpy(lang, buf);
            }
        }

        AJ_Printf("Please enter the message's text in language %u. Max 499 characters, Empty string acceptable\n", i + 1);

        if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
            if (strlen(buf)) {
                strcpy(msg, buf);
            }
        }
        if (asprintf((char**)&texts[i].key, "%s", lang) == -1) {
            break;
        }
        if (asprintf((char**)&texts[i].value, "%s", msg) == -1) {
            break;
        }
    }
    notificationContent->texts = texts;

    uint8_t defaultNumCustomAttributes = notificationContent->numCustomAttributes = 0;
    AJ_Printf("Please enter the number of Custom Attributes you wish to send. Maximum %d\n", MAX_MSG);
    AJ_Printf("Empty string or invalid data will default to %u\n", defaultNumCustomAttributes);

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            notificationContent->numCustomAttributes = atoi(buf);
            if (notificationContent->numCustomAttributes < 0 || notificationContent->numCustomAttributes > MAX_MSG) {
                notificationContent->numCustomAttributes = defaultNumCustomAttributes;
            }
        }
    }

    for (i = 0; i < notificationContent->numCustomAttributes; i++) {
        char key[BUF_SIZE] = "key";
        char val[BUF_SIZE] = "val";

        AJ_Printf("Please enter the Custom Attribute key. Max 499 characters, Empty string acceptable\n");

        if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
            if (strlen(buf)) {
                strcpy(key, buf);
            }
        }

        AJ_Printf("Please enter the Custom Attribute value. Max 499 characters, Empty string acceptable\n");

        if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
            if (strlen(buf)) {
                strcpy(val, buf);
            }
        }
        if (asprintf((char**)&customAttributes[i].key, "%s", key) == -1) {
            break;
        }
        if (asprintf((char**)&customAttributes[i].value, "%s", val) == -1) {
            break;
        }
    }
    notificationContent->customAttributes = customAttributes;

    uint16_t defaultTtl = *ttl;
    AJ_Printf("Please enter the ttl in the range %u - %u seconds.\n", AJNS_NOTIFICATION_TTL_MIN, AJNS_NOTIFICATION_TTL_MAX);
    AJ_Printf("Empty string input will default to %u\n", defaultTtl);

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            *ttl = (uint8_t)atoi(buf);
            char stringType[10];
            sprintf(stringType, "%u", *ttl);
            if (!(strcmp(buf, stringType) == 0)) {             //they do not match, it is not int
                AJ_Printf("Ttl is not an integer value. Defaulting to %u\n", defaultTtl);
                *ttl = defaultTtl;
            } else if (*ttl < NOTIFICATION_TTL_MIN || NOTIFICATION_TTL_MAX < *ttl) {
                AJ_Printf("Ttl is not in the range %u - %u. Defaulting to %u\n", NOTIFICATION_TTL_MIN, NOTIFICATION_TTL_MAX, defaultTtl);
                *ttl = defaultTtl;
            }
        }
    }

    AJ_Printf("Please enter an optional rich icon url. Max 499 characters.\n");
    AJ_Printf("Empty string will default to not setting this value\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            if (asprintf((char**)&notificationContent->richIconUrl, "%s", buf) == -1) {
            }
        } else {
            notificationContent->richIconUrl = NULL;
        }

        AJ_Printf("Please enter the number of rich audio urls you wish to send. Maximum %d\n", MAX_MSG);
    }
    AJ_Printf("Empty string or invalid data will default to 0\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            notificationContent->numAudioUrls = atoi(buf);
            if (notificationContent->numAudioUrls > MAX_MSG) {
                notificationContent->numAudioUrls = 0;
            }
        }
    }

    for (i = 0; i < notificationContent->numAudioUrls; i++) {
        char lang[BUF_SIZE] = "en";
        char url[BUF_SIZE] = "http://myAudioUrl.wv";

        AJ_Printf("Please enter the url's language %u. Max 499 characters, Empty string acceptable\n", i + 1);

        if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
            if (strlen(buf)) {
                strcpy(lang, buf);
            }
        }

        AJ_Printf("Please enter the url in language %u. Max 499 characters, Empty string acceptable\n", i + 1);

        if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
            if (strlen(buf)) {
                strcpy(url, buf);
            }
        }
        if (asprintf((char**)&richAudioUrls[i].key, "%s", lang) == -1) {
            break;
        }
        if (asprintf((char**)&richAudioUrls[i].value, "%s", url) == -1) {
            break;
        }
    }
    notificationContent->richAudioUrls = richAudioUrls;

    AJ_Printf("Please enter an optional icon object path. Max 499 characters.\n");
    AJ_Printf("Empty string will default to not setting this value\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            if (asprintf((char**)&notificationContent->richIconObjectPath, "%s", buf) == -1) {
            }
        } else {
            notificationContent->richIconObjectPath = NULL;
        }
    }

    AJ_Printf("Please enter an optional audio object path. Max 499 characters.\n");
    AJ_Printf("Empty string will default to not setting this value\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            if (asprintf((char**)&notificationContent->richAudioObjectPath, "%s", buf) == -1) {
            }
        } else {
            notificationContent->richAudioObjectPath = NULL;
        }
    }

    AJ_Printf("Please enter an optional control panel service object path. Max 499 characters.\n");
    AJ_Printf("Empty string will default to not setting this value\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            if (asprintf((char**)&notificationContent->controlPanelServiceObjectPath, "%s", buf) == -1) {
            }
        } else {
            notificationContent->controlPanelServiceObjectPath = NULL;
        }
    }

    uint16_t defaultNextMessageTime = *nextMessageTime;
    AJ_Printf("Please enter the interval for the next message. Default %u.\n", *nextMessageTime);
    AJ_Printf("Empty string will keep default value\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            *nextMessageTime = (uint8_t)atoi(buf);
            char stringType[10];
            sprintf(stringType, "%u", *nextMessageTime);
            if (!(strcmp(buf, stringType) == 0)) {             //they do not match, it is not int
                AJ_Printf("Next message interval is not an integer value. Defaulting to %u\n", defaultNextMessageTime);
                *nextMessageTime = defaultNextMessageTime;
            }
        }
    }
}

void Producer_FreeNotification(AJNS_NotificationContent* notificationContent)
{
    uint8_t i;

    for (i = 0; i < notificationContent->numTexts; i++) {
        free((char*)texts[i].key);
        free((char*)texts[i].value);
    }

    for (i = 0; i < notificationContent->numCustomAttributes; i++) {
        free((char*)customAttributes[i].key);
        free((char*)customAttributes[i].value);
    }

    for (i = 0; i < notificationContent->numAudioUrls; i++) {
        free((char*)richAudioUrls[i].key);
        free((char*)richAudioUrls[i].value);
    }

    if (notificationContent->richIconUrl) {
        free((char*)notificationContent->richIconUrl);
    }
    if (notificationContent->richIconObjectPath) {
        free((char*)notificationContent->richIconObjectPath);
    }
    if (notificationContent->richAudioObjectPath) {
        free((char*)notificationContent->richAudioObjectPath);
    }
    if (notificationContent->controlPanelServiceObjectPath) {
        free((char*)notificationContent->controlPanelServiceObjectPath);
    }
}

void Producer_PossiblyDeleteNotification(AJ_BusAttachment* busAttachment, uint16_t isMessageTime)
{
    if (isMessageTime == 1) {
        uint16_t delMsgType;
        char buf[BUF_SIZE];
        AJ_Printf("Please enter 0-2 to select message type you want to delete.\n");
        AJ_Printf("Empty string or invalid input will default to not sending delete message\n");

        if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
            if (strlen(buf)) {
                AJ_Status status = AJ_OK;
                delMsgType = atoi(buf);
                status = AJNS_Producer_DeleteLastNotification(busAttachment, delMsgType);
                AJ_Printf("Delete Last Message Type: %d returned: '%s'\n\n", delMsgType, AJ_StatusText(status));
            }
        }
    }
}
