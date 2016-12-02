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
#import "AJTMTimeServiceClient.h"
#import "alljoyn/time/TimeServiceObjectIntrospector.h"
#import "AJTMTimeServiceClientBase.h"

@interface AJTMTimeServiceObjectIntrospector : AJTMTimeServiceClientBase

/* Constructor
 * @param handle A handle to a cpp object
 */
- (id)initWithHandle:(ajn::services::TimeServiceObjectIntrospector*)handle;

/**
 * Constructor
 *
 * @param tsClient TimeServiceClient managing this object
 * @param objectPath Object path of the Time Service server side object
 */
-(id)initWithClient:(AJTMTimeServiceClient *) tsClient objectPath:(NSString *)objectPath;

/**
 * Retrieves array of description languages supported by the introspected object.
 *
 * @return vector of the description languages.
 * If the returned vector is empty, it means that the introspected object has no description.
 */
//const std::vector<qcc::String>&
-(NSArray *)retrieveDescriptionLanguages;

/**
 * Retrieve description of the introspected object.
 * The description is retrieved in the requested language. The language should be
 * one of those returned by the retrieveDescriptionLanguages().
 *
 * @param language Language of the retrieved object description
 *
 * @return Object description or empty string if no description was found
 */
-(NSString *)retrieveObjectDescription:(NSString *)language;

@end