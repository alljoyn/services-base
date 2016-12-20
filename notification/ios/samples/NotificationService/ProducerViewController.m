/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
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

#import "ProducerViewController.h"
#import <alljoyn/Status.h>
#import "AJNVersion.h"
#import "samples_common/AJSCCommonBusListener.h"
#import "alljoyn/notification/AJNSNotificationSender.h"
#import "alljoyn/notification/AJNSNotificationService.h"
#import "alljoyn/services_common/AJSVCGenericLoggerUtil.h"
#import "NotificationUtils.h"
#import "samples_common/AJSCAlertController.h"
#import "AJNMessageArgument.h"
#import "AJNAboutObject.h"
#import "AJNAboutData.h"

static NSString *const DEVICE_ID_PRODUCER = @"ProducerBasic";
static NSString *const DEVICE_NAME_PRODUCER = @"ProducerBasic";
static NSString *const DEFAULT_LANG_PRODUCER = @"en";
static NSString *const RICH_ICON_OBJECT_PATH = @"rich/Icon/Object/Path";
static NSString *const RICH_AUDIO_OBJECT_PATH = @"rich/Audio/Object/Path";
static short const SERVICE_PORT = 900;

static const uint16_t TTL_MIN = 30;
static const uint16_t TTL_MAX = 43200;
static NSString *const DEFAULT_TTL = @"30";
static NSString *const DEFAULT_MSG_TYPE = @"INFO";


@interface ProducerViewController () <AJNSessionPortListener, AJNSessionListener>

@property (weak, nonatomic) AJNSNotificationSender *sender;
@property (strong, nonatomic) AJNSNotificationService *producerService;
@property (strong, nonatomic) AJSCCommonBusListener *commonBusListener;
@property (strong, nonatomic) AJSCAlertController *selectLanguage;
@property (strong, nonatomic) AJSCAlertController *selectMessageType;
@property (strong, nonatomic) AJNSNotification *notification;
@property (strong, nonatomic) NSMutableArray *notificationTextArr;
@property (strong, nonatomic) NSMutableDictionary *customAttributesDictionary;
@property (strong, nonatomic) NSMutableArray *richAudioUrlArray;

@property (strong, nonatomic) NSString *defaultTTL; // Hold the default ttl as set in the UI
@property (strong, nonatomic) NSString *defaultMessageType; // Hold the default message type as set in the UI

@property (nonatomic) AJNSNotificationMessageType messageType;
@property (strong, nonatomic) NSString *otherLang;

@property (strong, nonatomic) AJNAboutData *aboutData;
@property (strong, nonatomic) AJNAboutObject *aboutObj;

@end

@implementation ProducerViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self.view setHidden:YES];
}

- (QStatus)startProducer
{
    QStatus status;
    
    // Set TextField.delegate to enable dissmiss keyboard
    self.notificationEnTextField.delegate = self;
    self.notificationLangTextField.delegate = self;
    self.ttlTextField.delegate = self;
    self.audioTextField.delegate = self;
    self.iconTextField.delegate = self;

    // Set default ttl
    self.defaultTTL = DEFAULT_TTL;
    self.ttlTextField.text = DEFAULT_TTL;
    [self.ttlTextField setKeyboardType:UIKeyboardTypeNumberPad];

    // Set default messageType
    self.defaultMessageType = DEFAULT_MSG_TYPE;
    self.messageType = [self convertMessageType:self.defaultMessageType];
    self.messageTypeButton.titleLabel.text = DEFAULT_MSG_TYPE;

    // Set switch to off
    self.audioSwitch.on = false;
    self.iconSwitch.on = false;
    [self didChangeAudioSwitchValue:nil];
    [self didChangeIconSwitchValue:nil];

    // Initialize a AJNSNotificationService object
    self.producerService = [[AJNSNotificationService alloc] init];

    // Set logger (see a logger implementation example in  ConsumerViewController.m)
    [self.producerService setLogLevel:QLEVEL_DEBUG];

    // Confirm that bus is valid
    if (!self.busAttachment) {
        [self.logger fatalTag:[[self class] description] text:@"BusAttachment is nil"];
        return ER_FAIL;
    }

    // Prepare AJNSBusListener
    self.commonBusListener = [[AJSCCommonBusListener alloc] init];

    // Setup AboutData
    self.aboutData = [[AJNAboutData alloc] initWithLanguage:@"en"];
    [self provisionAboutData:self.aboutData];
    
    // Call initSend
    self.sender = [self.producerService startSendWithBus:self.busAttachment andAboutData:self.aboutData];
    if (!self.sender) {
        [self.logger fatalTag:[[self class] description] text:@"Could not initialize Sender"];
        return ER_FAIL;
    }
    
    // Bind a session to the announce service port
    AJNSessionOptions *sessionOptions = [[AJNSessionOptions alloc] initWithTrafficType:kAJNTrafficMessages supportsMultipoint:YES proximity:kAJNProximityAny transportMask:kAJNTransportMaskAny];
    
    status = [self.busAttachment bindSessionOnPort:SERVICE_PORT withOptions:sessionOptions withDelegate:self];
    if (ER_OK != status) {
        NSLog(@"ERROR: Could not bind session on port (%d)", SERVICE_PORT);
    }
    
    // Call announce
    self.aboutObj = [[AJNAboutObject alloc] initWithBusAttachment:self.busAttachment withAnnounceFlag:ANNOUNCED];
    status = [self.aboutObj announceForSessionPort:SERVICE_PORT withAboutDataListener:self.aboutData];
    
    if(status == ER_OK) {
        NSLog(@"AboutObject announcement succeeded.");
    } else {
        NSLog(@"AboutObject announcement failed.");
    }
    
    return ER_OK;
}

