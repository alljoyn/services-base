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

#import "MainViewController.h"
#import "AJNStatus.h"
#import "alljoyn/controlpanel/AJCPSGetControlPanelViewController.h"
#include <qcc/Log.h>
#import "AppDelegate.h"
#import "samples_common/AJSCAboutAnnouncement.h"
#import "samples_common/AJSCAboutDataConverter.h"
#import "samples_common/AJSCClientInformation.h"
#import "samples_common/AJSCGetAboutCallViewController.h"
#import "samples_common/AJSCAuthenticationListenerImpl.h"
#import "samples_common/AJSCAlertController.h"
#import "samples_common/AJSCAnnounceTextViewController.h"
#import "AJNAboutObjectDescription.h"


static bool ALLOWREMOTEMESSAGES = true; // About Client -  allow Remote Messages flag
static NSString *const APPNAME = @"AboutClientMain"; // About Client - default application name
static NSString *const DAEMON_QUIET_PREFIX = @"quiet@";   // About Client - quiet advertising
static NSString *const CONTROLPANEL_OBJECT_PATH = @"/ControlPanel/";
static NSString *const CONTROLPANEL_INTERFACE_NAME = @"org.alljoyn.ControlPanel.ControlPanel";
static NSString *const HTTPCONTROL_INTERFACE_NAME = @"org.alljoyn.ControlPanel.HTTPControl";
static NSString *const DEFAULT_REALM_BUS_NAME = @"org.alljoyn.BusNode.ControlPanelClient";

static NSString *const KEYSTORE_FILE_PATH = @"Documents/alljoyn_keystore/s_central.ks";
static NSString *const AUTH_MECHANISM = @"ALLJOYN_SRP_KEYX ALLJOYN_ECDHE_PSK";

static NSString *const DEFAULT_AUTH_PASSWORD = @"121212";

@interface MainViewController ()
@property NSString *className;

// About Client properties
@property (strong, nonatomic) AJNBusAttachment *clientBusAttachment;
@property (strong, nonatomic) NSString *realmBusName;
@property (nonatomic) bool isAboutClientConnected;
@property (strong, nonatomic) NSMutableDictionary *clientInformationDict; // Store the client related information

// Announcement
@property (strong, nonatomic) NSString *announcementButtonCurrentTitle; // The pressed button's announcementUniqueName
@property (strong, nonatomic) dispatch_queue_t annBtnCreationQueue;

// About Client strings
@property (strong, nonatomic) NSString *ajconnect;
@property (strong, nonatomic) NSString *ajdisconnect;
@property (strong, nonatomic) NSString *annSubvTitleLabelDefaultTxt;

// About Client alerts
@property (strong, nonatomic) AJSCAlertController *announcementOptionsAlert;
@property (strong, nonatomic) AJSCAlertController *announcementOptionsAlertNoCPanel;

@property (strong, nonatomic) AJSCAuthenticationListenerImpl *authenticationListenerImpl;
@end

@implementation MainViewController

