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
#import "alljoyn/time/TimeServiceClientTimerFactory.h"
#import "AJTMTimeServiceObjectIntrospector.h"

@interface AJTMTimeServiceClientTimerFactory : AJTMTimeServiceObjectIntrospector
/* Constructor
 * @param handle A handle to a cpp object
 */
- (id)initWithHandle:(ajn::services::TimeServiceClientTimerFactory*)handle;

/**
 * Release object resources
 */
-(void)releaseObject;

/**
 * Creates new Timer
 *
 * @param Out variable. Object path of the new created Timer.
 * Use this object path to create TimeServiceClientTimer.
 *
 * @return status ER_OK, or a failure status if failed to create new Timer
 */
-(QStatus)newTimer:(NSString**)objectPath;

/**
 * Delete Timer identified by the given object path.
 *
 * @param object path of the Timer to be deleted.
 * Retrieve the object path by the call to TimeServiceClientTimer.getObjectPath()
 *
 * @return status ER_OK, or a failure status if failed to delete the Timer
 */
-(QStatus)deleteTimer:(NSString *)objectPath;

/**
 * Retrieves a list of timer object paths created by the TimerFactory.
 * Use this object path to create TimeServiceClientTimer.
 *
 * @param objectPathList Pointer to the vector to be populated with the object paths of the
 * Timers created by this TimerFactory
 *
 * @return status ER_OK, or a failure status if failed to retrieve list of Timers
 */
//std::vector<qcc::String>* objectPathList
-(QStatus)retrieveTimerList:(NSArray**)objectPathList;

@end