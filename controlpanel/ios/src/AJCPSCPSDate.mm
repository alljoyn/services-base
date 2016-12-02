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

#import "AJCPSCPSDate.h"

@interface AJCPSCPSDate ()
@property (nonatomic) ajn::services::CPSDate *handle;
@end

@implementation AJCPSCPSDate

- (id)initWithDay:(uint16_t) day month:(uint16_t) month year:(uint16_t) year
{
    self = [super init];
	if (self) {
		self.handle = new ajn::services::CPSDate(day, month, year);
	}
	return self;
    
}
- (id)initWithHandle:(ajn::services::CPSDate *)handle
{
	self = [super init];
	if (self) {
		self.handle = handle;
	}
	return self;
}

- (uint16_t)getDay
{
	return self.handle->getDay();
}

- (void)setDay:(uint16_t)day
{
	return self.handle->setDay(day);
}

- (uint16_t)getMonth
{
	return self.handle->getMonth();
}

- (void)setMonth:(uint16_t)month
{
	return self.handle->setMonth(month);
}

- (uint16_t)getYear
{
	return self.handle->getYear();
}

- (void)setYear:(uint16_t)year
{
	return self.handle->setYear(year);
}

@end