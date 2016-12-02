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

REGULAR_NAME_HERE::REGULAR_NAME_HERE(qcc::String name, Widget* rootWidget) : Dialog(name, rootWidget)
{

}

REGULAR_NAME_HERE::~REGULAR_NAME_HERE()
{

}

bool REGULAR_NAME_HERE::executeAction1CallBack()
{
    CODE_OF_EXECUTE_ACTION1_HERE

    //by default return success
    return true;
}

bool REGULAR_NAME_HERE::executeAction2CallBack()
{
    CODE_OF_EXECUTE_ACTION2_HERE

    //by default return success
    return true;
}

bool REGULAR_NAME_HERE::executeAction3CallBack()
{
    CODE_OF_EXECUTE_ACTION3_HERE

    //by default return success
    return true;
}

} /* namespace services */
} /* namespace ajn */