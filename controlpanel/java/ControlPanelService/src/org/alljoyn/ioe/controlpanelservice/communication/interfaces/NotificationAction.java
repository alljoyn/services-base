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

package org.alljoyn.ioe.controlpanelservice.communication.interfaces;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusProperty;
import org.alljoyn.bus.annotation.BusSignal;
import org.alljoyn.ioe.controlpanelservice.ControlPanelService;

/**
 * This interface indicates whether the object is a notification action object
 */
@BusInterface (name = NotificationAction.IFNAME)
public interface NotificationAction {
	public static final String IFNAME  = ControlPanelService.INTERFACE_PREFIX + ".NotificationAction"; 

	public static final int ID_MASK    = 0x04;
	
	public static final short  VERSION = 1;
	
	/**
	 * @return Interface version
	 */
	@BusProperty(signature="q")
	public short getVersion() throws BusException;
	
	/**
	 * The controller needs to dismiss this NotificationAction panel
	 */
	@BusSignal
	public void Dismiss() throws BusException;
	
}