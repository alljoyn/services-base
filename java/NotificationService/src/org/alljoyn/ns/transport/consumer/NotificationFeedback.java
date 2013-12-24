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

package org.alljoyn.ns.transport.consumer;

import java.lang.reflect.Method;
import java.util.UUID;

import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusException;
import org.alljoyn.bus.ErrorReplyBusException;
import org.alljoyn.bus.OnJoinSessionListener;
import org.alljoyn.bus.ProxyBusObject;
import org.alljoyn.bus.SessionListener;
import org.alljoyn.bus.SessionOpts;
import org.alljoyn.bus.Status;
import org.alljoyn.ns.Notification;
import org.alljoyn.ns.NotificationServiceException;
import org.alljoyn.ns.commons.GenericLogger;
import org.alljoyn.ns.transport.DismissSender;
import org.alljoyn.ns.transport.Transport;
import org.alljoyn.ns.transport.interfaces.NotificationProducer;
import org.alljoyn.ns.transport.producer.SenderSessionListener;

/**
 * The class implements the functionality of sending feedback about the received {@link Notification}
 * There are two feedback types: <br>
 * 		1) acknowledge - To tell the Notification Producer to stop broadcasting the notification <br>
 * 		2) dismiss     - Dismiss includes the functionality of "acknowledge" and additionally tells the Notification
 * 					 	 Producer to send a Dismiss session-less-signal to update other Notification Consumers that 
 * 						 this {@link Notification} message has been dismissed. <br>
 * 
 * If there is a failure in reaching the Notification Producer to dismiss the {@link Notification}, the dismiss 
 * session-less-signal is sent by {@link NotificationFeedback} 
 * 
 */
public class NotificationFeedback extends OnJoinSessionListener {
	private static final String TAG = "ioe" + NotificationFeedback.class.getSimpleName();
	
	private class NFSessionListener extends SessionListener {
		
		/**
		 * @see org.alljoyn.bus.SessionListener#sessionLost(int, int)
		 */
		@Override
		public void sessionLost(int sessionId, int reason) {
			if ( logger != null ) {
				logger.debug(TAG, "Received session lost for sid: '" + sessionId + "', reason: '" + reason + "'");
			}
		}//sessionLest
	}
	
	//============================================//
	
	/**
	 * The logger
	 */
	private GenericLogger logger;

	/**
	 * Session target
	 */
	private final String origSender;
	
	/**
	 * The notification id
	 */
	private final int notifId;
	
	/**
	 * The application 
	 */
	private final UUID appId;
	
	/**
	 * The established session id
	 */
	private Integer sessionId = null; 
	
	/**
	 * Notification version
	 */
	private int version;
	
	/**
	 * Constructor
	 * @param notification The {@link Notification} to send feedback
	 * @throws NotificationServiceException If failed to create the {@link NotificationFeedback}
	 */
	public NotificationFeedback(Notification notification) throws NotificationServiceException {
		super();
		logger = Transport.getInstance().getLogger();
		
		version    = notification.getVersion();
		origSender = notification.getOriginalSenderBusName();
		notifId    = notification.getMessageId();
		appId      = notification.getAppId();
	}
	
	/**
	 * Call the acknowledge
	 */
	public void acknowledge() {
		
		//Version less than 2 means that the Notification sender doesn't support the NotificationProducer interface 
		if ( version < 2 || origSender == null ) {
			logger.debug(TAG, "The notification sender doesn't support the NotificationProducer interface, version:'" + version + "', notifId: '" + notifId + "' can't be acknowledged");
			return;
		}
		
		Method invokeAckMethod = getMethod("invokeAck", Status.class);
		if ( invokeAckMethod == null ) {
			return;
		}
		
		//Asynchronously establish the session 
		Status status = establishSession(invokeAckMethod);
		logger.debug(TAG, "establishSession was called, Status: '" + status + "'");
		
		//If the session is already joined call acknowledge on the remote peer 
		if ( status == Status.ALLJOYN_JOINSESSION_REPLY_ALREADY_JOINED ) {
			invokeAck(status);
		}
	}//acknowledge
	
