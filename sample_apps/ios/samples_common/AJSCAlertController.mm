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
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
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