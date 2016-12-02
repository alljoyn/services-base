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

package org.alljoyn.ns.transport.producer;

import java.util.EnumMap;
import java.util.Map;

import org.alljoyn.bus.Variant;
import org.alljoyn.ns.Notification;
import org.alljoyn.ns.NotificationMessageType;
import org.alljoyn.ns.transport.TransportNotificationText;
import org.alljoyn.ns.transport.interfaces.NotificationTransport;

/** 
 * The class is used to send {@link Notification} session less signals
 */
class NotificationTransportProducer implements NotificationTransport {

	/**
	 * Stores AJ BusObject identifiers
	 * Maps from Message type to ServicePath string
	 */
	private static Map<NotificationMessageType, String> servicePath;


	static {
		servicePath = new EnumMap<NotificationMessageType, String>(NotificationMessageType.class);
		servicePath.put(NotificationMessageType.EMERGENCY, "/emergency");
		servicePath.put(NotificationMessageType.WARNING, "/warning");
		servicePath.put(NotificationMessageType.INFO, "/info");
	}
	
	/**
	 * Returns the servicePath for each of the transport objects
	 * @return
	 */
	public static Map<NotificationMessageType, String> getServicePath() {
		return servicePath;
	}
	
	/**
	 * Use this method to send session less signals
	 * @see org.alljoyn.ns.transport.interfaces.NotificationTransport#notify(int, int, short, String, String, byte[], String, Map, Map, TransportNotificationText[])
	 */
	@Override
	public void notify(int version, int msgId,
			short messageType, String deviceId, String deviceName, byte[] appId, String appName,
			Map<Integer, Variant> attributes,
			Map<String, String> customAttributes,
			TransportNotificationText[] text) {}
}