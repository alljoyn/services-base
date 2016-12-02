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

import org.allseen.timeservice.client.Timer;
import org.allseen.timeservice.client.TimerFactory;


/**
 *
 * Encapsulates Timer information for display. Contains reference of Timer and TimerFactory for easy data access.
 *
 */
public class TimerItem {

    public String remaining;
    public String interval;
    public String description;
    public String objectpath;
    public boolean isSignalRegistered;
    public boolean running;
    public short repeat;
    public int version;
    public boolean isLocked;
    public TimerFactory timerFactory;
    public Timer timer;

}