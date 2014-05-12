/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
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

package org.alljoyn.ioe.controlpanelservice.application;

import org.alljoyn.about.AboutServiceImpl;
import org.alljoyn.bus.AuthListener;
import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusAttachment.RemoteMessage;
import org.alljoyn.bus.PasswordManager;
import org.alljoyn.bus.SessionOpts;
import org.alljoyn.bus.Status;
import org.alljoyn.bus.alljoyn.DaemonInit;
import org.alljoyn.ioe.controlpanelservice.ControlPanelException;
import org.alljoyn.ioe.controlpanelservice.ControlPanelService;

import android.app.Application;
import android.util.Log;

public class ControlPanelApplication extends Application {
	private static final String TAG = "cpan" + ControlPanelApplication.class.getSimpleName(); 
	
	private class SrpKeyXListener implements AuthListener {
		
        /* Returns the list of supported mechanisms. */
        public String getMechanisms() {
            return "ALLJOYN_PIN_KEYX ALLJOYN_SRP_KEYX";
        }
		
        /**
         * Persistent authentication and encryption data is stored at this location.  
         * 
         * This uses the private file area associated with the application package.
         */
        public String getKeyStoreFileName() {
            return getFileStreamPath("alljoyn_keystore").getAbsolutePath();
        }//getKeyStoreFileName
        
        /**
         * @see org.alljoyn.bus.AuthListener#requested(java.lang.String, java.lang.String, int, java.lang.String, org.alljoyn.bus.AuthListener.AuthRequest[])
         */
		@Override
		public boolean requested(String mechanism, String peerName, int count, String userName, AuthListener.AuthRequest[] requests) {
			 Log.d(TAG, "AuthListener requested() called, checking the Auth mechanisms. Mechanism is: '" + mechanism + "'");
			 
			 if ( !getMechanisms().contains(mechanism) ) {
				 Log.w(TAG, "Recieved an unsupported Auth mechanism: '" + mechanism + "'");
				 return false;
			 }
			 
		     for (AuthRequest request : requests) {
                 if (request instanceof PasswordRequest) {
                	 Log.d(TAG, "Password request was found, setting it, returning TRUE");
                     ((PasswordRequest) request).setPassword(SECURED_SRP_PASSWORD.toCharArray());
                     return true;
                 }
             }//for :: AuthRequest
		     
		     Log.d(TAG, "Password request was NOT found, returning FALSE");
			 return false;
		}//requested

		/**
		 * @see org.alljoyn.bus.AuthListener#completed(java.lang.String, java.lang.String, boolean)
		 */
		@Override
		public void completed(String authMechanism, String authPeer, boolean authenticated) {
			if ( authenticated ) {
				Log.d(TAG, "The peer: '" + authPeer + "', authenticated successfully for authMechanism: '" + authMechanism + "'");
			}
			else {
				Log.w(TAG, "The peer: '" + authPeer + "', WAS NOT authenticated for authMechanism: '" + authMechanism + "'");
			}
		}//completed
		
	}
	
	// ================================================== //
	
	/**
	 * The daemon should advertise itself "quietly" (directly to the calling port)
	 * This is to reply directly to a TC looking for a daemon 
	 */
	private static final String DAEMON_NAME= "org.alljoyn.BusNode.IoeService";
	
	/**
	 * The daemon should advertise itself "quietly" (directly to the calling port)
	 * This is to reply directly to a TC looking for a daemon 
	 */
	private static final String DAEMON_QUIET_PREFIX      = "quiet@";
	
	private static final String DAEMON_CONN_PASSWORD     = "000000";
	
	private static final String SECURED_SRP_PASSWORD     = "000000";         // ABCDEFGH
		
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
		
		//bus.setDaemonDebug("ALL", 7);
		//bus.setLogLevels("ALLJOYN=7");
		//bus.setLogLevels("ALL=7");
		//bus.useOSLogging(true);
		
		//setting the password for the daemon to allow thin clients to connect
		Log.d(TAG, "Setting daemon password");
		Status pasStatus = PasswordManager.setCredentials("ALLJOYN_PIN_KEYX", DAEMON_CONN_PASSWORD);	
		if ( pasStatus != Status.OK ) {
			Log.e(TAG, "Failed to set password for daemon, Error: " + pasStatus);
		}
		
		Log.d(TAG, "Setting the AuthListener");
		
		SrpKeyXListener authListener = new SrpKeyXListener();
		Status status = bus.registerAuthListener(authListener.getMechanisms(), authListener, authListener.getKeyStoreFileName());
		
		if ( status != Status.OK ) {
			Log.e(TAG, "Failed to register AuthListener");
		}
		
		status = bus.connect();
		if ( status != Status.OK ) {
			Log.e(TAG, "Failed to connect bus attachment, bus: '" + status + "'");
			return;
		}
		
		//Advertise the daemon so that the thin client can find it
		advertiseDaemon();
		
		try {
			AboutServiceImpl.getInstance().startAboutClient(bus);
			service.init(bus, new ControlPanelTestApp());
		} catch (ControlPanelException cpe) {
			Log.e(TAG, "Failure happened, Error: '" + cpe.getMessage() + "'");
		}
		catch (Exception e) {
			Log.e(TAG, "Unexpected failure occurred, Error: '" + e.getMessage() + "'");
		}
	}//onCreate
		
	/**
	 * Advertise the daemon so that the thin client can find it
	 * @param logger
	 */
	private void advertiseDaemon() {
		//request the name	
		int flag = BusAttachment.ALLJOYN_REQUESTNAME_FLAG_DO_NOT_QUEUE;
		Status reqStatus = bus.requestName(DAEMON_NAME, flag);
		if (reqStatus == Status.OK) {
			//advertise the name with a quite prefix for TC to find it
			Status adStatus = bus.advertiseName(DAEMON_QUIET_PREFIX + DAEMON_NAME, SessionOpts.TRANSPORT_ANY);
			if (adStatus != Status.OK){
				bus.releaseName(DAEMON_NAME);
				Log.d(TAG, "failed to advertise daemon name " + DAEMON_NAME);
			}
			else{
				Log.d(TAG, "Succefully advertised daemon name " + DAEMON_NAME);
			}
		}	
	}

}
