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

#include "SampleLogger.h"
#include <stdio.h>

using namespace ajn;
using namespace services;

SampleLogger::SampleLogger() : m_logLevel(Log::LEVEL_DEBUG)
{

}
SampleLogger::~SampleLogger()
{

}

Log::LogLevel SampleLogger::setLogLevel(Log::LogLevel newLogLevel)
{
    Log::LogLevel prevLogLevel = m_logLevel;
    m_logLevel = newLogLevel;
    return prevLogLevel;
}

Log::LogLevel SampleLogger::getLogLevel()
{
    return m_logLevel;
}

void SampleLogger::debug(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_DEBUG) {
        fprintf(stderr, "[DEBUG]: %s: %s\n", Tag, logText);
    }
}

void SampleLogger::info(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_INFO) {
        fprintf(stderr, "[INFO] : %s: %s\n", Tag, logText);
    }
}
void SampleLogger::warn(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_WARN) {
        fprintf(stderr, "[WARN] : %s: %s\n", Tag, logText);
    }
}

void SampleLogger::error(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_ERROR) {
        fprintf(stderr, "[ERROR]: %s: %s\n", Tag, logText);
    }
}

void SampleLogger::fatal(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_FATAL) {
        fprintf(stderr, "[FATAL]: %s: %s\n", Tag, logText);
    }
}
