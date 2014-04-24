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

#import <Foundation/Foundation.h>
#import "RichAudioUrl.h"

/**
 AJNSRichAudioUrl class stores RichAudio urls, a url per language
 */
@interface AJNSRichAudioUrl : NSObject

///---------------------
/// @name Properties
///---------------------
/** richAudioUrlHandler */
@property (nonatomic)ajn::services::RichAudioUrl * richAudioUrlHandler;
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