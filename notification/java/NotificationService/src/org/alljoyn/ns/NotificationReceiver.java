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

package org.alljoyn.ns;

import java.util.UUID;

/**
 * An interface to be implemented in order to receive notification messages
 */
public interface NotificationReceiver {

	/**
	 * Receive a {@link Notification}
	 * @param notification
	 */
	public void receive(Notification notification);

	/**
	 * The appId with the msgId uniquely identifies the {@link Notification} that should be dismissed, since
	 * the msgId alone identifies the {@link Notification} only in the scope of the Notification sender service
	 * @param msgId The message id of the {@link Notification} that should be dismissed
	 * @param appId The appId of the Notification sender service
	 */
	public void dismiss(int msgId, UUID appId);
	
}//NotificationReceiver