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

#import "AJSCAlertControllerManager.h"
#import "AJSCAlertController.h"


@implementation AJSCAlertControllerManager

static NSMutableArray *alertControllerList = [[NSMutableArray alloc] init];

+ (void)queueAlertWithTitle:(nonnull NSString *)title message:(nonnull NSString *)message viewController:(nonnull UIViewController *)viewController
{
    AJSCAlertController *alertController = [AJSCAlertController alertControllerWithTitle:title
                                            message:message
                                            viewController:viewController];
    if (alertController) {
        [alertController addActionWithName:@"OK"
         handler:^(UIAlertAction *action) {
             [alertControllerList removeObjectAtIndex:0];
             [AJSCAlertControllerManager showNextAlert];
         }];
        [alertControllerList addObject:alertController];

        if (alertControllerList.count == 1) {
            [AJSCAlertControllerManager showNextAlert];
        }
    }
}

+ (void)showNextAlert
{
    if (alertControllerList.count == 0) {
        return;
    }

    AJSCAlertController *nextAlert = alertControllerList[0];
    [nextAlert show];
}

@end