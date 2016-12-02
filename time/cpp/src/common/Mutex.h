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

#ifndef _MUTEX_H_
#define _MUTEX_H_

#ifdef _WIN32
#include <Windows.h>
#define pthread_mutex_t CRITICAL_SECTION
#define pthread_cond_t CONDITION_VARIABLE
#else
#include <pthread.h>
#endif
#include <errno.h>
#include <alljoyn/Status.h>

namespace ajn {
namespace services {

/**
 * a wrapper class to mutex
 */
class Mutex {
  public:

    /**
     * Mutex constructor
     */
    Mutex();

    /**
     * Mutex destructor
     */
    ~Mutex();

    /**
     * Lock mutex
     */
    QStatus Lock();

    /**
     * Unlock mutex
     */
    QStatus Unlock();

    /**
     * Get mutex object
     */
    pthread_mutex_t* GetMutex() { return &mutex; }

  private:

    pthread_mutex_t mutex;

};

} /* services */
} /* ajn */

#endif /* _MUTEX_H_ */