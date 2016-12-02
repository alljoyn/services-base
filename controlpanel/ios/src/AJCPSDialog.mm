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

#import "AJCPSDialog.h"
#import "alljoyn/about/AJNConvertUtil.h"

@interface AJCPSDialog ()

@end


@implementation AJCPSDialog

- (id)initWithHandle:(ajn::services::Dialog *)handle
{
	self = [super initWithHandle:handle];
	if (self) {
	}
	return self;
}

- (uint16_t)getNumActions
{
	return ((ajn::services::Dialog *)self.handle)->getNumActions();
}

- (NSString *)getMessage
{
	return [AJNConvertUtil convertQCCStringtoNSString:((ajn::services::Dialog *)self.handle)->getMessage()];
}

- (NSString *)getLabelAction1
{
	return [AJNConvertUtil convertQCCStringtoNSString:((ajn::services::Dialog *)self.handle)->getLabelAction1()];
}

- (NSString *)getLabelAction2
{
	return [AJNConvertUtil convertQCCStringtoNSString:((ajn::services::Dialog *)self.handle)->getLabelAction2()];
}

- (NSString *)getLabelAction3
{
	return [AJNConvertUtil convertQCCStringtoNSString:((ajn::services::Dialog *)self.handle)->getLabelAction3()];
}

- (QStatus)executeAction1
{
	return ((ajn::services::Dialog *)self.handle)->executeAction1();
}

- (QStatus)executeAction2
{
	return ((ajn::services::Dialog *)self.handle)->executeAction2();
}

- (QStatus)executeAction3
{
	return ((ajn::services::Dialog *)self.handle)->executeAction3();
}

@end