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

#include "Mutex.h"

using namespace ajn;
using namespace services;

#define QCC_MODULE "MUTEX"

#ifdef _WIN32
#include <process.h>
#endif

Mutex::Mutex()
{
#ifdef _WIN32
    InitializeCriticalSection(&mutex);
#else
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
#endif
}

Mutex::~Mutex()
{
#ifdef _WIN32
    DeleteCriticalSection(&mutex);
#else
    pthread_mutex_destroy(&mutex);
#endif
}

QStatus Mutex::Lock()
{
#ifdef _WIN32
    EnterCriticalSection(&mutex);
    return ER_OK;
#else
    int ret = pthread_mutex_lock(&mutex);

    if (ret == 0) {
        return ER_OK;
    } else if ((ret == EBUSY) || (ret == EAGAIN)) {
        return ER_WOULDBLOCK;
    } else {
        return ER_FAIL;
    }
#endif
}

QStatus Mutex::Unlock()
{
#ifdef _WIN32
    LeaveCriticalSection(&mutex);
    return ER_OK;
#else
    int ret = pthread_mutex_unlock(&mutex);

    if (ret == 0) {
        return ER_OK;
    } else {
        return ER_FAIL;
    }
#endif
}