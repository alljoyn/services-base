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
/**
 * The package includes classes that represent the UI widgets and its metadata<br>
 * The UI widget classes are used to receive the initial values that were set by the remotely controllable device. <br>
 * In addition these classes used to perform the remote control of the device.
 * For example: <br>
 * {@link org.alljoyn.ioe.controlpanelservice.ui.ActionWidget#exec()} is used to invoke an action 
 * on the remote device <br>
 * {@link org.alljoyn.ioe.controlpanelservice.ui.PropertyWidget#setCurrentValue(Object)} is used to set a
 * new property value on the remote device <br>  
 * {@link org.alljoyn.ioe.controlpanelservice.ui.PropertyWidget#getCurrentValue()} is used to receive the current
 * property value <br>
 * Each UI widget has its signal handler to be notified about the state changes in the remote device.
 * The user receives these notifications by implementing the {@link org.alljoyn.ioe.controlpanelservice.ui.ControlPanelEventsListener} interface <br>
 * The signal {@link org.alljoyn.ioe.controlpanelservice.ui.ControlPanelEventsListener#metadataChanged(DeviceControlPanel, UIElement)}
 * notifies about a possible change in a widget UI state. <br>
 * The method refreshProperties, i.e: {@link org.alljoyn.ioe.controlpanelservice.ui.PropertyWidget#refreshProperties()}
 * is used to receive the updated state of a Property 
 */
package org.alljoyn.ioe.controlpanelservice.ui;