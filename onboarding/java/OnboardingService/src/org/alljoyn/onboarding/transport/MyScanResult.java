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

package org.alljoyn.onboarding.transport;

import org.alljoyn.bus.annotation.Position;
import org.alljoyn.onboarding.OnboardingService.AuthType;

/**
 * A struct that represents a WiFi scan result: SSID and authType.
 */
public class MyScanResult
{
	/**
	 * The personal AP SSID
	 */
    @Position(0) public String m_ssid;
    
    /**
     * The personal AP authentication type
     * -3 - wpa2_auto, onboardee should try WPA2_TKIP and WPA2_CCMP
	 * -2 - wpa_auto, onboardee should try WPA_TKIP and WPA_CCMP
	 * -1 - any, The onboardee chooses the authentication
	 * 0 - Open Personal AP is open 
	 * 1 - WEP
	 * 2 - WPA_TKIP
	 * 3 - WPA_CCMP
	 * 4 - WPA2_TKIP
	 * 5 - WPA2_CCMP
	 * 6 - WPS
     * @see {@link AuthType}
     */
    @Position(1) public short m_authType;
}