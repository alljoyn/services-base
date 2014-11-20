/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
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

#import "ViewController.h"
#import "alljoyn/about/AJNAnnouncementReceiver.h"
#import "alljoyn/about/AJNAboutServiceApi.h"
#import "alljoyn/time/AJTMTimeServiceServer.h"
#import "AJNVersion.h"
#import "AppDelegate.h"
#import "CommonBusListener.h"
#import "AuthenticationListenerImpl.h"
#import "SampleClock.h"
#import "SampleAlarm.h"
#import "SampleTimer.h"
#import "alljoyn/time/AJTMTimeServiceConstants.h"


static bool ALLOWREMOTEMESSAGES = true; // About Client -  allow Remote Messages flag
static NSString *const APPNAME = @"TimeServiceApp";  //About Client - default application name
static NSString *const KEYSTORE_FILE_PATH = @"Documents/alljoyn_keystore/s_central.ks";
static NSString * const AUTH_MECHANISM = @"ALLJOYN_SRP_KEYX ALLJOYN_PIN_KEYX ALLJOYN_ECDHE_PSK";
static NSString *const DAEMON_QUIET_PREFIX = @"quiet@";    //About Client - quiet advertising
static NSString *const DEFAULT_PASSCODE = @"000000";
static AJNSessionPort SERVICE_PORT = 900; // About Service - service port

@interface ViewController ()

- (IBAction)startServiceButtonDidTouchUpInside:(id)sender;
@property AJNBusAttachment* bus;

@property (strong, nonatomic) NSMutableDictionary *peersPasscodes; // Store the peers passcodes
@property (strong, nonatomic) AJNAnnouncementReceiver *announcementReceiver;
@property (strong, nonatomic) NSString *realmBusName;

@property (strong, nonatomic) AJNAboutPropertyStoreImpl *aboutPropertyStoreImpl;
@property (strong, nonatomic) AJNAboutServiceApi *aboutServiceApi;
@property (strong, nonatomic) CommonBusListener *aboutSessionPortListener;
@property (strong, nonatomic) NSString *uniqueID;

@property (strong, nonatomic) AuthenticationListenerImpl *authenticationListenerImpl;

//BusAttachment* bus                           = NULL;
//CommonBusListener* busListener               = NULL;
//AboutPropertyStoreImpl* propertyStoreImpl    = NULL;
//SrpKeyXListener* srpKeyXListener             = NULL;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.uniqueID = [[NSUUID UUID] UUIDString];
    self.realmBusName = @"org.alljoyn.BusNode.timeService";
    [self startAboutService];
}