#pragma mark - Built In methods
- (void)viewDidLoad
{
    [super viewDidLoad];
    [self loadNewSession];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
// This currently does not work due to issues in AJNBusAttachment but can be uncommented once ASACORE-3323 is resolved.
//    if (self.isAboutClientConnected) {
//        [self.clientInformationDict removeAllObjects];
//        [self.servicesTable performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];
//        [self registerAboutListener];
//    }
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
// This currently does not work due to issues in AJNBusAttachment but can be uncommented once ASACORE-3323 is resolved.
//    if (self.isAboutClientConnected) {
//        [self unregisterAboutListener];
//    }
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
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
    AJSCClientInformation *clientInformation = [[AJSCClientInformation alloc] init];
    
    // Save the announcement in a AJSCAboutAnnouncement
    clientInformation.announcement = [[AJSCAboutAnnouncement alloc] initWithBusName:busName
                                                                            version:version
                                                                        sessionPort:port
                                                               objectDescriptionArg:objectDescriptionArg
                                                                       aboutDataArg:aboutDataArg];

    NSMutableDictionary *aboutDataDict = [AJSCAboutDataConverter aboutDataArgToDict:aboutDataArg];
    
    // Generate an announcement unique name in a format of <busName DeviceName>
    NSString *announcementUniqueName = [NSString stringWithFormat:@"%@ %@", [clientInformation.announcement busName], aboutDataDict[@"DeviceName"]];
    NSLog(@"[%@] [%@] Announcement unique name [%@]", @"DEBUG", [[self class] description], announcementUniqueName);

    NSString *appId = aboutDataDict[@"AppId"];
    if (appId == nil) {
        NSLog(@"[%@] [%@] Failed to read appId for key [%@]", @"DEBUG", [[self class] description], announcementUniqueName);
        return;
    }

    // Dealing with announcement entries should be syncronized, so we add it to a queue
    dispatch_sync(self.annBtnCreationQueue, ^{
        bool isAppIdExists = false;
        
        // Iterate over the announcements dictionary
        for (NSString *key in self.clientInformationDict.allKeys) {
            AJSCClientInformation *clientInfo = [self.clientInformationDict valueForKey:key];
            AJSCAboutAnnouncement *announcement = [clientInfo announcement];
            NSMutableDictionary *aboutDataDict = [AJSCAboutDataConverter aboutDataArgToDict:[announcement aboutDataArg]];
            NSString *savedAppId = aboutDataDict[@"AppId"];

            if (savedAppId == nil) {
                NSLog(@"[%@] [%@] Failed to read appId for key [%@]", @"DEBUG", [[self class] description], key);
                return;
            }

            if ([savedAppId compare:appId] == NSOrderedSame) {
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
                        QStatus status = [self.clientBusAttachment cancelFindAdvertisedName:prevBusName];
                        if (status != ER_OK) {
                            NSLog(@"[%@] [%@] failed to cancelAdvertisedName for %@. status:%@", @"DEBUG", [[self class] description], prevBusName, [AJNStatus descriptionForStatusCode:status]);
                        }
                    }
                    // Remove existed record from the announcements dictionary
                    [self.clientInformationDict removeObjectForKey:key];
                    // Add new record to the announcements dictionary
                    [self.clientInformationDict setValue:clientInformation forKey:announcementUniqueName];
                    // Update UI
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
    QStatus status = [self.clientBusAttachment findAdvertisedName:busName];
    if (status != ER_OK) {
        NSLog(@"[%@] [%@] failed to findAdvertisedName for %@. status:%@", @"ERROR", [[self class] description], busName, [AJNStatus descriptionForStatusCode:status]);
    }
}

#pragma mark AJNBusListener protocol methods

- (void)didFindAdvertisedName:(NSString *)name withTransportMask:(AJNTransportMask)transport namePrefix:(NSString *)namePrefix
{
    NSLog(@"didFindAdvertisedName %@", name);
}

- (void)didLoseAdvertisedName:(NSString *)name withTransportMask:(AJNTransportMask)transport namePrefix:(NSString *)namePrefix
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
            [self.servicesTable performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];
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
    self.connectButton.backgroundColor = [UIColor darkGrayColor]; //button bg color
    [self.connectButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal]; //button font color
    [self.connectButton setTitle:self.ajconnect forState:UIControlStateNormal]; //default text

    [self prepareAlerts];

}

//  Initialize alerts
- (void)prepareAlerts
{
    [self prepareAnnouncementOptionsAlert];
    [self prepareAnnouncementOptionsAlertNoCPanel];
}

- (void)prepareAnnouncementOptionsAlert
{
    __weak MainViewController *weakSelf = self;

    self.announcementOptionsAlert = [AJSCAlertController alertControllerWithTitle:@"Choose option:"
                                     message:@""
                                     viewController:self];
    [self.announcementOptionsAlert addActionWithName:@"Show Announce"
     handler:^(UIAlertAction *) {
         [weakSelf performSegueWithIdentifier:@"AboutShowAnnounceSegue" sender:weakSelf];
     }];
    [self.announcementOptionsAlert addActionWithName:@"About"
     handler:^(UIAlertAction *) {
         [weakSelf performSegueWithIdentifier:@"AboutClientSegue" sender:weakSelf]; // get the announcment object
     }];

    [self.announcementOptionsAlert addActionWithName:@"Control Panel"
     handler:^(UIAlertAction *) {
         AJSCAboutAnnouncement *announcement = [(AJSCClientInformation *)(weakSelf.clientInformationDict)[weakSelf.announcementButtonCurrentTitle] announcement];
         NSMutableDictionary *announcementObjDecs = [AJSCAboutDataConverter objectDescriptionArgToDict:[announcement objectDescriptionArg]];
         AJCPSGetControlPanelViewController *getCpanelView =
             [[AJCPSGetControlPanelViewController alloc] initWithBusName:announcement.busName
                                                      objectDescription:[[AJNAboutObjectDescription alloc] initWithMsgArg:announcement.objectDescriptionArg]
                                                                     bus:weakSelf.clientBusAttachment];
         [weakSelf.navigationController pushViewController:getCpanelView animated:YES];
     }];
}

- (void)prepareAnnouncementOptionsAlertNoCPanel
{
    __weak MainViewController *weakSelf = self;

    self.announcementOptionsAlertNoCPanel = [AJSCAlertController alertControllerWithTitle:@"Choose option:"
                                             message:@""
                                             viewController:self];
    [self.announcementOptionsAlertNoCPanel addActionWithName:@"Cancel"
     handler:^(UIAlertAction *action) {

     }];
    [self.announcementOptionsAlertNoCPanel addActionWithName:@"Show Announce"
     handler:^(UIAlertAction *action) {
         [weakSelf performSegueWithIdentifier:@"AboutShowAnnounceSegue" sender:weakSelf];
     }];
    [self.announcementOptionsAlertNoCPanel addActionWithName:@"About"
     handler:^(UIAlertAction *action) {
         [weakSelf performSegueWithIdentifier:@"AboutClientSegue" sender:weakSelf]; // get the announcment object
     }];
}

- (void)alertAndLog:(NSString *)message status:(QStatus)status
{
    NSString *alertText = [NSString stringWithFormat:@"%@ (%@)", message, [AJNStatus descriptionForStatusCode:status]];

    NSLog(@"%@", alertText);

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

    NSLog(@"[%@] [%@] Start About Client", @"DEBUG", [[self class] description]);

    // Init AJNBusAttachment
    self.clientBusAttachment = [[AJNBusAttachment alloc] initWithApplicationName:APPNAME allowRemoteMessages:ALLOWREMOTEMESSAGES];
    
    NSLog(@"[%@] [%@] Register BusListener", @"DEBUG", [[self class] description]);
    [self.clientBusAttachment registerBusListener:self];
    
    // Start AJNBusAttachment
    status = [self.clientBusAttachment start];
    if (status != ER_OK) {
        [self alertAndLog:@"Failed AJNBusAttachment start" status:status];
        [self stopAboutClient];
        return;
    }
    
    // Connect AJNBusAttachment
    status = [self.clientBusAttachment connectWithArguments:@""];
    if (status != ER_OK) {
        [self alertAndLog:@"Failed AJNBusAttachment connectWithArguments" status:status];
        [self stopAboutClient];
        return;
    }
    
    [self registerAboutListener];

    NSUUID *UUID = [NSUUID UUID];
    NSString *stringUUID = [UUID UUIDString];
    self.realmBusName = [NSString stringWithFormat:@"%@-%@", DEFAULT_REALM_BUS_NAME, stringUUID];

    // Advertise Daemon for tcl
    status = [self.clientBusAttachment requestWellKnownName:self.realmBusName withFlags:kAJNBusNameFlagDoNotQueue];
    if (status == ER_OK) {
        status = [self.clientBusAttachment advertiseName:[NSString stringWithFormat:@"%@%@", DAEMON_QUIET_PREFIX, self.realmBusName] withTransportMask:kAJNTransportMaskAny];
        if (status != ER_OK) {
            [self alertAndLog:@"Failed to advertise name" status:status];
            [self stopAboutClient];
            return;
        } else {
            NSLog(@"[%@] [%@] Successfully advertised: %@%@", @"DEBUG", [[self class] description], DAEMON_QUIET_PREFIX, self.realmBusName);
        }
    } else {
        [self alertAndLog:@"Failed to requestWellKnownName" status:status];
        [self stopAboutClient];
        return;
    }

    // Enable Client Security
    self.authenticationListenerImpl = [[AJSCAuthenticationListenerImpl alloc] initWithViewController:self];
    self.authenticationListenerImpl.defaultPasscode = DEFAULT_AUTH_PASSWORD;
    status = [self enableClientSecurity];
    if (ER_OK != status) {
        NSLog(@"Failed to enable security.");
    } else {
        NSLog(@"Successfully enabled security for the bus");
    }

    [self.connectButton setTitle:self.ajdisconnect forState:UIControlStateNormal]; //change title to "Disconnect from AllJoyn"
    self.isAboutClientConnected = true;
}

- (QStatus)enableClientSecurity
{
    QStatus status;
    status = [self.clientBusAttachment enablePeerSecurity:AUTH_MECHANISM authenticationListener:self.authenticationListenerImpl keystoreFileName:KEYSTORE_FILE_PATH sharing:YES];

    if (status != ER_OK) { //try to delete the keystore and recreate it, if that fails return failure
        NSError *error;
        NSString *keystoreFilePath = [NSString stringWithFormat:@"%@/alljoyn_keystore/s_central.ks", [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0]];
        [[NSFileManager defaultManager] removeItemAtPath:keystoreFilePath error:&error];
        if (error) {
            NSLog(@"ERROR: Unable to delete keystore. %@", error);
            return ER_AUTH_FAIL;
        }
        status = [self.clientBusAttachment enablePeerSecurity:AUTH_MECHANISM authenticationListener:self.authenticationListenerImpl keystoreFileName:KEYSTORE_FILE_PATH sharing:YES];
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

    // Check if announcement has icon object path
    if ([self announcementHasCPanel:self.announcementButtonCurrentTitle]) {
        [self.announcementOptionsAlert show]; // Event is forward to alertView: clickedButtonAtIndex:
    } else {
        [self.announcementOptionsAlertNoCPanel show]; // Event is forward to alertView: clickedButtonAtIndex:
    }
}

// Return true if an announcement supports icon interface
- (bool)announcementHasCPanel:(NSString *)announcementKey
{
    bool hascPanel = false;
    AJSCAboutAnnouncement *announcement = [(AJSCClientInformation *)[self.clientInformationDict valueForKey:announcementKey] announcement];
    NSMutableDictionary *announcementObjDecs = [AJSCAboutDataConverter objectDescriptionArgToDict:[announcement objectDescriptionArg]]; //Dictionary of ObjectDescriptions NSStrings

    // iterate over the object descriptions dictionary
    for (NSString *key in announcementObjDecs.allKeys) {
        if ([key hasPrefix:CONTROLPANEL_OBJECT_PATH]) {
            // Iterate over the NSMutableArray
            for (NSString *intf in[announcementObjDecs valueForKey:key]) {
                if ([intf isEqualToString:(NSString *)CONTROLPANEL_INTERFACE_NAME]) {
                    hascPanel = true;
                }
            }
        }
    }
    return hascPanel;
}

- (void)registerAboutListener
{
    NSLog(@"[%@] [%@] Register AboutListener", @"DEBUG", [[self class] description]);
    [self.clientBusAttachment registerAboutListener:self];
    
    QStatus status = [self.clientBusAttachment whoImplementsInterface:CONTROLPANEL_INTERFACE_NAME];
    if (status != ER_OK) {
        [self alertAndLog:@"Failed to registerAnnouncementReceiver" status:status];
        [self stopAboutClient];
        return;
    }
    
    status = [self.clientBusAttachment whoImplementsInterface:HTTPCONTROL_INTERFACE_NAME];
    if (status != ER_OK) {
        [self alertAndLog:@"Failed to registerAnnouncementReceiver" status:status];
        [self stopAboutClient];
        return;
    }
}

- (void)unregisterAboutListener
{
    [self.clientBusAttachment unregisterAboutListener:self];
    
    QStatus status = [self.clientBusAttachment cancelWhoImplementsInterface:CONTROLPANEL_INTERFACE_NAME];
    if (status != ER_OK) {
        NSLog(@"Failed to unregister %@", CONTROLPANEL_INTERFACE_NAME);
    }
    
    status = [self.clientBusAttachment cancelWhoImplementsInterface:HTTPCONTROL_INTERFACE_NAME];
    if (status != ER_OK) {
        NSLog(@"Failed to unregister %@", HTTPCONTROL_INTERFACE_NAME);
    }
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
    }
    status = [self.clientBusAttachment releaseWellKnownName:self.realmBusName];
    if (status == ER_OK) {
        NSLog(@"[%@] [%@] Successfully release WellKnownName", @"DEBUG", [[self class] description]);
    }
    status = [self.clientBusAttachment removeMatchRule:@"sessionless='t',type='error'"];
    if (status == ER_OK) {
        NSLog(@"[%@] [%@] Successfully remove MatchRule", @"DEBUG", [[self class] description]);
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

    [self unregisterAboutListener];

    // Stop bus attachment
    status = [self.clientBusAttachment stop];
    if (status == ER_OK) {
        NSLog(@"[%@] [%@] Successfully stopped bus", @"DEBUG", [[self class] description]);
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

@end