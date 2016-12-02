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


import org.alljoyn.bus.BusException;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusMethod;
import org.alljoyn.bus.annotation.BusProperty;
import org.alljoyn.ns.Notification;

/**
 * The interface provides to a notification receiver the functionality to dismiss
 * the received {@link Notification}.
 */
@BusInterface(name = NotificationProducer.IFNAME)
public interface NotificationProducer extends BusObject {

    /**
     * The AllJoyn interface name
     */
    public static final String IFNAME = "org.alljoyn.Notification.Producer";

    /**
     * Notification producer object
     */
    public static final String OBJ_PATH = "/notificationProducer";

    /**
     * The interface version
     */
    public static final short VERSION = 1;

    /**
     * When the notification message is dismissed, it's first of all deleted and then a dismiss signal is sent
     * to all notification consumers to update them that the {@link Notification} has been dismissed
     * @param msgId The notification message identifier
     * @throws BusException
     */
    @BusMethod(name = "Dismiss", signature = "i")
    public void dismiss(int msgId) throws BusException;

    /**
     * @return The interface version
     * @throws BusException
     */
    @BusProperty(signature = "q")
    public short getVersion() throws BusException;

}