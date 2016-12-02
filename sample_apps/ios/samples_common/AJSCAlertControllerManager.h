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

/**
 AJSCAlertControllerManager class - overcomes "one at a time" limitation of UIAlertController by allowing
 AJSCAlertControllers to queue their UIAlertControllers for display, ensuring an alert doesn't disappear
 when another alert is currently active.
 */
@interface AJSCAlertControllerManager : NSObject

+ (void)queueAlertWithTitle:(nonnull NSString *)title message:(nonnull NSString *)message viewController:(nonnull UIViewController *)viewController;

@end