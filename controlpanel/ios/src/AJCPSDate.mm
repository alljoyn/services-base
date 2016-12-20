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
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#import "AJCPSDate.h"

@interface AJCPSDate ()
@property (nonatomic) ajn::services::CPSDate *handle;
@end

@implementation AJCPSDate

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