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

#import "AJSVCGenericLoggerUtil.h"

@implementation AJSVCGenericLoggerUtil
+ (ajn::services::Log::LogLevel)convertQLogLevel:(QLogLevel)logLevel
{
	switch (logLevel) {
		case QLEVEL_FATAL:
		{
			return ajn::services::Log::LEVEL_FATAL;
		}
            
		case QLEVEL_ERROR:
		{
			return ajn::services::Log::LEVEL_ERROR;
		}
            
		case QLEVEL_WARN:
		{
			return ajn::services::Log::LEVEL_WARN;
		}
            
		case QLEVEL_INFO:
		{
			return ajn::services::Log::LEVEL_INFO;
		}
            
		case QLEVEL_DEBUG:
		{
			return ajn::services::Log::LEVEL_DEBUG;
		}
	}
    
	return ajn::services::Log::LEVEL_FATAL;
}

+ (QLogLevel)revertLogLevel:(ajn::services::Log::LogLevel)logLevel
{
	switch (logLevel) {
		case ajn::services::Log::LEVEL_FATAL:
		{
			return QLEVEL_FATAL;
		}
            
		case ajn::services::Log::LEVEL_ERROR:
		{
			return QLEVEL_ERROR;
		}
            
		case ajn::services::Log::LEVEL_WARN:
		{
			return QLEVEL_WARN;
		}
            
		case ajn::services::Log::LEVEL_INFO:
		{
			return QLEVEL_INFO;
		}
            
		case ajn::services::Log::LEVEL_DEBUG:
		{
			return QLEVEL_DEBUG;
		}
	}
}

+ (NSString *)toStringQLogLevel:(QLogLevel)logLevel
{
	switch (logLevel) {
		case QLEVEL_FATAL:
		{
			return @"FATAL";
		}
            
		case QLEVEL_ERROR:
		{
			return @"ERROR";
		}
            
		case QLEVEL_WARN:
		{
			return @"WARN";
		}
            
		case QLEVEL_INFO:
		{
			return @"INFO";
		}
            
		case QLEVEL_DEBUG:
		{
			return @"DEBUG";
		}
	}
    
	return @"UNKNOWN";
}

@end
