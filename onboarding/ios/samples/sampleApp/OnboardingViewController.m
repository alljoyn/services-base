/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
 *    Project (AJOSP) Contributors and others.
 *    
 *    SPDX-License-Identifier: Apache-2.0
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *    
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *    
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *    
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
******************************************************************************/

#import "OnboardingViewController.h"
#import "alljoyn/onboarding/AJOBSOnboardingClient.h"
#import "SystemConfiguration/CaptiveNetwork.h"
#import "samples_common/AJSCAlertController.h"
#import "samples_common/AJSCAlertControllerManager.h"
#import "alljoyn/onboarding/AJOBScanInfo.h"

#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v) ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)

static NSString * const AUTH_OPEN_STR = @"OPEN";
static NSString * const AUTH_WEP_STR = @"WEP";
static NSString * const AUTH_WPA_AUTO_STR = @"WPA_AUTO";
static NSString * const AUTH_WPA_CCMP_STR = @"WPA_CCMP";
static NSString * const AUTH_WPA_TKIP_STR = @"WPA_TKIP";
static NSString * const AUTH_WPA2_AUTO_STR = @"WPA2_AUTO";
static NSString * const AUTH_WPA2_CCMP_STR = @"WPA2_CCMP";
static NSString * const AUTH_WPA2_TKIP_STR = @"WPA2_TKIP";
static NSString * const AUTH_WPS_STR = @"WPS";

@interface OnboardingViewController () <AJOBOnboardingClientListener>

typedef NS_ENUM(NSInteger, ONBOARDING_PICKER) {
    AUTH_PICKER,
    NETWORK_PICKER
};

@property (strong, nonatomic) AJOBSOnboardingClient *onboardingClient;
@property (strong, nonatomic) NSString *onboardeeBus;
@property (strong, nonatomic) NSArray *authPickerData;
@property (strong, nonatomic) NSMutableArray *onboardeeNetworks;
@end

@implementation OnboardingViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    // UI
    [self.statusLbl setText:@"  "];
    [self.instructLbl setText:@"  "];
    [self displayPreOnbordingElements:0];
    self.offBoardingBtn.alpha = 0;

    [self updateStatusLabel:@"Loading Onboarding client"];
    QStatus status = [self startOnboardingClient];
    if (ER_OK != status) {
        AJSCAlertController *alertController = [AJSCAlertController alertControllerWithTitle:@"Error"
                                                message:@"Failed to start onboarding client"
                                                viewController:self];
        [alertController addActionWithName:@"OK" handler:^(UIAlertAction *action) { }];
        [alertController show];
    }

    self.networkPicker = [[UIPickerView alloc] init];
    [self initPicker:self.networkPicker];
    self.networkPicker.tag = NETWORK_PICKER;
    
    self.authTypePicker = [[UIPickerView alloc] init];
    [self initPicker:self.authTypePicker];
    self.authTypePicker.tag = AUTH_PICKER;
    
    self.authPickerData = @[AUTH_OPEN_STR, AUTH_WEP_STR, AUTH_WPA_AUTO_STR, AUTH_WPA_CCMP_STR, AUTH_WPA_TKIP_STR, AUTH_WPA2_AUTO_STR, AUTH_WPA2_CCMP_STR, AUTH_WPA2_TKIP_STR, AUTH_WPS_STR];

    if ([self.authTextField.text length] <= 0) {
        self.authTextField.text = self.authPickerData[0];
    }

    self.authTextField.inputView = self.authTypePicker;
    self.networkTextField.inputView = self.networkPicker;
    
    self.networkTextField.text = @"Select a network...";
}

- (void)initPicker:(UIPickerView *)pickerView
{
    pickerView.delegate = self;
    pickerView.dataSource = self;
    pickerView.showsSelectionIndicator = YES;
    pickerView.hidden = NO;
}

- (void)displayPreOnbordingElements:(CGFloat)alpha
{
    //text
    self.ssidTextField.alpha = alpha;
    self.ssidPassTextField.alpha = alpha;

    // auth type
    self.authLabel.alpha = alpha;
    self.authTextField.alpha = alpha;

    //buttons
    self.connectBtn.alpha = alpha;
    self.configureBtn.alpha = alpha;

    //labels
    self.onboardTitleLbl.alpha = alpha;
    self.ssidLbl.alpha = alpha;
    self.ssidPassLbl.alpha = alpha;
}

