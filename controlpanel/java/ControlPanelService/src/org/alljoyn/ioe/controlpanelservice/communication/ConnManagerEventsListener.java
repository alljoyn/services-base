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

package org.alljoyn.ioe.controlpanelservice.communication;

import java.util.Map;

/**
 * Used to notify observers about the connection manager events
 */
public interface ConnManagerEventsListener {
	
	/**
	 * The method is launched when the event happened in ConnectionManager
	 * @param eventType The type of the event
	 * @param args
	 */
	public void connMgrEventOccured(ConnManagerEventType eventType, Map<String, Object> args);
}	