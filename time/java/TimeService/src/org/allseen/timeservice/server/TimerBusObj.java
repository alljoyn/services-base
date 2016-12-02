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

package org.allseen.timeservice.server;

import org.allseen.timeservice.ajinterfaces.Timer;

/**
 * This class implements {@link Timer} interface and realizes AllJoyn communication
 * with this {@link Timer}.
 */
//All the functionality of this class is in the extended BaseTimerBusObj.
//We need this class to implement Timer interface, because currently JavaBinding doesn't support
//finding AJ interfaces in the SuperClasses.
class TimerBusObj extends BaseTimerBusObj implements Timer {
}