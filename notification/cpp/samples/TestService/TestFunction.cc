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

#include <iostream>
#include "TestFunction.h"

using namespace qcc;

TestFunction::TestFunction() : activateTest(0)
{
}

TestFunction::~TestFunction()
{
}

bool TestFunction::checkRequiredParams(std::map<qcc::String, qcc::String>& params)
{
    if (requiredParams.size() == 0) {
        return true;
    }

    for (std::vector<qcc::String>::const_iterator reqParams_it = requiredParams.begin(); reqParams_it != requiredParams.end(); ++reqParams_it) {
        if (params.find(*reqParams_it) == params.end()) {
            std::cout << "Missing required parameter " << reqParams_it->c_str() << std::endl;
            std::cout << usage.c_str() << std::endl;
            return false;
        }
    }
    return true;
}

void TestFunction::checkOptionalParams(std::map<qcc::String, qcc::String>& params)
{
    for (std::map<qcc::String, qcc::String>::const_iterator params_it = params.begin(); params_it != params.end(); ++params_it) {
        if (find(optionalParams.begin(), optionalParams.end(), params_it->first) == optionalParams.end()) {
            if (find(requiredParams.begin(), requiredParams.end(), params_it->first) == requiredParams.end()) {
                // Element is NOT in either vector so let the user know
                std::cout << "Parameter " << params_it->first.c_str() << " is not a valid parameter for " <<
                    functionName.c_str() << ". Ignoring value" << std::endl;
            }
        }
    }
}