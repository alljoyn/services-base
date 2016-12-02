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

#include "TestAlarmFactory.h"

using namespace ajn;
using namespace services;

//Constructor
TestAlarmFactory::TestAlarmFactory() : TimeServiceServerAlarmFactory()
{
}

//Destructor
TestAlarmFactory::~TestAlarmFactory()
{

    testRelease();
}

//Release object resources
void TestAlarmFactory::testRelease()
{

    for (std::vector<TestAlarm*>::iterator iter = m_Alarms.begin(); iter < m_Alarms.end(); ++iter) {

        delete *iter;
    }

    m_Alarms.clear();
}

//Create new Alarm
QStatus TestAlarmFactory::newAlarm(TimeServiceServerAlarm** ptrAlarm, tsConsts::ErrorInfo* errorInfo)
{

    TestAlarm* alarm = new TestAlarm();
    (*ptrAlarm)      = alarm;

    m_Alarms.push_back(alarm);

    return ER_OK;
}

//Find alarm by the object path
std::vector<TestAlarm*>::iterator TestAlarmFactory::findAlarm(const qcc::String& objectPath, TestAlarm** ptrAlarm)
{

    if (m_Alarms.size() == 0) {

        return m_Alarms.end();
    }

    for (std::vector<TestAlarm*>::iterator iter = m_Alarms.begin(); iter < m_Alarms.end(); ++iter) {

        if (0 == strcmp((*iter)->getObjectPath().c_str(), objectPath.c_str())) {

            (*ptrAlarm) = *iter;
            return iter;
        }
    }

    return m_Alarms.end();
}

//Delete Alarm
QStatus TestAlarmFactory::deleteAlarm(const qcc::String& objectPath, tsConsts::ErrorInfo* errorInfo)
{

    TestAlarm* alarm = NULL;
    std::vector<TestAlarm*>::iterator alarmIter = findAlarm(objectPath, &alarm);

    if (!alarm) {

        std::cout << "Not found Alarm object with path: '" << objectPath.c_str() << "'" << std::endl;

        errorInfo->errorName        = "ER_BUS_NO_SUCH_OBJECT";
        errorInfo->errorDescription = "No such object: '"  + objectPath + "'";
        return ER_BUS_NO_SUCH_OBJECT;
    }

    std::cout << "Releasing Alarm, objectPath: '" << objectPath.c_str() << std::endl;
    delete alarm;

    m_Alarms.erase(alarmIter);

    return ER_OK;
}

//handleError
void TestAlarmFactory::handleError(TimeServiceServerAlarm* alarm)
{

    std::cout << "Error on creating Alarm, releasing it..." << std::endl;
    delete alarm;
}

void TestAlarmFactory::printAlarmObjectPaths()
{
    for (std::vector<TestAlarm*>::iterator i = m_Alarms.begin(); i != m_Alarms.end(); ++i) {

        printf("\t\t%s\n", (*i)->getObjectPath().c_str());
    }

}
