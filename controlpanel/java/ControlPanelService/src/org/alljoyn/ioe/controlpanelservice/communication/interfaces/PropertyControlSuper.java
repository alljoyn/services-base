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
 * The parent interface of the {@link PropertyControl} and {@link PropertyControlSecured} interfaces
 */
public interface PropertyControlSuper {
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
     * @return Returns the property current value
     */
    public Variant getValue() throws BusException;

    /**
     * @param value The property value
     */
    public void setValue(Variant value) throws BusException;

    /**
     * Signal is sent when the property value changed
     * @param newValue The new property value
     */
    public void ValueChanged(Variant newValue) throws BusException;

    /**
     * Signal is sent when the property metadata changed
     * @param metadata
     */
    public void MetadataChanged() throws BusException;
}