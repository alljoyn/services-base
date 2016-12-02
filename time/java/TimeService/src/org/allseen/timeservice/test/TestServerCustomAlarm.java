 /******************************************************************************
  *   * 
  *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
  *    Source Project Contributors and others.
  *    
  *    All rights reserved. This program and the accompanying materials are
  *    made available under the terms of the Apache License, Version 2.0
  *    which accompanies this distribution, and is available at
  *    http://www.apache.org/licenses/LICENSE-2.0

  ******************************************************************************/

package org.allseen.timeservice.test;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.ErrorReplyBusException;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusMethod;
import org.allseen.timeservice.TimeServiceConst;
import org.allseen.timeservice.ajinterfaces.Alarm;
import org.allseen.timeservice.server.BaseAlarmBusObj;

public class TestServerCustomAlarm extends TestServerAlarm {

    @BusInterface(name=AlarmCoolIface.IFNAME)
    public static interface AlarmCoolIface {

        public static final String IFNAME = TimeServiceConst.IFNAME_PREFIX+".Alarm.CoolAlarm";

        @BusMethod
        String getCoolTitle() throws BusException;
    }

    //====================================================//

    public static class TestServerCustomAlarmBusObj extends BaseAlarmBusObj implements Alarm, AlarmCoolIface {

        @Override
        public String getCoolTitle() throws BusException {

            try {
                return "~~~~~~~~>>>" + getTitle() + "<<<~~~~~~~~~" ;
            } catch (BusException be) {

                throw new ErrorReplyBusException(TimeServiceConst.INVALID_VALUE_ERROR, "Failed to retrieve title");
            }
        }
    }
}