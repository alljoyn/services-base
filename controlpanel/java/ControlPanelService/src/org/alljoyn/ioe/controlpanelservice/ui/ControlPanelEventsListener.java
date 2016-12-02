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


/**
 *  The interface to be implemented in order to receive {@link DeviceControlPanel} relevant events
 */
public interface ControlPanelEventsListener {
	
	/**
	 * Notify the interface implementer about the changed value in the {@link UIElement}
	 * @param panel {@link DeviceControlPanel} where the value has changed
	 * @param uielement The {@link UIElement} where the change has occurred
	 * @param newValue The new value
	 */
	public void valueChanged(DeviceControlPanel panel, UIElement uielement, Object newValue);
	
	/**
	 * Notify the interface implementer about the metadata changed in the {@link UIElement}
	 * @param panel {@link DeviceControlPanel} where the metadata has changed
	 * @param uielement The {@link UIElement} where the change has occurred
	 */
	public void metadataChanged(DeviceControlPanel panel, UIElement uielement);
	
	/**
	 * Dismiss the NotificationAction of the given {@link DeviceControlPanel} 
	 * @param panel The NotificationAction control panel to dismissed
	 */
	public void notificationActionDismiss(DeviceControlPanel panel);
	
	/**
	 * Notify the interface implementer about an error in the {@link DeviceControlPanel} activities
	 * @param panel {@link DeviceControlPanel} where the error has occurred
	 * @param reason The error reason
	 */
	public void errorOccurred(DeviceControlPanel panel, String reason);
	
}