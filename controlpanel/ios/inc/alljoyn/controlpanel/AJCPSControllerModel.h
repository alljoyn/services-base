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

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "alljoyn/Status.h"
#import "AJCPSContainer.h"
#import "AJCPSControlPanelListener.h"
#import "AJCPSControllerUpdateEvents.h"
#import "AJCPSControlPanel.h"

@interface ControllerModel : NSObject <AJCPSControlPanelListener>
@property (strong, nonatomic) NSString *unit;
@property (strong, nonatomic) AJCPSControlPanel *controlPanel;
@property (strong, atomic) NSArray *widgetsContainer;
@property (strong, nonatomic) id<ControllerUpdateEvents> delegate;
@property (strong, nonatomic,readonly) NSArray *supportedLanguages;

-(void)pushChildContainer:(AJCPSContainer *)containerToMoveTo;

-(NSInteger)popChildContainer;

-(QStatus)switchLanguage:(NSString *)language;

-(QStatus)switchLanguageForNotificationAction:(AJCPSRootWidget *)rootWidget;

-(QStatus)loadRootWidget:(AJCPSRootWidget *)rootWidget;

-(void)setSupportedLanguagesForNotificationAction:(AJCPSNotificationAction *) notificationAction;

-(NSInteger)childContainerPosition;
@end