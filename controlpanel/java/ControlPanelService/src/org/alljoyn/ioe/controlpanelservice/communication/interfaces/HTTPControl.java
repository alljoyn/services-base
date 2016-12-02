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

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusMethod;
import org.alljoyn.bus.annotation.BusProperty;
import org.alljoyn.ioe.controlpanelservice.ControlPanelService;

/**
 * HTTPControl interface
 */
@BusInterface(name = HTTPControl.IFNAME)
public interface HTTPControl {
    public static final String IFNAME = ControlPanelService.INTERFACE_PREFIX + ".HTTPControl";

    public static final int ID_MASK = 0x02;

    public static final short VERSION = 1;

    /**
     * @return Interface version
     */
    @BusProperty(signature = "q")
    public short getVersion() throws BusException;

    /**
     * @return Retrieve the URL of the home page
     * @throws BusException
     */
    @BusMethod
    public String GetRootURL() throws BusException;

}