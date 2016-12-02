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

package org.alljoyn.ioe.controlpanelservice.ui;

import java.util.Map;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.Variant;
import org.alljoyn.ioe.controlpanelservice.ControlPanelException;
import org.alljoyn.ioe.controlpanelservice.communication.TaskManager;
import org.alljoyn.ioe.controlpanelservice.communication.interfaces.Container;
import org.alljoyn.ioe.controlpanelservice.communication.interfaces.ContainerSecured;

import android.util.Log;

/**
 * The class is a signal handler that is responsible to receive signals of UIContainer interface. <br>
 *  Signals: <br>
 *  - MetadataChanged
 */
public class ContainerWidgetSignalHandler implements Container, ContainerSecured {
	private static final String TAG = "cpan" + ContainerWidgetSignalHandler.class.getSimpleName();
	
	
	/**
	 * Container widget to be notified about signal receiving
	 */
	private ContainerWidget containerWidget;
	
	/**
	 * Constructor
	 * @param containerWidget
	 */
	public ContainerWidgetSignalHandler(ContainerWidget containerWidget) {
		this.containerWidget = containerWidget;
	}

	/**
	 * @see org.alljoyn.ioe.controlpanelservice.communication.interfaces.Container#getVersion()
	 */
	@Override
	public short getVersion() throws BusException {
		return 0;
	}

	@Override
	public int getStates() throws BusException {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public Map<Short, Variant> getOptParams() throws BusException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Signal handler
	 * @see org.alljoyn.ioe.controlpanelservice.communication.interfaces.Container#MetadataChanged()
	 */
	@Override
	public void MetadataChanged() throws BusException {
		
		String msg = "Device: '" + containerWidget.device.getDeviceId() + 
                "', ContainerWidget: '" + containerWidget.objectPath + "', received METADATA_CHANGED signal";
		
		Log.d(TAG, msg);
		
		final ControlPanelEventsListener eventsListener = containerWidget.controlPanel.getEventsListener();
		try {
			containerWidget.refreshProperties();
		} catch (ControlPanelException cpe) {
			msg += ", but failed to refresh the Container properties";
			Log.e(TAG, msg);
			eventsListener.errorOccurred(containerWidget.controlPanel, msg);
			return;
		}
		
		//Delegate to the listener on a separate thread
		TaskManager.getInstance().execute( new Runnable() {
			@Override
			public void run() {
				eventsListener.metadataChanged(containerWidget.controlPanel, containerWidget);
			}
		});
	}//MetadataChanged

}