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

#include <alljoyn/services_common/PropertyStore.h>
#include <alljoyn/services_common/Services_Common.h>

#ifndef AJ_MARSHAL_TIMEOUT
#define AJ_MARSHAL_TIMEOUT   (1000 * 5)
#endif

int8_t Common_IsLanguageSupported(AJ_Message* msg, AJ_Message* reply, const char* language)
{
    int8_t langIndex = PropertyStore_GetLanguageIndex(language);
    if (langIndex == ERROR_LANGUAGE_INDEX) {
        AJ_MarshalErrorMsg(msg, reply, LanguageNotSupported);
    }
    return langIndex;
}

AJ_Status BusSetSignalRule(AJ_BusAttachment* bus, const char* ruleString, uint8_t rule)
{
    AJ_Status status;
    AJ_Message msg;
    uint32_t msgId = (rule == AJ_BUS_SIGNAL_ALLOW) ? AJ_METHOD_ADD_MATCH : AJ_METHOD_REMOVE_MATCH;

    status = AJ_MarshalMethodCall(bus, &msg, msgId, AJ_DBusDestination, 0, AJ_FLAG_NO_REPLY_EXPECTED, AJ_MARSHAL_TIMEOUT);
    if (status == AJ_OK) {
        uint32_t sz = 0;
        uint8_t nul = 0;
        sz = (uint32_t)strlen(ruleString);
        status = AJ_DeliverMsgPartial(&msg, sz + 5);
        AJ_MarshalRaw(&msg, &sz, 4);
        AJ_MarshalRaw(&msg, ruleString, strlen(ruleString));
        AJ_MarshalRaw(&msg, &nul, 1);
    }
    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }

    return status;
}
