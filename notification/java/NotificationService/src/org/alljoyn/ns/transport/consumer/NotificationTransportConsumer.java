/******************************************************************************
 *  *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

package org.alljoyn.ns.transport.consumer;

import java.util.Map;

import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.Variant;
import org.alljoyn.ns.NotificationServiceException;
import org.alljoyn.ns.PayloadAdapter;
import org.alljoyn.ns.commons.GenericLogger;
import org.alljoyn.ns.commons.NativePlatformFactory;
import org.alljoyn.ns.commons.NativePlatformFactoryException;
import org.alljoyn.ns.transport.Transport;
import org.alljoyn.ns.transport.TransportNotificationText;
import org.alljoyn.ns.transport.interfaces.NotificationTransport;


/**
 * The class is used to receive AllJoyn notify session-less-signals
 */
class NotificationTransportConsumer implements NotificationTransport {
    private static final String TAG = "ioe" + NotificationTransportConsumer.class.getSimpleName();

    /**
     * Identify object that receives signals arrived from producer
     */
    public static final String FROM_PRODUCER_RECEIVER_PATH = "/producerReceiver";

    /**
     * The service path identifying the object
     */
    private String servicePath;

    /**
     * The object service path
     * @param objServicePath
     */
    public NotificationTransportConsumer(String objServicePath) {
        servicePath = objServicePath;
    }

    /**
     * Return the object service path
     * @return
     */
    public String getServicePath() {
        return servicePath;
    }

    /**
     * This method will be called by the AJ bus when a notification is received
     * @see org.alljoyn.ns.transport.interfaces.NotificationTransport#notify(int, int, short, String, String, byte[], String, Map, Map, TransportNotificationText[])
     */
    @Override
    public void notify(int version, int msgId, short messageType, String deviceId, String deviceName, byte[] appId, String appName, Map<Integer, Variant> attributes, Map<String, String> customAttributes, TransportNotificationText[] text) {

        Transport transport = Transport.getInstance();
        BusAttachment busAttachment = transport.getBusAttachment();
        busAttachment.enableConcurrentCallbacks();

        try {

            GenericLogger logger = NativePlatformFactory.getPlatformObject().getNativeLogger();

            try {
                String sender = busAttachment.getMessageContext().sender;
                logger.debug(TAG, "Received notification from: '" + sender + "' by '" + servicePath + "' object, notification id: '" + msgId + "', handling");

                logger.debug(TAG, "Forwarding the received notification id: '" + msgId + "' to PayloadAdapter");
                PayloadAdapter.receivePayload(version, msgId, sender, messageType, deviceId, deviceName, appId, appName, attributes, customAttributes, text);
            } catch (NotificationServiceException nse) {
                logger.error(TAG, "Failed to read the received notification, Error: " + nse.getMessage());
            }
        } catch (NativePlatformFactoryException npfe) {
            System.out.println(TAG + ": Unexpected error occurred: " + npfe.getMessage());
        }
    }//notify

}