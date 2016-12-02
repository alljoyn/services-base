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

package org.alljoyn.ioe.controlpanelservice;

import java.util.Collection;

import org.alljoyn.ioe.controlpanelservice.ui.DeviceControlPanel;

/**
 *  The interface to be implemented in order to receive {@link ControllableDevice} relevant events
 */
public interface DeviceEventsListener {

    /**
     * Notify the interface implementer about session establishment with the remote device
     * @param device The {@link ControllableDevice}
     * @param controlPanelContainer Used to request the Root UI Container and a relevant to it information
     * @see DeviceControlPanel
     */
    public void sessionEstablished(ControllableDevice device, Collection<ControlPanelCollection> controlPanelContainer);

    /**
     * Notify the interface implementer about loosing session with the remote device
     * @param device The {@link ControllableDevice} where the session has lost
     */
    public void sessionLost(ControllableDevice device);

    /**
     * Notify the interface implementer about an error in the device activities
     * @param device The {@link ControllableDevice} where the error has occurred
     * @param reason The error reason
     */
    public void errorOccurred(ControllableDevice device, String reason);
}