/******************************************************************************
* * 
*    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
*    Source Project Contributors and others.
*    
*    All rights reserved. This program and the accompanying materials are
*    made available under the terms of the Apache License, Version 2.0
*    which accompanies this distribution, and is available at
*    http://www.apache.org/licenses/LICENSE-2.0

******************************************************************************/
package org.alljoyn.ioe.onboardingtest;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**
 * This BroadcastReceiver reacts to a BOOT_COMPLETED intent, starting the service after device boot.
 */
public class BootReceiver extends BroadcastReceiver
{
	public BootReceiver()
	{
	}

	/* (non-Javadoc)
	 * @see android.content.BroadcastReceiver#onReceive(android.content.Context, android.content.Intent)
	 */
	@Override
	public void onReceive(Context context, Intent intent)
	{
		if("android.intent.action.BOOT_COMPLETED".equalsIgnoreCase(intent.getAction()))
		{
			Log.d("BootReceiver", "onReceive(android.intent.action.BOOT_COMPLETED). starting OnboardingService");
	        Intent serviceIntent = new Intent(context, OnboardingServer.class);
	        context.startService(serviceIntent); 
		}
	}
}