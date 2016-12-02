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

#include <qcc/StringUtil.h>

namespace ajn {
namespace services {
namespace common {

/*
 * Checks if a valid AllJoyn Object path has been specified in str.
 * Please note that this function is same as ajn::BusUtil::IsLegalObjectPath()
 * but since this is not exposed to external applications, this function is
 * being added here
 */
bool IsLegalObjectPath(const char* str)
{
    if (!str) {
        return false;
    }
    /* Must begin with slash */
    char c = *str++;
    if (c != '/') {
        return false;
    }
    while ((c = *str++) != 0) {
        if ((!(qcc::IsAlphaNumeric(c))) && (c != '_')) {
            if ((c != '/') || (*str == '/') || (*str == 0)) {
                return false;
            }
        }
    }
    return true;
}

}         //namespace common
}     //namespace services
} //namespace ajn