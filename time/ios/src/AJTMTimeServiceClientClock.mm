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

#import <Foundation/Foundation.h>
#import "AJTMTimeServiceClientClock.h"
#import "alljoyn/time/TimeServiceClientClock.h"


@interface AJTMTimeServiceClientClock()

@property ajn::services::TimeServiceClientClock* handle;

@end


@implementation AJTMTimeServiceClientClock
/* Constructor
 * @param handle A handle to a cpp object
 */
- (id)initWithHandle:(ajn::services::TimeServiceClientClock*)handle
{
    self = [super initWithTimeServiceClient:[[AJTMTimeServiceClient alloc]initWithHandle:(ajn::services::TimeServiceClient*)&handle->getTsClient()] objectPath:[NSString stringWithUTF8String:handle->getObjectPath().c_str()]];
    if (self) {
        self.handle = (ajn::services::TimeServiceClientClock *)handle;

    }
    return self;
}

/**
 * Release object resources
 */
-(void)releaseObject
{
    self.handle->release();
}

/**
 * This method to be called by the TimeServiceClient after finding out
 * whether this Clock is a reliable source of time.
 */
-(void)setAuthority:(bool) isAuthority
{
    self.handle->setAuthority(isAuthority);
}


/**
 * Returns whether the server side Clock is a reliable source of time.
 *
 * @return TRUE if server side Clock is a reliable source of time.
 */
-(bool)isAuthority
{
    return self.handle->isAuthority();
}


/**
 * Retrieve DateTime from the server Clock object.
 *
 * @param dateTime Out variable. Is populated with the retrieved DateTime.
 *
 * @return status ER_OK, or a failure status if failed to retrieve DateTime
 */
-(QStatus)retrieveDateTime:(AJTMTimeServiceDateTime**) dateTime
{
    ajn::services::TimeServiceDateTime *nativeDateTime = new ajn::services::TimeServiceDateTime() ;
    QStatus status = self.handle->retrieveDateTime(nativeDateTime);

    if (status == ER_OK) {
        *dateTime = [[AJTMTimeServiceDateTime alloc] initWithHandle:nativeDateTime];
    }

    return status;
}


/**
 * Set DateTime to the server Clock object
 *
 * @param dateTime TimeServiceDateTime to set
 *
 * @return status
 */
-(QStatus)setDateTime:(AJTMTimeServiceDateTime *)dateTime
{
    return self.handle->setDateTime(*[dateTime getHandle]);
}


/**
 * Retrieve IsSet from the server Clock object.
 *
 * @param isSet Out variable.
 *
 * @return status ER_OK, or a failure status if failed to retrieve IsSet
 */
-(QStatus)retrieveIsSet:(bool*) isSet
{
    return self.handle->retrieveIsSet(isSet);
}


/**
 * Retrieves Authority Type of the server clock.
 * This method is only valid for the Time Authority clock object.
 *
 * @param authType Out variable.
 *
 * @return status ER_OK, or a failure status if failed to retrieve authority type
 */
-(QStatus)retrieveAuthorityType:(ajn::services::tsConsts::ClockAuthorityType*) authType
{
    return self.handle->retrieveAuthorityType(authType);
}


/*
 * Register TimeAuthorityHandler to receive TimeSync
 * events. This method is only valid for the Time Authority clocks.
 *
 * @param timeAuthorityHandler TimeAuthorityHandler
 *
 * @return status
 */
-(QStatus)registerTimeAuthorityHandler:(id<AJTMTimeAuthorityHandler>) handler
{
    TimeAuthorityHandlerAdapter *adapter = new TimeAuthorityHandlerAdapter(handler);
    return self.handle->registerTimeAuthorityHandler(adapter);
}


/**
 * Unregister TimeAuthorityHandler to stop receiving TimeSync
 * events. This method is only valid for objects for the Time Authority clocks.
 */
-(void)unRegisterTimeAuthorityHandler
{
    self.handle->unRegisterTimeAuthorityHandler();
}


/**
 * @return TimeAuthorityHandler that has been registered or NULL
 */
-(id<AJTMTimeAuthorityHandler>) getTimeAuthorityHandler
{
    return ((TimeAuthorityHandlerAdapter *)self.handle->getTimeAuthorityHandler())->handle;
}


@end