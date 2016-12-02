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
package org.alljoyn.onboarding.sdk;

/**
 *Holder of offboarding data: service name ,port
 */
public class OffboardingConfiguration {

    /**
     * The service name of the device
     */
    private final String serviceName;

    /**
     * The port of the onboarding service
     */
    private final short port;

    /**
     * Constructor of OffboardingConfiguration
     *
     * @param serviceName
     *            {@link #serviceName}
     * @param port
     *            {@link #port}
     *
     */
    public OffboardingConfiguration(String serviceName, short port) {
        this.serviceName = serviceName;
        this.port = port;
    }

    /**
     * Get {@link #serviceName}
     *
     * @return the device's service name {@link #serviceName}
     */
    public String getServiceName() {
        return serviceName;
    }

    /**
     * Get {@link #port}
     *
     * @return the devices's port {@link #port}
     */
    public short getPort() {
        return port;
    }

}