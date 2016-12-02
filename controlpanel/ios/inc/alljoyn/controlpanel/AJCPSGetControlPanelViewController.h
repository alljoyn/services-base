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

#import <UIKit/UIKit.h>
#import "alljoyn/about/AJNAnnouncement.h"
#import "AJNBusAttachment.h"
#import "AJCPSControllerModel.h"
#import "AJNAboutObjectDescription.h"

@interface AJCPSGetControlPanelViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>

- (id)initWithNotificationSenderBusName:(NSString *)senderBusName cpsObjectPath:(NSString *)cpsObjectPath bus:(AJNBusAttachment *)bus;
- (id)initWithBusName:(NSString *)busName objectDescription:(AJNAboutObjectDescription *)objDesc bus:(AJNBusAttachment *)bus;

@end