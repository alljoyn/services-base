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

#include <aj_target.h>
#include <aj_status.h>
#include <stdlib.h>
#include <alljoyn/notification/NotificationConsumer.h>
#include <NotificationConsumerSampleUtil.h>

#define BUF_SIZE 500

static uint8_t GetCmdLine(char* buf, size_t num)
{
    while (AJ_GetCmdLine(buf, num) == NULL) {
    }
    return (strlen(buf) > 0);
}

/**
 * A function that allows us to enter QA InputMode
 */
void Consumer_SetupEnv(uint8_t* inputMode, uint8_t* superAgentMode)
{
    char* value = getenv("INPUT_MODE");
    char buf[BUF_SIZE];
    uint8_t defaultSuperAgentMode = *superAgentMode;

    if (value) {
        *inputMode = (uint8_t)atoi(value);
    }
    if (*inputMode) {
        AJ_StartReadFromStdIn();
        AJ_Printf("Please enter 0=OFF or 1=ON to disable/enable SuperAgentMode. Default is %u\n", defaultSuperAgentMode);
        if (GetCmdLine(buf, BUF_SIZE)) {
            *superAgentMode = (uint8_t)atoi(buf);
            if (1 < *superAgentMode) {
                *superAgentMode = defaultSuperAgentMode;
            }
        }
        AJ_StopReadFromStdIn();
    }
}

void Consumer_GetActionFromUser(uint8_t* action)
{
    char buf[BUF_SIZE];
    uint8_t defaultAction = *action;
    AJ_StartReadFromStdIn();
    AJ_Printf("Please enter an action to perform on the recently received notification\n\t0=Nothing,\n\t1=Dismiss.\n");
    AJ_Printf("Empty string or invalid input will default to %u\n", defaultAction);
    if (GetCmdLine(buf, BUF_SIZE)) {
        *action = (uint8_t)atoi(buf);
        char stringType[10];
        sprintf(stringType, "%u", *action);
        if (!(strcmp(buf, stringType) == 0)) {             //they do not match, it is not int
            AJ_Printf("Action is not an integer value. Defaulting to %u\n", defaultAction);
            *action = defaultAction;
        }
    }
    AJ_StopReadFromStdIn();
}
