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

#ifndef TESTLOGGER_H_
#define TESTLOGGER_H_

#include <iostream>
#include <alljoyn/services_common/GenericLogger.h>

/**
 * An example of a Logger class implementing the Generic Logger
 * This class will write the log line to the screen
 */
class SampleLogger : public ajn::services::GenericLogger {
  public:

    /**
     * Constructor
     */
    SampleLogger();

    /**
     * Destructor
     */
    ~SampleLogger();

    /**
     * Set the log level
     * @param logLevel
     * @return previous loglevel
     */
    ajn::services::Log::LogLevel setLogLevel(ajn::services::Log::LogLevel logLevel);

    /**
     * Get the current logLevel
     * @return current logLevel
     */
    ajn::services::Log::LogLevel getLogLevel();

    /**
     * log in debug mode assuming current loglevel is debug
     * @param Tag
     * @param logText
     */
    void debug(const char* Tag, const char* logText);

    /**
     * log in info mode assuming current loglevel is info or less
     * @param Tag
     * @param logText
     */
    void info(const char* Tag, const char* logText);

    /**
     * log in warn mode assuming current loglevel is warn or less
     * @param Tag
     * @param logText
     */
    void warn(const char* Tag, const char* logText);

    /**
     * log in error mode assuming current loglevel is error or less
     * @param Tag
     * @param logText
     */
    void error(const char* Tag, const char* logText);

    /**
     * log in fatal mode assuming current loglevel is fatal or less
     * @param Tag
     * @param logText
     */
    void fatal(const char* Tag, const char* logText);

  private:

    /**
     * current log level
     */
    ajn::services::Log::LogLevel m_logLevel;
};

#endif /* TESTLOGGER_H_ */
