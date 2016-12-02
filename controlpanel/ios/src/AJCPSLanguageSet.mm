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

#import "AJCPSLanguageSet.h"
#import "alljoyn/about/AJNConvertUtil.h"

@interface AJCPSLanguageSet ()
@property (nonatomic) ajn::services::LanguageSet *handle;
@end


@implementation AJCPSLanguageSet

- (id)initWithHandle:(ajn::services::LanguageSet *)handle
{
	self = [super init];
	if (self) {
		self.handle = handle;
	}
	return self;
}

- (NSString *)getLanguageSetName
{
	return [AJNConvertUtil convertQCCStringtoNSString:self.handle->getLanguageSetName()];
}

- (size_t)getNumLanguages
{
	return self.handle->getNumLanguages();
}

- (void)addLanguage:(NSString *)language
{
	return self.handle->addLanguage([AJNConvertUtil convertNSStringToQCCString:language]);
}

- (NSArray *)getLanguages
{
    if (!self.handle)
        return nil;
    
	const std::vector <qcc::String> cpp_languages = self.handle->getLanguages();
    
	NSMutableArray *languages = [[NSMutableArray alloc]init];
    
	for (int i = 0; i != cpp_languages.size(); i++) {
		[languages addObject:[AJNConvertUtil convertQCCStringtoNSString:cpp_languages.at(i)]];
	}
    
	return languages;
}

@end