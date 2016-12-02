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
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;

/**
 * This activity shows no UI. It serves the purpose of starting the MPQService by the user. As of Android 3.0,
 * Services cannot be started from BOOT_COMPLETE Intent unless app had previously been explicitly started by the user.
 */
public class DummyActivity extends Activity
{
	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
        Intent serviceIntent = new Intent(getApplicationContext(), OnboardingServer.class);
        getApplicationContext().startService(serviceIntent);
        // exit this activity after service was started
        finish();
	}
}