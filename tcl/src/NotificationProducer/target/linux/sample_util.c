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
#include <producer_sample_util.h>

#define BUF_SIZE 500
#define TTL_MIN 30
#define TTL_MAX 43200
#define MAX_MSG 5

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

void Producer_GetNotificationFromUser()
{
    uint16_t messageType = 0;
    uint16_t ttl = TTL_MIN;
    NotificationContent_t notificationContent;
    struct keyValue richAudioUrls[100];
    struct keyValue texts[100];
    struct keyValue customAttributes[100];
    char richIconUrl[BUF_SIZE] = "/richIconUrl";
    char richIconObjectPath[BUF_SIZE] = "/richIconObjectPath";
    char richAudioObjectPath[BUF_SIZE] = "/richAudioObjectPath";
    char controlPanelServiceObjectPath[BUF_SIZE];

    char buf[BUF_SIZE];
    int8_t i;

    notificationContent.numCustomAttributes = 0;
    notificationContent.numTexts = 1;
    notificationContent.numAudioUrls = 0;

    AJ_Printf("Please enter the messageType.\n");
    AJ_Printf("Empty string or invalid input will default to 0\n");
    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            messageType = (uint8_t)atoi(buf);
            char stringType[8];
            sprintf(stringType, "%d", messageType);
            if (!(strcmp(buf, stringType) == 0)) {             //they do not match, it is not int
                AJ_Printf("Message Type is not an integer value. Defaulting to 0\n");
                messageType = 0;
            }
        }
    }

    AJ_Printf("Please enter the number of languages you wish to send the message in. Maximum %d\n", MAX_MSG);
    AJ_Printf("Empty string or invalid data will default to 1\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            notificationContent.numTexts = atoi(buf);
            if (notificationContent.numTexts < 1 || notificationContent.numTexts > MAX_MSG) {
                notificationContent.numTexts = 1;
            }
        }
    }

    for (i = 0; i < notificationContent.numTexts; i++) {
        char lang[BUF_SIZE] = "en";
        char msg[BUF_SIZE] = "Hello World";

        AJ_Printf("Please enter the message's language %d. Max 499 characters, Empty string acceptable\n", i + 1);

        if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
            if (strlen(buf)) {
                strcpy(lang, buf);
            }
        }

        AJ_Printf("Please enter the message's text in language %d. Max 499 characters, Empty string acceptable\n", i + 1);

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
    notificationContent.texts = texts;

    AJ_Printf("Please enter the number of Custom Attributes you wish to send. Maximum %d\n", MAX_MSG);
    AJ_Printf("Empty string or invalid data will default to 0\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            notificationContent.numCustomAttributes = atoi(buf);
            if (notificationContent.numCustomAttributes > MAX_MSG) {
                notificationContent.numCustomAttributes = 0;
            }
        }
    }

    for (i = 0; i < notificationContent.numCustomAttributes; i++) {
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
    notificationContent.customAttributes = customAttributes;

    AJ_Printf("Please enter the ttl.\n");
    AJ_Printf("Empty string input will default to 30\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            ttl = (uint8_t)atoi(buf);
        }
    }

    AJ_Printf("Please enter an optional rich icon url. Max 499 characters.\n");
    AJ_Printf("Empty string will default to not setting this value\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            strcpy(richIconUrl, buf);
        }
    }
    notificationContent.richIconUrl = richIconUrl;

    AJ_Printf("Please enter the number of rich audio urls you wish to send. Maximum %d\n", MAX_MSG);
    AJ_Printf("Empty string or invalid data will default to 0\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            notificationContent.numAudioUrls = atoi(buf);
            if (notificationContent.numAudioUrls > MAX_MSG) {
                notificationContent.numAudioUrls = 0;
            }
        }
    }

    for (i = 0; i < notificationContent.numAudioUrls; i++) {
        char lang[1024] = "en";
        char url[1024] = "http://myAudioUrl.wv";

        AJ_Printf("Please enter the url's language %d. Max 499 characters, Empty string acceptable\n", i + 1);

        if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
            if (strlen(buf)) {
                strcpy(lang, buf);
            }
        }

        AJ_Printf("Please enter the url in language %d. Max 499 characters, Empty string acceptable\n", i + 1);

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
    notificationContent.richAudioUrls = richAudioUrls;

    AJ_Printf("Please enter an optional icon object path. Max 499 characters.\n");
    AJ_Printf("Empty string will default to not setting this value\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            strcpy(richIconObjectPath, buf);
        }
    }
    notificationContent.richIconObjectPath = richIconObjectPath;

    AJ_Printf("Please enter an optional audio object path. Max 499 characters.\n");
    AJ_Printf("Empty string will default to not setting this value\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL) {
        if (strlen(buf)) {
            strcpy(richAudioObjectPath, buf);
        }
    }
    notificationContent.richAudioObjectPath = richAudioObjectPath;

    AJ_Printf("Please enter an optional control panel service object path. Max 499 characters.\n");
    AJ_Printf("Empty string will default to not setting this value\n");

    if (NS_GetLine(buf, BUF_SIZE, stdin) != NULL && strlen(buf)) {
        strcpy(controlPanelServiceObjectPath, buf);
        notificationContent.controlPanelServiceObjectPath = controlPanelServiceObjectPath;
    } else {
        notificationContent.controlPanelServiceObjectPath = NULL;
    }

    ProducerSetNotification(&notificationContent, messageType, ttl);

    for (i = 0; i < notificationContent.numTexts; i++) {
        free((char*)texts[i].key);
        free((char*)texts[i].value);
    }

    for (i = 0; i < notificationContent.numCustomAttributes; i++) {
        free((char*)customAttributes[i].key);
        free((char*)customAttributes[i].value);
    }

    for (i = 0; i < notificationContent.numAudioUrls; i++) {
        free((char*)richAudioUrls[i].key);
        free((char*)richAudioUrls[i].value);
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
                status = ProducerDeleteLastMsg(busAttachment, delMsgType);
                AJ_Printf("Delete Last Message Type: %d returned: '%s'\n\n", delMsgType, AJ_StatusText(status));
            }
        }
    }
}