	/**
	 * Call the dismiss
	 */
	public void dismiss() {
		
		//Version less than 2 means that the Notification sender doesn't support the NotificationProducer interface
		if ( version < 2 || origSender == null ) {
			logger.debug(TAG, "The notification sender doesn't support the NotificationProducer interface, version: '" +
		                       version + "', appId: '" + appId + "', notifId '" + notifId + "', sending the Dismiss signal");
			
			spawnThreadSendDismissSignal();
			return;
		}
		
		Method invokeDismissMethod = getMethod("invokeDismiss", Status.class);
		if ( invokeDismissMethod == null ) {
			return;
		}
		
		//Asynchronously establish the session 
		Status status = establishSession(invokeDismissMethod);
		logger.debug(TAG, "The establishSession was called, Status: '" + status + "'");
		
		if ( status != Status.OK ) {
			
			//If the session is already joined call dismiss on the remote peer 
			if ( status == Status.ALLJOYN_JOINSESSION_REPLY_ALREADY_JOINED  ) {
				invokeDismiss(status);
			}
			else {
				logger.warn(TAG, "Failed to establish session with the Notification sender, senging Dismiss signal, Error: '" + status + "'");
				spawnThreadSendDismissSignal();
			}
		}
		
	}//dismiss
	
	/**
	 * @see org.alljoyn.bus.OnJoinSessionListener#onJoinSession(org.alljoyn.bus.Status, int, org.alljoyn.bus.SessionOpts, java.lang.Object)
	 */
	@Override
	public void onJoinSession(Status status, int sessionId, SessionOpts opts, Object context) {
		logger.debug(TAG, "Received sessionJoin callback, status: '" + status + "', sid: '" + sessionId + "'");
		
		if ( status == Status.OK  || status == Status.ALLJOYN_JOINSESSION_REPLY_ALREADY_JOINED ) {
			this.sessionId = sessionId;
		}
		
		if ( !(context instanceof Method) ) {
			logger.warn(TAG, "sessionJoin context isn't a Method");
			return;
		}
		
		Method method = (Method) context;
		try {
			method.invoke(this, status);
		} catch (Exception e) {
			logger.error(TAG, "Failed to invoke the method: '" + method.getName() + "'");
		}
	}//onJoinSession

	/**
	 * Calls to establish a session with a NotificationProducer
	 * @param callbackMethod The method to be invoked once the session is established
	 * @return Status The synchronous session establishment status
	 */
	private Status establishSession(Method callbackMethod) {
		BusAttachment bus = Transport.getInstance().getBusAttachment();
		return bus.joinSession( origSender, 
							    SenderSessionListener.PORT_NUM,
							    SenderSessionListener.getSessionOpts(), 
							    new NFSessionListener(),
							    this,
							    callbackMethod);
	}//establishSession
	
	/**
	 * Leave the session if the sessionId isn't NULL and not zero <br>
	 */
	private void leaveSession() {
		
		if ( sessionId == null || sessionId == 0 ) { // This means that the session wasn't established by this object, or the session has been already closed
			return;  
		}
		
		BusAttachment bus = Transport.getInstance().getBusAttachment();
		Status status     = bus.leaveSession(sessionId);
		
		if ( status == Status.OK ) {
			logger.debug(TAG, "The session: '" + sessionId + "' was disconnected successfully");
		}
		else {
			logger.error(TAG, "Failed to disconnect the session: '" + sessionId + "', Error: '" + status + "'");
		}
	}//leaveSession
	
