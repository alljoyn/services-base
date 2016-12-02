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

package org.allseen.timeservice.sample.server.logic;

import org.allseen.timeservice.DateTime;
import org.allseen.timeservice.TimeServiceException;
import org.allseen.timeservice.sample.server.ui.TimeSampleServer;
import org.allseen.timeservice.server.TimeAuthorityClock;

/**
 * Represents an TimeAuthorityClock {@link  org.allseen.timeservice.server.TimeAuthorityClock}.
 */
public class ServerAuthorityClock extends TimeAuthorityClock {

    /**
     * ServerClock storage.
     */
    private final ServerClock clock;

    /**
     * Constructor
     * @param clock
     */
    public ServerAuthorityClock(ServerClock clock) {

        TimeSampleServer.sendMessage(clock.context, TimeSampleServer.CLOCK_EVENT_ACTION, getObjectPath(), "ServerAuthorityClock created");
        this.clock = clock;
    }

    @Override
    public DateTime getDateTime() {

        TimeSampleServer.sendMessage(clock.context, TimeSampleServer.CLOCK_EVENT_ACTION, getObjectPath(), "ServerAuthorityClock getDateTime request");
        return clock.getDateTime();
    }

    @Override
    public void setDateTime(DateTime dateTime) {

        TimeSampleServer.sendMessage(clock.context, TimeSampleServer.CLOCK_EVENT_ACTION, getObjectPath(), "ServerAuthorityClock setDateTime request");
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
        TimeSampleServer.sendMessage(clock.context, TimeSampleServer.CLOCK_EVENT_ACTION, getObjectPath(), "ServerAuthorityClock timeSync");

        super.timeSync();
    }
}