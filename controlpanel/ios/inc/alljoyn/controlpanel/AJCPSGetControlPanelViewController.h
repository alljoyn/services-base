/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
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

#import <UIKit/UIKit.h>
#import "alljoyn/about/AJNAnnouncement.h"
#import "AJNBusAttachment.h"
#import "AJCPSControllerModel.h"

@interface AJCPSGetControlPanelViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>

- (id)initWithNotificationSenderBusName:(NSString*) senderBusName cpsObjectPath:(NSString*) cpsObjectPath bus:(AJNBusAttachment*) bus;
- (id)initWithAnnouncement:(AJNAnnouncement*) announcement bus:(AJNBusAttachment*) bus;

// ASABASE-774 - Added temporarily to aid in deprecation cleanup without relying directly on
// sample common's AJSCAboutAnnouncement from ControlPanel service.
// This should eventually be replaced with the following:
// - (id)initWithBusName:(NSString*) busName aboutDataArg:(AJNMessageArgument*) aboutDataArg;
- (id)initWithBusName:(NSString*) busName objectDescriptions:(NSMutableDictionary*) objectDescriptions bus:(AJNBusAttachment*) bus;

@end
