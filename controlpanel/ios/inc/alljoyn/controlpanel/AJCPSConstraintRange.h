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
#import "alljoyn/controlpanel/ConstraintRange.h"
#import "AJCPSControlPanelEnums.h"

typedef union {
    /**
     * Value of type uint16_t
     */
    uint16_t uint16Value;
    /**
     * Value of type int16_t
     */
    int16_t int16Value;
    /**
     * Value of type uint32_t
     */
    uint32_t uint32Value;
    /**
     * Value of type int32_t
     */
    int32_t int32Value;
    /**
     * Value of type uint64_t
     */
    uint64_t uint64Value;
    /**
     * Value of type int64_t
     */
    int64_t int64Value;
    /**
     * Value of type double
     */
    double doubleValue;
} AJCPSConstraintRangeVal;

/**
 * AJCPSConstraintRange defines a range of
 * Values and constrain a property to those values.
 * The Constraint is applied on the controller side.
 * No validations are done in the Controlee.
 */
@interface AJCPSConstraintRange : NSObject

- (id)initWithHandle:(ajn::services::ConstraintRange *)handle;

/**
 * get the IncrementValue of the Constraint Range
 * @return IncrementValue
 */
- (uint16_t)getIncrementValue;

/**
 * Get the MaxValue of the Constraint Range
 * @return MaxValue
 */
- (uint16_t)getMaxValue;

/**
 * Get the MinValue of the Constraint Range
 * @return MinValue
 */
- (uint16_t)getMinValue;

@end