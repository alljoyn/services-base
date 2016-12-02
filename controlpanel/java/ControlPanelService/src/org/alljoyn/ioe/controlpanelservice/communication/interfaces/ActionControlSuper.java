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

/**
 * The parent interface of the {@link ActionControl} and {@link ActionControlSecured} interfaces
 */
public interface ActionControlSuper {

    /**
     * @return Interface version
     */
    public short getVersion() throws BusException;

    /**
     * @return States bitmask
     * @throws BusException
     */
    public int getStates() throws BusException;

    /**
     * @return Optional parameters
     * @throws BusException
     */
    public Map<Short, Variant> getOptParams() throws BusException;

    /**
     * Called when the action is executed on the widget
     */
    public void Exec() throws BusException;

    /**
     * Signal is sent when the UI container metadata changed
     * @param metadata
     */
    public void MetadataChanged() throws BusException;

}