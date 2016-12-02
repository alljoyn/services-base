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

#import "AJNSNotificationEnums.h"

@implementation AJNSNotificationEnums

+ (NSString *)AJNSMessageTypeToString:(AJNSNotificationMessageType)msgType
{
	switch (msgType) {
		case 0:
			return @"EMERGENCY";
			break;
            
		case 1:
			return @"WARNING";
			break;
            
		case 2:
			return @"INFO";
			break;
            
		default:
			return @"UNSET";
			break;
	}
}

@end