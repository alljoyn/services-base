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

import org.allseen.timeservice.DateTime;
import org.allseen.timeservice.TimeServiceException;
import org.allseen.timeservice.server.TimeAuthorityClock;

/**
 * Test Server Time Authority Clock
 */
public class TestServerAuthorityClock extends TimeAuthorityClock {


    private final TestServerClock clock;

    /**
     * Constructor
     * @param clock
     */
    public TestServerAuthorityClock(TestServerClock clock) {

        this.clock = clock;
    }

    @Override
    public DateTime getDateTime() {

        return clock.getDateTime();
    }

    @Override
    public void setDateTime(DateTime dateTime) {

        clock.setDateTime(dateTime);
    }

    @Override
    public boolean getIsSet() {

        return true;
    }

    /**
     * Send TimeSync signal
     */
     @Override
    public void timeSync() throws TimeServiceException {

        super.timeSync();
    }
}