- (IBAction)startServiceButtonDidTouchUpInside:(id)sender {

    AJTMTimeServiceServer* server = [AJTMTimeServiceServer sharedInstance];

    NSLog(@"Server is %@", [server isStarted] ? @"started" : @"stopped");

    QStatus status = [server initWithBus:self.bus];

    NSLog(@"Server is %@", [server isStarted] ? @"started" : @"stopped");

    if (status != ER_OK) {
        NSLog(@"Failed to start the TimeServiceServer: %@", [AJNStatus descriptionForStatusCode:status]);
        return;
    } else {
        [_startServiceButton setTitle :@"Stop Server" forState:(UIControlStateNormal)];
        NSLog(@"Got AJTMTimeServiceServer!");
    }

    //Clock
    AJTMTimeServiceServerClockBase *clock = [[SampleClock alloc] init];
    NSLog(@"Clock objectPath: %@", [clock objectPath]);

    status = [server createClock:clock];
    if (status != ER_OK) {
        NSLog(@"Failed to create clock: %@", [AJNStatus descriptionForStatusCode:status]);
        return;
    }

    /*
    AJTMTimeServiceServerAuthorityClock* authClock = [[AJTMTimeServiceServerAuthorityClock alloc] init]; // object should conformsToProtocol AJTMTimeServiceServerClock
    AJTMClockAuthorityType authType = CELLULAR;
    status = [server createTimeAuthorityClock:authClock authorityType:authType];
    if (status != ER_OK) {
        NSLog(@"Failed to create time authority clock: %@", [AJNStatus descriptionForStatusCode:status]);
        return;
    } else {
        status = [authClock timeSync];
        if (status != ER_OK) {
            NSLog(@"Failed to run time sync: %@", [AJNStatus descriptionForStatusCode:status]);
            return;
        }
    }
     
     */

    //Alarm
    AJTMTimeServiceServerAlarmBase *alarm = [[SampleAlarm alloc] init];
    status = [server createAlarm:alarm];
    if (status != ER_OK) {
        NSLog(@"Failed to create alarm: %@", [AJNStatus descriptionForStatusCode:status]);
        return;
    }

    //    -(QStatus)createAlarm:(AJTMTimeServiceServerAlarmBase*) alarm description:(NSString*) description language:(NSString*) language translator:(id<AJNTranslator>) translator;
    //    -(QStatus)registerCustomAlarm:(AJTMTimeServiceServerAlarmBase*) alarmBusObj alarm:(id<AJTMTimeServiceServerAlarm>) alarm notAnnounced:(NSArray*) notAnnounced;
    //    -(QStatus)registerCustomAlarm:(id<AJTMTimeServiceAlarmBusObj>) alarmBusObj alarm:(AJTMTimeServiceServerAlarmBase*) alarm notAnnounced:(NSArray*) notAnnounced description:(NSString*) description language:(NSString*) language translator:(id<AJNTranslator>) translator;

    //Timer
    AJTMTimeServiceServerTimerBase *timer = [[SampleTimer alloc] init];
    status = [server createTimer:timer];
    if (status != ER_OK) {
        NSLog(@"Failed to create timer: %@", [AJNStatus descriptionForStatusCode:status]);
        return;
    }

    /*  Server API's
     //Factory
     -(QStatus) createAlarmFactory:(AJTMTimeServiceServerAlarmFactoryBase*) alarmFactory;
     -(QStatus) createAlarmFactory:(AJTMTimeServiceServerAlarmFactoryBase*) alarmFactory description:(NSString*) description language:(NSString*) language translator:(id<AJNTranslator>) translator;
     -(QStatus) createTimerFactory:(AJTMTimeServiceServerTimerFactoryBase*) timerFactory;
     -(QStatus) createTimerFactory:(AJTMTimeServiceServerTimerFactoryBase*) timerFactory description:(NSString*) description language:(NSString*) language translator:(id<AJNTranslator>) translator;


     -(QStatus)generateObjectPath:(NSString*) objectPath;
     */

    [self announce];
}

