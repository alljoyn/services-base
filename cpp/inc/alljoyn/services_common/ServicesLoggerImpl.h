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

#ifndef SERVICESLOGGERIMPL_H_
#define SERVICESLOGGERIMPL_H_

#include <alljoyn/services_common/GenericLogger.h>
#include <alljoyn/Status.h>
#include <qcc/Debug.h>

namespace ajn {
namespace services {

/**
 * Logger implementation of Generic Logger
 * This is the default logger of the service
 */
class ServicesLoggerImpl : public GenericLogger {
  public:

    /**
     * Constructor for Service Logger
     */
    ServicesLoggerImpl();

    /**
     * Destructor for Service Logger
     */
    virtual ~ServicesLoggerImpl();

    /**
     * Set log level filter for subsequent logging messages
     * @param logLevel enum value
     * @return logLevel enum value that was in effect prior to this change
     */
    Log::LogLevel setLogLevel(Log::LogLevel logLevel);

    /**
     * Get log level filter value currently in effect
     * @return logLevel enum value currently in effect
     */
    Log::LogLevel getLogLevel();

    /**
     * Get the class' default log level filter
     * @return logLevel enum value
     */
    static Log::LogLevel getDefaultLogLevel();

    /**
     * Log in Debug Level
     * @param Tag identifies area of code
     * @param logText Text to be logged
     */
    void debug(const char* Tag, const char* logText);

    /**
     * Log in Info Level
     * @param Tag identifies area of code
     * @param logText Text to be logged
     */
    void info(const char* Tag, const char* logText);

    /**
     * Log in Warn Level
     * @param Tag identifies area of code
     * @param logText Text to be logged
     */
    void warn(const char* Tag, const char* logText);

    /**
     * Log in Error Level
     * @param Tag identifies area of code
     * @param logText Text to be logged
     */
    void error(const char* Tag, const char* logText);

    /**
     * Log in Fatal Level
     * @param Tag identifies area of code
     * @param logText Text to be logged
     */
    void fatal(const char* Tag, const char* logText);

    /**
     * Registers the callback function to be used for the passed-in
     * logger object. For the built-in Service logger,
     * the AllJoyn built-in callback function is registered. For a
     * user-supplied logger, the Services's logger callback
     * function, GenericLoggerCallBack, is registered.
     * @param logger GenericLogger object currently in use
     * @param cb to register
     * @param context Context to be returned as part of callback
     */
    static void RegisterCallBack(GenericLogger* logger, QCC_DbgMsgCallback cb, void* context);

  private:

    /**
     * Stores and keeps track of the initial, default logger object.
     */
    static GenericLogger* m_initialLogger;

    /**
     * Currently-configured log level filter value.
     */
    Log::LogLevel m_logLevel;

    /**
     * Stores the class' default log level filter value.
     */
    static Log::LogLevel m_defaultLogLevel;
};
} //namespace services
} //namespace ajn

#endif /* SERVICESLOGGERIMPL_H_ */
