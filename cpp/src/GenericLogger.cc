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

#include "alljoyn/services_common/GenericLogger.h"

using namespace ajn;
using namespace services;

void GenericLogger::debug(qcc::String const& Tag, qcc::String const& logText)
{
    return debug(Tag.c_str(), logText.c_str());
}

void GenericLogger::info(qcc::String const& Tag, qcc::String const& logText)
{
    return info(Tag.c_str(), logText.c_str());
}

void GenericLogger::warn(qcc::String const& Tag, qcc::String const& logText)
{
    return warn(Tag.c_str(), logText.c_str());
}

void GenericLogger::error(qcc::String const& Tag, qcc::String const& logText)
{
    return error(Tag.c_str(), logText.c_str());
}

void GenericLogger::fatal(qcc::String const& Tag, qcc::String const& logText)
{
    return fatal(Tag.c_str(), logText.c_str());
}