- (void)startAboutService
{
    QStatus status;

    NSLog(@"[%@] [%@] Start About Service", @"DEBUG", [[self class] description]);

    NSLog(@"[%@] [%@] AboutService using port %hu", @"DEBUG", [[self class] description], SERVICE_PORT);

    // Create message bus

    NSLog(@"[%@] [%@] Create a message bus", @"DEBUG", [[self class] description]);

    self.bus = [[AJNBusAttachment alloc] initWithApplicationName:@"AboutServiceName" allowRemoteMessages:ALLOWREMOTEMESSAGES];
    if (!self.bus) {

        NSLog(@"[%@] [%@] Failed to create a message bus - exiting application", @"DEBUG", [[self class] description]);

        status = ER_OUT_OF_MEMORY;
        exit(status);
    }
    // Start the bus
    NSLog(@"[%@] [%@] Start bus", @"DEBUG", [[self class] description]);

    status = [self.bus start];
    if (status != ER_OK) {

        NSLog(@"[%@] [%@] Failed to start bus - exiting application", @"FATAL", [[self class] description]);

        exit(status);
    }

    NSLog(@"[%@] [%@] Connect to an AllJoyn daemon", @"DEBUG", [[self class] description]);

    status = [self.bus connectWithArguments:@""];
    if (status != ER_OK) {
        NSLog(@"[%@] [%@] Failed to start bus - exiting application", @"FATAL", [[self class] description]);

        exit(status);
    }

    NSLog(@"[%@] [%@] Create AboutSessionPortListener", @"DEBUG", [[self class] description]);

    self.aboutSessionPortListener = [[CommonBusListener alloc] initWithServicePort:SERVICE_PORT];

    if (self.aboutSessionPortListener) {
        NSLog(@"[%@] [%@] Register AboutSessionPortListener", @"DEBUG", [[self class] description]);

        [self.bus registerBusListener:self.aboutSessionPortListener];
    }

    // prepare data for propertyStoreImpl
    self.aboutPropertyStoreImpl = [[AJNAboutPropertyStoreImpl alloc] init];

    if (!self.aboutPropertyStoreImpl) {
        NSLog(@"Failed to create propertyStoreImpl object - exiting application");
        [AppDelegate alertAndLog:@"Failed to create propertyStoreImpl object - exiting application" status:ER_FAIL];
        exit(status);
    }
    status = [self fillAboutPropertyStoreImplData];

    if (status != ER_OK) {
        NSLog(@"Failed to fill propertyStore - exiting application");
        [AppDelegate alertAndLog:@"Failed to fill propertyStore - exiting application" status:status];
        exit(status);
    }

    NSLog(@"[%@] [%@] Create aboutServiceApi", @"DEBUG", [[self class] description]);

    self.aboutServiceApi = [AJNAboutServiceApi sharedInstance];
    if (!self.aboutServiceApi) {
        NSLog(@"Failed to create aboutServiceApi - exiting application");
        [AppDelegate alertAndLog:@"Failed to create aboutServiceApi - exiting application" status:status];
        exit(status);
    }

    NSLog(@"[%@] [%@] Start aboutServiceApi", @"DEBUG", [[self class] description]);

    [self.aboutServiceApi startWithBus:self.bus andPropertyStore:self.aboutPropertyStoreImpl];

    // Register Port
    NSLog(@"[%@] [%@] Register the AboutService on the AllJoyn bus", @"DEBUG", [[self class] description]);

    if (self.aboutServiceApi.isServiceStarted) {
        status = [self.aboutServiceApi registerPort:(SERVICE_PORT)];
    }

    if (status != ER_OK) {
        NSLog(@"Failed register port - exiting application");
        [AppDelegate alertAndLog:@"Failed register port - exiting application" status:status];
        exit(status);
    }

    // Bind session port
    AJNSessionOptions *opt = [[AJNSessionOptions alloc] initWithTrafficType:kAJNTrafficMessages supportsMultipoint:false proximity:kAJNProximityAny transportMask:kAJNTransportMaskAny];

    NSLog(@"[%@] [%@] Bind session", @"DEBUG", [[self class] description]);


    status = [self.bus bindSessionOnPort:SERVICE_PORT withOptions:opt withDelegate:self.aboutSessionPortListener];

    if (status == ER_ALLJOYN_BINDSESSIONPORT_REPLY_ALREADY_EXISTS) {
        NSLog(@"[%@] [%@] SessionPort already exists", @"DEBUG", [[self class] description]);
    }


    self.authenticationListenerImpl = [[AuthenticationListenerImpl alloc] init];
    status = [self enableClientSecurity];

    if (ER_OK != status) {
        NSLog(@"Failed to enable security on the bus. %@", [AJNStatus descriptionForStatusCode:status]);
    }
    else {
        NSLog(@"Successfully enabled security for the bus");
    }


    // self.isAboutServiceConnected = true;
}

- (void)announce
{
    QStatus serviceStatus;

    NSLog(@"[%@] [%@] Calling Announce", @"DEBUG", [[self class] description]);
    serviceStatus = [self.aboutServiceApi announce];

    if (serviceStatus == ER_OK) {
        NSLog(@"[%@] [%@] Successfully announced", @"DEBUG", [[self class] description]);
    }
}


