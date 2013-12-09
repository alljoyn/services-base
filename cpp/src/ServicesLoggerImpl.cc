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

#include <alljoyn/services_common/ServicesLoggerImpl.h>
#include <qcc/Log.h>

using namespace ajn;
using namespace services;
using namespace qcc;

#define QCC_MODULE "Services"

Log::LogLevel ServicesLoggerImpl::m_defaultLogLevel = Log::LEVEL_ERROR;
GenericLogger* ServicesLoggerImpl::m_initialLogger = NULL;

ServicesLoggerImpl::ServicesLoggerImpl()
{
    /*
     * AllJoyn loglevels
     * default is 0 (none, except for errors)
     * 1 = high-level-debug (binary 1)                   "debug"
     * 2 = general-debug   (binary 10)                   "info"
     * 3 = high-level-debug, general-debug (binary 11)   "debug + info"
     * 4 = trace (binary 100)                            "warn"
     * 5 = trace, high-level-debug (binary 101)          "warn + debug"
     * 6 = trace, general-debug (binary 110)             "warn + info"
     * 7 = trace, general-debug, high-level-debug (binary 111)  "warn + info + debug"
     *     error is always printed
     */
    setLogLevel(m_defaultLogLevel);
    // save this initial default logger. we don't want to register AJ callback
    // on it (because then our callback calls itself endlessly). if it's passed
    // to us later on in setLogger, we will not set our callback on it.
    m_initialLogger = this;
}

ServicesLoggerImpl::~ServicesLoggerImpl()
{

}

Log::LogLevel ServicesLoggerImpl::setLogLevel(Log::LogLevel newLogLevel)
{
    Log::LogLevel prevLogLevel = m_logLevel;
    m_logLevel = newLogLevel;
    QCC_SetDebugLevel(QCC_MODULE, 7);     // AJ should not block any of our log messages
    return prevLogLevel;
}

Log::LogLevel ServicesLoggerImpl::getLogLevel()
{
    return m_logLevel;
}

Log::LogLevel ServicesLoggerImpl::getDefaultLogLevel()
{
    return m_defaultLogLevel;
}

void ServicesLoggerImpl::debug(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_DEBUG)
        QCC_DbgTrace(("[DEBUG] %s: %s", Tag, logText));
}

void ServicesLoggerImpl::info(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_INFO)
        QCC_DbgPrintf(("[INFO] %s: %s", Tag, logText));
}
void ServicesLoggerImpl::warn(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_WARN)
        QCC_DbgHLPrintf(("[WARN] %s: %s", Tag, logText));
}

void ServicesLoggerImpl::error(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_ERROR)
        QCC_LogError(ER_FAIL, ("[ERROR] %s: %s", Tag, logText));
}

void ServicesLoggerImpl::fatal(const char* Tag, const char* logText)
{
    if (m_logLevel >= Log::LEVEL_FATAL)
        QCC_LogError(ER_FAIL, ("[FATAL] %s: %s", Tag, logText));
}

void ServicesLoggerImpl::RegisterCallBack(GenericLogger* newLogger, QCC_DbgMsgCallback cb, void* context)
{
    // arrange for AJ logging to come back to us, for redirection to the logger instance
    if (newLogger == m_initialLogger) {
        QCC_RegisterOutputFile(stderr);
    } else {
        QCC_RegisterOutputCallback(cb, context);
    }
}

