/******************************************************************************
 *  * 
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
import org.allseen.timeservice.ajinterfaces.Clock;

/**
 * This class implements {@link Clock} interface and realizes AllJoyn
 * communication with this clock.
 */
class ClockBusObj extends BaseClockBusObj implements Clock {

    /**
     * Prefix of the Clock object path
     */
    private static final String OBJ_PATH_PREFIX = "/Clock";

    /**
     * Constructor
     *
     * @param clock
     *            Clock events delegate. This clock receives all the
     *            {@link org.allseen.timeservice.server.Clock} related events.
     */
    ClockBusObj(org.allseen.timeservice.server.Clock clock) throws TimeServiceException {

        super(clock, GlobalStringSequencer.append(OBJ_PATH_PREFIX));

    }

    /**
     * @see org.allseen.timeservice.server.BaseClockBusObj#release()
     */
    @Override
    protected void release() {

        super.release();

    }
}