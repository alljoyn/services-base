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
 * The package includes classes that allow to start the control panel service {@link org.alljoyn.ioe.controlpanelservice.ControlPanelService},
 * and manage the list of controllable devices {@link org.alljoyn.ioe.controlpanelservice.ControllableDevice}. <br>
 * The list of controllable devices is managed by the {@link org.alljoyn.ioe.controlpanelservice.DeviceRegistry} interface
 * and its default implementation {@link org.alljoyn.ioe.controlpanelservice.DefaultDeviceRegistry} <br>
 * For each controllable device there is a list of the device control panels {@link org.alljoyn.ioe.controlpanelservice.ui.DeviceControlPanel}
 * that is passed to the user when {@link org.alljoyn.ioe.controlpanelservice.ControllableDevice#startSession(DeviceEventsListener)}
 * is called. By using the {@link org.alljoyn.ioe.controlpanelservice.ui.DeviceControlPanel} a user may call
 * {@link org.alljoyn.ioe.controlpanelservice.ui.DeviceControlPanel#getRootElement(org.alljoyn.ioe.controlpanelservice.ui.ControlPanelEventsListener)} to retrieve the root
 * UI container
 */
package org.alljoyn.ioe.controlpanelservice;