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

import java.util.Map;

/**
 * Used as a callback interface to inform the interface implementer about new discovered devices
 * and changes in their reachability state
 */
public interface DeviceRegistry {

    /**
     * Called when a new controllable device was found in the control panel service proximity <br>
     * @param device
     */
    public void foundNewDevice(ControllableDevice device);

    /**
     * Called when a controllable device left the control panel service proximity <br>
     * The method implementation should be thread safe
     * @param device The controllable device
     * @param isReachable Whether the device is in reachable state
     */
    public void reachabilityChanged(ControllableDevice device, boolean isReachable);

    /**
     * Remove a device from registry <br>
     * When a device is removed from the registry it will be inserted back after foundDevice method is called
     * @param device
     */
    public void removeDevice(ControllableDevice device);

    /**
     * @return Returns collection of controllable devices
     */
    public Map<String, ControllableDevice> getDevices();

}