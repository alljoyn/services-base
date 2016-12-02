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

package org.alljoyn.ns.commons;

/**
 * The exception is thrown if there was a failure in loading and instantiating the class 
 * implementing platform dependent logic
 */
public class NativePlatformFactoryException extends Exception {

	/**
	 * Serializable id
	 */
	private static final long serialVersionUID = 8554726459608620712L;

	public NativePlatformFactoryException() {
		super();
	}

	public NativePlatformFactoryException(String message, Throwable throwable) {
		super(message, throwable);
	}

	public NativePlatformFactoryException(String message) {
		super(message);
	}

	public NativePlatformFactoryException(Throwable throwable) {
		super(throwable);
	}
	
}