- (void)stopProducer:(bool)isConsumerOn
{
    QStatus status;

    [self.logger debugTag:[[self class] description] text:@"Stop Producer service"];

    if (self.sender) {
        self.sender = nil;
    }
    
    [self.aboutObj unannounce];
    
    // Shutdown producer
    if (self.producerService && isConsumerOn) {
        [self.logger debugTag:[[self class] description] text:@"calling shutdownSender"];
        [self.producerService shutdownSender];
    } else {
        [self.logger debugTag:[[self class] description] text:@"calling shutdown"];
        [self.producerService shutdown];
    }
    self.producerService = nil;

    // Unregister bus listener from the common
    if (self.busAttachment && self.commonBusListener) {
        [self.busAttachment unregisterBusListener:self.commonBusListener];
        [self.busAttachment unbindSessionFromPort:([self.commonBusListener sessionPort])];
        self.commonBusListener = nil;
    }
}

// ENUM convertMessageType
- (AJNSNotificationMessageType)convertMessageType:(NSString *)tMsgType
{
    if ([tMsgType isEqualToString:@"INFO"]) {
        return INFO;
    } else if ([tMsgType isEqualToString:@"WARNING"]) {
        return WARNING;
    } else if ([tMsgType isEqualToString:@"EMERGENCY"]) {
        return EMERGENCY;
    } else {
        return UNSET;
    }
}

- (void)producerPostSendView
{
    self.notificationEnTextField.text = nil;
    self.notificationLangTextField.text = nil;

    // Lang field-save perv selection
    // Color save perv selection
    self.ttlTextField.text = self.defaultTTL;
    if (self.iconSwitch.on) {
        self.iconSwitch.on = false;
        [self didChangeIconSwitchValue:nil];
    }
    if (self.audioSwitch.on) {
        self.audioSwitch.on = false;
        [self didChangeAudioSwitchValue:nil];
    }
    [self.messageTypeButton setTitle:self.defaultMessageType forState:(UIControlStateNormal)];

    self.messageType = [self convertMessageType:DEFAULT_MSG_TYPE];

    // Clear sent parameters:
    [self.notificationTextArr removeAllObjects];
}

- (void)logNotification:(AJNSNotification *)ajnsNotification ttl:(uint16_t)ttl
{
    [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"Sending message with message type: '%@'", [AJNSNotificationEnums AJNSMessageTypeToString:[ajnsNotification messageType]]]];

    [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"TTL: %hu", ttl]];

    for (AJNSNotificationText *notificationText in self.notificationTextArr) {
        [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"message: '%@'", [notificationText getText]]];
    }

    [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"richIconUrl: '%@'", [ajnsNotification richIconUrl]]];

    NSMutableArray *array = [[NSMutableArray alloc] init];

    [ajnsNotification richAudioUrl:array];

    for (AJNSRichAudioUrl *richAudioURL in array) {
        [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"RichAudioUrl: '%@'", [richAudioURL url]]];
    }

    [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"richIconObjPath: '%@'", [ajnsNotification richIconObjectPath]]];

    [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"RichAudioObjPath: '%@'", [ajnsNotification richAudioObjectPath]]];

    [self.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"Sending notification message for messageType '%d'", [ajnsNotification messageType]]];

}

#pragma mark - IBActions

