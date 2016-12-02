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
 * Implement this interface to provide platform dependent functionality for the Notification service
 */
public interface NativePlatform {
	
	/**
	 * Returns reference to GenericLogger object
	 */
	public GenericLogger getNativeLogger();
	
	/**
	 * Set a GenericLogger object
	 * @param logger
	 */
	public void setNativeLogger(GenericLogger logger);
}