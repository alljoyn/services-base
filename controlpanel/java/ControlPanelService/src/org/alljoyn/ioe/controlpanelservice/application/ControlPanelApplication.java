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

package org.alljoyn.ioe.controlpanelservice.application;

import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusAttachment.RemoteMessage;
import org.alljoyn.bus.SessionOpts;
import org.alljoyn.bus.Status;
import org.alljoyn.bus.alljoyn.DaemonInit;
import org.alljoyn.ioe.controlpanelservice.ControlPanelException;
import org.alljoyn.ioe.controlpanelservice.ControlPanelService;
import org.alljoyn.services.android.security.AuthPasswordHandler;
import org.alljoyn.services.android.security.SrpAnonymousKeyListener;
import org.alljoyn.services.android.utils.AndroidLogger;

import android.app.Application;
import android.util.Log;

public class ControlPanelApplication extends Application implements AuthPasswordHandler {
    private static final String TAG = "cpan" + ControlPanelApplication.class.getSimpleName();

    /**
     * The daemon should advertise itself "quietly" (directly to the calling
     * port) This is to reply directly to a TC looking for a daemon
     */
    private static final String DAEMON_NAME = "org.alljoyn.BusNode.IoeService";

    /**
     * The daemon should advertise itself "quietly" (directly to the calling
     * port) This is to reply directly to a TC looking for a daemon
     */
    private static final String DAEMON_QUIET_PREFIX = "quiet@";

    private static final String[] authMechanisms = new String[] { "ALLJOYN_SRP_KEYX", "ALLJOYN_ECDHE_PSK" };

    static {
        System.loadLibrary("alljoyn_java");
    }

    private BusAttachment bus;

    private ControlPanelService service;

    @Override
    public void onCreate() {
        super.onCreate();
        service = ControlPanelService.getInstance();

        DaemonInit.PrepareDaemon(this);
        bus = new BusAttachment("ControlPanel", RemoteMessage.Receive);

        // bus.setDaemonDebug("ALL", 7);
        // bus.setLogLevels("ALLJOYN=7");
        // bus.setLogLevels("ALL=7");
        // bus.useOSLogging(true);

        Log.d(TAG, "Setting the AuthListener");

        SrpAnonymousKeyListener authListener = new SrpAnonymousKeyListener(this, new AndroidLogger(), authMechanisms);
        Status status = bus.registerAuthListener(authListener.getAuthMechanismsAsString(), authListener, getFileStreamPath("alljoyn_keystore").getAbsolutePath());

        if (status != Status.OK) {
            Log.e(TAG, "Failed to register AuthListener");
        }

        status = bus.connect();
        if (status != Status.OK) {
            Log.e(TAG, "Failed to connect bus attachment, bus: '" + status + "'");
            return;
        }

        // Advertise the daemon so that the thin client can find it
        advertiseDaemon();

        try {
            service.init(bus, new ControlPanelTestApp());
        } catch (ControlPanelException cpe) {
            Log.e(TAG, "Failure happened, Error: '" + cpe.getMessage() + "'");
        } catch (Exception e) {
            Log.e(TAG, "Unexpected failure occurred, Error: '" + e.getMessage() + "'");
        }
    }// onCreate

    /**
     * @see org.alljoyn.services.android.security.AuthPasswordHandler#completed(java.lang.String,
     *      java.lang.String, boolean)
     */
    @Override
    public void completed(String mechanism, String authPeer, boolean authenticated) {

        Log.d(TAG, "The peer: '" + authPeer + "' has been authenticated: '" + authenticated + "', mechanism: '" + mechanism + "'");
    }

    /**
     * @see org.alljoyn.services.android.security.AuthPasswordHandler#getPassword(java.lang.String)
     */
    @Override
    public char[] getPassword(String peerName) {

        return SrpAnonymousKeyListener.DEFAULT_PINCODE;
    }

    /**
     * Advertise the daemon so that the thin client can find it
     *
     * @param logger
     */
    private void advertiseDaemon() {
        // request the name
        int flag = BusAttachment.ALLJOYN_REQUESTNAME_FLAG_DO_NOT_QUEUE;
        Status reqStatus = bus.requestName(DAEMON_NAME, flag);
        if (reqStatus == Status.OK) {
            // advertise the name with a quite prefix for TC to find it
            Status adStatus = bus.advertiseName(DAEMON_QUIET_PREFIX + DAEMON_NAME, SessionOpts.TRANSPORT_ANY);
            if (adStatus != Status.OK) {
                bus.releaseName(DAEMON_NAME);
                Log.d(TAG, "failed to advertise daemon name " + DAEMON_NAME);
            } else {
                Log.d(TAG, "Succefully advertised daemon name " + DAEMON_NAME);
            }
        }
    }

}