// Send a notification
- (IBAction)didTouchSendNotificationButton:(id)sender
{
    uint16_t nttl;
    NSString *nsender = @"iosTestApp";
    NSString *richIconUrl;

    // Dismiss keyboard
    [self touchesBegan:sender withEvent:nil];

    // Build notification object

    // Set flags
    bool hasEnNotificationText = false;
    bool hasOtherLangNotificationText = false;
    bool ttlIsValid = false;

    // Create containers
    self.notificationTextArr = [[NSMutableArray alloc] init];
    self.customAttributesDictionary = [[NSMutableDictionary alloc] init];  // this dictionary holds custom attributes that are sent in notification. see below for an example on how to send the color custom attribute
    self.richAudioUrlArray = [[NSMutableArray alloc] init];

    // Set enNotificationText
    if ([NotificationUtils textFieldIsValid:self.notificationEnTextField.text]) {
        NSString *enNotificationText;
        enNotificationText = self.notificationEnTextField.text;
        // Insert into the array text and lang
        [self.notificationTextArr addObject:[[AJNSNotificationText alloc] initWithLang:@"en" andText:enNotificationText]];
        hasEnNotificationText = true;
    } else {
        AJSCAlertController *alertController = [AJSCAlertController alertControllerWithTitle:@"Error"
                                                message:@"At least one of the messages should be sent in english"
                                                viewController:self];
        [alertController addActionWithName:@"Cancel" handler:^(UIAlertAction *action) { }];
        [alertController show];
        return;
    }

    // Set other lang message
    if ([NotificationUtils textFieldIsValid:self.notificationLangTextField.text] && self.otherLang) {
        NSString *langNotificationText;
        langNotificationText = self.notificationLangTextField.text;
        [self.notificationTextArr addObject:[[AJNSNotificationText alloc] initWithLang:self.otherLang andText:langNotificationText]];
        hasOtherLangNotificationText = true;
    }

    // Set ttl
    unsigned long rcv_ttl;
    rcv_ttl = [[self.ttlTextField text] intValue];
    if ((rcv_ttl >= TTL_MIN) && (rcv_ttl <= TTL_MAX)) {
        ttlIsValid = true;
        nttl = rcv_ttl;
    } else {
        NSString *ttlErrReport = [NSString stringWithFormat:@"ttl range is %hu - %hu. %lu is invalid", TTL_MIN, TTL_MAX, rcv_ttl];
        AJSCAlertController *alertController = [AJSCAlertController alertControllerWithTitle:@"Error"
                                                message:ttlErrReport
                                                viewController:self];
        [alertController addActionWithName:@"Cancel" handler:^(UIAlertAction *action) { }];
        [alertController show];
    }

    // Set English audio
    if (self.audioSwitch.on == true && ([NotificationUtils textFieldIsValid:(self.audioTextField.text)])) {
        AJNSRichAudioUrl *richAudioUrlObj = [[AJNSRichAudioUrl alloc] initRichAudioUrlWithLang:@"en" andUrl:self.audioTextField.text];
        [self.richAudioUrlArray addObject:richAudioUrlObj];

        // Set other language audio
        if (self.otherLang) {
            AJNSRichAudioUrl *richAudioUrlObj = [[AJNSRichAudioUrl alloc] initRichAudioUrlWithLang:self.otherLang andUrl:self.audioTextField.text];
            [self.richAudioUrlArray addObject:richAudioUrlObj];
        }
    }

    // Set icon
    if (self.iconSwitch.on == true && ([NotificationUtils textFieldIsValid:(self.iconTextField.text)])) {
        richIconUrl = self.iconTextField.text;
    }

    // Set color example - this is how to set the color
    // [customAttributesDictionary setValue:@"Blue" forKey:@"color"];

    NSString *curr_controlPanelServiceObjectPath = @"";

    // Set the Notification object
    if ((hasEnNotificationText) && ttlIsValid && (self.messageType != UNSET)) {

        self.notification = [[AJNSNotification alloc] initWithMessageType:self.messageType andNotificationText:self.notificationTextArr];

        // This is an exaple of using AJNSNotification setters :
        [self.notification setMessageId:-1];
        [self.notification setDeviceId:nil];
        [self.notification setDeviceName:nil];
        [self.notification setAppId:nil];
        [self.notification setAppName:self.appName];
        [self.notification setSender:nsender];
        [self.notification setCustomAttributes:self.customAttributesDictionary];
        [self.notification setRichIconUrl:richIconUrl];
        [self.notification setRichAudioUrl:self.richAudioUrlArray];
        [self.notification setRichIconObjectPath:RICH_ICON_OBJECT_PATH];
        [self.notification setRichAudioObjectPath:RICH_AUDIO_OBJECT_PATH];
        [self.notification setControlPanelServiceObjectPath:curr_controlPanelServiceObjectPath];

        [self logNotification:self.notification ttl:nttl];

        [self.richAudioUrlArray removeAllObjects];

        // Call send
        QStatus sendStatus = [self.sender send:self.notification ttl:nttl];
        if (sendStatus != ER_OK) {
            [self.logger infoTag:[[self class] description] text:[NSString stringWithFormat:@"Send has failed"]];
        } else {
            [self.logger infoTag:[[self class] description] text:[NSString stringWithFormat:@"Successfully sent!"]];
        }
        [self producerPostSendView];
    } else {
        [self.logger infoTag:[[self class] description] text:[NSString stringWithFormat:@"Invalid Notification input:\n EnNotificationText=%u\n OtherLangNotificationText=%u\n ttlIsValid=%u\n hasMessageType=%u", hasEnNotificationText, hasOtherLangNotificationText, ttlIsValid, self.messageType]];
    }
    [self producerPostSendView];
}

