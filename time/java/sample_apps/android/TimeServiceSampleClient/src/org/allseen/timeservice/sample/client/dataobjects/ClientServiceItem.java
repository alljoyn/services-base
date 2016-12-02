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
package org.allseen.timeservice.sample.client.dataobjects;

import java.util.UUID;

/**
 *
 * Encapsulates ClientService information for display.
 *
 */
public class ClientServiceItem {

    public UUID uuid;
    public String name;
    public String busName;
    public boolean alarms;
    public boolean timers;
    public boolean clock;
    public boolean timeAuthority;
    public boolean syncTimeRegistered;
    public long lastSeen;

    public ClientServiceItem(String name, String busName) {
        this.name = name;
        this.busName = busName;
    }
}