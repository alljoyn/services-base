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

package org.allseen.timeservice.ajinterfaces;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusProperty;
import org.alljoyn.bus.annotation.BusSignal;
import org.allseen.timeservice.TimeServiceConst;
import org.allseen.timeservice.server.TimeAuthorityClock;

/**
 * Time Service Time Authority interface. {@link BusObject} implementing this
 * interface declares that it is a Time Authority.
 */
@BusInterface(name = TimeAuthority.IFNAME, announced = "true")
public interface TimeAuthority {

    /**
     * The name of the AllJoyn interface
     */
    public static final String IFNAME = TimeServiceConst.IFNAME_PREFIX + ".TimeAuthority";

    /**
     * This interface version
     */
    public static final short VERSION = 1;

    /**
     * Returns the interface version
     * 
     * @return Interface version number
     */
    @BusProperty(signature = "q")
    short getVersion() throws BusException;

    /**
     * Source from which {@link TimeAuthorityClock} synchronizes its time
     * against.
     * 
     * @return {@link TimeAuthorityClock} source
     * @throws BusException
     */
    @BusProperty(signature = "y")
    byte getAuthorityType() throws BusException;

    /**
     * Sessionless signal suggesting to synchronize time with the time authority
     * clock that has sent the signal.
     */
    @BusSignal(name = "TimeSync", sessionless = true)
    void timeSync() throws BusException;
}