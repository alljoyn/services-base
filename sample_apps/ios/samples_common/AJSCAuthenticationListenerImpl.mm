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

#import "AJSCAuthenticationListenerImpl.h"
#import "AJSCAlertController.h"

@interface AJSCAuthenticationListenerImpl ()

@property (strong, nonatomic) AJSCAlertController *setPassCodeAlert;
@property (strong, nonatomic) NSString *passCodeText;
@property (strong, nonatomic) NSString *peerName;
@property (strong, nonatomic) NSMutableDictionary *peersPasscodes; // store the peers passcodes
@property (weak, nonatomic) UIViewController *viewController;

@end

@implementation AJSCAuthenticationListenerImpl

- (id)init {
    self = [super init];
	if (self) {
        [self prepareAlerts];
        // Create NSMutableDictionary dictionary of peers passcodes
        self.peersPasscodes = [[NSMutableDictionary alloc] init];
	}
	return self;
}

- (id)initWithViewController:(UIViewController*)viewController
{
    if([self init]){
        _viewController = viewController;
    }
    return nil;
}

- (void)prepareAlerts
{
    self.setPassCodeAlert = [AJSCAlertController alertControllerWithTitle:@""
                                                                  message:@"Enter device password"
                                                           viewController:_viewController];
    [self.setPassCodeAlert addActionWithName:@"Cancel" handler:^(UIAlertAction *action) {
    }];
    
    __weak AJSCAuthenticationListenerImpl *weakSelf = self;
    [self.setPassCodeAlert addActionWithName:@"OK" handler:^(UIAlertAction *action) {
        [weakSelf getInputFromAlertDialog];
    }];
    
    [self.setPassCodeAlert.iosAlertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        textField.text = @"";
    }];
}

// Get the user's input from the alert dialog
- (void)getInputFromAlertDialog
{
    // get the input pass
    self.passCodeText = [self.setPassCodeAlert.iosAlertController textFields][0].text;
    // [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"Passcode is: %@",  self.passCodeText]];
    NSLog(@"Passcode is: %@",  self.passCodeText);
    
    bool foundPeer = false;
    
    // Check that peername is not empty
    if ([self.peerName length]) {
        if (![self.passCodeText length]) {
            // set the pass to default if input is empty
            self.passCodeText = _defaultPasscode;
        }
        
        // Iterate over the dictionary and add/update
        for (NSString *key in self.peersPasscodes.allKeys) {
            if ([key isEqualToString:self.peerName]) {
                // Update passcode for key
                (self.peersPasscodes)[self.peerName] = self.passCodeText;
                // [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"Update peer %@ with passcode %@",  self.peerName, self.passCodeText]];
                NSLog(@"Update peer %@ with passcode %@",  self.peerName, self.passCodeText);
                // Set flag
                foundPeer = true;
                break;
            }
        }
        
        if (!foundPeer) {
            // Add new set of key/value
            [self.peersPasscodes setValue:self.passCodeText forKey:self.peerName];
            // [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"add new peers %@ %@", self.peerName, self.passCodeText]];
            NSLog(@"add new peers %@ %@", self.peerName, self.passCodeText);
            
        }
        
        [[NSNotificationCenter defaultCenter] postNotificationName:@"hasPasscodeForBus" object:self.peerName];
    }
}

#pragma mark - AJNAuthenticationListener protocol methods

- (AJNSecurityCredentials *)requestSecurityCredentialsWithAuthenticationMechanism:(NSString *)authenticationMechanism peerName:(NSString *)peerName authenticationCount:(uint16_t)authenticationCount userName:(NSString *)userName credentialTypeMask:(AJNSecurityCredentialType)mask
{
	AJNSecurityCredentials *creds = nil;
	bool credFound = false;
    
    NSLog(@"requestSecurityCredentialsWithAuthenticationMechanism:%@ forRemotePeer%@ userName:%@", authenticationMechanism, peerName, userName);
    
	if ([authenticationMechanism isEqualToString:@"ALLJOYN_SRP_KEYX"] || [authenticationMechanism isEqualToString:@"ALLJOYN_ECDHE_PSK"]) {
		if (mask & kAJNSecurityCredentialTypePassword) {
			if (authenticationCount <= 3) {
				creds = [[AJNSecurityCredentials alloc] init];
                
				// Check if the password stored in peersPasscodes
				for (NSString *key in self.peersPasscodes.allKeys) {
					if ([key isEqualToString:peerName]) {
						creds.password = (self.peersPasscodes)[key];
                        NSLog(@"Found password %@ for peer %@", creds.password, key);
                        
						credFound = true;
						break;
					}
				}
				// Use the default password
				if (!credFound) {
					creds.password = _defaultPasscode;
                    NSLog(@"Using default password %@ for peer %@", _defaultPasscode, peerName);
                    
				}
			}
		}
	}
	return creds;
}

- (void)authenticationUsing:(NSString *)authenticationMechanism forRemotePeer:(NSString *)peerName didCompleteWithStatus:(BOOL)success
{
	NSString *status;
	status = (success == YES ? @"was successful" : @"failed");
    
    NSLog(@"authenticationUsing:%@ forRemotePeer%@ %@", authenticationMechanism, peerName, status);
    
    //	[self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"authenticationUsing:%@ forRemotePeer%@ %@", authenticationMechanism, peerName, status]];
    
	//get the passcode for this bus
	if (!success) {
		self.peerName = peerName;
		self.passCodeText = nil;
		dispatch_async(dispatch_get_main_queue(), ^{
		    [self.setPassCodeAlert show];
		});
	}
}

@end
