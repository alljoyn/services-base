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

#ifndef SERVICES_COMMON_CONSTANTS_H_
#define SERVICES_COMMON_CONSTANTS_H_

#include <qcc/String.h>

/*
 * Common Macros
 */

namespace ajn {
namespace services {
namespace commonConsts {

static const uint16_t UUID_LENGTH = 16;

static const qcc::String AJPARAM_BOOL = "b";
static const qcc::String AJPARAM_UINT16 = "q";
static const qcc::String AJPARAM_STR = "s";
static const qcc::String AJPARAM_INT = "i";
static const qcc::String AJPARAM_ARR_BYTE = "ay";
static const qcc::String AJPARAM_VAR = "v";
static const qcc::String AJPARAM_INT16 = "n";
static const qcc::String AJPARAM_UINT32 = "u";
static const qcc::String AJPARAM_UINT64 = "t";
static const qcc::String AJPARAM_INT64 = "x";
static const qcc::String AJPARAM_DOUBLE = "d";


}         //namespace commonConsts
}     //namespace services
} //namespace ajn


#endif /* SERVICES_COMMON_CONSTANTS_H_ */
