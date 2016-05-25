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
//

#import "GetAboutCallViewController.h"
#import "samples_common/AJSCAlertController.h"

static NSString * const CLIENTDEFAULTLANG=@"";

@interface GetAboutCallViewController ()
@property (weak, nonatomic) IBOutlet UILabel *lblVersion;
@property (weak, nonatomic) IBOutlet UILabel *lblAboutLanguage;
@property (weak, nonatomic) IBOutlet UITextView *txtViewBusObjectDesc;
@property (weak, nonatomic) IBOutlet UITextView *txtViewAboutMap;
@property (weak, nonatomic) IBOutlet UIButton *optionsButton;

@property (strong, nonatomic) AJNMessageArgument *supportedLanguagesMsgArg;
@property (nonatomic) AJNSessionId sessionId;
@property (nonatomic) AJSCAlertController *alertBusName;
@property (nonatomic) UITextField *alertChooseLanguage;
@property (nonatomic) AJSCAlertController *alertAnnouncementOptions;
@property (nonatomic) AJSCAlertController *alertNoSession;

@end

@implementation GetAboutCallViewController

- (void)prepareAlerts
{
    [self prepareBusNameAlert];
    [self prepareAnnouncementOptionsAlert];
    [self prepareAlertNoSession];
}

- (void)prepareBusNameAlert
{
    __weak GetAboutCallViewController *weakSelf = self;
    
    self.alertBusName = [AJSCAlertController alertControllerWithTitle:@"Set language"
                                                              message:@""
                                                       viewController:self];
    
    [self.alertBusName addActionWithName:@"Cancel" handler:^(UIAlertAction *action) {
    }];
    
    [self.alertBusName addActionWithName:@"OK" handler:^(UIAlertAction *action) {
        if ([weakSelf.alertChooseLanguage.text isEqualToString:@""]) {
            weakSelf.alertChooseLanguage.text = [AJNAboutDataConverter messageArgumentToString:[weakSelf.clientInformation.announcement aboutData][@"DefaultLanguage"]];
        }
        if (![weakSelf isValidLanguage:weakSelf.alertChooseLanguage.text]) {
            AJSCAlertController *errorAlert = [AJSCAlertController alertControllerWithTitle:@"Error"
                                                                                    message:@"Requested language is not supported"
                                                                             viewController:weakSelf];
            [errorAlert addActionWithName:@"OK" handler:^(UIAlertAction *action) {
            }];
            [errorAlert show];
            return;
        }
        
        weakSelf.clientInformation.currLang = weakSelf.alertChooseLanguage.text;
        
        [weakSelf UpdateCallViewInformation];
    }];
    
    [self.alertBusName.iosAlertController addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.text = @"";
        weakSelf.alertChooseLanguage = textField;
    }];
}

- (void)prepareAnnouncementOptionsAlert
{
    __weak GetAboutCallViewController *weakSelf = self;
    
    self.alertAnnouncementOptions = [AJSCAlertController alertControllerWithTitle:@"Choose option:"
                                                                          message:@""
                                                                   viewController:self];
    
    [self.alertAnnouncementOptions addActionWithName:@"Cancel" handler:^(UIAlertAction *action) {
    }];
    
    [self.alertAnnouncementOptions addActionWithName:@"Refresh" handler:^(UIAlertAction *action) {
        [weakSelf UpdateCallViewInformation];
    }];
    
    [self.alertAnnouncementOptions addActionWithName:@"Set Language" handler:^(UIAlertAction *action) {
        weakSelf.alertBusName.iosAlertController.message = [NSString stringWithFormat:@"Available:%@", [AJNAboutDataConverter messageArgumentToString:weakSelf.supportedLanguagesMsgArg]];
        [weakSelf.alertBusName show];
    }];
}

- (void)prepareAlertNoSession
{
    self.alertNoSession = [AJSCAlertController alertControllerWithTitle:@"Error"
                                                                message:@"Session is not connected, check the connection and reconnect."
                                                         viewController:self];
    
    [self.alertNoSession addActionWithName:@"OK" handler:^(UIAlertAction *action) {
    }];
}

