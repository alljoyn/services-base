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

#import "AJSCAlertController.h"

@interface AJSCAlertController ()

@property (nonatomic, weak) UIViewController *viewController;

@end


@implementation AJSCAlertController

- (id)init
{
    self = [super init];

    if (self) {
    }

    return self;
}

+ (AJSCAlertController *)alertControllerWithTitle:(NSString *)title message:(NSString *)message viewController:(UIViewController *)viewController
{
    AJSCAlertController *alertController = [[AJSCAlertController alloc] init];
    if (alertController) {
        alertController.iosAlertController = [UIAlertController alertControllerWithTitle:title message:message preferredStyle:UIAlertControllerStyleAlert];
        alertController.viewController = viewController;
        return alertController;
    }
    return nil;
}

- (void)addActionWithName:(NSString *)name handler:(void (^)(UIAlertAction *action))handler
{
    UIAlertAction *action = [UIAlertAction actionWithTitle:name
                             style:UIAlertActionStyleDefault
                             handler:handler];
    [_iosAlertController addAction:action];
}

- (void)show
{
    [_viewController presentViewController:_iosAlertController animated:YES completion:nil];
}

@end