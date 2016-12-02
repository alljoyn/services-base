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
#import "alljoyn/Status.h"
#import "AJNBusAttachment.h"
#import "alljoyn/config/AJCFGConfigServiceListener.h"
#import "alljoyn/config/AJCFGPropertyStoreImpl.h"

/**
 ConfigServiceListener is the sample implementation. it creates and initialize a Config service Listener to handle Config service callbacks.
 */
@interface ConfigServiceListener : NSObject <AJCFGConfigServiceListener>

/**
 Designated initializer.
 Create a ConfigServiceListener Object using the passed propertyStore and AJNBusAttachment.
 @param propertyStore A reference to a property store.
 @param bus A reference to the AJNBusAttachment.
 @return ConfigServiceListener if successful.
 */
- (id)initWithPropertyStore:(AJCFGPropertyStoreImpl *)propertyStore andBus:(AJNBusAttachment *)bus;

/**
 Restart of the device - method not implemented.
 @return ER_OK if successful.
 */
- (QStatus)restart;

/**
 Factory reset of the device - return to default values including password!
 @return ER_OK if successful.
 */
- (QStatus)factoryReset;

/**
 Receive Passphrase info and persist it.
 @param daemonRealm Daemon realm to persist.
 @param passcode passcode content.
 @return ER_OK if successful.
 */
- (QStatus)setPassphrase:(NSString *)daemonRealm withPasscode:(NSString *)passcode;


@end