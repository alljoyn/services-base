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
 * The abstract parent class for all the classes implementing platform dependent logic 
 */
public abstract class NativePlatformAbstrImpl implements NativePlatform {
	
	/**
	 * Reference to logger
	 */
	protected GenericLogger logger; 
	
	/**
	 * Constructor
	 */
	public NativePlatformAbstrImpl() {
		createLogger();
	}

	/**
	 * Creates and set logger object
	 */
	protected abstract void createLogger();
	
	/**
	 * @see org.alljoyn.ns.commons.NativePlatform#getNativeLogger()
	 */
	@Override
	public GenericLogger getNativeLogger() {
		return logger;
	}

	/**
	 * @see org.alljoyn.ns.commons.NativePlatform#setNativeLogger(org.alljoyn.ns.commons.GenericLogger)
	 */
	@Override
	public void setNativeLogger(GenericLogger logger) {
		this.logger = logger;
	}
}