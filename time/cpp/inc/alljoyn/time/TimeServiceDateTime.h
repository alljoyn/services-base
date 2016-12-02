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

#ifndef DATETIME_H_
#define DATETIME_H_

#include <alljoyn/time/LogModule.h>
#include <alljoyn/time/TimeServiceDate.h>
#include <alljoyn/time/TimeServiceTime.h>
#include <alljoyn/Status.h>

namespace ajn {
namespace services {

/**
 * Date & Time data type
 */
class TimeServiceDateTime {

  public:

    /**
     * Constructor
     */
    TimeServiceDateTime();

    /**
     * Destructor
     */
    ~TimeServiceDateTime();

    /**
     * Initialize the object with data.
     *
     * @param date
     * @param time
     * @param offsetMinutes
     * @return ER_OK if the object was initialized successfully and valid arguments have been passed to this method,
     * otherwise ER_BAD_ARGUMENT of the appropriate argument is returned
     */
    QStatus init(const TimeServiceDate& date, const TimeServiceTime& time, int16_t offsetMinutes);

    /**
     * Checks whether data of the object is valid, the object variables have a correct values, date and time are valid
     */
    bool isValid() const;

    /**
     * Returns Date
     *
     * @return Date
     */
    const TimeServiceDate& getDate() const;


    /**
     * Returns Time
     *
     * @return Time
     */
    const TimeServiceTime& getTime() const;

    /**
     * Returns Offset minutes
     *
     * @return Offset minutes
     */
    const int16_t getOffsetMinutes() const;

  private:

    /**
     * Date
     */
    TimeServiceDate m_Date;

    /**
     * Time
     */
    TimeServiceTime m_Time;

    /**
     * Offset minutes
     */
    int16_t m_OffsetMinutes;
};

} /* namespace services */
} /* namespace ajn */

#endif /* DATETIME_H_ */