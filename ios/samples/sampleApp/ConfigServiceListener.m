/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
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

#import "ConfigServiceListener.h"
#import "alljoyn/about/AJNAboutServiceApi.h"
#import "AJCFGConfigLogger.h"

@interface ConfigServiceListener ()

@property AJCFGPropertyStoreImpl *propertyStore;

@property AJNBusAttachment *bus;

@end

@implementation ConfigServiceListener

- (id)initWithPropertyStore:(AJCFGPropertyStoreImpl *)propertyStore andBus:(AJNBusAttachment *)bus
{
	self = [super init];
	if (self) {
		self.propertyStore = propertyStore;
		self.bus = bus;
	}
	return self;
}

- (QStatus)restart
{
	[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:@"Restart has been called !"];
	return ER_OK;
}

- (QStatus)factoryReset
{
	[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:@"Factory Reset called"];
    
	[self.propertyStore factoryReset];
    
	[self.bus clearKeyStore];
    
	return [[AJNAboutServiceApi sharedInstance] announce];
}

- (QStatus)setPassphrase:(NSString *)daemonRealm withPasscode:(NSString *)passcode
{
	QStatus status = [self.propertyStore setPasscode:passcode];
    
	[self.bus clearKeyStore];
    
	return status;
}

@end