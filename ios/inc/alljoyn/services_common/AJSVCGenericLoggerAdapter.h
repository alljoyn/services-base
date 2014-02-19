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

#ifndef AJSVCGenericLoggerAdapter_H
#define AJSVCGenericLoggerAdapter_H
#import "GenericLogger.h"
#import "AJSVCGenericLogger.h"

/**
 AJSVCGenericLoggerAdapter enable bind the C++ GenericLogger API with an objective-c generic logger handle.
 */
class AJSVCGenericLoggerAdapter : public ajn::services::GenericLogger {
public:
	/**
     Constructor
	 */
	AJSVCGenericLoggerAdapter(id <AJSVCGenericLogger> qscGenericLogger);
    
	/**
     Destructor
	 */
	~AJSVCGenericLoggerAdapter();
    
	/**
     Log in Debug Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void debug(const char *Tag, const char *logText);
    
	/**
     Log in Info Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void info(const char *Tag, const char *logText);
    
	/**
     Log in Warn Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void warn(const char *Tag, const char *logText);
    
	/**
     Log in Error Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void error(const char *Tag, const char *logText);
    
	/**
     Log in Fatal Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void fatal(const char *Tag, const char *logText);
    
	/**
     Log in Debug Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void debug(qcc::String const& Tag, qcc::String const& logText);
    
	/**
     Log in Info Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void info(qcc::String const& Tag, qcc::String const& logText);
    
	/**
     Log in Warn Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void warn(qcc::String const& Tag, qcc::String const& logText);
    
	/**
     Log in Error Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void error(qcc::String const& Tag, qcc::String const& logText);
    
	/**
     Log in Fatal Level.
     @param Tag Identifies area of code.
     @param logText Text to be logged.
	 */
	void fatal(qcc::String const& Tag, qcc::String const& logText);
    
	/**
     Set log level filter for subsequent logging messages.
     @param logLevel Enum value.
     @return logLevel Enum value that was in effect prior to this change.
	 */
ajn::services::Log::LogLevel setLogLevel(ajn::services::Log::LogLevel logLevel);
    
	/**
     Return the log level filter value currently in effect.
     @return logLevel Enum value currently in effect.
	 */
ajn::services::Log::LogLevel getLogLevel();
    
private:
	// Objective-c generic logger handle.
	id <AJSVCGenericLogger> kAJSVCGenericLogger;
};


#endif
