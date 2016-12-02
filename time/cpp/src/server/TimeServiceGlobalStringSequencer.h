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

#ifndef TIMESERVICEGLOBALSTRINGSEQUENCER_H_
#define TIMESERVICEGLOBALSTRINGSEQUENCER_H_

#include <alljoyn/Status.h>
#include <qcc/String.h>
#include "../common/Mutex.h"

namespace ajn {
namespace services {

/**
 * This is utility singleton class that is used to append a unique number to received string.
 */
class TimeServiceGlobalStringSequencer {

  public:

    /**
     * Get instance of the TimeServiceGlobalStringSequencer
     */
    static TimeServiceGlobalStringSequencer* getInstance();

    /**
     * Initialize the sequencer
     */
    QStatus init();

    /**
     * Appends to the given prefix the current number.
     * @param objPath contains the prefix as input, receives the object path in return
     */
    QStatus append(qcc::String* objPath);

    /**
     * Destructor
     */
    ~TimeServiceGlobalStringSequencer();

  private:

    /**
     * GlobalStringSequencer is a singleton
     */
    static TimeServiceGlobalStringSequencer* s_Instance;

    /**
     * Current mutex
     */
    Mutex m_Mutex;

    /**
     * The current incremented value
     */
    uint32_t m_Current;

    /**
     * Private Constructor - the class is singleton
     */
    TimeServiceGlobalStringSequencer();

    /**
     * Converts to string the received number
     */
    void numToString(uint32_t num, qcc::String* convString);
};

} /* namespace services */
} /* namespace ajn */

#endif /* TIMESERVICEGLOBALSTRINGSEQUENCER_H_ */