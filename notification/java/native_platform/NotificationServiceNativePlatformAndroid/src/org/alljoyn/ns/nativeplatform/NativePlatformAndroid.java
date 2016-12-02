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

package org.alljoyn.ns.nativeplatform;

import org.alljoyn.ns.commons.NativePlatform;
import org.alljoyn.ns.commons.NativePlatformAbstrImpl;

/**
 * Android implementation of the {@link NativePlatform} interface
 */
public class NativePlatformAndroid extends NativePlatformAbstrImpl {
	
	/**
	 * Constructor
	 */
	public NativePlatformAndroid() {
		super();
	}

	/**
	 * @see org.alljoyn.ns.commons.NativePlatformAbstrImpl#createLogger()
	 */
	@Override
	protected void createLogger() {
		logger = new AndroidLogger();
	}//createLogger
	
}