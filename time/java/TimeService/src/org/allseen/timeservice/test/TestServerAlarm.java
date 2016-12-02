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

import org.alljoyn.bus.ErrorReplyBusException;
import org.allseen.timeservice.Schedule;
import org.allseen.timeservice.TimeServiceException;
import org.allseen.timeservice.server.Alarm;

import android.text.format.Time;

public class TestServerAlarm extends Alarm {

    /**
     * {@link Schedule}
     */
    private Schedule schedule;

    /**
     * Alarm title
     */
    private String title;

    /**
     * Is Alarm enabled
     */
    private boolean isEnabled;

    /**
     * Constructor
     */
    public TestServerAlarm() {
        super();

        Time t = new Time(Time.getCurrentTimezone());
        t.setToNow();

        org.allseen.timeservice.Time ts = new org.allseen.timeservice.Time((byte)0, (byte) 0, (byte)0, (short)0);
        schedule = new Schedule(ts);

        title = "";
    }

    @Override
    public Schedule getSchedule() {

        return schedule;
    }

    @Override
    public void setSchedule(Schedule schedule) throws ErrorReplyBusException {

        this.schedule = schedule;
    }

    @Override
    public String getTitle() {

        return title;
    }

    @Override
    public void setTitle(String title) {

        this.title = title;
    }

    @Override
    public boolean isEnabled() {

        return isEnabled;
    }

    @Override
    public void setEnabled(boolean enabled) {

        this.isEnabled = enabled;
    }

    @Override
    public void alarmReached() throws TimeServiceException {

        super.alarmReached();
    }

}