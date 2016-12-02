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
import org.alljoyn.bus.annotation.BusMethod;
import org.alljoyn.bus.annotation.BusProperty;
import org.alljoyn.bus.annotation.Secure;
import org.allseen.timeservice.TimeServiceConst;

/**
 * Time Service Timer Factory interface
 */
@BusInterface(name = TimerFactory.IFNAME, announced = "true")
@Secure
public interface TimerFactory extends BusObject {

    /**
     * The name of the AllJoyn interface
     */
    public static final String IFNAME = TimeServiceConst.IFNAME_PREFIX + ".TimerFactory";

    /**
     * The version of this interface
     */
    public static final short VERSION = 1;

    /**
     * Returns the interface version
     * 
     * @return Interface version number
     * @throws BusException
     */
    @BusProperty(signature = "q")
    short getVersion() throws BusException;

    /**
     * Creates a {@link Timer} under this {@link TimerFactory}. The newly
     * created {@link Timer} implements {@link Timer#IFNAME}
     * 
     * @return Returns the object path of the newly created {@link Timer}.
     * @throws BusException
     */
    @BusMethod(name = "NewTimer", replySignature = "o")
    String newTimer() throws BusException;

    /**
     * Removes {@link Timer} created by this {@link TimerFactory}.
     * 
     * @param objectPath
     *            The object path of the {@link Timer} to remove
     * @throws BusException
     */
    @BusMethod(name = "DeleteTimer", signature = "o")
    void deleteTimer(String objectPath) throws BusException;

}