- (QStatus)fillAboutPropertyStoreImplData
{
    QStatus status;

    // AppId
    status = [self.aboutPropertyStoreImpl setAppId:self.uniqueID];
    if (status != ER_OK) {
        return status;
    }

    // AppName
    status = [self.aboutPropertyStoreImpl setAppName:@"AboutConfig"];
    if (status != ER_OK) {
        return status;
    }

    // DeviceId
    status = [self.aboutPropertyStoreImpl setDeviceId:@"1231232145667745675477"];
    if (status != ER_OK) {
        return status;
    }

    // DeviceName
    status = [self.aboutPropertyStoreImpl setDeviceName:@"TimeService" language:@"en"];
    if (status != ER_OK) {
        return status;
    }

    status = [self.aboutPropertyStoreImpl setDeviceName:@"TempsService" language:@"fr"];
    if (status != ER_OK) {
        return status;
    }

    status = [self.aboutPropertyStoreImpl setDeviceName:@"HoraService" language:@"sp"];
    if (status != ER_OK) {
        return status;
    }

    // SupportedLangs
    NSArray *languages = @[@"en", @"sp", @"fr"];
    status = [self.aboutPropertyStoreImpl setSupportedLangs:languages];
    if (status != ER_OK) {
        return status;
    }

    //  DefaultLang
    status = [self.aboutPropertyStoreImpl setDefaultLang:@"en"];
    if (status != ER_OK) {
        return status;
    }

    //  ModelNumbe
    status = [self.aboutPropertyStoreImpl setModelNumber:@"Wxfy388i"];
    if (status != ER_OK) {
        return status;
    }

    //  DateOfManufacture
    status = [self.aboutPropertyStoreImpl setDateOfManufacture:@"10/1/2199"];
    if (status != ER_OK) {
        return status;
    }
    //  SoftwareVersion
    status = [self.aboutPropertyStoreImpl setSoftwareVersion:@"12.20.44 build 44454"];
    if (status != ER_OK) {
        return status;
    }

    //  AjSoftwareVersion
    status = [self.aboutPropertyStoreImpl setAjSoftwareVersion:[AJNVersion versionInformation]];
    if (status != ER_OK) {
        return status;
    }

    //  HardwareVersion
    status = [self.aboutPropertyStoreImpl setHardwareVersion:@"355.499. b"];
    if (status != ER_OK) {
        return status;
    }
    //  Description
    status = [self.aboutPropertyStoreImpl setDescription:@"This is an Alljoyn Application" language:@"en"];
    if (status != ER_OK) {
        return status;
    }
    status = [self.aboutPropertyStoreImpl setDescription:@"Esta es una Alljoyn aplicación" language:@"sp"];
    if (status != ER_OK) {
        return status;
    }
    status = [self.aboutPropertyStoreImpl setDescription:@"C'est une Alljoyn application"  language:@"fr"];
    if (status != ER_OK) {
        return status;
    }
    //  Manufacturer
    status = [self.aboutPropertyStoreImpl setManufacturer:@"Company" language:@"en"];
    if (status != ER_OK) {
        return status;
    }
    status = [self.aboutPropertyStoreImpl setManufacturer:@"Empresa" language:@"sp"];
    if (status != ER_OK) {
        return status;
    }
    status = [self.aboutPropertyStoreImpl setManufacturer:@"Entreprise" language:@"fr"];
    if (status != ER_OK) {
        return status;
    }

    //  SupportedUrl
    status = [self.aboutPropertyStoreImpl setSupportUrl:@"http://www.alljoyn.org"];
    if (status != ER_OK) {
        return status;
    }
    return status;
}

