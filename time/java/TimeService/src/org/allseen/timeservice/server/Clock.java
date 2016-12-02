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

package org.allseen.timeservice.server;

import org.allseen.timeservice.DateTime;

import android.util.Log;

/**
 * Extend this class to receive events related to this {@link Clock} object.
 */
public abstract class Clock {
    private static final String TAG = "ajts" + Clock.class.getSimpleName();

    /**
     * {@link BaseClockBusObj} implementing AllJoyn functionality
     */
    private BaseClockBusObj baseClockBusObj;

    /**
     * @return {@link DateTime} object.
     */
    public abstract DateTime getDateTime();

    /**
     * Set the {@link DateTime} object
     * @param dateTime {@link DateTime}
     */
    public abstract void setDateTime(DateTime dateTime);

    /**
     * Whether or not this clock has ever been set.
     * @return Returns TRUE if the {@link Clock} has been set since the last reboot
     */
    public abstract boolean getIsSet();

    /**
     * Returns the object path of this {@link Clock} object
     * @return object path
     */
    public String getObjectPath() {

        if ( baseClockBusObj == null ) {

            Log.w(TAG, "This Clock hasn't been created yet");
            return null;
        }

        return baseClockBusObj.getObjectPath();
    }

    /**
     * Release object resources. <br>
     * It's a programming error to call another method on this object after the release method has been called.
     */
    public void release() {

        if ( baseClockBusObj == null ) {

            Log.w(TAG, "This Clock hasn't been created yet");
            return;
        }

        Log.i(TAG, "Releasing Server Clock");
        baseClockBusObj.release();
        baseClockBusObj = null;
    }

    /**
     * Associate this {@link Clock} with the {@link ClockBusObj} which implements AllJoyn functionality
     * @param clockBusObj {@link ClockBusObj}
     */
    void setClockBusObj(BaseClockBusObj clockBusObj) {

        this.baseClockBusObj = clockBusObj;
    }
}