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

package org.alljoyn.common;

import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusException;
import org.alljoyn.bus.Status;

/**
 * A base class for service clients.
 */
public interface ClientBase
{
    /**
     * Stop the AllJoyn session with the device.
     */
    public void disconnect();

    /**
     * Start an AllJoyn session with the device.
     * @return AllJoyn Bus Status
     */
    public Status connect();

    /**
     * Is there an open session with the device.
     * @return true if there is a session with the device.
     */
    public boolean isConnected();

    /**
     * Interface version
     * @return Interface version
     * @throws BusException indicating failure obtain Version property
     */
    public short getVersion() throws BusException;

    /**
     * The peer device's bus name, as advertised by Announcements
     * @return Unique bus name
     */
    public String getPeerName();

    /**
     * The id of the open session with the peer device.
     * @return AllJoyn session id
     */
    public int getSessionId();

    /**
     * Initialize client by passing the BusAttachment
     * @param busAttachment BusAttachment associated with this ClientBase instance
     * @throws Exception error indicating failure to initialize the client
     */
    void initBus(BusAttachment busAttachment) throws Exception;

}