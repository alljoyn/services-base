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
package org.alljoyn.ioe.controlpanelservice.communication.interfaces;

import java.util.Map;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.Variant;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusMethod;
import org.alljoyn.bus.annotation.BusProperty;
import org.alljoyn.bus.annotation.BusSignal;
import org.alljoyn.bus.annotation.Secure;
import org.alljoyn.ioe.controlpanelservice.ControlPanelService;

/**
 * Action control secured interface
 */
@BusInterface(name = ActionControlSecured.IFNAME)
@Secure
public interface ActionControlSecured extends ActionControlSuper {
    public static final String IFNAME = ControlPanelService.INTERFACE_PREFIX + ".SecuredAction";

    public static final short VERSION = 1;

    /**
     * @return Interface version
     */
    @BusProperty(signature = "q")
    public short getVersion() throws BusException;

    /**
     * @return States bitmask
     * @throws BusException
     */
    @BusProperty(signature = "u")
    public int getStates() throws BusException;

    /**
     * @return Optional parameters
     * @throws BusException
     */
    @BusProperty(signature = "a{qv}")
    public Map<Short, Variant> getOptParams() throws BusException;

    /**
     * Called when the action is executed on the widget
     */
    @BusMethod
    public void Exec() throws BusException;

    /**
     * Signal is sent when the UI container metadata changed
     * @param metadata
     */
    @BusSignal
    public void MetadataChanged() throws BusException;

}