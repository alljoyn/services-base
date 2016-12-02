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

/**
 * The exception is thrown if a {@link NotificationService} failure has occurred 
 */
public class NotificationServiceException extends Exception {
	private static final long serialVersionUID = -4596126694105018269L;

	public NotificationServiceException() {
		super();
	}

	public NotificationServiceException(String message, Throwable throwable) {
		super(message, throwable);
	}

	public NotificationServiceException(String message) {
		super(message);
	}

	public NotificationServiceException(Throwable throwable) {
		super(throwable);
	}
}