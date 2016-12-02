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

package org.alljoyn.services.android.utils;


import org.alljoyn.common.GenericLogger;

import android.util.Log;

/**
 * A default GenericLogger implementation for Android
 */
public class AndroidLogger implements GenericLogger {

	/**
	 * @see GenericLogger#debug(java.lang.String, java.lang.String)
	 */
	@Override
	public void debug(String TAG, String msg) {
		Log.d(TAG, msg);
	}

	/**
	 * @see GenericLogger#info(java.lang.String, java.lang.String)
	 */
	@Override
	public void info(String TAG, String msg) {
		Log.i(TAG, msg);
	}

	/**
	 * @see GenericLogger#warn(java.lang.String, java.lang.String)
	 */
	@Override
	public void warn(String TAG, String msg) {
		Log.w(TAG, msg);
	}

	/**
	 * @see GenericLogger#error(java.lang.String, java.lang.String)
	 */
	@Override
	public void error(String TAG, String msg) {
		Log.e(TAG, msg);
	}

	/**
	 * @see GenericLogger#fatal(java.lang.String, java.lang.String)
	 */
	@Override
	public void fatal(String TAG, String msg) {
		Log.wtf(TAG, msg);
	}
}