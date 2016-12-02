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

package org.alljoyn.ns.transport.interfaces;

import java.util.Map;

import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.Variant;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusSignal;
import org.alljoyn.ns.transport.TransportNotificationText;

/**
 * The interface used to send and receive "session less" notification signals
 */
@BusInterface(name = NotificationTransport.IF_NAME, announced = "true")
public interface NotificationTransport extends BusObject
{

    /**
     * AllJoyn interface name
     */
    public static final String IF_NAME = "org.alljoyn.Notification";

    /**
     * The interface version
     */
    public static final short VERSION = 1;

    /**
     * Use the method to send or receive AJ session less signal
     * 
     * @param version
     *            The version of the message signature
     * @param msgId
     *            The notification message id
     * @param messageType
     *            Notification message type id
     * @param deviceId
     *            Device Id
     * @param deviceName
     *            Device Name
     * @param appId
     *            App Id
     * @param appName
     *            App Name
     * @param attributes
     *            Attributes key-value pair
     * @param customAttributes
     *            customAttributes
     * @param text
     *            Array of NotificationText objects
     */
    @BusSignal(signature = "qiqssaysa{iv}a{ss}ar")
    public void notify(int version, int msgId, short messageType, String deviceId, String deviceName, byte[] appId, String appName, Map<Integer, Variant> attributes,
            Map<String, String> customAttributes, TransportNotificationText[] text);
}