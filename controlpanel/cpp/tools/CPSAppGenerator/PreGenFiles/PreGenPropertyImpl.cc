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

#include "REGULAR_NAME_HERE.h"
ADDITIONAL_INCLUDES_HERE

namespace ajn {
namespace services {

REGULAR_NAME_HERE::REGULAR_NAME_HERE(qcc::String name, Widget* rootWidget, PropertyType propertyType) :
    Property(name, rootWidget, propertyType)
{

}

REGULAR_NAME_HERE::~REGULAR_NAME_HERE()
{

}

QStatus REGULAR_NAME_HERE::setValue(VALUE_TYPE_HERE value)
{
    CODE_OF_SET_VALUE_HERE;
    return ER_OK;
}

} /* namespace services */
} /* namespace ajn */