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

/**
 * This class is experimental, and as such has not been tested.
 * Please help make it more robust by contributing fixes if you find issues
 **/

#import "AJTMTimeServiceClientAlarmFactory.h"

@interface AJTMTimeServiceClientAlarmFactory()

@property ajn::services::TimeServiceClientAlarmFactory* handle;

@end


@implementation AJTMTimeServiceClientAlarmFactory
/* Constructor
 * @param handle A handle to a cpp object
 */
- (id)initWithHandle:(ajn::services::TimeServiceClientAlarmFactory*)handle
{
    self = [super initWithHandle:handle];
    if (self) {
        self.handle = (ajn::services::TimeServiceClientAlarmFactory *)handle;
    }
    return self;
}

-(void)releaseObject
{
    self.handle->release();
}

-(QStatus)newAlarm:(NSString**)objectPath
{
    qcc::String nativeObjectPath;
    QStatus status = self.handle->newAlarm(&nativeObjectPath);

    if (status == ER_OK) {
        *objectPath = [NSString stringWithUTF8String:nativeObjectPath.c_str()];
    }

    return status;
}

-(QStatus)deleteAlarm:(NSString *)objectPath
{
    return self.handle->deleteAlarm([objectPath UTF8String]);
}

//std::vector<qcc::String>* objectPathList
-(QStatus)retrieveAlarmList:(NSArray**)objectPathList
{
    std::vector<qcc::String> nativeObjectPathList;

    QStatus status = self.handle->retrieveAlarmList(&nativeObjectPathList);

    if (status == ER_OK) {
        NSMutableArray *list = [[NSMutableArray alloc]init];
        std::vector<qcc::String>::iterator itr;

        for (itr = nativeObjectPathList.begin(); itr!=nativeObjectPathList.end(); itr++) {
            [list addObject:[NSString stringWithUTF8String:(*itr).c_str()]];
        }

        *objectPathList = list;

    }
    return status;
}


@end