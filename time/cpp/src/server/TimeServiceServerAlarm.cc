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

#include <alljoyn/time/TimeServiceServerAlarm.h>
#include <alljoyn/time/TimeServiceAlarmBusObj.h>

using namespace ajn;
using namespace services;

//Constructor
TimeServiceServerAlarm::TimeServiceServerAlarm() : m_AlarmBusObj(NULL)
{

    QCC_DbgTrace(("%s", __FUNCTION__));
}

//Destructor
TimeServiceServerAlarm::~TimeServiceServerAlarm()
{

    QCC_DbgTrace(("%s", __FUNCTION__));
    release();
}

//Releases object resources
void TimeServiceServerAlarm::release()
{

    QCC_DbgTrace(("%s", __FUNCTION__));

    if (!m_AlarmBusObj) {

        return;
    }

    QCC_DbgHLPrintf(("Releasing the Alarm '%s'", m_AlarmBusObj->getObjectPath().c_str()));

    delete m_AlarmBusObj;
    m_AlarmBusObj = NULL;
}

//Returns object path of this Alarm
qcc::String const& TimeServiceServerAlarm::getObjectPath() const
{

    if (!m_AlarmBusObj) {

        QCC_LogError(ER_FAIL, ("This Alarm hasn't been created yet"));
        return qcc::String::Empty;
    }

    return m_AlarmBusObj->getObjectPath();
}

//Set Alarm Bus Object
QStatus TimeServiceServerAlarm::setAlarmBusObj(TimeServiceAlarmBusObj* alarmBusObj)
{
    if (!alarmBusObj) {

        QCC_LogError(ER_FAIL, ("No alarmBusObj given"));
        return ER_FAIL;
    }

    if (m_AlarmBusObj) {

        QCC_LogError(ER_FAIL, ("alarmBusObj has been previously set"));
        return ER_FAIL;
    }

    m_AlarmBusObj = alarmBusObj;
    return ER_OK;
}

//Returns AlarmBusObj
const TimeServiceAlarmBusObj* TimeServiceServerAlarm::getAlarmBusObj()
{

    return m_AlarmBusObj;
}

// Alarm reached
QStatus TimeServiceServerAlarm::alarmReached()
{

    QCC_DbgTrace(("%s", __FUNCTION__));

    if (!m_AlarmBusObj) {

        QCC_LogError(ER_FAIL, ("This Alarm hasn't been created yet"));
        return ER_FAIL;
    }

    return m_AlarmBusObj->sendAlarmReached();
}