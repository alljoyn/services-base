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

#ifndef TESTFUNCTION_H_
#define TESTFUNCTION_H_

#include <algorithm>
#include <map>
#include <vector>
#include <qcc/String.h>

/**
 * TestFunction class used to represent on of the possible API calls
 * in the TestService application
 */
class TestFunction {
  public:

    /**
     * Constructor
     */
    TestFunction();

    /**
     * Destructor
     */
    virtual ~TestFunction();

    /**
     * Validate that required params were added
     * @param params
     * @return true/false
     */
    bool checkRequiredParams(std::map<qcc::String, qcc::String>& params);

    /**
     * Check the optional parameters
     * @param params
     */
    void checkOptionalParams(std::map<qcc::String, qcc::String>& params);

    /**
     * The name of the function
     */
    qcc::String functionName;

    /**
     * The usage string
     */
    qcc::String usage;

    /**
     * The required Parameters
     */
    std::vector<qcc::String> requiredParams;

    /**
     * The optional Parameters
     */
    std::vector<qcc::String> optionalParams;

    /**
     * Preconditions - steps that need to be taken before this API call
     */
    std::vector<qcc::String> requiredSteps;

    /**
     * functionPointer to function that will execute the API call
     * @param params
     * @return
     */
    bool (*activateTest)(std::map<qcc::String, qcc::String>& params);
};

#endif /* TESTFUNCTION_H_ */