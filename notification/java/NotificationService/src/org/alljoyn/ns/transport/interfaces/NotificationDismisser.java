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


package org.alljoyn.ns.transport.interfaces;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusProperty;
import org.alljoyn.bus.annotation.BusSignal;

/**
 * The interface provides the functionality to send and receive Dismiss session-less-signals
 */
@BusInterface(name = NotificationDismisser.IF_NAME)
public interface NotificationDismisser extends BusObject {
	
	/**
	 * The AllJoyn interface name
	 */
	public static final String IF_NAME   = "org.alljoyn.Notification.Dismisser";
	
	/**
	 * The interface version
	 */
	public static final short  VERSION   = 1;
	
	/**
	 * The Dismiss signal to be sent 
	 * @param msgId The notification id the Dismiss signal belongs
	 * @param appId The application id of the Notification Sender
	 */
	@BusSignal(signature="iay", name="Dismiss")
	public void dismiss(int msgId, byte[] appId) throws BusException;
	
	
	/**
	 * @return The interface version
	 * @throws BusException
	 */
	@BusProperty(signature="q")
	public short getVersion() throws BusException;
}