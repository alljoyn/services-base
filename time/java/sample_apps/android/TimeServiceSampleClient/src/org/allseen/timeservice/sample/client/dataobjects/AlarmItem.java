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

import org.allseen.timeservice.client.Alarm;
import org.allseen.timeservice.client.AlarmFactory;

/**
 *
 * Encapsulates Alarm information for display. Contains reference of Alarm and AlarmFactory for easy data access.
 *
 */
public class AlarmItem {

    public int hour;
    public int minutes;
    public int seconds;
    public String description;
    public String objectpath;
    public boolean enabled;
    public int version;
    public boolean[] arrayDays = new boolean[7];
    public boolean isLocked;
    public boolean isSignalRegistered;
    public AlarmFactory alarmFactory;
    public Alarm alarm;
}