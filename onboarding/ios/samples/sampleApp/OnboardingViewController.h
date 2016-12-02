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
#import "ClientInformation.h"

static NSString * const AJ_AP_PREFIX = @"AJ_";
static NSString * const AJ_AP_SUFFIX = @"_AJ";

@interface OnboardingViewController : UIViewController
@property (strong, nonatomic) AJNBusAttachment *clientBusName;
@property (weak, nonatomic) ClientInformation *clientInformation;

@property (weak, nonatomic) IBOutlet UITextField *ssidTextField;
@property (weak, nonatomic) IBOutlet UITextField *ssidPassTextField;
@property (weak, nonatomic) IBOutlet UIButton *configureBtn;
@property (weak, nonatomic) IBOutlet UIButton *connectBtn;
@property (weak, nonatomic) IBOutlet UIButton *offBoardingBtn;
@property (weak, nonatomic) IBOutlet UILabel *onboardTitleLbl;
@property (weak, nonatomic) IBOutlet UILabel *ssidLbl;
@property (weak, nonatomic) IBOutlet UILabel *ssidPassLbl;
@property (weak, nonatomic) IBOutlet UILabel *statusLbl;
@property (weak, nonatomic) IBOutlet UILabel *instructLbl;

- (IBAction)configureBtnDidTouchUpInside:(id)sender;

- (IBAction)connectBtnDidTouchUpInside:(id)sender;

- (IBAction)offBoardingBtnDidTouchUpInside:(id)sender;

@end