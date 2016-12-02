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

@interface ProducerViewController : UIViewController <UITextFieldDelegate>

@property (weak, nonatomic) UIViewController *mainVC;

@property (weak, nonatomic) IBOutlet UITextField *notificationEnTextField;
@property (weak, nonatomic) IBOutlet UITextField *notificationLangTextField;
@property (weak, nonatomic) IBOutlet UITextField *ttlTextField;
@property (weak, nonatomic) IBOutlet UITextField *audioTextField;
@property (weak, nonatomic) IBOutlet UITextField *iconTextField;

@property (weak, nonatomic) IBOutlet UILabel *defaultLangLabel;
@property (weak, nonatomic) IBOutlet UILabel *ttlLabel;
@property (weak, nonatomic) IBOutlet UILabel *audioLabel;
@property (weak, nonatomic) IBOutlet UILabel *iconLabel;
@property (weak, nonatomic) IBOutlet UILabel *messageTypeLabel;

@property (weak, nonatomic) IBOutlet UISwitch *audioSwitch;
@property (weak, nonatomic) IBOutlet UISwitch *iconSwitch;

@property (weak, nonatomic) IBOutlet UIButton *messageTypeButton;
@property (weak, nonatomic) IBOutlet UIButton *sendNotificationButton;
@property (weak, nonatomic) IBOutlet UIButton *deleteButton;
@property (weak, nonatomic) IBOutlet UIButton *langButton;

// Shared properties
@property (strong, nonatomic) AJNBusAttachment *busAttachment;
@property (strong, nonatomic) AJSVCGenericLoggerDefaultImpl *logger;
@property (strong, nonatomic) NSString *appName;

- (IBAction)didTouchSendNotificationButton:(id)sender;

- (IBAction)didChangeAudioSwitchValue:(id)sender;

- (IBAction)didChangeIconSwitchValue:(id)sender;

- (IBAction)didTouchDeleteButton:(id)sender;

- (QStatus)startProducer;

- (void)stopProducer:(bool) isConsumerOn;
@end