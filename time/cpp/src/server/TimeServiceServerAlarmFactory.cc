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

#include <alljoyn/time/TimeServiceServerAlarmFactory.h>
#include "TimeServiceAlarmFactoryBusObj.h"

using namespace ajn;
using namespace services;

//Constructor
TimeServiceServerAlarmFactory::TimeServiceServerAlarmFactory() : m_AlarmFactoryBusObj(NULL)
{

    QCC_DbgTrace(("%s", __FUNCTION__));
}

//Destructor
TimeServiceServerAlarmFactory::~TimeServiceServerAlarmFactory()
{

    QCC_DbgTrace(("%s", __FUNCTION__));
    release();
}

//Releases object resources
void TimeServiceServerAlarmFactory::release()
{

    QCC_DbgTrace(("%s", __FUNCTION__));

    if (!m_AlarmFactoryBusObj) {

        return;
    }

    QCC_DbgHLPrintf(("Releasing the AlarmFactory '%s'", m_AlarmFactoryBusObj->getObjectPath().c_str()));

    delete m_AlarmFactoryBusObj;
    m_AlarmFactoryBusObj = NULL;
}

//Returns object path of this AlarmFactory
qcc::String const& TimeServiceServerAlarmFactory::getObjectPath() const
{

    if (!m_AlarmFactoryBusObj) {

        QCC_LogError(ER_FAIL, ("This AlarmFactory hasn't been created yet"));
        return qcc::String::Empty;
    }

    return m_AlarmFactoryBusObj->getObjectPath();
}

//Associate this AlarmFactory with the AlarmFactoryBusObj which implements AllJoyn functionality
QStatus TimeServiceServerAlarmFactory::setAlarmFactoryBusObj(TimeServiceAlarmFactoryBusObj* alarmFactoryBusObj)
{

    if (!alarmFactoryBusObj) {

        QCC_LogError(ER_FAIL, ("No alarmFActoryBusObj given"));
        return ER_FAIL;
    }

    if (m_AlarmFactoryBusObj) {

        QCC_LogError(ER_FAIL, ("alarmFactoryBusObj has been previously set"));
        return ER_FAIL;
    }

    m_AlarmFactoryBusObj = alarmFactoryBusObj;
    return ER_OK;
}