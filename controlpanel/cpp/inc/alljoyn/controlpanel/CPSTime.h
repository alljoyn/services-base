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

#ifndef CPSTIME_H_
#define CPSTIME_H_

#include <qcc/platform.h>

namespace ajn {
namespace services {

/**
 * A class to allow sending of a Time as a Property
 */
class CPSTime {
  public:

    /**
     * Constructor of a CPSTime.
     * @param hour - hour of date
     * @param minute - minute of date
     * @param second - second of date
     */
    CPSTime(uint16_t hour, uint16_t minute, uint16_t second);

    /**
     * Destructor of CPSTime
     */
    virtual ~CPSTime();

    /**
     * Get the hour value of the date
     * @return hour value
     */
    uint16_t getHour() const;

    /**
     * Set the hour Value of the date
     * @param hour value
     */
    void setHour(uint16_t hour);

    /**
     * Get the Minute value of the date
     * @return minute value
     */
    uint16_t getMinute() const;

    /**
     * Set the Minute value of the date
     * @param minute value
     */
    void setMinute(uint16_t minute);

    /**
     * Get the Second value of the date
     * @return second value
     */
    uint16_t getSecond() const;

    /**
     * Set the Second value of the date
     * @param second value
     */
    void setSecond(uint16_t second);

  private:

    /**
     * Hour value of the Time
     */
    uint16_t m_Hour;

    /**
     * Minute value of the Time
     */
    uint16_t m_Minute;

    /**
     * Second value of the Time
     */
    uint16_t m_Second;
};

} /* namespace services */
} /* namespace ajn */

#endif /* CPSTIME_H_ */