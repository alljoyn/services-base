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

#import "AJCFGConfigServiceListenerAdapter.h"

/**
 AJCFGConfigServiceListenerImpl is the default implementation. it creates and initialize a Config service Listener to handle Config service callbacks.
 */
@interface AJCFGConfigServiceListenerImpl : NSObject

@property AJCFGConfigServiceListenerImplAdapter *handle;
/**
 Create a AJCFGConfigServiceListenerImpl.
 @param configServiceListener Config Service Listener .
 @return AJCFGConfigServiceListenerImpl if successful.
 */
- (id)initWithConfigServiceListener:(id <AJCFGConfigServiceListener> )configServiceListener;

@end