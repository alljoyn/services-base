/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

package org.alljoyn.onboarding.client;

import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusException;
import org.alljoyn.bus.ProxyBusObject;
import org.alljoyn.onboarding.transport.OBLastError;
import org.alljoyn.onboarding.transport.OnboardingTransport;
import org.alljoyn.onboarding.transport.ScanInfo;
import org.alljoyn.services.common.ClientBaseImpl;
import org.alljoyn.services.common.ServiceAvailabilityListener;

/**
 * A default implementation of the OnboardingClient interface
 */ 
public class OnboardingClientImpl extends ClientBaseImpl implements OnboardingClient
{
	public final static String TAG = OnboardingClient.class.getName();
	
	public OnboardingClientImpl(String m_deviceName, BusAttachment bus, ServiceAvailabilityListener serviceAvailabilityListener, short port)
	{
		super(m_deviceName, bus, serviceAvailabilityListener,OnboardingTransport.OBJ_PATH, OnboardingTransport.class, port);
	}

	@Override
	public short getVersion() throws BusException {
		ProxyBusObject proxyObj = getProxyObject();
		/* We make calls to the methods of the AllJoyn object through one of its interfaces. */
		OnboardingTransport onboardingTransport =  proxyObj.getInterface(OnboardingTransport.class);
		return onboardingTransport.getVersion();
	}

	@Override
	public void connectWiFi() throws BusException
	{
		ProxyBusObject proxyObj = getProxyObject();
		// We make calls to the methods of the AllJoyn object through one of its interfaces. 
		OnboardingTransport onboardingTransport =  proxyObj.getInterface(OnboardingTransport.class);
		onboardingTransport.Connect();
	}

	@Override
	public short configureWiFi(String ssid, String passphrase, short authType) throws BusException
	{
		ProxyBusObject proxyObj = getProxyObject();
		// We make calls to the methods of the AllJoyn object through one of its interfaces. 
		OnboardingTransport onboardingTransport =  proxyObj.getInterface(OnboardingTransport.class);
		return onboardingTransport.ConfigureWiFi(ssid, passphrase, authType);
	}

	@Override
	public void offboard() throws BusException {
		ProxyBusObject proxyObj = getProxyObject();
		// We make calls to the methods of the AllJoyn object through one of its interfaces. 
		OnboardingTransport onboardingTransport =  proxyObj.getInterface(OnboardingTransport.class);
		onboardingTransport.Offboard();
	}

	@Override
	public ScanInfo getScanInfo() throws BusException {
		ProxyBusObject proxyObj = getProxyObject();
		// We make calls to the methods of the AllJoyn object through one of its interfaces. 
		OnboardingTransport onboardingTransport =  proxyObj.getInterface(OnboardingTransport.class);
		return onboardingTransport.GetScanInfo();
	}

	@Override
	public short getState() throws BusException {
		ProxyBusObject proxyObj = getProxyObject();
		// We make calls to the methods of the AllJoyn object through one of its interfaces. 
		OnboardingTransport onboardingTransport =  proxyObj.getInterface(OnboardingTransport.class);
		return onboardingTransport.getState();
	}

	@Override
	public OBLastError GetLastError() throws BusException {
		ProxyBusObject proxyObj = getProxyObject();
		// We make calls to the methods of the AllJoyn object through one of its interfaces. 
		OnboardingTransport onboardingTransport =  proxyObj.getInterface(OnboardingTransport.class);
		return onboardingTransport.getLastError();
	}

}