- (BOOL)isSimulatorDevice
{
    if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"9.0")) {
#ifdef TARGET_OS_SIMULATOR
        return TARGET_OS_SIMULATOR != 0;
#endif
#ifdef TARGET_IPHONE_SIMULATOR
        return TARGET_IPHONE_SIMULATOR != 0;
#endif
    } else {
        NSString *deviceModel = [[UIDevice currentDevice] model];
        return [deviceModel isEqualToString:@"iPhone Simulator"] || [deviceModel isEqualToString:@"iPad Simulator"];
    }
}

- (QStatus)startOnboardingClient
{
    self.onboardeeBus = [self.clientInformation.announcement busName];
    
    if (!self.onboardeeBus) {
        [self updateStatusLabel:@"Bus attachment hasn't been initialized"];
        return ER_FAIL;
    }

    self.onboardingClient = [[AJOBSOnboardingClient alloc] initWithBus:self.clientBusName listener:self];

    if ([self isSimulatorDevice]) {
        NSString *simulatorMessage = @"NOTE: Using the simulator does not support network detection - all possible actions are displayed";
        [self updateStatusLabel:[NSString stringWithFormat:simulatorMessage]];
        [self displayPreOnbordingElements:1];
        self.offBoardingBtn.alpha = 1;
        self.connectBtn.enabled = NO;
    } else {
        if ([self isOnSAPNetwork]) {
            [self.instructLbl setText:@"Press Configure after filling in the WiFi network details"];
            [self displayPreOnbordingElements:1];
            self.connectBtn.enabled = NO;
            [self.ssidTextField setText:[[NSUserDefaults standardUserDefaults] objectForKey:@"lastVisitedNetwork"]];
        } else {
            [self.instructLbl setText:@"To offboard the device - press Offboard"];
            [self displayPreOnbordingElements:0];
            //device already onboarded
            self.offBoardingBtn.alpha = 1;
        }
    }

    return ER_OK;
}

- (bool)isOnSAPNetwork
{
    NSString *currentSSID;
    // this code does not work in the simulator
    NSArray *supportedInterfaces = (__bridge_transfer id)CNCopySupportedInterfaces();
    id interfaceInformation = nil;
    for (NSString *interfaceName in supportedInterfaces) {
        interfaceInformation = (__bridge_transfer id)CNCopyCurrentNetworkInfo((__bridge CFStringRef)interfaceName);
        NSDictionary *dict = interfaceInformation;
        currentSSID = dict[@"SSID"];
        NSLog(@"Current SSID: %@", currentSSID);
    }
    return [currentSSID hasPrefix:AJ_AP_PREFIX] | [currentSSID hasSuffix:AJ_AP_SUFFIX];
}

- (void)updateStatusLabel:(NSString *)status
{
    NSLog(@"%@", status);
    [self.statusLbl setText:status];
}

- (void)stopOnboardingClient
{
    self.onboardingClient = nil;
}

