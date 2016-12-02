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

import java.util.List;

import org.alljoyn.bus.AboutObjectDescription;

/**
 * A base class for Services
 */
public interface ServiceCommon
{
    /**
     * The list of BusObjects that are registered for this service, and the
     * BusInterfaces that they implement.
     * @return the list of AboutObjectDescriptions
     * @see About#announce(short, short, AboutObjectDescription[], java.util.Map)
     */
    public List<AboutObjectDescription> getAboutObjectDescriptions();

    /**
     * Set the platform specific logger
     * @param logger a Java logger utility
     */
    void setLogger(GenericLogger logger);

    /**
     * Is the service running in a client mode. Note: a service can run in both
     * modes simultaneously
     * @return true if the service is running in a client mode
     */
    boolean isClientRunning();

    /**
     * Is the service running in a server mode. Note: a service can run in both
     * modes simultaneously
     * @return true if the service is running in a server mode
     */
    boolean isServerRunning();

}