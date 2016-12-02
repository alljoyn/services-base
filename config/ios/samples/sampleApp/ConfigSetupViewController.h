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
#import "alljoyn/config/AJCFGConfigClient.h"
#import "AJNSessionListener.h"

@interface ConfigSetupViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, weak) AJNBusAttachment *clientBusAttachment;
@property (nonatomic, strong) ClientInformation *clientInformation;
@property (nonatomic, weak) AJCFGConfigClient *configClient;
@property (nonatomic, strong) NSString *realmBusName;
@property (nonatomic, strong) NSMutableDictionary *peersPasscodes; // Store the peers passcodes
@property (nonatomic, weak) IBOutlet UIBarButtonItem *btnFactoryReset;
@property (nonatomic, weak) IBOutlet UIBarButtonItem *btnRestart;
@property (nonatomic, weak) IBOutlet UIBarButtonItem *btnSetPassword;

-(IBAction)factoryResetPressed: (id)sender;
-(IBAction)restartPressed: (id)sender;
-(IBAction)setPasswordPressed: (id)sender;

@end