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

#include <alljoyn/time/TimeServiceServerTimerFactory.h>
#include "TimeServiceTimerFactoryBusObj.h"

using namespace ajn;
using namespace services;

//Constructor
TimeServiceServerTimerFactory::TimeServiceServerTimerFactory() : m_TimerFactoryBusObj(NULL)
{

    QCC_DbgTrace(("%s", __FUNCTION__));
}

//Destructor
TimeServiceServerTimerFactory::~TimeServiceServerTimerFactory()
{

    QCC_DbgTrace(("%s", __FUNCTION__));
    release();
}

//Releases object resources
void TimeServiceServerTimerFactory::release()
{

    QCC_DbgTrace(("%s", __FUNCTION__));

    if (!m_TimerFactoryBusObj) {

        return;
    }

    QCC_DbgHLPrintf(("Releasing the TimerFactory '%s'", m_TimerFactoryBusObj->getObjectPath().c_str()));

    delete m_TimerFactoryBusObj;
    m_TimerFactoryBusObj = NULL;
}

//Returns object path of this TimerFactory
qcc::String const& TimeServiceServerTimerFactory::getObjectPath() const
{

    if (!m_TimerFactoryBusObj) {

        QCC_LogError(ER_FAIL, ("This TimerFactory hasn't been created yet"));
        return qcc::String::Empty;
    }

    return m_TimerFactoryBusObj->getObjectPath();
}

//Associate this TimerFactory with the TimerFactoryBusObj which implements AllJoyn functionality
QStatus TimeServiceServerTimerFactory::setTimerFactoryBusObj(TimeServiceTimerFactoryBusObj* TimerFactoryBusObj)
{

    if (!TimerFactoryBusObj) {

        QCC_LogError(ER_FAIL, ("No TimerFActoryBusObj given"));
        return ER_FAIL;
    }

    if (m_TimerFactoryBusObj) {

        QCC_LogError(ER_FAIL, ("TimerFactoryBusObj has been previously set"));
        return ER_FAIL;
    }

    m_TimerFactoryBusObj = TimerFactoryBusObj;
    return ER_OK;
}