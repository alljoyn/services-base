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

package org.alljoyn.ns.sampleapp.controlpanel;

import org.alljoyn.ioe.controlpanelservice.ControlPanelException;
import org.alljoyn.ioe.controlpanelservice.ui.DeviceControlPanel;

import android.os.AsyncTask;

/**
 * Calls {@link DeviceControlPanel#getRootElement(org.alljoyn.ioe.controlpanelservice.ui.ControlPanelEventsListener)}
 */
public class RootElementRequest extends AsyncTask<DeviceControlPanel, Void, Object> {

	/**
	 * Control Panel manager
	 */
	private final ControlPanelManager manager;

	/**
	 * Constructor
	 */
	public RootElementRequest(ControlPanelManager manager) {
		super();
		this.manager = manager;
	}

	/**
	 * @see android.os.AsyncTask#doInBackground(java.lang.Object[])
	 */
	@Override
	protected Object doInBackground(DeviceControlPanel... controlPanel) {
		
		try {
			return controlPanel[0].getRootElement(manager);
		} catch (ControlPanelException cpe) {
			return cpe;
		}
	}//doInBackground

	/**
	 * @see android.os.AsyncTask#onPostExecute(java.lang.Object)
	 */
	@Override
	protected void onPostExecute(Object result) {
		manager.onReadyRootElementRequest(result);
	}//onPostExecute
	
}