- (IBAction)didChangeAudioSwitchValue:(id)sender
{
    self.audioTextField.alpha = self.audioSwitch.on;
}

- (IBAction)didChangeIconSwitchValue:(id)sender
{
    self.iconTextField.alpha = self.iconSwitch.on;
}

- (IBAction)didTouchDeleteButton:(id)sender
{
    [self.logger errorTag:[[self class] description] text:[NSString stringWithFormat:@"message type is %u", self.messageType]];

    if (self.messageType == UNSET) {
        return;
    }

    QStatus deleteStatus = [self.sender deleteLastMsg:self.messageType];

    if (deleteStatus != ER_OK) {
        [self.logger errorTag:[[self class] description] text:@"Failed to delete a message"];
    }
}

- (QStatus)provisionAboutData:(AJNAboutData *)aboutData
{
    QStatus status;

    // AppId
    uuid_t uuid;
    [[NSUUID UUID] getUUIDBytes:uuid];
    status = [aboutData setAppId:uuid];
    if (status != ER_OK) {
        return status;
    }

    // AppName
    status = [aboutData setAppName:self.appName andLanguage:DEFAULT_LANG_PRODUCER];
    if (status != ER_OK) {
        return status;
    }

    // DeviceId
    status = [aboutData setDeviceId:DEVICE_ID_PRODUCER];
    if (status != ER_OK) {
        return status;
    }

    // DeviceName
    status = [aboutData setDeviceName:DEVICE_NAME_PRODUCER andLanguage:DEFAULT_LANG_PRODUCER];
    if (status != ER_OK) {
        return status;
    }

    // SupportedLangs
    NSArray *languages = @[@"en", @"sp", @"fr"];
    for (NSString *lang in languages) {
        status = [aboutData setSupportedLanguage:lang];
        if (status != ER_OK) {
            return status;
        }
    }

    // DefaultLang
    status = [aboutData setDefaultLanguage:DEFAULT_LANG_PRODUCER];
    if (status != ER_OK) {
        return status;
    }

    // ModelNumber
    status = [aboutData setModelNumber:@"Wxfy388i"];
    if (status != ER_OK) {
        return status;
    }

    // DateOfManufacture
    status = [aboutData setDateOfManufacture:@"10/1/2199"];
    if (status != ER_OK) {
        return status;
    }

    // SoftwareVersion
    status = [aboutData setSoftwareVersion:@"12.20.44 build 44454"];
    if (status != ER_OK) {
        return status;
    }

    // HardwareVersion
    status = [aboutData setHardwareVersion:@"355.499. b"];
    if (status != ER_OK) {
        return status;
    }

    // Description
    status = [aboutData setDescription:@"This is an Alljoyn Application" andLanguage:@"en"];
    if (status != ER_OK) {
        return status;
    }

    status = [aboutData setDescription:@"Esta es una Alljoyn aplicación" andLanguage:@"sp"];
    if (status != ER_OK) {
        return status;
    }

    status = [aboutData setDescription:@"C'est une Alljoyn application"  andLanguage:@"fr"];
    if (status != ER_OK) {
        return status;
    }

    // Manufacturer
    status = [aboutData setManufacturer:@"Company" andLanguage:@"en"];
    if (status != ER_OK) {
        return status;
    }

    status = [aboutData setManufacturer:@"Empresa" andLanguage:@"sp"];
    if (status != ER_OK) {
        return status;
    }

    status = [aboutData setManufacturer:@"Entreprise" andLanguage:@"fr"];
    if (status != ER_OK) {
        return status;
    }

    // SupportedUrl
    status = [aboutData setSupportUrl:@"http://www.alljoyn.org"];
    if (status != ER_OK) {
        return status;
    }

    return status;
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (IBAction)selectLanguagePressed:(UIButton *)sender
{
    [self showSelectLanguageAlert];
}

- (IBAction)selectMessageTypePressed:(UIButton *)sender
{
    [self showMessageTypeAlert];
}

- (void)showSelectLanguageAlert
{
    NSArray *langArray = [[NSMutableArray alloc] initWithObjects:@"English", @"Hebrew", @"Russian", nil];

    self.selectLanguage = [AJSCAlertController alertControllerWithTitle:@"Select Language"
                           message:@""
                           viewController:self];

    [self.selectLanguage addActionWithName:@"Cancel" handler:^(UIAlertAction *action) { }];

    __weak ProducerViewController *weakSelf = self;
    for (NSString *str in langArray) {
        [self.selectLanguage addActionWithName:str handler:^(UIAlertAction *action) {
             weakSelf.otherLang = [weakSelf convertToLangCode:str];
             [weakSelf.langButton setTitle:str forState:UIControlStateNormal];
         }];
    }

    [self.selectLanguage show];
}

- (void)showMessageTypeAlert
{
    NSArray *messageTypeArray = [[NSMutableArray alloc] initWithObjects:@"INFO", @"WARNING", @"EMERGENCY", nil];

    self.selectMessageType = [AJSCAlertController alertControllerWithTitle:@"Select Message Type"
                              message:@""
                              viewController:self];

    [self.selectMessageType addActionWithName:@"Cancel" handler:^(UIAlertAction *action) { }];

    __weak ProducerViewController *weakSelf = self;
    for (NSString *str in messageTypeArray) {
        [self.selectMessageType addActionWithName:str handler:^(UIAlertAction *action) {
             weakSelf.messageType = [weakSelf convertMessageType:str];
             [weakSelf.messageTypeButton setTitle:str forState:UIControlStateNormal];
         }];
    }

    [self.selectMessageType show];
}

#pragma mark - Application util methods

- (NSString *)convertToLangCode:(NSString *)value
{
    if ([value isEqualToString:@"English"]) {
        return @"en";
    } else if ([value isEqualToString:@"Hebrew"]) {
        return @"he";
    } else if ([value isEqualToString:@"Russian"]) {
        return @"ru";
    }
    return nil;
}

// Set dismiss keyboard for each UITextField
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UIView *aSubview in[self.view subviews]) {
        if ([aSubview isKindOfClass:[UITextField class]]) {
            [(UITextField *) aSubview resignFirstResponder];
        }
    }
}