#pragma marks - event methods
- (IBAction)configureBtnDidTouchUpInside:(id)sender
{
    QStatus status;
    [self updateStatusLabel:@"Calling ConfigureWiFi"];

    AJOBInfo obInfo;
    obInfo.SSID = self.ssidTextField.text;
    obInfo.passcode = self.ssidPassTextField.text;
    obInfo.authType = [self pickerAuthStringToInt];

    NSLog(@"input SSID:%@ passcode:%@", obInfo.SSID, obInfo.passcode);

    if (![obInfo.SSID length]) {
        [self updateStatusLabel:@"Error: SSID is empty"];
        AJSCAlertController *alertController = [AJSCAlertController alertControllerWithTitle:@"Error"
                                                message:@"SSID can't be empty"
                                                viewController:self];
        [alertController addActionWithName:@"OK" handler:^(UIAlertAction *action) { }];
        [alertController show];
        return;
    }

    bool shouldConvert = false;
    if ([AJOBSOnboarding isValidWPAKey:obInfo.passcode]) {
        [self updateStatusLabel:@"Input passcode is a valid WPA - will convert to Hex"];
        shouldConvert = true;
    } else if ([AJOBSOnboarding isValidWEPKey:obInfo.passcode]) {
        if ([obInfo.passcode length] % 2) {
            [self updateStatusLabel:@"Input passcode is valid WEP - converting to Hex"];
            shouldConvert = true;
        } else {
            [self updateStatusLabel:@"Input passcode is valid WEP - Hex conversion is not required"];
        }
    } else {
        [self updateStatusLabel:@"Input passcode is none of the following: WPA, WEP"];
    }

    if (shouldConvert) {
        NSString *tPasscode = [AJOBSOnboarding passcodeToHex:obInfo.passcode];
        obInfo.passcode = tPasscode;
        NSLog(@"Passcode has been converted to:%@", obInfo.passcode);
    }

    short resultStatus;
    status = [self.onboardingClient configureWiFi:self.onboardeeBus obInfo:obInfo resultStatus:resultStatus sessionId:self.sessionId];
    if (status == ER_OK) {
        [self updateStatusLabel:[NSString stringWithFormat:@"Call to configureWiFi succeeded.\nResult status is %hd", resultStatus]];
        [self.instructLbl setText:@"Press Connect to complete the onboarding."];

        if (resultStatus == 1) {
            self.connectBtn.enabled = YES;
        }

        if (resultStatus == 2) {
            [self updateStatusLabel:@"Waiting for connectionResult signal"];
        }

    } else {
        NSString *errorMessage = [NSString stringWithFormat:@"Call to configureWiFi failed: %@ ", [AJNStatus descriptionForStatusCode:status]];
        AJSCAlertController *alertController = [AJSCAlertController alertControllerWithTitle:@"Error"
                                                message:errorMessage
                                                viewController:self];
        [alertController addActionWithName:@"OK" handler:^(UIAlertAction *action) { }];
        [alertController show];
        [self updateStatusLabel:errorMessage];
    }

    self.configureBtn.enabled = NO;
    self.ssidTextField.enabled = NO;
    self.ssidPassTextField.enabled = NO;
}

- (IBAction)connectBtnDidTouchUpInside:(id)sender
{
    QStatus status;

    [self updateStatusLabel:@"Calling connect"];
    status = [self.onboardingClient connectTo:self.onboardeeBus sessionId:self.sessionId];
    if (status == ER_OK) {
        [self updateStatusLabel:@"Call to connect succeeded"];
        NSString *message = [NSString stringWithFormat:@"Go to Settings -> Wi-Fi to manually connect this device to the '%@' network to complete the onboarding process.", self.ssidTextField.text];
        [AJSCAlertControllerManager queueAlertWithTitle:@"Onboarding started" message:message viewController:self];
    } else {
        [self updateStatusLabel:[NSString stringWithFormat:@"Call to connect failed: %@", [AJNStatus descriptionForStatusCode:status]]];
    }

    [self.connectBtn setEnabled:NO];
    [self.instructLbl setText:@"  "];

    // Notify listener that onboarding has started.
    [self.onboardingStartedListener onOnboardingStarted:self.ssidTextField.text];
}

- (IBAction)offBoardingBtnDidTouchUpInside:(id)sender
{

    [self updateStatusLabel:@"Calling offboard"];
    QStatus status = [self.onboardingClient offboardFrom:self.onboardeeBus sessionId:self.sessionId];
    if (status == ER_OK) {
        [self updateStatusLabel:@"Call to offboard succeeded"];
    } else {
        [self updateStatusLabel:[NSString stringWithFormat:@"Call to offboard failed %@", [AJNStatus descriptionForStatusCode:status]]];
    }
    [self.offBoardingBtn setEnabled:NO];
    [self.instructLbl setText:@"  "];
}

