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
package org.allseen.timeservice.sample.client.ui;

public interface FragmentCommunicationInterface {

    public final int ALARMS_MANAGER = 1;
    public final int TIMERS_MANAGER = 2;
    public final int SET_CLOCK = 3;
    public final int GET_CLOCK = 4;
    public final int ALARM_NEW = 10;
    public final int ALARM_EDIT = 11;
    public final int TIMER_NEW = 20;
    public final int TIMER_EDIT = 21;

    public void displayFragment(int fragmentID, Object extraData);

    public void setObject(Object obj);

    public Object getObject();

}