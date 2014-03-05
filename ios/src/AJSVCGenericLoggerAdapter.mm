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

#import "AJSVCGenericLoggerAdapter.h"
#import "alljoyn/about/AJNConvertUtil.h"
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
	[kAJSVCGenericLogger debugTag:[AJNConvertUtil convertConstCharToNSString:Tag] text:[AJNConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::info(const char *Tag, const char *logText)
{
	[kAJSVCGenericLogger infoTag:[AJNConvertUtil convertConstCharToNSString:Tag] text:[AJNConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::warn(const char *Tag, const char *logText)
{
	[kAJSVCGenericLogger warnTag:[AJNConvertUtil convertConstCharToNSString:Tag] text:[AJNConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::error(const char *Tag, const char *logText)
{
	[kAJSVCGenericLogger errorTag:[AJNConvertUtil convertConstCharToNSString:Tag] text:[AJNConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::fatal(const char *Tag, const char *logText)
{
	[kAJSVCGenericLogger fatalTag:[AJNConvertUtil convertConstCharToNSString:Tag] text:[AJNConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::debug(qcc::String const& Tag, qcc::String const& logText)
{
	[kAJSVCGenericLogger debugTag:[AJNConvertUtil convertQCCStringtoNSString:Tag] text:[AJNConvertUtil convertQCCStringtoNSString:logText]];
}

void AJSVCGenericLoggerAdapter::info(qcc::String const& Tag, qcc::String const& logText)
{
	[kAJSVCGenericLogger infoTag:[AJNConvertUtil convertQCCStringtoNSString:Tag] text:[AJNConvertUtil convertQCCStringtoNSString:logText]];
}

void AJSVCGenericLoggerAdapter::warn(qcc::String const& Tag, qcc::String const& logText)
{
	[kAJSVCGenericLogger warnTag:[AJNConvertUtil convertQCCStringtoNSString:Tag] text:[AJNConvertUtil convertQCCStringtoNSString:logText]];
}

void AJSVCGenericLoggerAdapter::error(qcc::String const& Tag, qcc::String const& logText)
{
	[kAJSVCGenericLogger errorTag:[AJNConvertUtil convertQCCStringtoNSString:Tag] text:[AJNConvertUtil convertQCCStringtoNSString:logText]];
}

void AJSVCGenericLoggerAdapter::fatal(qcc::String const& Tag, qcc::String const& logText)
{
	[kAJSVCGenericLogger fatalTag:[AJNConvertUtil convertQCCStringtoNSString:Tag] text:[AJNConvertUtil convertQCCStringtoNSString:logText]];
}

ajn::services::Log::LogLevel AJSVCGenericLoggerAdapter::setLogLevel(ajn::services::Log::LogLevel logLevel)
{
	[kAJSVCGenericLogger setLogLevel:[AJSVCGenericLoggerUtil revertLogLevel:logLevel]];
	return logLevel;
}

ajn::services::Log::LogLevel AJSVCGenericLoggerAdapter::getLogLevel()
{
	return [AJSVCGenericLoggerUtil convertQLogLevel:[kAJSVCGenericLogger logLevel]];
}