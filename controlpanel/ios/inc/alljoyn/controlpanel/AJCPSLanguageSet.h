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

#import <Foundation/Foundation.h>
#import "alljoyn/controlpanel/LanguageSet.h"

/**
 * AJCPSLanguageSet is used to define a LanguageSet
 */
@interface AJCPSLanguageSet : NSObject

- (id)initWithHandle:(ajn ::services ::LanguageSet *)handle;


/**
 * Get the LanguageSetName
 * @return LanguageSetName
 */
- (NSString *)getLanguageSetName;

/**
 * Get the number of Languages defined
 * @return number of Languages
 */
- (size_t)getNumLanguages;

/**
 * Add a language to the LanguageSet
 * @param language - language to Add
 */
- (void)addLanguage:(NSString *)language;

/**
 *
 * Get the Languages defined in the LanguageSet
 * @return languages vector
 */
//const std::vector<qcc::String>&
- (NSArray *)getLanguages;


@end