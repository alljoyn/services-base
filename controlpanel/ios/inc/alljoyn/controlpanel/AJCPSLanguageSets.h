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
#import "alljoyn/controlpanel/LanguageSets.h"
#import "AJCPSLanguageSet.h"

/**
 * AJCPSLanguageSets is used to store the LanguageSets defined.
 */
@interface AJCPSLanguageSets : NSObject
- (id)initWithHandle:(ajn ::services ::LanguageSets *)handle;

/**
 * Get a LanguageSet
 * @param languageSetName - the name of the languageSet to get
 * @return the languageSet requested or NULL if it does not exist
 */
- (AJCPSLanguageSet *)getLanguageSet:(NSString *)languageSetName;

@end