	/**
	 * @return Creates and returns the {@link ProxyBusObject}, casted to the {@link NotificationProducer} object
	 */
	private NotificationProducer getRemoteProxyObject() {
		BusAttachment bus = Transport.getInstance().getBusAttachment();
		
		int sid = (sessionId != null) ? sessionId : 0;
		
		logger.debug(TAG, "Creating ProxyBusObject with sender: '" + origSender + "', SID: '" + sid + "'");
		ProxyBusObject proxyObj = bus.getProxyBusObject(origSender,
														NotificationProducer.OBJ_PATH,
														sid, 
														new Class<?>[]{NotificationProducer.class});
		
		return proxyObj.getInterface(NotificationProducer.class);
	}//getRemoteProxyObject
	
	/**
	 * Calls the remote acknowledgement method
	 * @param status Session establishment status   
	 */
	private void invokeAck(Status status) {
		
		if ( status == Status.ALLJOYN_JOINSESSION_REPLY_ALREADY_JOINED || status == Status.OK) {
			
			new Thread(new Runnable() {
				@Override
				public void run() {
					NotificationProducer notifProducer = getRemoteProxyObject();
					try {
						logger.debug(TAG, "Invoking acknowledgement for notifId: '" + notifId + "'");
						notifProducer.acknowledge(notifId);
					}
					catch (ErrorReplyBusException erbe) {
						logger.error(TAG, "Failed to call acknowledge for notifId: '" + notifId + "', ErrorName: '" + erbe.getErrorName() + "', ErrorMessage: '" + erbe.getErrorMessage() + "'");
					}
					catch (BusException be) {
						logger.error(TAG, "Failed to call acknowledge for notifId: '" + notifId + "', Error: '" + be.getMessage() + "'");
					}
					finally {
						leaveSession();
					}
				}//run
			}).start();
			
		}//if :: status
	}//invokeAck
	
	/**
	 * Calls the remote dismiss method if fails, then send the Dismiss signal
	 * @param status Session establishment status
	 */
	private void invokeDismiss(final Status status) {

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					
					if ( status == Status.ALLJOYN_JOINSESSION_REPLY_ALREADY_JOINED || status == Status.OK ) {
						
						NotificationProducer notifProducer = getRemoteProxyObject();
						try {
							logger.debug(TAG, "Invoking dismiss method for notifId: '" + notifId + "'");
							notifProducer.dismiss(notifId);
						}
						catch (ErrorReplyBusException erbe) {
							logger.error(TAG, "Failed to call dismiss for notifId: '" + notifId + "', ErrorName: '" + erbe.getErrorName() + "', ErrorMessage: '" + erbe.getErrorMessage() + "', sending Dismiss signal");
							DismissSender.send(notifId, appId);
						}
						catch (BusException be) {
							logger.error(TAG, "Failed to call dismiss method for notifId: '" + notifId + "', Error: '" + be.getMessage() + "', Sending Dismiss signal");
							DismissSender.send(notifId, appId);
						}
						finally {
							leaveSession();
						}
						
					}//if :: status
					else {
						logger.warn(TAG, "Failed to establish session with the Notification sender, senging Dismiss signal, Error: '" + status + "'");
						DismissSender.send(notifId, appId);
					}
				}//run
			}).start();
	}//invokeDismiss
	
	/**
	 * Spawns a thread and invokes {@link DismissSender#send(int, UUID)} on it 
	 */
	private void spawnThreadSendDismissSignal() {
		new Thread(new Runnable() {
			@Override
			public void run() {
				DismissSender.send(notifId, appId);
			}
		}).start();
	}//sendDismissSignal
	
	/**
	 * @param methodName Get the reflection of the method of this method name. 
	 * @return 	Returns the reflection of the requested method
	 */
	private Method getMethod(String methodName, Class<?>... args) {
		try {
			return this.getClass().getDeclaredMethod(methodName, args);
		} catch (Exception e) {
			logger.error(TAG, "Failed to find the reflection method of '" + methodName + "', Error: '" + e.getMessage() + "'");
			return null;
		}
	}//getMethod
	
	
}
