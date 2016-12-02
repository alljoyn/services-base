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

#import "AJTMTimeServiceSessionListenerAdapter.h"
#import "AJTMTimeServiceClient.h"

AJTMTimeServiceSessionListenerAdapter::AJTMTimeServiceSessionListenerAdapter(id<AJTMTimeServiceSessionListener> timeServiceSessionListener)
{
    handle = timeServiceSessionListener;
}

AJTMTimeServiceSessionListenerAdapter::~AJTMTimeServiceSessionListenerAdapter()
{
}

void AJTMTimeServiceSessionListenerAdapter::sessionLost(ajn::services::TimeServiceClient* timeServiceClient, ajn::SessionListener::SessionLostReason reason)
{
    AJTMTimeServiceClient *client = [[AJTMTimeServiceClient alloc]initWithHandle:timeServiceClient];
    [handle sessionLost:client SessionListener:ALLJOYN_SESSIONLOST_LINK_TIMEOUT];
}

void AJTMTimeServiceSessionListenerAdapter::sessionJoined(ajn::services::TimeServiceClient* timeServiceClient, QStatus status)
{
    AJTMTimeServiceClient *client = [[AJTMTimeServiceClient alloc]initWithHandle:timeServiceClient];

    [handle sessionJoined:client staus:status];

}