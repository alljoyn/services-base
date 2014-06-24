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

#import "ConfigSetupViewController.h"
#import "qcc/String.h"
#import "AJNMessageArgument.h"
#import "alljoyn/about/AJNAboutIconClient.h"
#import "alljoyn/about/AJNConvertUtil.h"
#import "alljoyn/config/AJCFGConfigLogger.h"
#import "alljoyn/about/AJNAboutDataConverter.h"


@interface ConfigSetupViewController  () <UITextFieldDelegate, AJNSessionListener>
@property (nonatomic) AJNSessionId sessionId;

@property (strong, nonatomic) NSMutableDictionary *writableElements;
@property (strong, nonatomic) NSString *annBusName;
@property (nonatomic) UIAlertView *setPasswordAlert;
@property (nonatomic) UIAlertView *alertNoSession;

@end

@implementation ConfigSetupViewController

- (void)hasPasscodeInput:(NSNotification *)notification
{
	if ([notification.name isEqualToString:@"passcodeForBus"]) {
		if ([notification.object isEqualToString:self.annBusName]) {
			[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:@"Successfully received PasscodeInput notification!"];
			[self updateWritableDictionary];
			[self updateUI];
		}
	}
}

- (void)prepareAlerts
{
	/* setPasswordAlert.tag = 1 */
	self.setPasswordAlert = [[UIAlertView alloc] initWithTitle:@"" message:@"Enter device password" delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
	self.setPasswordAlert.alertViewStyle = UIAlertViewStylePlainTextInput;
	self.setPasswordAlert.tag = 1;
    
	//  alertNoSession.tag = 2
	self.alertNoSession = [[UIAlertView alloc] initWithTitle:@"Error" message:@"Session is not connected, check the connection and reconnect." delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
	self.alertNoSession.alertViewStyle = UIAlertViewStyleDefault;
	self.alertNoSession.tag = 2;
}

//  Get the user's input from the alert dialog
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	QStatus status;
	switch (alertView.tag) {
		case 1: // setPasswordAlert
		{
			if (buttonIndex == 1) { //user pressed OK
				// Get the password input
				NSString *pass = [self.setPasswordAlert textFieldAtIndex:0].text;
				[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:[NSString stringWithFormat:@"Trying to set Passcode to %@", pass]];
				// Validate
                
				// Prepare password for sending
                                
                NSString *guid = [self.clientBusAttachment guidForPeerNamed:self.annBusName];
				status = [self.clientBusAttachment clearKeysForRemotePeerWithId:guid];
                
				if (ER_OK == status) {
					[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:@"Successfully clearKeysForRemotePeer"];
				}
				else {
					[[[AJCFGConfigLogger sharedInstance] logger] errorTag:[[self class] description] text:[NSString stringWithFormat:@"Failed to clearKeysForRemotePeer: %@", [AJNStatus descriptionForStatusCode:status]]];
				}

				NSData *passcodeData = [pass dataUsingEncoding:NSUTF8StringEncoding];
				const void *bytes = [passcodeData bytes];
				int length = [passcodeData length];
                
				// Set new password
				status = [self.configClient setPasscodeWithBus:self.annBusName daemonRealm:self.realmBusName newPasscodeSize:length newPasscode:(const uint8_t *)bytes sessionId:self.sessionId];
                
				if (ER_OK == status) {
					[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:@"Successfully setPasscodeWithBus"];
					// add passcode to peersPass
					(self.peersPasscodes)[self.annBusName] = pass;
					[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:[NSString stringWithFormat:@"update peer %@ with passcode %@", self.annBusName, pass]];
				}
				else {
					[[[AJCFGConfigLogger sharedInstance] logger] errorTag:[[self class] description] text:[NSString stringWithFormat:@"Failed to setPasscodeWithBus: %@", [AJNStatus descriptionForStatusCode:status]]];
				}
                

			}
		}
            break;
            
		case 2: //NoSessionAlert
		{
		}
            break;
            
		default:
			[[[AJCFGConfigLogger sharedInstance] logger] errorTag:[[self class] description] text:@"alertView.tag is wrong"];
			break;
	}
}

- (void)updateWritableDictionary
{
	QStatus status;
    
	self.annBusName = [self.clientInformation.announcement busName];
	NSMutableDictionary *configDict = [[NSMutableDictionary alloc] init];
	status = [self.configClient configurationsWithBus:self.annBusName languageTag:@"" configs:&configDict sessionId:self.sessionId];
	if (status != ER_OK) {
		[[[AJCFGConfigLogger sharedInstance] logger] errorTag:[[self class] description] text:[NSString stringWithFormat:@"Failed to get configuration from bus: %@", [AJNStatus descriptionForStatusCode:status]]];
        [[[UIAlertView alloc] initWithTitle:@"Error" message:@"Failed to get configuration" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
        // disable buttons
		[self.btnFactoryReset setEnabled:NO];
		[self.btnRestart setEnabled:NO];
		[self.btnSetPassword setEnabled:NO];
        
        self.writableElements = nil;
	}
	else {
		[self.btnFactoryReset setEnabled:YES];
		[self.btnRestart setEnabled:YES];
		[self.btnSetPassword setEnabled:YES];
        
        self.writableElements = configDict;
        
        [self.clientInformation setCurrLang:[self getConfigurableValueForKey:DEFAULT_LANGUAGE_STR]];
        
        NSLog(@"updateWritableDictionary count %d",[self.writableElements count]);
	}
    

}

- (void)viewDidLoad
{
	[super viewDidLoad];
    
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(hasPasscodeInput:) name:@"passcodeForBus" object:nil];
    
	[self prepareAlerts];
    
	[self loadSession];
    
    int ver = 0;
    
    [self.configClient versionWithBus:[self.clientInformation.announcement busName] version:ver];
    
    NSLog(@"%d",ver);
}

- (void)loadSession
{
	if (!self.sessionId) {
		//create sessionOptions
		AJNSessionOptions *opt = [[AJNSessionOptions alloc] initWithTrafficType:(kAJNTrafficMessages) supportsMultipoint:(false) proximity:(kAJNProximityAny) transportMask:(kAJNTransportMaskAny)];
        
        
		//call joinSession
		self.sessionId = [self.clientBusAttachment joinSessionWithName:[self.clientInformation.announcement busName] onPort:[self.clientInformation.announcement port] withDelegate:self options:opt];
	}
    
	// Session is not connected
	if (self.sessionId == 0 || self.sessionId == -1) {
		[[[AJCFGConfigLogger sharedInstance] logger] errorTag:[[self class] description] text:[NSString stringWithFormat:@"Failed to join session. sid = %u", self.sessionId]];
        
		[self.alertNoSession show];
	}
    
	[self updateWritableDictionary];
    
	[self updateUI];
}

- (NSString *)getConfigurableValueForKey:(NSString *)key
{
    AJNMessageArgument *msgArg = [self.writableElements valueForKey:key];
    
    
    return [AJNAboutDataConverter messageArgumentToString:msgArg];
}

- (void)updateUI
{
	int i = 0;
    
	// generate UI based on the received writableElements
	for (NSString *key in[self.writableElements allKeys]) {
		CGFloat y = 80 + (i * 70);
		UILabel *lbl = [[UILabel alloc] initWithFrame:CGRectMake(20, y, 275, 40)];
		lbl.backgroundColor = [UIColor clearColor];
		[lbl setTextColor:[UIColor blackColor]];
		[lbl setFont:[UIFont systemFontOfSize:15]];
		lbl.text = [NSString stringWithFormat:@"%@:", key];
		lbl.tag = i;
		[self.view addSubview:lbl];
        
		UITextField *txt = [[UITextField alloc] initWithFrame:CGRectMake(160, y, 275, 40)];
        
        [txt resignFirstResponder];
        [txt setSpellCheckingType:UITextSpellCheckingTypeNo];
        [txt setAutocapitalizationType:UITextAutocapitalizationTypeNone];
        [txt becomeFirstResponder];
        
		txt.text = [self getConfigurableValueForKey:key];
        
		txt.delegate = self;
		txt.tag = i;
        
		[self.view addSubview:txt];
        
        
        UIButton *resetButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        
        [resetButton setFrame:CGRectMake(20, y+40, 275, 40)];
        resetButton.tag = i;
        
        [resetButton setTitle:[NSString stringWithFormat:@"Reset %@",key] forState:UIControlStateNormal];
        [resetButton setContentHorizontalAlignment:UIControlContentHorizontalAlignmentLeft];
        
        [resetButton addTarget:self
                       action:@selector(resetButtonTouchUpInside:)
             forControlEvents: UIControlEventTouchUpInside];
        
        [self.view addSubview:resetButton];
        
		i++;
	}
}

-(void)resetButtonTouchUpInside:(UIButton *)resetButton{
    
    QStatus status;
    
	NSMutableArray *configNames = [[NSMutableArray alloc]init];
    
	// Get the property name
	NSString *key = [self.writableElements allKeys][resetButton.tag];
    
    [configNames addObject:key];
    
    status = [self.configClient resetConfigurationsWithBus:self.annBusName languageTag:[self.clientInformation currLang] configNames:configNames sessionId:self.sessionId];

	if (status != ER_OK) {
		[[[UIAlertView alloc] initWithTitle:@"Reset Property Store Failed" message:[NSString stringWithFormat:@"Error occured:%@", [AJNStatus descriptionForStatusCode:status]] delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
        
		[[[AJCFGConfigLogger sharedInstance] logger] errorTag:[[self class] description] text:[NSString stringWithFormat:@"Failed to reset Property Store for key '%@': %@", key, [AJNStatus descriptionForStatusCode:status]]];
	}
	else {
		[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:[NSString stringWithFormat:@"Successfully reset Property Store for key '%@'", key]];
        
        
        [self updateWritableDictionary];
        
        [self updateTextfieldValues];
	}
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
	QStatus status;
    
	NSMutableDictionary *configElements = [[NSMutableDictionary alloc] init];
    
	// Get the property name
	NSString *key = [self.writableElements allKeys][textField.tag];
	// Get the property value
	AJNMessageArgument *msgArgValue = [[AJNMessageArgument alloc] init];
	const char *char_str_value = [AJNConvertUtil convertNSStringToConstChar:textField.text];
	[msgArgValue setValue:@"s", char_str_value];
    
	// Add the property name/value
	configElements[key] = msgArgValue;
    

    NSString *useLang;
    if ([key isEqualToString:DEFAULT_LANGUAGE_STR]) {
        useLang = @"";
    } else {
        useLang = [self.clientInformation currLang];
    }
    
	status = [self.configClient updateConfigurationsWithBus:self.annBusName languageTag:useLang configs:&configElements sessionId:self.sessionId];
    
    
    
	if (status != ER_OK) {
        NSString *str= [NSString stringWithFormat:@"Failed to update property '%@' to '%s'", key, char_str_value ];
                        
		[[[UIAlertView alloc] initWithTitle:@"Update Property Store Failed" message:str delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
        
		[[[AJCFGConfigLogger sharedInstance] logger] errorTag:[[self class] description] text:str];
	}
	else {
		[[[AJCFGConfigLogger sharedInstance] logger] debugTag:[[self class] description] text:[NSString stringWithFormat:@"Successfully update Property Store with %@ = %s for tag[%d]", key, char_str_value, textField.tag]];
        
        
        [self updateWritableDictionary];
        
        [self updateTextfieldValues];

	}
    
}

- (void)disableAllDynamicFields
{
    for (UIView *subView in[self.view subviews]) {
		if ([subView isKindOfClass:[UITextField class]]) {
            [((UITextField*)subView) setEnabled:NO];
		}
        if ([subView isKindOfClass:[UIButton class]]) {
            [((UIButton*)subView) setEnabled:NO];
		}
        
	}
}

- (void)updateTextfieldValues
{
    if ([self.writableElements count] == 0) {
        [[[UIAlertView alloc]initWithTitle:@"no configurable elements" message:@"the server did not return any configurable elements" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
        
        [self disableAllDynamicFields];
        return; // no elements to show
    }
    
    for (int x = 0; x != [self.writableElements count]; x++) {
		for (UIView *aSubview in[self.view subviews]) {
			if ([aSubview isKindOfClass:[UITextField class]]) {
				NSString *key = [self.writableElements allKeys][((UITextField *)aSubview).tag];
                
				((UITextField *)aSubview).text = [self getConfigurableValueForKey:key];
			}
		}
	}
}

#pragma mark - IBAction methods
- (IBAction)factoryResetPressed:(UIButton *)sender
{
	QStatus status = [self.configClient factoryResetWithBus:self.annBusName sessionId:self.sessionId];
    
    if (status!=ER_OK) {
        [[[UIAlertView alloc]initWithTitle:@"Factory reset failed" message:[NSString stringWithFormat:@"Factory reset failed with error:%@",[AJNStatus descriptionForStatusCode:status]] delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
    } else {
        [[[UIAlertView alloc]initWithTitle:@"Factory reset success" message:@"Factory reset done. Your wifi connection may have changed." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
    }
    
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)setPasswordPressed:(id)sender
{
    [self disableAllDynamicFields];
    
	[self.setPasswordAlert show];
}

- (IBAction)restartPressed:(UIButton *)sender
{
	[self.configClient restartWithBus:self.annBusName sessionId:self.sessionId];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	[textField resignFirstResponder];
	return YES;
}

- (void)viewWillDisappear:(BOOL)animated
{
	[self.clientBusAttachment leaveSession:self.sessionId];
	
    for (UIView *subView in[self.view subviews]) {
		if ([subView isKindOfClass:[UITextField class]]) {
            ((UITextField*)subView).delegate = nil;
		}
	}
	
    [super viewWillDisappear:animated];
}

- (void)sessionWasLost:(AJNSessionId)sessionId forReason:(AJNSessionLostReason)reason
{
    NSLog(@"session on bus %@ lost. reason:%d",self.annBusName,reason);
    [self.navigationController popViewControllerAnimated:YES];
}

@end
