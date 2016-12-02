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

#ifndef TESTAUTHORITYCLOCK_H_
#define TESTAUTHORITYCLOCK_H_

#include "TestClock.h"
#include <alljoyn/time/TimeServiceServerClock.h>
#include <alljoyn/time/TimeServiceServerAuthorityClock.h>
#include <alljoyn/time/TimeServiceDateTime.h>

namespace ajn {
namespace services {

class TestAuthorityClock : public TimeServiceServerAuthorityClock {

  public:

    /**
     * Constructor
     */
    TestAuthorityClock();

    /**
     * Destructor
     */
    ~TestAuthorityClock();

    /**
     * Returns DateTime object
     *
     * @return DateTime object
     */
    const TimeServiceDateTime& getDateTime() const;

    /**
     * Set the DateTime object
     *
     * @param dateTime DateTime
     */
    void setDateTime(TimeServiceDateTime const& dateTime);

    /**
     * Whether or not this clock has ever been set.
     *
     * @return Returns TRUE if the Clock has been set since the last reboot
     */
    bool isSet() const;

  private:

    TestClock m_Clock;
};

} /* namespace services */
} /* namespace ajn */

#endif /* TESTAUTHORITYCLOCK_H_ */