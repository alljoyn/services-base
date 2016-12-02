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

package org.alljoyn.ns.transport.consumer;

import java.util.UUID;

import org.alljoyn.bus.BusException;
import org.alljoyn.ns.NotificationServiceException;
import org.alljoyn.ns.PayloadAdapter;
import org.alljoyn.ns.commons.GenericLogger;
import org.alljoyn.ns.transport.Transport;
import org.alljoyn.ns.transport.interfaces.NotificationDismisser;

/**
 * The class provides a functionality of receiving AllJoyn Dismiss session-less-signals
 */
class DismissConsumer implements NotificationDismisser {
    private static final String TAG = "ioe" + DismissConsumer.class.getSimpleName();

    public static final String OBJ_PATH = "/dismissReceiver";

    /**
     * @see org.alljoyn.ns.transport.interfaces.NotificationDismisser#dismiss(int, byte[])
     */
    @Override
    public void dismiss(int msgId, byte[] bAppId) throws BusException {

        Transport transport = Transport.getInstance();
        transport.getBusAttachment().enableConcurrentCallbacks();

        GenericLogger logger;

        try {
            logger = transport.getLogger();
        } catch (NotificationServiceException nse) {
            System.out.println(TAG + ": Unexpected error occurred: " + nse.getMessage());
            return;
        }

        UUID appId = PayloadAdapter.byteArrayToUUID(bAppId);
        if (appId == null) {
            logger.error(TAG, "Received the Dismiss signal for the notifId: '" + msgId + "' with an invalid ApplicationId");
            return;
        }

        logger.debug(TAG, "Received a dismiss signal for notifId: '" + msgId + "', from appId: '" + appId + "', handling...");
        transport.onReceivedNotificationDismiss(msgId, appId);
    }//dismiss

    /**
     * @see org.alljoyn.ns.transport.interfaces.NotificationDismisser#getVersion()
     */
    @Override
    public short getVersion() throws BusException {
        return VERSION;
    }//getVersion

}