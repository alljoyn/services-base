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

#include <alljoyn/services_common/PropertyStore.h>
#include <alljoyn/services_common/Services_Common.h>

uint8_t Common_IsLanguageSupported(AJ_Message* msg, AJ_Message* reply, const char* language, int8_t* langIndex)
{
    uint8_t supported = TRUE;
    int8_t foundLangIndex = PropertyStore_GetLanguageIndex(language);
    if (foundLangIndex == AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX) {
        AJ_MarshalErrorMsg(msg, reply, AJSVC_ERROR_LANGUAGE_NOT_SUPPORTED);
        supported = FALSE;
    }
    if (langIndex != NULL) {
        *langIndex = foundLangIndex;
    }
    return supported;
}

AJ_Status Common_MarshalAppId(AJ_Message*msg, const char* appId)
{
    AJ_Arg appIdArg;
    uint8_t binAppId[UUID_LENGTH];
    uint32_t sz = strlen(appId);
    if (sz > UUID_LENGTH * 2) { // Crop application id that is too long
        sz = UUID_LENGTH * 2;
    }
    AJ_HexToRaw(appId, sz, binAppId, UUID_LENGTH);

    AJ_InitArg(&appIdArg, AJ_ARG_BYTE, AJ_ARRAY_FLAG, binAppId, UUID_LENGTH);

    return AJ_MarshalArg(msg, &appIdArg);
}