- (IBAction)TouchUpInsideRefreshandSetLanguage:(UIButton *)sender
{
	[self.alertAnnouncementOptions show];
}

- (bool)isValidLanguage:(NSString *)inputLanguage
{
	bool found = false;
	const ajn::MsgArg *stringArray;
	size_t fieldListNumElements;
	QStatus status = [self.supportedLanguagesMsgArg value:@"as", &fieldListNumElements, &stringArray];
	if (status == ER_OK) {
		for (size_t i = 0; i < fieldListNumElements; i++) {
			char *tempString;
			stringArray[i].Get("s", &tempString);
			if ([inputLanguage isEqualToString:@(tempString)]) {
				found = true;
				break;
			}
		}
	}
	return found;
}

- (void)UpdateCallViewInformation
{
	self.lblVersion.text = [NSString stringWithFormat:@"%u", [self.clientInformation.announcement version]];
    
	if (!self.sessionId) {
		//create sessionOptions
		AJNSessionOptions *opt = [[AJNSessionOptions alloc] initWithTrafficType:kAJNTrafficMessages supportsMultipoint:false proximity:kAJNProximityAny transportMask:kAJNTransportMaskAny];
		//call joinSession
		self.sessionId = [self.clientBusAttachment
		                  joinSessionWithName:[self.clientInformation.announcement busName]
                          onPort:[self.clientInformation.announcement port]
                          withDelegate:(nil) options:opt];
	}
    
	if (self.sessionId == 0 || self.sessionId == -1) {
         NSLog(@"[%@] [%@] Failed to join session. sid=%u", @"DEBUG", [[self class] description],self.sessionId);

		[self.alertNoSession show];
	}
	else {
		NSMutableDictionary *aboutData;
		NSMutableDictionary *objDesc;
        
		AJNAboutClient *ajnAboutClient = [[AJNAboutClient alloc] initWithBus:self.clientBusAttachment];
        
        QStatus qStatus = [ajnAboutClient aboutDataWithBusName:[self.clientInformation.announcement busName] andLanguageTag:self.clientInformation.currLang andAboutData:&aboutData andSessionId:self.sessionId];
        
        if (qStatus != ER_OK) {
            AJSCAlertController *errorAlert = [AJSCAlertController alertControllerWithTitle:@"Error"
                                                                                    message:@"Calling the about method returned with an error"
                                                                             viewController:self];
            [errorAlert addActionWithName:@"OK" handler:^(UIAlertAction *action) {}];
            [errorAlert show];
        }
        else {
            [ajnAboutClient objectDescriptionsWithBusName:[self.clientInformation.announcement busName] andObjectDescriptions:&objDesc andSessionId:self.sessionId];
            
            NSLog(@"[%@] [%@] AboutData:  %@", @"DEBUG", [[self class] description], [AJNAboutDataConverter aboutDataDictionaryToString:aboutData]);
            
            NSLog(@"[%@] [%@] objectDescriptions:  %@", @"DEBUG", [[self class] description], [AJNAboutDataConverter objectDescriptionsDictionaryToString:objDesc]);

            self.supportedLanguagesMsgArg = aboutData[@"SupportedLanguages"];
            
            self.lblAboutLanguage.text = self.clientInformation.currLang;
            self.txtViewAboutMap.text = [AJNAboutDataConverter aboutDataDictionaryToString:aboutData];
            self.txtViewBusObjectDesc.text = [AJNAboutDataConverter objectDescriptionsDictionaryToString:objDesc];
        }
    }
}

- (void)viewDidLoad
{
	[super viewDidLoad];
    
	[self prepareAlerts];
    
	[self UpdateCallViewInformation];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[self.clientBusAttachment leaveSession:self.sessionId];
    
	[super viewWillDisappear:animated];
}

@end
