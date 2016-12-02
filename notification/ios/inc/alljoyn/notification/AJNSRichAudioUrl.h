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
#import "alljoyn/notification/RichAudioUrl.h"

/**
 AJNSRichAudioUrl class stores RichAudio urls, a url per language
 */
@interface AJNSRichAudioUrl : NSObject

///---------------------
/// @name Properties
///---------------------
/** richAudioUrlHandler */
@property (nonatomic) ajn::services::RichAudioUrl *richAudioUrlHandler;
@property (strong, nonatomic) NSString *url;
@property (strong, nonatomic) NSString *language;


#pragma mark – Initializers
///---------------------
/// @name Initialization
///---------------------

/**
 Designated initializer - Returns a AJNSRichAudioUrl object by a given parameters.
 @param language Language of Audio Content
 @param url URL for Audio Content
 */
- (AJNSRichAudioUrl *)initRichAudioUrlWithLang:(NSString *)language andUrl:(NSString *)url;

- (void)setUrl:(NSString *)url;

- (void)setLanguage:(NSString *)language;

@end