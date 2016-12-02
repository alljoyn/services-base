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

package org.allseen.timeservice;

/**
 * Date & Time data type
 */
public class DateTime {

    private final Date date;

    private final Time time;

    /**
     * UTC offset in minutes
     */
    private final short offsetMinutes;

    /**
     * Constructor
     *
     * @param date {@link Date}
     * @param time {@link Time}
     * @param offsetMinutes UTC offset in minutes
     * @throws IllegalArgumentException Is thrown if received undefined arguments
     */
    public DateTime(Date date, Time time, short offsetMinutes) {

        if ( date == null ) {

            throw new IllegalArgumentException("Undefined date");
        }

        if ( time == null ) {

            throw new IllegalArgumentException("Undefined time");
        }

        this.date = date;
        this.time = time;
        this.offsetMinutes = offsetMinutes;
    }

    public Date getDate() {
        return date;
    }

    public Time getTime() {
        return time;
    }

    /**
     * UTC offset in minutes
     *
     * @return UTC offset
     */
    public short getOffsetMinutes() {
        return offsetMinutes;
    }

    /**
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {

        return "{DateTime " + date + " " + time + " offset: '" + offsetMinutes + "'}";
    }
}