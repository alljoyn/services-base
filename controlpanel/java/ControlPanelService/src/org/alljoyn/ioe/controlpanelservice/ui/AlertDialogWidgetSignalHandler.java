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

package org.alljoyn.ioe.controlpanelservice.ui;

import java.util.Map;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.Variant;
import org.alljoyn.ioe.controlpanelservice.ControlPanelException;
import org.alljoyn.ioe.controlpanelservice.communication.TaskManager;
import org.alljoyn.ioe.controlpanelservice.communication.interfaces.AlertDialog;
import org.alljoyn.ioe.controlpanelservice.communication.interfaces.AlertDialogSecured;

import android.util.Log;

public class AlertDialogWidgetSignalHandler implements AlertDialog, AlertDialogSecured {
    private static final String TAG = "cpan" + AlertDialogWidgetSignalHandler.class.getSimpleName();

    /**
     * AlertDialog to be notified about signal receiving
     */
    private AlertDialogWidget alertDialog;

    /**
     * @param alertDialog
     */
    public AlertDialogWidgetSignalHandler(AlertDialogWidget alertDialog) {
        this.alertDialog = alertDialog;
    }

    @Override
    public short getVersion() throws BusException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int getStates() throws BusException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public Map<Short, Variant> getOptParams() throws BusException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String getMessage() throws BusException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public short getNumActions() throws BusException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void Action1() throws BusException {
        // TODO Auto-generated method stub

    }

    @Override
    public void Action2() throws BusException {
        // TODO Auto-generated method stub

    }

    @Override
    public void Action3()  throws BusException {
        // TODO Auto-generated method stub

    }

    /**
     * Signal handler
     * @see org.alljoyn.ioe.controlpanelservice.communication.interfaces.AlertDialog#MetadataChanged()
     */
    @Override
    public void MetadataChanged() throws BusException {

        String msg = "Device: '" + alertDialog.device.getDeviceId() +
                     "', AlertDialogWidget: '" + alertDialog.objectPath + "', received METADATA_CHANGED signal";

        Log.d(TAG, msg);

        final ControlPanelEventsListener eventsListener = alertDialog.controlPanel.getEventsListener();
        try {
            alertDialog.refreshProperties();
        } catch (ControlPanelException cpe) {
            msg += ", but failed to refresh the Container properties";
            Log.e(TAG, msg);
            eventsListener.errorOccurred(alertDialog.controlPanel, msg);
            return;
        }

        //Delegate to the listener on a separate thread
        TaskManager.getInstance().execute(new Runnable() {
            @Override
            public void run() {
                eventsListener.metadataChanged(alertDialog.controlPanel, alertDialog);
            }
        });
    }//MetadataChanged

}