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

#import "NotificationUtils.h"
#import <alljoyn/Status.h>


@interface NotificationUtils ()


@end

@implementation NotificationUtils


//validate a string (not empty or nil)
+ (bool)textFieldIsValid:(NSString *)txt
{
    return !([txt isEqualToString:(@"")]) && txt != nil;
}


@end