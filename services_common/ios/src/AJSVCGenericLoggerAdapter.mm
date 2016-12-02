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

#import "AJSVCGenericLoggerAdapter.h"
#import "AJSVCConvertUtil.h"
#import "AJSVCGenericLoggerUtil.h"

AJSVCGenericLoggerAdapter::AJSVCGenericLoggerAdapter(id <AJSVCGenericLogger> qscGenericLogger)
{
    kAJSVCGenericLogger = qscGenericLogger;
}

AJSVCGenericLoggerAdapter::~AJSVCGenericLoggerAdapter()
{
}

void AJSVCGenericLoggerAdapter::debug(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger debugTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::info(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger infoTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::warn(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger warnTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::error(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger errorTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::fatal(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger fatalTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}