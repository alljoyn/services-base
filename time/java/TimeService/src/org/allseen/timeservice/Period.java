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

import org.allseen.timeservice.ajinterfaces.Timer;

/**
 * Period is used to indicate time interval of the {@link Timer}.
 */
public class Period {

    private final int HOUR;

    private final byte MINUTE;

    private final byte SECOND;

    private final short MILLISECOND;

    /**
     * Constructor
     * @param hour A positive number
     * @param minute Expected range: 0-59
     * @param second Expected range: 0-59
     * @param millisecond Expected range: 0-999
     * @throws IllegalArgumentException Is thrown if received arguments are not in the correct range
     */
    public Period(int hour, byte minute, byte second, short millisecond) {

        checkValidity(hour, minute, second, millisecond);

        HOUR         = hour;
        MINUTE       = minute;
        SECOND       = second;
        MILLISECOND  = millisecond;
    }

    public int getHour() {
        return HOUR;
    }

    public byte getMinute() {
        return MINUTE;
    }

    public byte getSeconds() {
        return SECOND ;
    }

    public short getMilliseconds() {
        return MILLISECOND;
    }

    /**
     * Check arguments validity.
     * @throws IllegalArgumentException Is thrown if received arguments are not in the correct range
     */
    private void checkValidity(int hour, byte minute, byte second, short millisecond) {

        if ( hour < 0 ) {

            throw new IllegalArgumentException("Hour is not a positive number");
        }

        if ( minute < 0 || minute > 59 ) {

            throw new IllegalArgumentException("Minute is not in the correct range");
        }

        if ( second < 0 || second > 59 ) {

            throw new IllegalArgumentException("Second is not in the correct range");
        }

        if ( millisecond < 0 || millisecond > 999 ) {

            throw new IllegalArgumentException("Millisecond is not in the correct range");
        }
    }

    /**
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {

        return String.format("{Period %s:%s:%s.%s}", HOUR, MINUTE, SECOND, MILLISECOND);
    }
}