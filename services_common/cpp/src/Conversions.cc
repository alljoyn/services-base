/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
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
#include <qcc/platform.h>
#include "alljoyn/services_common/Conversions.h"
#include <alljoyn/MsgArg.h>
#include <qcc/String.h>
#include <alljoyn/BusAttachment.h>
#include <qcc/platform.h>
#include <qcc/String.h>
#include <alljoyn/Status.h>
#include "alljoyn/services_common/ServicesCommonConstants.h"
#include <iostream>
#include <sstream>
#include "alljoyn/services_common/LogModule.h"

using namespace ajn;
using namespace services;
using namespace commonConsts;
using namespace qcc;

QStatus Conversions::MsgArgToArrayOfBytes(const MsgArg* msgArg, uint8_t** byteArray, size_t* len)
{
    QStatus status = ER_OK;
    if (msgArg->typeId != ALLJOYN_BYTE_ARRAY) {
        status = ER_BUS_BAD_VALUE_TYPE;
        QCC_LogError(status, ("ERROR- Problem receiving message: Can not unmarshal this array of bytes argument."));
        return status;
    }

    status = msgArg->Get(AJPARAM_ARR_BYTE.c_str(), len, byteArray);
    if (*len != UUID_LENGTH) {
        status = ER_BUS_BAD_VALUE;
        QCC_LogError(status, ("ERROR- Array of bytes length is not equal to %d  but to %d", UUID_LENGTH * 2, *len));
        return status;
    }
    return status;
}

void Conversions::ArrayOfBytesToString(uint8_t const*const* byteArray, size_t len, qcc::String* byteArrayStr)
{
    uint32_t i;
    static const char* const chars = "0123456789ABCDEF";

    (*byteArrayStr).reserve(2 * len);
    for (i = 0; i < len; ++i) {
        const unsigned char c = (*byteArray)[i];
        (*byteArrayStr).push_back(chars[c >> 4]);
        (*byteArrayStr).push_back(chars[c & 15]);
    }
}

char Conversions::HexToChar(char c)
{
    if ('0' <= c && c <= '9') {
        return c - '0';
    } else if ('a' <= c && c <= 'f') {
        return c + 10 - 'a';
    } else if ('A' <= c && c <= 'F') {
        return c + 10 - 'A';
    }

    return -1;
}

QStatus Conversions::HexStringToBytes(const qcc::String& hex, uint8_t* outBytes, size_t len)
{
    unsigned char achar, bchar;
    for (size_t i = 0; i < len; i++) {

        if ((char) (achar = HexToChar(hex[i * 2])) ==  -1) {
            return ER_FAIL;
        }
        if ((char) (bchar = HexToChar(hex[i * 2 + 1])) == -1) {
            return ER_FAIL;
        }
        outBytes[i] = ((achar << 4) | bchar);
    }
    return ER_OK;
}
