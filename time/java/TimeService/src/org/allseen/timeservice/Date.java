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

import org.allseen.timeservice.ajinterfaces.Clock;

/**
 * Date is used by the {@link Clock} to set its {@link DateTime}
 */
public class Date {

    private final short YEAR;

    private final byte MONTH;

    private final byte DAY;

    /**
     * Constructor
     * @param year Expected: four digit format
     * @param month Expected range: 1-12
     * @param day Expected range: 1-31
     * @throws IllegalArgumentException Is thrown if received arguments are not in the correct range
     */
    public Date(short year, byte month, byte day) {

        checkValidity(year, month, day);

        YEAR   = year;
        MONTH  = month;
        DAY    = day;
    }

    public short getYear() {
        return YEAR;
    }

    public byte getMonth() {
        return MONTH;
    }

    public byte getDay() {
        return DAY;
    }

    /**
     * Check arguments validity.
     * @throws IllegalArgumentException Is thrown if received arguments are not in the correct range
     */
    private void checkValidity(short year, byte month, byte day) {

        if ( year <= 0 ) {

            throw new IllegalArgumentException("Year is not a positive number");
        }

        if ( month < 1 || month > 12 ) {

            throw new IllegalArgumentException("Month is not in the expected range");
        }

        if ( day < 1 || day > 31 ) {

            throw new IllegalArgumentException("Day is not in the expected range");
        }
    }

    /**
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {

        return String.format("{Date %s-%s-%s}", DAY, MONTH, YEAR);
    }
}