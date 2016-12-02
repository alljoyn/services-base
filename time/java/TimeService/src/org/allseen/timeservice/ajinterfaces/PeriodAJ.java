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
import org.allseen.timeservice.Period;

/**
 * Period structure
 */
public class PeriodAJ {

    //Positive
    @Position(0)
    public int hour;

    //0-59
    @Position(1)
    public byte minute;

    //0-59
    @Position(2)
    public byte second;

    //0-999
    @Position(3)
    public short millisecond;

    /**
     * Default Constructor
     */
    public PeriodAJ() {
    }

    /**
     * Constructor
     * @param period
     */
    public PeriodAJ(Period period) {

        hour          = period.getHour();
        minute        = period.getMinute();
        second        = period.getSeconds();
        millisecond   = period.getMilliseconds();
    }

    /**
     * Converts {@link PeriodAJ} into {@link Period}
     * @return {@link Period}
     * @throws IllegalArgumentException
     */
    public Period toPeriod() {

        return new Period(hour, minute, second, millisecond);
    }

    /**
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {

        return String.format("{TimeAJ %s:%s:%s.%s}", hour, minute, second, millisecond);
    }
}