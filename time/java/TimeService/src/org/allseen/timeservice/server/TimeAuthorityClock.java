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

import org.allseen.timeservice.TimeServiceException;

import android.util.Log;

/**
 * Extend this class to receive events related to this {@link TimeAuthorityClock} object
 * and send {@link TimeAuthorityClock#timeSync()} signal.
 */
public abstract class TimeAuthorityClock extends Clock {
    private static final String TAG = "ajts" + TimeAuthorityClock.class.getSimpleName();

    /**
     * {@link TimeAuthorityClockBusObj} implementing AllJoyn functionality of the
     * time authority clock
     */
    private TimeAuthorityClockBusObj timeAuthorityClockBusObj;

    /**
     * Send TimeSync signal to suggest this {@link TimeAuthorityClock} clients to
     * synchronize their time
     * @throws TimeServiceException Is thrown if failed to send the signal
     */
    public void timeSync() throws TimeServiceException {

        if ( timeAuthorityClockBusObj == null ) {

            Log.w(TAG, "This Clock hasn't been created yet");
            return;
        }

        timeAuthorityClockBusObj.sendTimeSync();
    }

    /**
     * @see org.allseen.timeservice.server.Clock#setClockBusObj(org.allseen.timeservice.server.BaseClockBusObj)
     */
    @Override
    void setClockBusObj(BaseClockBusObj clockBusObj) {

        timeAuthorityClockBusObj = (TimeAuthorityClockBusObj) clockBusObj;
        super.setClockBusObj(clockBusObj);
    }

    /**
     * @see org.allseen.timeservice.server.Clock#release()
     */
    @Override
    public void release() {

        super.release();
        timeAuthorityClockBusObj = null;
    }
}