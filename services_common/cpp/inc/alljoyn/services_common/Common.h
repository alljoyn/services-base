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

#ifndef SERVICES_COMMON_COMMON_H_
#define SERVICES_COMMON_COMMON_H_

/*
 * Common utility functions, constants etc.
 */

namespace ajn {
namespace services {
namespace common {

/**
 * Checks if a valid AllJoyn Object path has been specified in str.
 */
extern bool IsLegalObjectPath(const char* str);

}         //namespace common
}     //namespace services
} //namespace ajn


#endif /* SERVICES_COMMON_COMMON_H_ */