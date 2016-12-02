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

#ifndef TestAlarm_H_
#define TestAlarm_H_

#include <alljoyn/time/TimeServiceServerAlarm.h>
#include <alljoyn/time/TimeServiceSchedule.h>

namespace ajn {
namespace services {

/**
 * Test Server Alarm functionality
 */
class TestAlarm : public TimeServiceServerAlarm {

  public:

    /**
     * Constructor
     */
    TestAlarm();

    /**
     * Destructor
     */
    ~TestAlarm();

    /**
     * Returns TimeServiceSchedule of this Alarm
     *
     * @return Alarm
     */

    const TimeServiceSchedule& getSchedule();

    /**
     * Set TimeServiceSchedule to this Alarm object
     *
     * @param schedule TimeServiceSchedule
     * @return Status ER_OK If succeeded to set the Schedule, otherwise, if this Alarm does not support
     * one of the Schedule values, return an appropriate failure status.
     */
    QStatus setSchedule(const TimeServiceSchedule& schedule);

    /**
     * Returns Alarm title.
     *
     * @return Optional textual description of what this Alarm is
     */
    const qcc::String& getTitle();

    /**
     * Set title to this Alarm.
     *
     * @param title Alarm title.
     * Title is an optional textual description of what this Alarm is set for.
     */
    void setTitle(const qcc::String& title);

    /**
     * Returns whether or not this Alarm is enabled
     *
     * @return Is Alarm enabled
     */
    const bool isEnabled();

    /**
     * Set whether or not this Alarm is enabled
     *
     * @param enabled Is Alarm enabled
     */
    void setEnabled(bool enabled);

  private:

    /**
     * Schedule of the Alarm
     */
    TimeServiceSchedule m_Schedule;

    /**
     * Title of the Alarm
     */
    qcc::String m_Title;

    /**
     * Alarm enabled
     */
    bool m_Enabled;


    /**
     * Creates String representation of the DateTime
     */
    void printSchedule() const;

    /**
     * Creates String representation of the week days
     */
    qcc::String getWeekdaysString() const;
};

} /* namespace services */
} /* namespace ajn */

#endif /* TestAlarm_H_ */