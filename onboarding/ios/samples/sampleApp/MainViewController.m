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

#import "MainViewController.h"
#import "SystemConfiguration/CaptiveNetwork.h"
#import "AJNStatus.h"
#import "OnboardingViewController.h"
#import "samples_common/AJSCAboutAnnouncement.h"
#import "samples_common/AJSCAboutDataConverter.h"
#import "samples_common/AJSCClientInformation.h"
#import "samples_common/AJSCGetAboutCallViewController.h"
#import "samples_common/AJSCAuthenticationListenerImpl.h"
#import "samples_common/AJSCAlertController.h"
#import "samples_common/AJSCAlertControllerManager.h"
#import "samples_common/AJSCAnnounceTextViewController.h"
#import "AJNAboutObjectDescription.h"

static bool ALLOWREMOTEMESSAGES = true; // About Client -  allow Remote Messages flag
static NSString *const APPNAME = @"AboutClientMain"; // About Client - default application name
static NSString *const DAEMON_QUIET_PREFIX = @"quiet@";   // About Client - quiet advertising
static NSString *const ONBOARDING_OBJECT_PATH = @"/Onboarding";
static NSString *const ONBOARDING_INTERFACE_NAME = @"org.alljoyn.Onboarding";
static NSString *const DEFAULT_REALM_BUS_NAME = @"org.alljoyn.BusNode.onboardingClient";

static NSString *const SSID_NOT_CONNECTED = @"SSID:not connected";
static NSString *const DEFAULT_AUTH_PASSCODE = @"000000";

@interface MainViewController ()
@property NSString *className;

// About Client properties
@property (strong, nonatomic) AJNBusAttachment *clientBusAttachment;
@property (strong, nonatomic) NSString *realmBusName;
@property (nonatomic) bool isAboutClientConnected;
@property (strong, nonatomic) NSMutableDictionary *clientInformationDict; // Store the client related information
@property (strong, nonatomic) NSMutableDictionary *deviceSessionDict; // Store the session id established with each device

// Announcement
@property (strong, nonatomic) NSString *announcementButtonCurrentTitle;                  // The pressed button's announcementUniqueName
@property (strong, nonatomic) dispatch_queue_t annBtnCreationQueue;

// About Client strings
@property (strong, nonatomic) NSString *ajconnect;
@property (strong, nonatomic) NSString *ajdisconnect;
@property (strong, nonatomic) NSString *annSubvTitleLabelDefaultTxt;

// About Client alerts
@property (strong, nonatomic) AJSCAlertController *announcementOptionsAlert;
@property (strong, nonatomic) AJSCAlertController *onboardingOptionsAlert;

@property (strong, nonatomic) AJSCAuthenticationListenerImpl *authenticationListenerImpl;

@property (strong, nonatomic) AJSCClientInformation *clientInformation;
@property (nonatomic) bool isWaitingForOnboardee;
@property (strong, nonatomic) NSString *currentSSID;
@property (strong, nonatomic) NSString *targetSSID;

@end

@implementation MainViewController

#pragma mark - Built In methods
- (void)viewDidLoad
{
    [super viewDidLoad];

    [self loadNewSession];

    [self updateSSIDinTitle];

    [NSTimer scheduledTimerWithTimeInterval:5.0
     target:self
     selector:@selector(updateSSIDinTitle)
     userInfo:nil
     repeats:YES];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.servicesTable performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];
}

