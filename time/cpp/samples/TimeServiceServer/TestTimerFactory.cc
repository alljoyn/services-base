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

#include "TestTimerFactory.h"

using namespace ajn;
using namespace services;

//Constructor
TestTimerFactory::TestTimerFactory() : TimeServiceServerTimerFactory()
{
}

//Destructor
TestTimerFactory::~TestTimerFactory()
{

    testRelease();
}

//Release object resources
void TestTimerFactory::testRelease()
{

    for (std::vector<TestTimer*>::iterator iter = m_Timers.begin(); iter < m_Timers.end(); ++iter) {

        delete *iter;
    }

    m_Timers.clear();
}

//Create new Timer
QStatus TestTimerFactory::newTimer(TimeServiceServerTimer** ptrTimer, tsConsts::ErrorInfo* errorInfo)
{

    TestTimer* Timer = new TestTimer();
    (*ptrTimer)      = Timer;

    m_Timers.push_back(Timer);

    return ER_OK;
}

//Find Timer by the object path
std::vector<TestTimer*>::iterator TestTimerFactory::findTimer(const qcc::String& objectPath, TestTimer** ptrTimer)
{

    if (m_Timers.size() == 0) {

        return m_Timers.end();
    }

    for (std::vector<TestTimer*>::iterator iter = m_Timers.begin(); iter < m_Timers.end(); ++iter) {

        if (0 == strcmp((*iter)->getObjectPath().c_str(), objectPath.c_str())) {

            (*ptrTimer) = *iter;
            return iter;
        }
    }

    return m_Timers.end();
}

//Delete Timer
QStatus TestTimerFactory::deleteTimer(const qcc::String& objectPath, tsConsts::ErrorInfo* errorInfo)
{

    TestTimer* timer = NULL;
    std::vector<TestTimer*>::iterator timerIter = findTimer(objectPath, &timer);

    if (!timer) {

        std::cout << "Not found Timer object with path: '" << objectPath.c_str() << "'" << std::endl;

        errorInfo->errorName        = "ER_BUS_NO_SUCH_OBJECT";
        errorInfo->errorDescription = "No such object: '"  + objectPath + "'";
        return ER_BUS_NO_SUCH_OBJECT;
    }

    std::cout << "Releasing Timer, objectPath: '" << objectPath.c_str() << std::endl;
    delete timer;

    m_Timers.erase(timerIter);

    return ER_OK;
}

//handleError
void TestTimerFactory::handleError(TimeServiceServerTimer* Timer)
{

    std::cout << "Error on creating Timer, releasing it..." << std::endl;
    delete Timer;
}


void TestTimerFactory::printTimerObjectPaths()
{
    for (std::vector<TestTimer*>::iterator i = m_Timers.begin(); i != m_Timers.end(); ++i) {

        printf("\t\t%s\n", (*i)->getObjectPath().c_str());
    }

}