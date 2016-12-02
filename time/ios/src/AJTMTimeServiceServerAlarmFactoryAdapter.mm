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

#include "AJTMTimeServiceServerAlarmFactoryAdapter.h"
#include "AJTMTimeServiceServerAlarmBase.h"

AJTMTimeServiceServerAlarmFactoryAdapter::AJTMTimeServiceServerAlarmFactoryAdapter(id<AJTMTimeServiceServerAlarmFactory> timeServiceServerAlarmFactory)
{
    handle = timeServiceServerAlarmFactory;
}


AJTMTimeServiceServerAlarmFactoryAdapter::~AJTMTimeServiceServerAlarmFactoryAdapter()
{

}

qcc::String const& AJTMTimeServiceServerAlarmFactoryAdapter::getObjectPath() const
{
    return this->getObjectPath();
}


QStatus AJTMTimeServiceServerAlarmFactoryAdapter::newAlarm(ajn::services::TimeServiceServerAlarm** ptrAlarm, ajn::services::tsConsts::ErrorInfo* errorInfo)
{
    AJTMTimeServiceServerAlarmBase *alarm;
    AJTMErrorInfo *error = [[AJTMErrorInfo alloc]init];
    QStatus status = [handle newAlarm:&alarm errorInfo:error];

    if (status == ER_OK) {
        *ptrAlarm = (__bridge ajn::services::TimeServiceServerAlarm*)[alarm getHandle];
    }
    else
    {
        errorInfo->errorDescription = [error errorDescription].UTF8String;
        errorInfo->errorName = [error errorName].UTF8String;
    }

    return status;
}


QStatus AJTMTimeServiceServerAlarmFactoryAdapter::deleteAlarm(const qcc::String& objectPath, ajn::services::tsConsts::ErrorInfo* errorInfo)
{
    AJTMErrorInfo* t_errorInfo = [[AJTMErrorInfo alloc] initWithErrorName:@((errorInfo->errorName).c_str()) errorDescription:@((errorInfo->errorDescription).c_str())];

    return [handle deleteAlarm:@(objectPath.c_str()) errorInfo:t_errorInfo];

}

void AJTMTimeServiceServerAlarmFactoryAdapter::handleError(ajn::services::TimeServiceServerAlarm* alarm)
{
    [handle handleError:alarm];


    alarm->release();    
}