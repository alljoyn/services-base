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

#import "AJCFGConfigServiceListenerImpl.h"

@implementation AJCFGConfigServiceListenerImpl

- (void)dealloc
{
	if (self.handle) {
		delete self.handle;
		self.handle = NULL;
	}
}

- (id)initWithConfigServiceListener:(id <AJCFGConfigServiceListener> )configServiceListener
{
	self = [super init];
	if (self) {
		self.handle = new AJCFGConfigServiceListenerImplAdapter(configServiceListener);
	}
	return self;
}

@end