- (void)updateSSIDinTitle
{
    static BOOL executing;

    if (executing) {
        return;
    }

    executing = YES;
    // this code does not work in the simulator
    NSArray *supportedInterfaces = (__bridge_transfer id)CNCopySupportedInterfaces();
//    NSLog(@"supportedInterfaces: %@", supportedInterfaces);
    id interfaceInformation = nil;
    for (NSString *interfaceName in supportedInterfaces) {
        interfaceInformation = (__bridge_transfer id)CNCopyCurrentNetworkInfo((__bridge CFStringRef)interfaceName);
//        NSLog(@"    %@:%@", interfaceName, interfaceInformation);
        NSDictionary *dict = interfaceInformation;
        NSString *title = [NSString stringWithFormat:@"Devices on: %@", dict[@"SSID"]];
        self.currentSSID = dict[@"SSID"];

        // Set the instructions Label text according to the network type
        if ([dict[@"SSID"] hasPrefix:AJ_AP_PREFIX] || [dict[@"SSID"] hasSuffix:AJ_AP_SUFFIX]) {
            self.instructionsLabel.text = @"You are currently connected to a device Access Point.\n\nPress \"Connect to AllJoyn\" to see the device in the list above.\n\nPress on the device name->onboarding to start onboarding.";
        } else {
            self.instructionsLabel.text = @"To onboard a new device:\nConnect to the device's Wi-Fi Access Point by going to Settings -> Wi-Fi\n\nTo see the devices on this network:\nPress \"Connect to AllJoyn\"";
        }

        if (![self.title isEqualToString:title]) {
            if ((![dict[@"SSID"] hasPrefix:AJ_AP_PREFIX] && ![dict[@"SSID"] hasSuffix:AJ_AP_SUFFIX]) && ![dict[@"SSID"] isEqualToString:[[NSUserDefaults standardUserDefaults] valueForKey:@"lastVisitedNetwork"]]) {
                NSLog(@"setting lastVisitedNetwork to: %@", dict[@"SSID"]);
                [[NSUserDefaults standardUserDefaults] setValue:dict[@"SSID"] forKey:@"lastVisitedNetwork"];
                [[NSUserDefaults standardUserDefaults] synchronize];
            }

            self.title = title;
            if (self.isAboutClientConnected) {
                NSLog(@"changing network to %@ trigger a restart", dict[@"SSID"]);

                [AJSCAlertControllerManager queueAlertWithTitle:@"Wi-Fi network changed"
                 message:@"Please reconnect to AllJoyn"
                 viewController:self];
                [self.navigationController popToRootViewControllerAnimated:YES];
                [self stopAboutClient];
            }
        }

        if (interfaceInformation && [interfaceInformation count]) {
            break;
        }
    }

    if ([self.title isEqualToString:@""]) {
        self.title = SSID_NOT_CONNECTED;
    }

    executing = NO;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue
    sender:(id)sender
{
    // AJSCGetAboutCallViewController
    if ([segue.destinationViewController isKindOfClass:[AJSCGetAboutCallViewController class]]) {
        AJSCGetAboutCallViewController *getAboutCallView = segue.destinationViewController;
        getAboutCallView.clientInformation = (self.clientInformationDict)[self.announcementButtonCurrentTitle];
        getAboutCallView.clientBusAttachment = self.clientBusAttachment;
    }
    // AJSCAnnounceTextViewController
    else if ([segue.destinationViewController isKindOfClass:[AJSCAnnounceTextViewController class]]) {
        AJSCAnnounceTextViewController *announceTextViewController = segue.destinationViewController;
        announceTextViewController.announcement = [(AJSCClientInformation *)(self.clientInformationDict)[self.announcementButtonCurrentTitle] announcement];
    } else if ([segue.destinationViewController isKindOfClass:[OnboardingViewController class]]) {
        OnboardingViewController *onboardingViewController = segue.destinationViewController;
        onboardingViewController.clientBusName = self.clientBusAttachment;
        onboardingViewController.clientInformation = (self.clientInformationDict)[self.announcementButtonCurrentTitle];
        onboardingViewController.onboardingStartedListener = self;
        onboardingViewController.sessionId = [(self.deviceSessionDict)[self.announcementButtonCurrentTitle] unsignedIntValue];

        // For devices: Persistent cache of onboardee information so this information is not lost when onboarding
        // network change occurs.
        if (!self.isWaitingForOnboardee) {
            self.clientInformation = (self.clientInformationDict)[self.announcementButtonCurrentTitle];
        }
    }
}

#pragma mark - IBAction Methods
- (IBAction)connectButtonDidTouchUpInside:(id)sender
{
    // Connect to the bus with the default realm bus name
    if (!self.isAboutClientConnected) {
        [self startAboutClient];
    } else {
        [self stopAboutClient];
    }
}

#pragma mark - AJNAboutListener protocol method
// Here we receive an announcement from AJN and add it to the client's list of services avaialble
- (void)didReceiveAnnounceOnBus:(NSString *)busName
                    withVersion:(uint16_t)version
                withSessionPort:(AJNSessionPort)port
          withObjectDescription:(AJNMessageArgument *)objectDescriptionArg
               withAboutDataArg:(AJNMessageArgument *)aboutDataArg
{
    QStatus status;
    
    NSString *announcementUniqueName; // Announcement unique name in a format of <busName DeviceName>
    AJSCClientInformation *clientInformation = [[AJSCClientInformation alloc] init];

    // Save the announcement in a AJSCAboutAnnouncement
    clientInformation.announcement = [[AJSCAboutAnnouncement alloc] initWithBusName:busName version:version sessionPort:port objectDescriptionArg:objectDescriptionArg aboutDataArg:aboutDataArg];
    
    NSDictionary *aboutDataDict = [AJSCAboutDataConverter aboutDataArgToDict:[clientInformation.announcement aboutDataArg]];
    
    // Generate an announcement unique name in a format of <busName DeviceName>
    announcementUniqueName = [NSString stringWithFormat:@"%@ %@", busName, aboutDataDict[@"DeviceName"]];
    NSLog(@"[%@] [%@] Announcement unique name [%@]", @"DEBUG", [[self class] description], announcementUniqueName);

    NSString *appId = aboutDataDict[@"AppId"];
    if (!appId) {
        NSLog(@"[%@] [%@] Failed to read appId for key [%@]", @"DEBUG", [[self class] description], announcementUniqueName);
        return;
    }

    // Dealing with announcement entries should be syncronized, so we add it to a queue
    dispatch_sync(self.annBtnCreationQueue, ^{
        bool isAppIdExists = false;
        QStatus tStatus;

        NSString *onboardeeAppId;

        // ASABASE-711 - Check whether onboarding succeeded here instead of in OnboardingViewController.
        // Devices disconnect from AllJoyn when onboarding disconnects from Wi-Fi hotspot, which in turn
        // unregisters any callbacks set in OnboardingViewController. Note that simulators can't check the
        // current network, so it's possible for a non-onboarded device to trigger this code.
        if (self.isWaitingForOnboardee && ([self.currentSSID isEqualToString:self.targetSSID] || [self isSimulatorDevice])) {
            
            // Get the app ID of the onboardee.
            AJSCAboutAnnouncement *clientAnnouncement = [self.clientInformation announcement];
            NSDictionary *onboardeeAboutDataDict = [AJSCAboutDataConverter aboutDataArgToDict:clientAnnouncement.aboutDataArg];
            
            onboardeeAppId = onboardeeAboutDataDict[@"AppId"];
            if (!onboardeeAppId) {
                return;
            }

            // Check if the announcement came from the onboardee.
            if([onboardeeAppId compare:appId] == NSOrderedSame) {
                [AJSCAlertControllerManager queueAlertWithTitle:@"Onboarding succeeded"
                                                        message:@""
                                                 viewController:self];
                self.isWaitingForOnboardee = false;
            }
        }

        // Iterate over the announcements dictionary
        for (NSString *key in self.clientInformationDict.allKeys) {
            AJSCClientInformation *clientInfo = [self.clientInformationDict valueForKey:key];
            AJSCAboutAnnouncement *announcement = [clientInfo announcement];
            
            onboardeeAppId = [AJSCAboutDataConverter aboutDataArgToDict:announcement.aboutDataArg][@"AppId"];
            if (!onboardeeAppId) {
                NSLog(@"[%@] [%@] Failed to read appId for key [%@]", @"DEBUG", [[self class] description], key);
                return;
            }
            
            if([onboardeeAppId compare:appId] == NSOrderedSame) {
                isAppIdExists = true;

                // Same AppId and the same announcementUniqueName
                if ([key isEqualToString:announcementUniqueName]) {
                    // Update only announcements dictionary

                    NSLog(@"[%@] [%@] Got an announcement from a known device - updating the announcement object", @"DEBUG", [[self class] description]);

                    (self.clientInformationDict)[announcementUniqueName] = clientInformation;
                    // Same AppId but *different* announcementUniqueName
                } else {
                    NSLog(@"[%@] [%@] Got an announcement from a known device(different bus name) - updating the announcement object and UI ", @"DEBUG", [[self class] description]);

                    // Cancel advertise name if the bus name has changed
                    NSString *prevBusName = [announcement busName];
                    if (!([busName isEqualToString:prevBusName])) {
                        tStatus = [self.clientBusAttachment cancelFindAdvertisedName:prevBusName];
                        if (tStatus != ER_OK) {
                            NSLog(@"[%@] [%@] failed to cancelAdvertisedName for %@. status:%@", @"DEBUG", [[self class] description], prevBusName, [AJNStatus descriptionForStatusCode:tStatus]);
                        }
                    }
                    // Remove existed record from the announcements dictionary
                    [self.clientInformationDict removeObjectForKey:key];
                    // Add new record to the announcements dictionary
                    [self.clientInformationDict setValue:clientInformation forKey:announcementUniqueName];
                    [self.servicesTable performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];
                }
                break;
            } //if
        } //for

        //appId doesn't exist and  there is no match announcementUniqueName
        if (!(self.clientInformationDict)[announcementUniqueName] && !isAppIdExists) {
            // Add new pair with this AboutService information (version,port,bus name, object description and about data)
            [self.clientInformationDict setValue:clientInformation forKey:announcementUniqueName];
            [self addNewAnnouncemetEntry];

            // AppId doesn't exist and BUT there is no match announcementUniqueName
        } // else No OP
    });

    // Register interest in a well-known name prefix for the purpose of discovery (didLoseAdertise)
    [self.clientBusAttachment enableConcurrentCallbacks];
    status = [self.clientBusAttachment findAdvertisedName:busName];
    if (status != ER_OK) {
        NSLog(@"[%@] [%@] failed to findAdvertisedName for %@. status:%@", @"ERROR", [[self class] description], busName, [AJNStatus descriptionForStatusCode:status]);
    }
    
    AJNSessionOptions *options = [[AJNSessionOptions alloc] initWithTrafficType:kAJNTrafficMessages supportsMultipoint:YES proximity:kAJNProximityAny transportMask:kAJNTransportMaskAny];
    AJNSessionId sessionId = [self.clientBusAttachment joinSessionWithName:busName onPort:port withDelegate:self options:options];
    [self.deviceSessionDict setValue:[NSNumber numberWithUnsignedInteger:sessionId] forKey:announcementUniqueName];
}

#pragma mark AJNBusListener protocol methods

- (void)didFindAdvertisedName:(NSString *)name
    withTransportMask:(AJNTransportMask)transport
    namePrefix:(NSString *)namePrefix
{
    NSLog(@"didFindAdvertisedName %@", name);
}

- (void)didLoseAdvertisedName:(NSString *)name
    withTransportMask:(AJNTransportMask)transport
    namePrefix:(NSString *)namePrefix
{
    NSLog(@"didLoseAdvertisedName");
    QStatus status;
    // Find the button title that should be removed
    for (NSString *key in[self.clientInformationDict allKeys]) {
        if ([[[[self.clientInformationDict valueForKey:key] announcement] busName] isEqualToString:name]) {
            // Cancel advertise name for that bus
            status = [self.clientBusAttachment cancelFindAdvertisedName:name];
            if (status != ER_OK) {
                NSLog(@"[%@] [%@] failed to cancelFindAdvertisedName for %@. status:%@", @"DEBUG", [[self class] description], name, [AJNStatus descriptionForStatusCode:status]);
            }
            // Remove the anouncement from the dictionary
            [self.clientInformationDict removeObjectForKey:key];
        }
    }

    [self.servicesTable performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];
}

#pragma mark - util methods
- (void)loadNewSession
{
    // About flags
    self.isAboutClientConnected = false;

    self.annBtnCreationQueue = dispatch_queue_create("org.alljoyn.announcementbuttoncreationQueue", NULL);

    // Set About Client strings
    self.ajconnect = @"Connect to AllJoyn";
    self.ajdisconnect = @"Disconnect from AllJoyn";
    self.realmBusName = DEFAULT_REALM_BUS_NAME;
    self.annSubvTitleLabelDefaultTxt = @"Announcement of ";
    // Set About Client connect button
    [self.connectButton setTitle:self.ajconnect forState:UIControlStateNormal]; //default text

    [self prepareAlerts];
}

//  Initialize alerts
- (void)prepareAlerts
{
    [self prepareAnnouncementOptionsAlert];
    [self prepareOnboardingOptionsAlert];
}

- (void)prepareAnnouncementOptionsAlert
{
    __weak MainViewController *weakSelf = self;

    self.announcementOptionsAlert = [AJSCAlertController alertControllerWithTitle:@"Choose option:"
                                     message:@""
                                     viewController:self];

    [self.announcementOptionsAlert addActionWithName:@"Cancel" handler:^(UIAlertAction *action) { }];

    [self.announcementOptionsAlert addActionWithName:@"Show Announce" handler:^(UIAlertAction *action) {
         [weakSelf performSegueWithIdentifier:@"AboutShowAnnounceSegue" sender:weakSelf];
     }];

    [self.announcementOptionsAlert addActionWithName:@"About" handler:^(UIAlertAction *action) {
         [weakSelf performSegueWithIdentifier:@"AboutClientSegue" sender:weakSelf];
     }];
}

- (void)prepareOnboardingOptionsAlert
{
    __weak MainViewController *weakSelf = self;

    self.onboardingOptionsAlert = [AJSCAlertController alertControllerWithTitle:@""
                                   message:@""
                                   viewController:self];

    [self.onboardingOptionsAlert addActionWithName:@"Cancel" handler:^(UIAlertAction *action) { }];

    [self.onboardingOptionsAlert addActionWithName:@"Show Announce" handler:^(UIAlertAction *action) {
         [weakSelf performSegueWithIdentifier:@"AboutShowAnnounceSegue" sender:weakSelf];
     }];

    [self.onboardingOptionsAlert addActionWithName:@"About" handler:^(UIAlertAction *action) {
         [weakSelf performSegueWithIdentifier:@"AboutClientSegue" sender:weakSelf];
     }];

    [self.onboardingOptionsAlert addActionWithName:@"Onboarding" handler:^(UIAlertAction *action) {
         [weakSelf performSegueWithIdentifier:@"OnboardingClientSegue" sender:weakSelf];
     }];
}

- (void)AlertAndLog:(NSString *)level message:(NSString *)message status:(QStatus)status
{
    NSString *alertText = [NSString stringWithFormat:@"%@ (%@)", message, [AJNStatus descriptionForStatusCode:status]];
    NSLog(@"[%@] [%@] %@", level, [[self class] description], alertText);

    AJSCAlertController *alertController = [AJSCAlertController alertControllerWithTitle:@"Startup Error"
                                            message:alertText
                                            viewController:self];
    [alertController addActionWithName:@"OK" handler:^(UIAlertAction *action) { }];
    [alertController show];
}


#pragma mark - AboutClient
#pragma mark start AboutClient

- (void)startAboutClient
{
    QStatus status;

    // Create a dictionary to contain announcements using a key in the format of: "announcementUniqueName + announcementObj"
    self.clientInformationDict = [[NSMutableDictionary alloc] init];
    self.deviceSessionDict = [[NSMutableDictionary alloc] init];
    
    NSLog(@"[%@] [%@] Start About Client", @"DEBUG", [[self class] description]);

    // Init AJNBusAttachment
    self.clientBusAttachment = [[AJNBusAttachment alloc] initWithApplicationName:APPNAME allowRemoteMessages:ALLOWREMOTEMESSAGES];

    // Start AJNBusAttachment
    status = [self.clientBusAttachment start];
    if (status != ER_OK) {
        [self AlertAndLog:@"FATAL" message:@"Failed AJNBusAttachment start" status:status];
        [self stopAboutClient];
        return;
    }

    // Connect AJNBusAttachment
    status = [self.clientBusAttachment connectWithArguments:@""];
    if (status != ER_OK) {
        [self AlertAndLog:@"FATAL" message:@"Failed AJNBusAttachment connectWithArguments" status:status];
        [self stopAboutClient];

        return;
    }

    NSLog(@"[%@] [%@] Create aboutClientListener", @"DEBUG", [[self class] description]);
    NSLog(@"[%@] [%@] Register aboutClientListener", @"DEBUG", [[self class] description]);

    [self.clientBusAttachment registerBusListener:self];
    [self.clientBusAttachment registerAboutListener:self];
    
    status = [self.clientBusAttachment whoImplementsInterface:ONBOARDING_INTERFACE_NAME];
    if (status != ER_OK) {
        [self AlertAndLog:@"FATAL" message:@"Failed to registerAnnouncementReceiver" status:status];
        [self stopAboutClient];
        return;
    }

    NSUUID *UUID = [NSUUID UUID];
    NSString *stringUUID = [UUID UUIDString];

    self.realmBusName = [NSString stringWithFormat:@"%@-%@", DEFAULT_REALM_BUS_NAME, stringUUID];

    // Advertise Daemon for tcl
    status = [self.clientBusAttachment requestWellKnownName:self.realmBusName withFlags:kAJNBusNameFlagDoNotQueue];
    if (status == ER_OK) {
        // Advertise the name with a quite prefix for TC to find it
        status = [self.clientBusAttachment advertiseName:[NSString stringWithFormat:@"%@%@", DAEMON_QUIET_PREFIX, self.realmBusName] withTransportMask:kAJNTransportMaskAny];
        if (status != ER_OK) {

            [self AlertAndLog:@"FATAL" message:@"Failed to advertise name" status:status];

            NSLog(@"[%@] [%@] Failed advertising: %@%@", @"ERROR", [[self class] description], DAEMON_QUIET_PREFIX, self.realmBusName);

            [self stopAboutClient];

            return;
        } else {
            NSLog(@"[%@] [%@] Successfully advertised: %@%@", @"DEBUG", [[self class] description], DAEMON_QUIET_PREFIX, self.realmBusName);
        }
    } else {
        [self AlertAndLog:@"FATAL" message:@"Failed to requestWellKnownName" status:status];
        [self stopAboutClient];

        return;

    }

    // Enable Client Security
    self.authenticationListenerImpl = [[AJSCAuthenticationListenerImpl alloc] initWithViewController:self];
    self.authenticationListenerImpl.defaultPasscode = DEFAULT_AUTH_PASSCODE;
    status = [self enableClientSecurity];
    if (ER_OK != status) {
        [self AlertAndLog:@"ERROR" message:@"Failed to enable security. Please uninstall the application and reinstall." status:status];
    } else {
        NSLog(@"Successfully enabled security for the bus");
    }

    // For devices: Start the onboarding successful validation timeout after manual reconnecting to AllJoyn (following
    // the onboarding procedure).
    // For simulator: This code won't be run because network changes cannot be detected, so there is no way to reliably
    // begin this timeout only when the onboarder has manually switched to the target network.
    if (self.isWaitingForOnboardee && [self.currentSSID isEqualToString:self.targetSSID]) {
        [self startOnboardSuccessfulTimeout];
    }

    [self.connectButton setTitle:self.ajdisconnect forState:UIControlStateNormal]; //change title to "Disconnect from AllJoyn"
    self.isAboutClientConnected = true;
}


- (void)startOnboardSuccessfulTimeout
{
    // Begin a timeout to wait for confirmation announcement from onboardee that they have onboarded successfully.
    [NSTimer scheduledTimerWithTimeInterval:30.0
     target:self
     selector:@selector(checkIfOnboardingFailed)
     userInfo:nil
     repeats:NO];
}


- (QStatus)enableClientSecurity
{
    QStatus status;
    status = [self.clientBusAttachment enablePeerSecurity:@"ALLJOYN_SRP_KEYX ALLJOYN_ECDHE_PSK" authenticationListener:self.authenticationListenerImpl keystoreFileName:@"Documents/alljoyn_keystore/s_central.ks" sharing:YES];

    if (status != ER_OK) { //try to delete the keystore and recreate it, if that fails return failure
        NSError *error;
        NSString *keystoreFilePath = [NSString stringWithFormat:@"%@/alljoyn_keystore/s_central.ks", [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0]];
        [[NSFileManager defaultManager] removeItemAtPath:keystoreFilePath error:&error];
        if (error) {
            NSLog(@"ERROR: Unable to delete keystore. %@", error);
            return ER_AUTH_FAIL;
        }

        status = [self.clientBusAttachment enablePeerSecurity:@"ALLJOYN_SRP_KEYX ALLJOYN_ECDHE_PSK" authenticationListener:self.authenticationListenerImpl keystoreFileName:@"Documents/alljoyn_keystore/s_central.ks" sharing:YES];
    }
    return status;
}

- (void)addNewAnnouncemetEntry
{
    [self.servicesTable performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];
}

// announcementGetMoreInfo is an IBAction triggered by pressing a dynamic announcement button
- (void)announcementGetMoreInfo:(NSInteger)requestedRow
{
    // set the announcementButtonCurrentTitle
    self.announcementButtonCurrentTitle = [self.clientInformationDict allKeys][requestedRow];

    NSLog(@"[%@] [%@] Requested:  [%@]", @"DEBUG", [[self class] description], self.announcementButtonCurrentTitle);

    if ([self announcementSupportsOnboardingInterface:self.announcementButtonCurrentTitle]) {
        [self.onboardingOptionsAlert show];
    } else {
        [self.announcementOptionsAlert show];
    }
}

- (bool)announcementSupportsOnboardingInterface:(NSString *)announcementKey
{
    AJSCAboutAnnouncement *announcement = [(AJSCClientInformation *)[self.clientInformationDict valueForKey:announcementKey] announcement];
    AJNAboutObjectDescription *aboutObjectDescription = [[AJNAboutObjectDescription alloc] initWithMsgArg: announcement.objectDescriptionArg];
    return [aboutObjectDescription hasInterface:ONBOARDING_INTERFACE_NAME withPath:ONBOARDING_OBJECT_PATH];
}

#pragma mark stop AboutClient
- (void)stopAboutClient
{
    QStatus status;
    NSLog(@"[%@] [%@] Stop About Client", @"DEBUG", [[self class] description]);

    // Bus attachment cleanup
    status = [self.clientBusAttachment cancelAdvertisedName:[NSString stringWithFormat:@"%@%@", DAEMON_QUIET_PREFIX, self.realmBusName] withTransportMask:kAJNTransportMaskAny];
    if (status == ER_OK) {
        NSLog(@"[%@] [%@] Successfully cancel advertised name", @"DEBUG", [[self class] description]);
    } else {
        NSLog(@"[%@] [%@]  Failed cancel advertised name, error:%@", @"DEBUG", [[self class] description], [AJNStatus descriptionForStatusCode:status]);

    }

    status = [self.clientBusAttachment releaseWellKnownName:self.realmBusName];
    if (status == ER_OK) {
        NSLog(@"[%@] [%@] Successfully release WellKnownName", @"DEBUG", [[self class] description]);
    } else {
        NSLog(@"[%@] [%@]  Failed release WellKnownName, error:%@", @"DEBUG", [[self class] description], [AJNStatus descriptionForStatusCode:status]);

    }

    status = [self.clientBusAttachment removeMatchRule:@"sessionless='t',type='error'"];
    if (status == ER_OK) {
        NSLog(@"[%@] [%@] Successfully remove MatchRule", @"DEBUG", [[self class] description]);
    } else {
        NSLog(@"[%@] [%@]  Failed remove MatchRule, error:%@", @"DEBUG", [[self class] description], [AJNStatus descriptionForStatusCode:status]);

    }

    // Cancel advertise name for each announcement bus
    for (NSString *key in[self.clientInformationDict allKeys]) {
        AJSCClientInformation *clientInfo = (self.clientInformationDict)[key];
        status = [self.clientBusAttachment cancelFindAdvertisedName:[[clientInfo announcement] busName]];
        if (status != ER_OK) {
            NSLog(@"[%@] [%@] failed to cancelAdvertisedName for %@. status:%@", @"ERROR", [[self class] description], key, [AJNStatus descriptionForStatusCode:status]);
        }
    }
    
    self.clientInformationDict = nil;
    
    // Leave all sessions
    for(NSString *key in self.deviceSessionDict) {
        AJNSessionId sessionId = [self.deviceSessionDict[key] unsignedIntValue];
        status = [self.clientBusAttachment leaveSession:sessionId];
        if (ER_OK != status) {
            NSLog(@"Failed to leave session %u, %@", sessionId, [AJNStatus descriptionForStatusCode:status]);
        }
    }
    
    self.deviceSessionDict = nil;

    [self.clientBusAttachment unregisterAboutListener:self];

    // Stop bus attachment
    status = [self.clientBusAttachment stop];
    if (status == ER_OK) {
        NSLog(@"[%@] [%@] Successfully stopped bus", @"DEBUG", [[self class] description]);
    } else {
        NSLog(@"[%@] [%@]  Failed stopping bus, error:%@", @"DEBUG", [[self class] description], [AJNStatus descriptionForStatusCode:status]);
    }

    self.clientBusAttachment = nil;

    // Set flag
    self.isAboutClientConnected = false;

    // UI cleanup
    [self.connectButton setTitle:self.ajconnect forState:UIControlStateNormal];

    [self.servicesTable performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];

    NSLog(@"[%@] [%@] About Client is stopped", @"DEBUG", [[self class] description]);
}

#pragma mark UITableView delegates
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.clientInformationDict count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView
    cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *MyIdentifier = @"AnnouncementCell";

    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:MyIdentifier forIndexPath:indexPath];

    cell.selectionStyle = UITableViewCellSelectionStyleNone;

    cell.textLabel.text = [self.clientInformationDict allKeys][indexPath.row];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [self announcementGetMoreInfo:indexPath.row];
}

