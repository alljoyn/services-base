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

package org.alljoyn.onboarding.test;

import java.util.List;

import android.net.wifi.ScanResult;


/**
 * Interface used to allow the creator of a IskWifiManager to run some code when a network scan is completed.
 */
public interface WifiManagerListener 
{
	/**
	 * This method will be invoked when a network scan is completed.
	 * @param results The result of the scan that has completed.
	 */
	public void OnScanResultComplete(List<ScanResult> results);
}