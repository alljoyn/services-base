/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include <alljoyn/time/TimeServiceServerAlarmFactory.h>
#include "TimeServiceAlarmFactoryBusObj.h"

using namespace ajn;
using namespace services;

//Constructor
TimeServiceServerAlarmFactory::TimeServiceServerAlarmFactory() : m_AlarmFactoryBusObj(NULL)
{

    QCC_DbgTrace(("%s", __func__));
}

//Destructor
TimeServiceServerAlarmFactory::~TimeServiceServerAlarmFactory()
{

    QCC_DbgTrace(("%s", __func__));
    release();
}

//Releases object resources
void TimeServiceServerAlarmFactory::release()
{

    QCC_DbgTrace(("%s", __func__));

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
