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

/**
 AJOBOnboardingClientListener
 A helper class used by an AllJoyn IoE client application to receive OnboardingService signal notification.
The user of the class need to implement it method(s)
 */

@protocol AJOBOnboardingClientListener <NSObject>

/**
 ConnectionResultSignalReceived
 Needs to be implemented by the user for the desired behaviour when receiving the ConnectResult Signal
 @param connectionResultCode connection result code
 @param connectionResultMessage connection result message
 */
- (void)connectionResultSignalReceived:(int)connectionResultCode connectionResultMessage:(NSString *)connectionResultMessage;

@end