#pragma mark - AJNSessionListener methods

- (void)sessionWasLost:(AJNSessionId)sessionId
{
    NSLog(@"AJNBusListener::sessionWasLost %u", sessionId);
}

- (void)sessionWasLost:(AJNSessionId)sessionId forReason:(AJNSessionLostReason)reason
{
    NSLog(@"AJNBusListener::sessionWasLost %u forReason:%u", sessionId, reason);
}

- (void)didAddMemberNamed:(NSString*)memberName toSession:(AJNSessionId)sessionId
{
    NSLog(@"AJNBusListener::didAddMemberNamed:%@ toSession:%u", memberName, sessionId);
}

- (void)didRemoveMemberNamed:(NSString*)memberName fromSession:(AJNSessionId)sessionId
{
    NSLog(@"AJNBusListener::didRemoveMemberNamed:%@ fromSession:%u", memberName, sessionId);
}

#pragma mark - AJNSessionPortListener implementation

- (BOOL)shouldAcceptSessionJoinerNamed:(NSString*)joiner onSessionPort:(AJNSessionPort)sessionPort withSessionOptions:(AJNSessionOptions*)options
{
    NSLog(@"AJNSessionPortListener::shouldAcceptSessionJoinerNamed:%@ onSessionPort:%u withSessionOptions:", joiner, sessionPort);
    return SERVICE_PORT == sessionPort;
}

- (void)didJoin:(NSString*)joiner inSessionWithId:(AJNSessionId)sessionId onSessionPort:(AJNSessionPort)sessionPort
{
    NSLog(@"AJNSessionPortListener::didJoin:%@ inSessionWithId:%u onSessionPort:%u withSessionOptions:", joiner, sessionId, sessionPort);
    [self.busAttachment enableConcurrentCallbacks];
    [self.busAttachment setSessionListener:self toSession:sessionId];
}

@end