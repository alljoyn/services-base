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

#import "AJNSRichAudioUrl.h"
#import "alljoyn/about/AJNConvertUtil.h"

@implementation AJNSRichAudioUrl

/**
 * Constructor for RichAudioUrl
 * @param language Language of Audio Content
 * @param text     Text of Audio Content
 */
- (AJNSRichAudioUrl *)initRichAudioUrlWithLang:(NSString *)language andUrl:(NSString *)url
{
	self = [super init];
	if (self) {
		self.richAudioUrlHandler = new ajn::services::RichAudioUrl([AJNConvertUtil convertNSStringToQCCString:language], [AJNConvertUtil convertNSStringToQCCString:(url)]);
	}
	return self;
}

- (void)setRichAudioUrlHandler:(ajn::services::RichAudioUrl *)richAudioUrlHandler
{
	_richAudioUrlHandler = richAudioUrlHandler;
	_language = [AJNConvertUtil convertQCCStringtoNSString:richAudioUrlHandler->getLanguage()];
	_url = [AJNConvertUtil convertQCCStringtoNSString:richAudioUrlHandler->getUrl()];
}

/**
 * Set Language for Audio Content
 * @param language
 */
- (void)setLanguage:(NSString *)language
{
	self.richAudioUrlHandler->setLanguage([AJNConvertUtil convertNSStringToQCCString:language]);
}

/**
 * Set URL for Audio Content
 * @param url
 */
- (void)setUrl:(NSString *)url
{
	self.richAudioUrlHandler->setUrl([AJNConvertUtil convertNSStringToQCCString:url]);
}

@end