- (void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath
{
    [self announcementGetMoreInfo:indexPath.row];
}

#pragma mark - Onboarding event methods

- (void)onOnboardingStarted:(NSString *)ssid
{
    // Start checking for announcement from onboardee to say that they have onboarded successfully.
    self.isWaitingForOnboardee = true;
    self.targetSSID = [NSString stringWithString:ssid];
}

- (void)checkIfOnboardingFailed
{
    // Assume the onboardee failed to onboard to the target network if an announcement wasn't received.
    if (self.isWaitingForOnboardee) {
        NSString *message = [NSString stringWithFormat:@"Confirmation announcement not received from onboardee."];
        [AJSCAlertControllerManager queueAlertWithTitle:@"Onboarding failed" message:message viewController:self];
        self.isWaitingForOnboardee = false;
    }
}

#pragma mark - AJNSessionListener

- (void)sessionWasLost:(AJNSessionId)sessionId
{
    NSMutableArray *removeKeys = [[NSMutableArray alloc]init];
    
    for (NSString *key in self.deviceSessionDict) {
        NSNumber *tmpSessionId = self.deviceSessionDict[key];
        if([tmpSessionId unsignedIntValue] == sessionId) {
            [removeKeys addObject:key];
        }
    }
    
    for (NSString *key in removeKeys) {
        [self.clientInformationDict removeObjectForKey:key];
        [self.deviceSessionDict removeObjectForKey:key];
        NSLog(@"Session was lost with device: %@", key);
    }
    
    [self.servicesTable performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];
}

- (void)sessionWasLost:(AJNSessionId)sessionId forReason:(AJNSessionLostReason)reason
{
    [self sessionWasLost:sessionId];
}

#pragma mark - Util methods

// Copied from OnboardingViewController - should ideally be in util class.
- (BOOL)isSimulatorDevice
{
    bool systemVersionGreaterOrEqualTo9 = ([[[UIDevice currentDevice] systemVersion] compare:@"9.0" options:NSNumericSearch] != NSOrderedAscending);

    if (systemVersionGreaterOrEqualTo9) {
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

@end
