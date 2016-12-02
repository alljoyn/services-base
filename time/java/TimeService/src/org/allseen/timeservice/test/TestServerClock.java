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

package org.allseen.timeservice.test;

import org.allseen.timeservice.Date;
import org.allseen.timeservice.DateTime;
import org.allseen.timeservice.server.Clock;

import android.text.format.Time;

/**
 * Test Server Clock
 */
public class TestServerClock extends Clock {

    private DateTime dt;
    private boolean isSet;

    /**
     * Constructor
     */
    public TestServerClock() {

        super();

        Time t = new Time(Time.getCurrentTimezone());
        t.setToNow();

        byte seconds = (byte) ((t.second > 59) ? 59 :  t.second);

        org.allseen.timeservice.Time ts = new org.allseen.timeservice.Time((byte)t.hour, (byte) t.minute,
                                                seconds, (short)0);

        org.allseen.timeservice.Date ds = new Date((short)t.year, (byte)(t.month + 1), (byte)t.monthDay);

        dt = new DateTime(ds, ts, (short)120);
    }

    @Override
    public DateTime getDateTime() {

        return dt;
    }

    @Override
    public void setDateTime(DateTime dateTime) {

        this.dt = dateTime;
        isSet   = true;
    }

    @Override
    public boolean getIsSet() {

        return isSet;
    }

}