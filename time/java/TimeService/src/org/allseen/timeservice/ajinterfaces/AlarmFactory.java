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
 * Time Service Alarm Factory interface
 */
@BusInterface(name = AlarmFactory.IFNAME, announced = "true")
@Secure
public interface AlarmFactory extends BusObject {

    /**
     * The name of the AllJoyn interface
     */
    public static final String IFNAME = TimeServiceConst.IFNAME_PREFIX + ".AlarmFactory";

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
     * Creates an {@link Alarm} under this {@link AlarmFactory}. The newly
     * created {@link Alarm} implements {@link Alarm#IFNAME}.
     * 
     * @return Returns the object path of the newly created {@link Alarm}.
     * @throws BusException
     */
    @BusMethod(name = "NewAlarm", replySignature = "o")
    String newAlarm() throws BusException;

    /**
     * Removes {@link Alarm} created by this {@link AlarmFactory}.
     * 
     * @param objectPath
     *            The object path of the {@link Alarm} to remove
     * @throws BusException
     */
    @BusMethod(name = "DeleteAlarm", signature = "o")
    void deleteAlarm(String objectPath) throws BusException;
}