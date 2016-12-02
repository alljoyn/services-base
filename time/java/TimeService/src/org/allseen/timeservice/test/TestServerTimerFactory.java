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

import java.util.ArrayList;
import java.util.List;

import org.alljoyn.bus.ErrorReplyBusException;
import org.allseen.timeservice.TimeServiceConst;
import org.allseen.timeservice.server.Timer;

import android.util.Log;

/**
 * Tests Timer Factory
 */
public class TestServerTimerFactory extends org.allseen.timeservice.server.TimerFactory  {
    private static final String TAG = "ajts" + TestServerTimerFactory.class.getSimpleName();

    /**
     * Timers created by this Timer Factory
     */
    private final List<Timer> timers;

    /**
     * Constructor
     */
    public TestServerTimerFactory() {

        timers = new ArrayList<Timer>();
    }

    /**
     * @see org.allseen.timeservice.server.AlarmFactory#release()
     */
    @Override
    public void release() {

        for ( Timer timer : timers ) {

            timer.release();
        }
        timers.clear();

        super.release();
    }

    /**
     * @see org.allseen.timeservice.server.TimerFactory#newTimer()
     */
    @Override
    public Timer newTimer() throws ErrorReplyBusException {

        Timer timer = new TestServerTimer();
        timers.add(timer);

        Log.i(TAG, "Created new Timer by Factory");
        return timer;
    }

    /**
     * @see org.allseen.timeservice.server.TimerFactory#deleteTimer(java.lang.String)
     */
    @Override
    public void deleteTimer(String objectPath) throws ErrorReplyBusException {

        Timer timer = findTimer(objectPath);

        if ( timer != null ) {

            Log.i(TAG, "Removing Timer, objectPath: '" + objectPath + "'");
            timer.release();

            timers.remove(timer);

            return;
        }

        throw new ErrorReplyBusException(TimeServiceConst.GENERIC_ERROR, "Timer: '" + objectPath + "' is not found");
    }

    /**
     * Looks for Timer
     */
    public Timer findTimer(String objectPath) {

        for ( Timer timer : timers ) {

            if ( timer.getObjectPath().equals(objectPath) ) {

                return timer;
            }
        }

        return null;
    }
}