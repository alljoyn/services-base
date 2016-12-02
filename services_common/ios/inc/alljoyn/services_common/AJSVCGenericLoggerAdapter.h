/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#ifndef AJSVCGenericLoggerAdapter_H
#define AJSVCGenericLoggerAdapter_H
#import "AJSVCGenericLogger.h"

/**
 AJSVCGenericLoggerAdapter enable bind the C++ GenericLogger API with an objective-c generic logger handle.
 */
class AJSVCGenericLoggerAdapter {
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
    
private:
	// Objective-c generic logger handle.
	id <AJSVCGenericLogger> kAJSVCGenericLogger;
};


#endif