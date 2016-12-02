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

#ifndef TIMESERVICESCHEDULE_H_
#define TIMESERVICESCHEDULE_H_

#include <alljoyn/time/LogModule.h>
#include <alljoyn/time/TimeServiceTime.h>
#include <alljoyn/Status.h>

namespace ajn {
namespace services {

/**
 * The Schedule indicates the when Alarm should fire.
 */
class TimeServiceSchedule {

  public:

    /**
     * Days of week used to define Alarm schedule.
     */
    enum WeekDay {

        SUNDAY      = 0x1,
        MONDAY      = 0x2,
        TUESDAY     = 0x4,
        WEDNESDAY   = 0x8,
        THURSDAY    = 0x10,
        FRIDAY      = 0x20,
        SATURDAY    = 0x40,
    };

    /**
     * Constructor
     */
    TimeServiceSchedule();

    /**
     * Destructor
     */
    ~TimeServiceSchedule();

    /**
     * Initialize the object
     *
     * @param time Time of the Alarm
     * @param weekDays Bitmap of WeekDay(s) when the Alarm should fire.
     * Value of zero means that the Alarm is fired only once.
     * @return ER_OK if the object was initialized successfully and valid arguments have been passed to this method
     * otherwise  ER_BAD_ARGUMENT of the appropriate argument is returned
     */
    QStatus init(const TimeServiceTime& time, uint8_t weekDays);

    /**
     * Checks whether data of the object is valid.
     */
    bool isValid() const;

    /**
     * Returns Time
     *
     * @return Time
     */
    const TimeServiceTime& getTime() const;

    /**
     * Returns Bitmap of WeekDay(s) when the Alarm should fire.
     * Value of zero means that the Alarm is fired only once.
     *
     * @return Bitmap of WeekDay(s) when the Alarm should fire
     */
    const uint8_t getWeekDays() const;

  private:

    /**
     * Time
     */
    TimeServiceTime m_Time;

    /**
     * WeekDay(s) bitmap
     */
    uint8_t m_WeekDays;
};

} /* namespace services */
} /* namespace ajn */

#endif /* TIMESERVICESCHEDULE_H_ */