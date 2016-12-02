 /******************************************************************************
  *   * 
  *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
  *    Source Project Contributors and others.
  *    
  *    All rights reserved. This program and the accompanying materials are
  *    made available under the terms of the Apache License, Version 2.0
  *    which accompanies this distribution, and is available at
  *    http://www.apache.org/licenses/LICENSE-2.0

  ******************************************************************************/

package org.allseen.timeservice.ajinterfaces;

import org.alljoyn.bus.annotation.Position;
import org.allseen.timeservice.Schedule;
import org.allseen.timeservice.Schedule.WeekDay;
import org.allseen.timeservice.ajinterfaces.DateTimeAJ.TimeAJ;

/**
 * Schedule structure for communicating with {@link Alarm}
 */
public class ScheduleAJ {

    /**
     * Time
     */
    @Position(0)
    public TimeAJ time;

    /**
     * Weekdays bitmap
     */
    @Position(1)
    public byte weekdays;

    /**
     * Constructor
     */
    public ScheduleAJ() {
    }

    /**
     * Constructor
     * @param schedule
     */
    public ScheduleAJ(Schedule schedule) {

         time       = new TimeAJ(schedule.getTime());
         weekdays   = WeekDay.getBitMap( schedule.getWeekDays() );
    }

    /**
     * Creates {@link Schedule} from {@link ScheduleAJ}
     * @return {@link ScheduleAJ}
     * @throws IllegalArgumentException Is thrown if failed to create the {@link Schedule}
     * @see Schedule
     */
    public Schedule toSchedule() {

        return new Schedule(time.toTime(), WeekDay.getWeekDays(weekdays));
    }

    /**
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {

        return String.format("{ScheduleAJ Time: '%s' WeekDays: '%s'}", time, weekdays);
    }
}