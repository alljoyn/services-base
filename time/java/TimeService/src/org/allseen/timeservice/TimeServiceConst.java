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

package org.allseen.timeservice;

import org.allseen.timeservice.server.TimeServiceServer;

/**
 * Constant definitions of the Time Service
 */
public class TimeServiceConst {

    /**
     * This is the prefix for all the interfaces of the Time Service
     */
    public static final String IFNAME_PREFIX       = "org.allseen.Time";

    /**
     * Port number that the {@link TimeServiceServer} listens to
     */
    public static final short PORT_NUM             = 1030;

    /**
     * General Error
     */
    public static final String GENERIC_ERROR       = "org.alljoyn.Error";

    /**
     * Invalid Value Error
     */
    public static final String INVALID_VALUE_ERROR = "org.alljoyn.Error.InvalidValue";

    /**
     * Out Of Range Error
     */
    public static final String OUT_OF_RANGE        = "org.alljoyn.Error.OutOfRange";

    /**
     * No need to initialize the class
     */
    private TimeServiceConst(){}
}