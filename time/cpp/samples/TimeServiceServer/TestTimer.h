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

#ifndef TestTimer_H_
#define TestTimer_H_

#include <alljoyn/time/TimeServiceServerTimer.h>
#include <alljoyn/time/TimeServiceSchedule.h>

namespace ajn {
namespace services {

/**
 * Test Server Timer functionality
 */
class TestTimer : public TimeServiceServerTimer {

  public:

    /**
     * Constructor
     */
    TestTimer();

    /**
     * Destructor
     */
    ~TestTimer();

    /**
     * Returns TimeServiceSchedule of this Timer
     *
     * @return interval
     */
    const TimeServicePeriod& getInterval();

    /**
     * Set interval of this Timer object
     *
     * @param schedule TimeServiceSchedule
     * @return Status ER_OK If succeeded to set the Schedule, otherwise, if this Timer does not support
     * one of the interval values, return an appropriate failure status.
     */
    QStatus setInterval(const TimeServicePeriod& interval);

    /**
     * Returns time left for this Timer
     *
     * @return time left
     */
    const TimeServicePeriod& getTimeLeft();

    /**
     * Returns Timer title.
     *
     * @return Optional textual description of what this Timer is
     */
    const qcc::String& getTitle();

    /**
     * Set title of this Timer.
     *
     * @param title Timer title.
     * Title is an optional textual description of what this Timer is set for.
     */
    void setTitle(const qcc::String& title);

    /**
     * Returns whether or not this Timer is running
     *
     * @return Is Timer running
     */
    const bool isRunning();

    /**
     * Returns number of repeats for this Timer
     *
     * @return repeats
     */
    const uint16_t getRepeat();

    /**
     * Sets number of repeats for this Timer
     *
     * @return time left
     */
    void setRepeat(uint16_t repeat);

    /**
     * Start Timer
     */
    void start();

    /**
     * Reset Timer
     */
    void reset();

    /**
     * Pause
     */
    void pause();

  private:

    /**
     * Timer Interval
     */
    TimeServicePeriod m_Interval;

    /**
     * Timer time left
     */
    TimeServicePeriod m_TimeLeft;


    /**
     * Title of the Timer
     */
    qcc::String m_Title;

    /**
     * Timer is running
     */
    bool m_IsRunning;

    /**
     * repeat number
     */
    uint16_t m_Repeat;

    /**
     * set if you are running and signal to the clients
     */
    void setIsRunning(bool isRunning);

    /**
     * Print a period
     */
    void printPeriod(const TimeServicePeriod& interval) const;



};

} /* namespace services */
} /* namespace ajn */

#endif /* TestTimer_H_ */