- (IBAction)getScanInfo:(id)sender
{
    self.onboardeeNetworks = [[NSMutableArray alloc] init];
    unsigned short age;
    
    QStatus status = [self.onboardingClient getScanInfo:self.onboardeeBus
                                                    age:age
                                               scanInfo:self.onboardeeNetworks
                                              sessionId:self.sessionId];
    
    AJSCAlertController *alertController;
    
    if(status == ER_OK) {
        alertController = [AJSCAlertController alertControllerWithTitle:@"Success"
                                                                message:@"Onboardee networks populated below"
                                                        viewController:self];
        [alertController addActionWithName:@"OK" handler:^(UIAlertAction *action) { }];
    } else {
        alertController = [AJSCAlertController alertControllerWithTitle:@"Error"
                                                                message:@"Failed to retrieve onboardee networks"
                                                         viewController:self];
        [alertController addActionWithName:@"OK" handler:^(UIAlertAction *action) { }];
    }
    
    [alertController show];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopOnboardingClient];

    [super viewWillDisappear:animated];
}

- (void)connectionResultSignalReceived:(int)connectionResultCode connectionResultMessage:(NSString *)connectionResultMessage
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self updateStatusLabel:[NSString stringWithFormat:@"connectionResultSignal has been received with code:%d message:%@", connectionResultCode, connectionResultMessage]];
        if (connectionResultCode == VALIDATED) {
            [self.connectBtn setEnabled:YES];
        }
    });
}

- (int)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (int)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    switch (pickerView.tag) {
        case AUTH_PICKER:
        {
            return [self.authPickerData count];
            break;
        }
        case NETWORK_PICKER:
        {
            return [self.onboardeeNetworks count];
            break;
        }
        default:
            break;
    }
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    switch (pickerView.tag) {
        case AUTH_PICKER:
        {
            return self.authPickerData[row];
            break;
        }
        case NETWORK_PICKER:
        {
            AJOBScanInfo *scanInfo = self.onboardeeNetworks[row];
            return scanInfo.ssid;
            break;
        }
        default:
            break;
    }
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    switch (pickerView.tag) {
        case AUTH_PICKER:
        {
            self.authTextField.text = self.authPickerData[row];
            [self.authTextField resignFirstResponder];
            break;
        }
        case NETWORK_PICKER:
        {
            AJOBScanInfo *scanInfo = self.onboardeeNetworks[row];
            self.ssidTextField.text = scanInfo.ssid;
            self.authTextField.text = [self authIntToString:scanInfo.authType];
            [self.networkTextField resignFirstResponder];
            break;
        }
        default:
            break;
    }
}

- (NSInteger)pickerAuthStringToInt
{
    if ([self.authTextField.text isEqualToString:AUTH_OPEN_STR]) {
        return OPEN;
    } else if ([self.authTextField.text isEqualToString:AUTH_WEP_STR]) {
        return WEP;
    } else if ([self.authTextField.text isEqualToString:AUTH_WPA_AUTO_STR]) {
        return WPA_AUTO;
    } else if ([self.authTextField.text isEqualToString:AUTH_WPA_CCMP_STR]) {
        return WPA_CCMP;
    } else if ([self.authTextField.text isEqualToString:AUTH_WPA_TKIP_STR]) {
        return WPA_TKIP;
    } else if ([self.authTextField.text isEqualToString:AUTH_WPA2_AUTO_STR]) {
        return WPA2_AUTO;
    } else if ([self.authTextField.text isEqualToString:AUTH_WPA2_CCMP_STR]) {
        return WPA2_CCMP;
    } else if ([self.authTextField.text isEqualToString:AUTH_WPA2_TKIP_STR]) {
        return WPA2_TKIP;
    } else if ([self.authTextField.text isEqualToString:AUTH_WPS_STR]) {
        return WPS;
    }
}

-(NSString *)authIntToString:(AJOBAuthType)authType
{
    switch(authType) {
        case OPEN:
            return AUTH_OPEN_STR;
            break;
        case WEP:
            return AUTH_WEP_STR;
            break;
        case WPA_AUTO:
            return AUTH_WPA_AUTO_STR;
            break;
        case WPA_CCMP:
            return AUTH_WPA_CCMP_STR;
            break;
        case WPA_TKIP:
            return AUTH_WPA_TKIP_STR;
            break;
        case WPA2_AUTO:
            return AUTH_WPA2_AUTO_STR;
            break;
        case WPA2_CCMP:
            return AUTH_WPA2_CCMP_STR;
            break;
        case WPA2_TKIP:
            return AUTH_WPA2_TKIP_STR;
            break;
        case WPS:
            return AUTH_WPS_STR;
            break;
        default:
            return @"Unknown";
            break;
    }
}

@end