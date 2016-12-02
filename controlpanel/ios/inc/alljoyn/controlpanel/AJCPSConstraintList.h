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

#import <Foundation/Foundation.h>
#import "alljoyn/controlpanel/ConstraintList.h"
#import "AJCPSControlPanelEnums.h"

/**
 * ConstraintValue - a union combining all possible values
 * that can make up a constraint
 */
typedef union {
    /**
     * Value for uint16_t
     */
    uint16_t uint16Value;
    /**
     * Value for int16_t
     */
    int16_t int16Value;
    /**
     * Value for uint32_t
     */
    uint32_t uint32Value;
    /**
     * Value for int32_t
     */
    int32_t int32Value;
    /**
     * Value for uint64_t
     */
    uint64_t uint64Value;
    /**
     * Value for int64_t
     */
    int64_t int64Value;
    /**
     * Value for double
     */
    double doubleValue;
    /**
     * Value for const char*
     */
    const char* charValue;
} AJCPSConstraintValue;

/**
 * AJCPSConstraintList defines a list of
 * Values and constrain a property to those values.
 * The Constraint is applied on the controller side.
 * No validations are done in the Controllee
 */
@interface AJCPSConstraintList : NSObject

- (id)initWithHandle:(ajn ::services ::ConstraintList *)handle;

/**
 * Get the Constraint Value
 * @return the Constraint Value
 */
- (AJCPSConstraintValue)getConstraintValue;

/**
 * Get the Property Type of the Constraint
 * @return propertyType of the Constraint
 */
- (AJCPSPropertyType)getPropertyType;

/**
 * Get the Display
 * @return Display Value
 */
- (NSString *)getDisplay;

@end