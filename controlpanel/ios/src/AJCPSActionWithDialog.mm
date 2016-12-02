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

#import "AJCPSActionWithDialog.h"
#import "alljoyn/controlpanel/ActionWithDialog.h"
#import "alljoyn/about/AJNConvertUtil.h"
#import "AJCPSControlPanelDevice.h"

@interface AJCPSActionWithDialog ()

@end


@implementation AJCPSActionWithDialog

- (id)initWithHandle:(ajn::services::ActionWithDialog *)handle
{
	self = [super initWithHandle:handle];
	if (self) {
	}
	return self;
}

- (AJCPSDialog *)getChildDialog
{
	return [[AJCPSDialog alloc]initWithHandle:((ajn::services::ActionWithDialog *)self.handle)->getChildDialog()];
}

- (QStatus)unregisterObjects:(AJNBusAttachment *)bus
{
	return ((ajn::services::ActionWithDialog *)self.handle)->unregisterObjects((ajn::BusAttachment *)[bus handle]);
}

@end