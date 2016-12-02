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

#include "TimeServiceGlobalStringSequencer.h"
#include <sstream>

using namespace ajn;
using namespace services;

//Initialize self pointer
TimeServiceGlobalStringSequencer* TimeServiceGlobalStringSequencer::s_Instance(NULL);

//Private Constructor
TimeServiceGlobalStringSequencer::TimeServiceGlobalStringSequencer() : m_Mutex(), m_Current(0)
{
}

//Desctructor
TimeServiceGlobalStringSequencer::~TimeServiceGlobalStringSequencer() {
}

//Get the object instance
TimeServiceGlobalStringSequencer* TimeServiceGlobalStringSequencer::getInstance()
{

    if (!s_Instance) {

        s_Instance = new TimeServiceGlobalStringSequencer();
    }

    return s_Instance;
}

// Initialize the sequencer
QStatus TimeServiceGlobalStringSequencer::init()
{

    QStatus status = m_Mutex.Lock();
    if (status != ER_OK) {

        return status;
    }

    m_Current = 0;

    status = m_Mutex.Unlock();

    return status;
}

// Appends to the given prefix the current number.
QStatus TimeServiceGlobalStringSequencer::append(qcc::String* objPath)
{

    if (!objPath) {

        return ER_FAIL;
    }

    uint32_t num;

    QStatus status = m_Mutex.Lock();

    if (status != ER_OK) {

        return status;
    }

    ++m_Current;
    num = m_Current;

    status  = m_Mutex.Unlock();

    qcc::String numStr;
    numToString(num,  &numStr);

    *objPath += numStr;

    return status;
}

//Converts received number to string
void TimeServiceGlobalStringSequencer::numToString(uint32_t num, qcc::String* convString)
{

    std::ostringstream strStream;
    strStream << num;

    convString->assign(strStream.str().c_str());
}