- (void)startAboutClient
{
    QStatus status;

    NSLog(@"Start About Client");

    // Init AJNBusAttachment
    self.bus = [[AJNBusAttachment alloc] initWithApplicationName:(APPNAME) allowRemoteMessages:(ALLOWREMOTEMESSAGES)];

    // Start AJNBusAttachment
    status = [self.bus start];
    if (status != ER_OK) {
        [AppDelegate alertAndLog:@"Failed at AJNBusAttachment start" status:status];
        [self stopAboutClient];
        return;
    }

    // Connect AJNBusAttachment
    status = [self.bus connectWithArguments:(@"")];
    if (status != ER_OK) {
        [AppDelegate alertAndLog:@"Failed at AJNBusAttachment connectWithArguments" status:status];
        [self stopAboutClient];
        return;
    }

    NSLog(@"set aboutClientListener");

    [self.bus registerBusListener:self];

    self.announcementReceiver = [[AJNAnnouncementReceiver alloc] initWithAnnouncementListener:self andBus:self.bus];
    status = [self.announcementReceiver registerAnnouncementReceiverForInterfaces:nil withNumberOfInterfaces:0];
    if (status != ER_OK) {
        [AppDelegate alertAndLog:@"Failed to register Announcement Receiver" status:status];
        [self stopAboutClient];
        return;
    }

    //    [AnnouncementManager sharedInstance]; //announcement manager for all no gw annoncement

    // Advertise Daemon for tcl
    status = [self.bus requestWellKnownName:self.realmBusName withFlags:kAJNBusNameFlagDoNotQueue];
    if (status == ER_OK) {
        // Advertise the name with a quite prefix for TC to find it
        NSUUID *UUID = [NSUUID UUID];
        NSString *stringUUID = [UUID UUIDString];

        self.realmBusName = [self.realmBusName stringByAppendingFormat:@"-%@", stringUUID];

        status = [self.bus advertiseName:[NSString stringWithFormat:@"%@%@", DAEMON_QUIET_PREFIX, self.realmBusName] withTransportMask:kAJNTransportMaskAny];
        if (status != ER_OK) {
            [AppDelegate alertAndLog:@"Failed at AJNBusAttachment advertiseName" status:status];
            [self stopAboutClient];
            return;
        }
        else {
            NSLog(@"Successfully advertised: %@%@", DAEMON_QUIET_PREFIX, self.realmBusName);
        }
    } else {
        [AppDelegate alertAndLog:@"Failed at AJNBusAttachment requestWellKnownName" status:status];
        [self stopAboutClient];
        return;
    }

    //    [self.connectButton setTitle:self.ajdisconnect forState:UIControlStateNormal]; //change title to "Disconnect from AllJoyn"


    // Create NSMutableDictionary dictionary of peers passcodes
    //    self.peersPasscodes = [[NSMutableDictionary alloc] init];

    // Enable Client Security
    self.authenticationListenerImpl = [[AuthenticationListenerImpl alloc] init];
    status = [self enableClientSecurity];

    if (ER_OK != status) {
        NSLog(@"Failed to enable security on the bus. %@", [AJNStatus descriptionForStatusCode:status]);
    }
    else {
        NSLog(@"Successfully enabled security for the bus");
    }
    //    self.isAboutClientConnected = true;
    //    [self.servicesTable reloadData];
}


- (void)stopAboutClient
{
    QStatus status;
    NSLog(@"Stop About Client");
    /*
     // Bus attachment cleanup
     status = [self.bus cancelAdvertisedName:[NSString stringWithFormat:@"%@%@", DAEMON_QUIET_PREFIX, self.realmBusName] withTransportMask:kAJNTransportMaskAny];
     if (status == ER_OK) {
     NSLog(@"Successfully cancel advertised name");
     }
     status = [self.bus releaseWellKnownName:self.realmBusName];
     if (status == ER_OK) {
     NSLog(@"Successfully release WellKnownName");
     }
     status = [self.bus removeMatchRule:@"sessionless='t',type='error'"];
     if (status == ER_OK) {
     NSLog(@"Successfully remove MatchRule");
     }

     // Cancel advertise name for each announcement bus
     for (NSString *key in[self.clientInformationDict allKeys]) {
     ClientInformation *clientInfo = (self.clientInformationDict)[key];
     status = [self.bus cancelFindAdvertisedName:[[clientInfo announcement] busName]];
     if (status != ER_OK) {
     NSLog(@"Failed to cancelAdvertisedName for %@: %@", key, [AJNStatus descriptionForStatusCode:status]);
     }
     }
     self.clientInformationDict = nil;

     [[AnnouncementManager sharedInstance] destroyInstance];

     status = [self.announcementReceiver unRegisterAnnouncementReceiverForInterfaces:nil withNumberOfInterfaces:0];
     if (status == ER_OK) {
     NSLog(@"Successfully unregistered AnnouncementReceiver");
     }

     self.announcementReceiver = nil;

     // Stop bus attachment
     status = [self.bus stop];
     if (status == ER_OK) {
     NSLog(@"Successfully stopped bus");
     }
     self.bus = nil;

     self.peersPasscodes = nil;


     // Set flag
     self.isAboutClientConnected  = false;

     // UI cleanup
     [self.connectButton setTitle:self.ajconnect forState:UIControlStateNormal];

     [self.servicesTable performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];

     NSLog(@"About Client is stopped");
     */

}

