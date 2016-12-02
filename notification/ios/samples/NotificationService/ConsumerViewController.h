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
#import "AJNBusAttachment.h"
#import "alljoyn/services_common/AJSVCGenericLoggerDefaultImpl.h"

@interface ConsumerViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>


@property (weak, nonatomic) UIViewController *mainVC;

@property (weak, nonatomic) IBOutlet UIButton *consumerLangButton;

@property (weak, nonatomic) IBOutlet UIButton *dismissChosen;

@property (weak, nonatomic) IBOutlet UIButton *actionOnChosen;

@property (weak, nonatomic) IBOutlet UITableView *notificationTableView;



// Shared
@property (strong, nonatomic) AJNBusAttachment *busAttachment;

@property (strong, nonatomic) AJSVCGenericLoggerDefaultImpl *logger;

@property (strong, nonatomic) NSString *appName;

@property (strong, nonatomic) NSString *consumerLang;

- (QStatus)startConsumer;

- (void)stopConsumer:(bool) isProducerOn;

@end