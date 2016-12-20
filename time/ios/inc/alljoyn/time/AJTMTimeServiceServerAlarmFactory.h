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

/**
 * This class is experimental, and as such has not been tested.
 * Please help make it more robust by contributing fixes if you find issues
 **/

#import <Foundation/Foundation.h>
#import "AJTMTimeServiceServerAlarm.h"
#import "AJTMErrorInfo.h"
#import "AJTMTimeServiceServerAlarmBase.h"

@protocol AJTMTimeServiceServerAlarmFactory <NSObject>

/**
 * Create new TimeServiceServerAlarm
 *
 * @param ptrAlarm Out variable. Pointer to the TimeServiceServerAlarm
 * @param errorInfo Out variable. If create new alarm fails due to max permitted number of Alarm objects
 * reached, or another reason. Populate ErrorInfo::errorName with tsConsts::OUT_OF_RANGE and
 * the errorInfo::errorDesc with the error description.
 *
 * @return ER_OK if creation of the TimeServiceServerAlarm succeeded, or return a failure status and
 * populate the ErrorInfo
 */
-(QStatus)newAlarm:(AJTMTimeServiceServerAlarmBase **) ptrAlarm  errorInfo:(AJTMErrorInfo*) errorInfo;

/**
 * Delete TimeServiceServerAlarm identified by the given objectPath.
 * To delete TimeServiceServerAlarm, call its release() method.
 *
 * @param objectPath of the TimeServiceServerAlarm to be deleted.
 * @param errorInfo To be populated if for any reason failed to delete the TimeServiceServerAlarm
 *
 * @return ER_OK if succeeded to delete TimeServiceServerAlarm or a failure status with population
 * of the ErrorInfo
 */
-(QStatus)deleteAlarm:(NSString*) objectPath errorInfo:(AJTMErrorInfo*) errorInfo;

/**
 * The method is invoked by the TimeService, when there was a failure to create NewAlarm,
 * or to construct a response to the client that has invoked the NewAlarm method.
 * Application that implementing this method is expected to invoke TimeServiceServerAlarm.release()
 * to release object resources.
 *
 * @param alarm TimeServiceServerAlarm that failed to be created
 */
-(void)handleError:(ajn::services::TimeServiceServerAlarm*) alarm;

@end