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
#import "AJTMTimeServiceServerTimer.h"
#import "AJTMTimeServiceServerTimerBase.h"
#import "AJTMErrorInfo.h"

@protocol AJTMTimeServiceServerTimerFactory <NSObject>

/**
 * Create new TimeServiceServerTimer
 *
 * @param ptrTimer Out variable. Pointer to the TimeServiceServerTimer
 * @param errorInfo Out variable. If create new Timer fails due to max permitted number of Timer objects
 * reached, or another reason. Populate ErrorInfo::errorName with tsConsts::OUT_OF_RANGE and
 * the errorInfo::errorDesc with the error description.
 *
 * @return ER_OK if creation of the TimeServiceServerTimer succeeded, or return a failure status and
 * populate the ErrorInfo
 */
-(QStatus)newTimer:(AJTMTimeServiceServerTimerBase **) ptrTimer errorInfo:(AJTMErrorInfo*) errorInfo;

/**
 * Delete TimeServiceServerTimer identified by the given objectPath.
 * To delete TimeServiceServerTimer, call its release() method.
 *
 * @param objectPath of the TimeServiceServerTimer to be deleted.
 * @param errorInfo To be populated if for any reason failed to delete the TimeServiceServerTimer
 *
 * @return ER_OK if succeeded to delete TimeServiceServerTimer or a failure status with population
 * of the ErrorInfo
 */
-(QStatus)deleteTimer:(NSString*) objectPath errorInfo:(AJTMErrorInfo*) errorInfo;

/**
 * The method is invoked by the TimeService, when there was a failure to create NewTimer,
 * or to construct a response to the client that has invoked the NewTimer method.
 * Application that implementing this method is expected to invoke TimeServiceServerTimer.release()
 * to release object resources.
 *
 * @param Timer TimeServiceServerTimer that failed to be created
 */
-(void)handleError:(ajn::services::TimeServiceServerTimer*) timer;

@end