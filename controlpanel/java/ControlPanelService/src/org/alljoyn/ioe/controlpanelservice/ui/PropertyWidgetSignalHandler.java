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
import org.alljoyn.ioe.controlpanelservice.communication.interfaces.PropertyControl;
import org.alljoyn.ioe.controlpanelservice.communication.interfaces.PropertyControlSecured;

import android.util.Log;

/**
 * The class is a signal handler that is responsible to receive signals of PropertyControl interface. <br>
 *  Signals: <br>
 *  - ValueChanged    <br>
 *  - MetadataChanged
 */
public class PropertyWidgetSignalHandler implements PropertyControlSecured, PropertyControl {
	private static final String TAG = "cpan" + PropertyWidgetSignalHandler.class.getSimpleName();

	
	/**
	 * Property widget to be notified about signal receiving
	 */
	private PropertyWidget propertyWidget;

	/**
	 * @param propertyWidget
	 */
	public PropertyWidgetSignalHandler(PropertyWidget propertyWidget) {
		this.propertyWidget = propertyWidget;
	}

	/**
	 * @see org.alljoyn.ioe.controlpanelservice.communication.interfaces.PropertyControl#getVersion()
	 */
	@Override
	public short getVersion() throws BusException {
		return 0;
	}

	/**
	 * @see org.alljoyn.ioe.controlpanelservice.communication.interfaces.PropertyControl#getValue()
	 */
	@Override
	public Variant getValue() throws BusException {
		return null;
	}

	/**
	 * @see org.alljoyn.ioe.controlpanelservice.communication.interfaces.PropertyControl#setValue(org.alljoyn.bus.Variant)
	 */
	@Override
	public void setValue(Variant value) throws BusException {
	}


	@Override
	public int getStates() throws BusException {
		return 0;
	}

	@Override
	public Map<Short, Variant> getOptParams() throws BusException {
		return null;
	}
	

	/**
	 * @see org.alljoyn.ioe.controlpanelservice.communication.interfaces.PropertyControl#ValueChanged(org.alljoyn.bus.Variant)
	 */
	@Override
	public void ValueChanged(Variant value) throws BusException {
		
	    ControlPanelEventsListener eventsListener = propertyWidget.controlPanel.getEventsListener();
	    
	    String msg = "Device: '" + propertyWidget.device.getDeviceId() +
                "', PropertyWidget: '" + propertyWidget.objectPath + "', received VALUE_CHANGED signal";
    
	    Log.d(TAG, msg);
	    
	    try {
			Object unmarshVal = propertyWidget.unmarshalCurrentValue(value);
			Log.d(TAG, "The new property: '" + propertyWidget.objectPath + "' value is: '" + unmarshVal +  "'");
			eventsListener.valueChanged(propertyWidget.controlPanel, propertyWidget, unmarshVal);
		} catch (ControlPanelException cpe) {
			msg += ", but failed to unmarshal the received data, Error: '" + cpe.getMessage() + "'";
			Log.e(TAG, msg);
			eventsListener.errorOccurred(propertyWidget.controlPanel, msg);
		}
	}//ValueChanged

	/**
	 * @see org.alljoyn.ioe.controlpanelservice.communication.interfaces.PropertyControl#MetadataChanged()
	 */
	@Override
	public void MetadataChanged() throws BusException {
        
        String msg = "Device: '" + propertyWidget.device.getDeviceId() +
                "', PropertyWidget: '" + propertyWidget.objectPath + "', received METADATA_CHANGED signal";

        Log.d(TAG, msg);
        
        final ControlPanelEventsListener eventsListener = propertyWidget.controlPanel.getEventsListener();
        
        try {
        	propertyWidget.refreshProperties();
        }
        catch(ControlPanelException cpe) {
        	 msg += ", but failed to refresh the widget properties";
        	 Log.e(TAG, msg);
             eventsListener.errorOccurred(propertyWidget.controlPanel, msg);
             return;
        }
        
        //Delegate to the listener on a separate thread
        TaskManager.getInstance().execute( new Runnable() {
			@Override
			public void run() {
				eventsListener.metadataChanged(propertyWidget.controlPanel, propertyWidget);
			}
		});
	}//MetadataChanged
	
}