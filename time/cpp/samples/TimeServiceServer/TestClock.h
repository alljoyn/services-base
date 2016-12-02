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

#ifndef TESTCLOCK_H_
#define TESTCLOCK_H_

#include <alljoyn/time/TimeServiceServerClock.h>
#include <alljoyn/time/TimeServiceDateTime.h>

namespace ajn {
namespace services {

/**
 * Test Server Clock functionality
 */
class TestClock : public TimeServiceServerClock {

  public:

    /**
     * Constructor
     */
    TestClock();

    /**
     * Destructor
     */
    ~TestClock();

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

    /**
     * If the Clock has ever been set since the last reboot
     */
    bool m_IsSet;

    /**
     * DateTime of the Clock
     */
    TimeServiceDateTime m_DateTime;

    /**
     * Creates String representation of the DateTime
     */
    void printDateTime(TimeServiceDateTime const& dateTime) const;
};

} /* namespace services */
} /* namespace ajn */

#endif /* TESTCLOCK_H_ */