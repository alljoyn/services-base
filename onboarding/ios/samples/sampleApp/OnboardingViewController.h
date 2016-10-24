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
#import "AJNBusAttachment.h"
#import "OnboardingStartedListener.h"
#import "samples_common/AJSCClientInformation.h"

static NSString *const AJ_AP_PREFIX = @"AJ_";
static NSString *const AJ_AP_SUFFIX = @"_AJ";

@interface OnboardingViewController : UIViewController <UIPickerViewDelegate, UIPickerViewDataSource>

@property (strong, nonatomic) AJNBusAttachment *clientBusName;
@property (strong, nonatomic) IBOutlet id <OnboardingStartedListener> onboardingStartedListener;
@property (weak, nonatomic) AJSCClientInformation *clientInformation;

@property (weak, nonatomic) IBOutlet UITextField *ssidTextField;
@property (weak, nonatomic) IBOutlet UITextField *ssidPassTextField;
@property (weak, nonatomic) IBOutlet UIButton *configureBtn;
@property (weak, nonatomic) IBOutlet UIButton *connectBtn;
@property (weak, nonatomic) IBOutlet UIButton *offBoardingBtn;
@property (weak, nonatomic) IBOutlet UIButton *scanInfoBtn;
@property (weak, nonatomic) IBOutlet UILabel *onboardTitleLbl;
@property (weak, nonatomic) IBOutlet UILabel *ssidLbl;
@property (weak, nonatomic) IBOutlet UILabel *ssidPassLbl;
@property (weak, nonatomic) IBOutlet UILabel *statusLbl;
@property (weak, nonatomic) IBOutlet UILabel *instructLbl;
@property (weak, nonatomic) IBOutlet UILabel *authLabel;
@property (weak, nonatomic) IBOutlet UITextField *authTextField;
@property (weak, nonatomic) IBOutlet UITextField *networkTextField;
@property (strong, nonatomic) UIPickerView *authTypePicker;
@property (strong, nonatomic) UIPickerView *networkPicker;

- (IBAction)configureBtnDidTouchUpInside:(id)sender;

- (IBAction)connectBtnDidTouchUpInside:(id)sender;

- (IBAction)offBoardingBtnDidTouchUpInside:(id)sender;

- (IBAction)getScanInfo:(id)sender;

@end