#pragma mark AJNBusListener protocol methods
- (void)didFindAdvertisedName:(NSString *)name withTransportMask:(AJNTransportMask)transport namePrefix:(NSString *)namePrefix
{
    NSLog(@"didFindAdvertisedName has been called");
}

- (void)didLoseAdvertisedName:(NSString *)name withTransportMask:(AJNTransportMask)transport namePrefix:(NSString *)namePrefix
{
    NSLog(@"didLoseAdvertisedName has been called");
    QStatus status;
    //    // Find the button title that should be removed
    //    for (NSString *key in[self.clientInformationDict allKeys]) {
    //        if ([[[[self.clientInformationDict valueForKey:key] announcement] busName] isEqualToString:name]) {
    //            // Cancel advertise name for that bus
    //            status = [self.bus cancelFindAdvertisedName:name];
    //            if (status != ER_OK) {
    //                NSLog(@"Failed to cancelFindAdvertisedName for %@. status:%@", name, [AJNStatus descriptionForStatusCode:status]);
    //            }
    //            // Remove the anouncement from the dictionary
    //            [self.clientInformationDict removeObjectForKey:key];
    //            [self.servicesTable reloadData];
    //        }
    //    }
}

- (QStatus)enableClientSecurity
{
    QStatus status;

    status = [self.bus enablePeerSecurity:AUTH_MECHANISM authenticationListener:self.authenticationListenerImpl keystoreFileName:KEYSTORE_FILE_PATH sharing:YES];

    if (status != ER_OK) { //try to delete the keystore and recreate it, if that fails return failure
        NSError *error;
        NSString *keystoreFilePath = [NSString stringWithFormat:@"%@/alljoyn_keystore/s_central.ks", [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0]];
        [[NSFileManager defaultManager] removeItemAtPath:keystoreFilePath error:&error];
        if (error) {
            NSLog(@"ERROR: Unable to delete keystore. %@", error);
            return ER_AUTH_FAIL;
        }
        status = [self.bus enablePeerSecurity:AUTH_MECHANISM authenticationListener:self.authenticationListenerImpl keystoreFileName:KEYSTORE_FILE_PATH sharing:YES];
    }

    return status;
}


#pragma mark - AJNAuthenticationListener protocol methods
- (AJNSecurityCredentials *)requestSecurityCredentialsWithAuthenticationMechanism:(NSString *)authenticationMechanism peerName:(NSString *)peerName authenticationCount:(uint16_t)authenticationCount userName:(NSString *)userName credentialTypeMask:(AJNSecurityCredentialType)mask
{
    AJNSecurityCredentials *creds = nil;
    bool credFound = false;

    NSLog(@"requestSecurityCredentialsWithAuthenticationMechanism:%@ forRemotePeer%@ userName:%@", authenticationMechanism, peerName, userName);

    if ([authenticationMechanism isEqualToString:@"ALLJOYN_SRP_KEYX"] || [authenticationMechanism isEqualToString:@"ALLJOYN_PIN_KEYX"] || [authenticationMechanism isEqualToString:@"ALLJOYN_ECDHE_PSK"]) {
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
                    creds.password = DEFAULT_PASSCODE;
                    NSLog(@"Using default password %@ for peer %@", DEFAULT_PASSCODE, peerName);
                }
            }
        }
    }
    return creds;
}

#pragma mark - AJNAnnouncementListener protocol method
// Here we receive an announcement from AJN and add it to the client's list of services avaialble
- (void)announceWithVersion:(uint16_t)version port:(uint16_t)port busName:(NSString *)busName objectDescriptions:(NSMutableDictionary *)objectDescs aboutData:(NSMutableDictionary **)aboutData
{
    //TODO
}

#pragma mark - AJNAuthenticationListener protocol method
- (void)authenticationUsing:(NSString *)authenticationMechanism forRemotePeer:(NSString *)peerName didCompleteWithStatus:(BOOL)success
{
    
}
@end
