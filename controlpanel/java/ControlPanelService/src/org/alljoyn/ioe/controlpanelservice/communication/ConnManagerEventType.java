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

/**
 * Connection Manager events 
 */
public enum ConnManagerEventType {
	ANNOUNCEMENT_RECEIVED,
	SESSION_JOINED,
	SESSION_JOIN_FAIL,
	SESSION_LOST,
	FOUND_DEVICE,
	LOST_DEVICE,
	;
}