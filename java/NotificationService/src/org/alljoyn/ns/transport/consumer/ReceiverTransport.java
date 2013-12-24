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
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.atomic.AtomicBoolean;

import org.alljoyn.about.AboutService;
import org.alljoyn.about.AboutServiceImpl;
import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.Status;
import org.alljoyn.bus.Variant;
import org.alljoyn.bus.annotation.BusSignal;
import org.alljoyn.ns.Notification;
import org.alljoyn.ns.NotificationReceiver;
import org.alljoyn.ns.NotificationServiceException;
import org.alljoyn.ns.commons.GenericLogger;
import org.alljoyn.ns.commons.NativePlatform;
import org.alljoyn.ns.transport.Transport;
import org.alljoyn.ns.transport.TransportNotificationText;
import org.alljoyn.ns.transport.interfaces.NotificationDismisser;
import org.alljoyn.ns.transport.interfaces.NotificationTransport;
import org.alljoyn.ns.transport.interfaces.NotificationTransportSuperAgent;
import org.alljoyn.services.common.AnnouncementHandler;
import org.alljoyn.services.common.BusObjectDescription;

/**
 * The class manages NotificationReceiver transport logic
 */
public class ReceiverTransport implements AnnouncementHandler {
	private static final String TAG = "ioe" + ReceiverTransport.class.getSimpleName();
	
	/**
	 * The reference to the platform dependent object
	 */
	private NativePlatform nativePlatform;

	/**
	 * The basic addMatch rule to receive session less signals(SLS) 
	 */
	private static final String NOTIF_SLS_BASIC_RULE        = "type='signal'";
	
	/**
	 * The name of the notification signal
	 */
	private static final String NOTIF_SIGNAL_NAME           = "notify";
	
	/**
	 * The name of the Dismiss signal
	 */
	private static final String DISMISS_SIGNAL_NAME         = "dismiss";
	
	/**
	 * TRUE if we need to look for the SuperAgent
	 */
	private boolean isNeedSearchSA    = true;
	
	/**
	 * TRUE means stop forwarding notification messages to notificationReceiver
	 */
	private boolean stopReceiving     = false;
	
	/**
	 * The addMatch rule we used to receive signals
	 */
	private StringBuilder signalReceiverMatchRule;
	
	/**
	 * TRUE means received message from SuperAgent, possible to stop receiving message from a regular consumers
	 */
	private AtomicBoolean isSuperAgentFound;
	
	/**
	 * Notification transport producer
	 * Receives and handles session less signals from a regular producers
	 */
	private NotificationTransport fromProducerChannel;
	
	/**
	 * Notification transport super agent
	 * Receives and handles session less signals from Super Agent
	 */
	private NotificationTransport fromSuperAgentChannel; 
	
	/**
	 * Receives the Dismiss signals
	 */
	private DismissConsumer dismissSignalHandler;
	
	/**
	 * Reference to NotificationReceiver object
	 */
	private NotificationReceiver notificationReceiver;
	
	/**
	 * Constructor
	 * @param nativePlatform The reference to the platform dependent object
	 */
	public ReceiverTransport(NativePlatform nativePlatform, NotificationReceiver receiver) {
		this.notificationReceiver = receiver;
		this.nativePlatform       = nativePlatform;
	}
	
	/**
	 * @param Is SuperAgent found
	 */
	public boolean getIsSuperAgentFound() {
		return isSuperAgentFound.get();
	}//getIsSuperAgentFound
	
	/**
	 * Starts the service in the Notification Receiver mode
	 * @throws NotificationServiceException Is thrown if failed to start the SenderTransport	 
	 */
	public void startReceiverTransport() throws NotificationServiceException {
		
		GenericLogger logger = nativePlatform.getNativeLogger();
		
		logger.debug(TAG, "Starting receiver transport");
		
		Transport transport         = Transport.getInstance();
		BusAttachment busAttachment = transport.getBusAttachment();
		
		//Save the basic rule of receiving session less signals
		signalReceiverMatchRule = new StringBuilder(NOTIF_SLS_BASIC_RULE);
		
		isSuperAgentFound       = new AtomicBoolean(false);
		
		if ( isNeedSearchSA ) {
			logger.debug(TAG, "Need to search for SuperAgent, register SuperAgent signal receiver, announcement receiver and then Producer signal receiver");
			
			//Register to receive signals directly from SA
			fromSuperAgentChannel = new NotificationTransportConsumer(NotificationTransportConsumer.FROM_SUPERAGENT_RECEIVER_PATH);
			boolean regSAHandler  = registerNotificationSignalHandlerChannel(fromSuperAgentChannel, NotificationTransportConsumer.FROM_SUPERAGENT_RECEIVER_PATH, NotificationTransportSuperAgent.IF_NAME);
			
			if ( !regSAHandler ) {
				logger.error(TAG, "Failed to register a SuperAgent signal handler");
				throw new NotificationServiceException("Failed to register a SuperAgent signal handler");
			}
			
			AboutService aboutService = AboutServiceImpl.getInstance();
			if ( !aboutService.isClientRunning() ) {
				logger.error(TAG, "The AboutClient wasn't started, unable to register to receive Announcement signals");
				stopReceiverTransport();
		        throw new NotificationServiceException("The AboutClient wasn't started");
			}
			
			//Register to receive announcements from SA
			logger.debug(TAG, "Registering AnnouncementReceiver");
			aboutService.addAnnouncementHandler(this);
		}//if :: isNeedSearchSA
		
		//Register to receive signals directly from Producers
		logger.debug(TAG, "Registering to receive signals from producers");
		
		fromProducerChannel        = new NotificationTransportConsumer(NotificationTransportConsumer.FROM_PRODUCER_RECEIVER_PATH);
		boolean regProducerHandler = registerNotificationSignalHandlerChannel(fromProducerChannel, NotificationTransportConsumer.FROM_PRODUCER_RECEIVER_PATH, NotificationTransport.IF_NAME);
		
		if ( !regProducerHandler ) {
			logger.error(TAG, "Failed to register a Producer signal handler");
			throw new NotificationServiceException("Failed to register a Producer signal handler");
		}
		
		//Register to receive Dismiss signals
		dismissSignalHandler      = new DismissConsumer();
		boolean regDismissHandler = registerDismissSignalHandler(dismissSignalHandler);
		
		if ( !regDismissHandler ) {
			logger.error(TAG, "Failed to register Dismiss signal handler");
			throw new NotificationServiceException("Failed to register Dismiss signal handler");
		}
		
		String matchRuleStr = signalReceiverMatchRule.toString();
		Status status       = busAttachment.addMatch(matchRuleStr);
		if ( status == Status.OK ) {
			logger.debug(TAG, "The signal handler match rule: '" + matchRuleStr + "' added successfully, signal handler established");
		}
		else {			
			logger.error(TAG, "Failed to add match rule: '" + matchRuleStr + "', signal handler not established");
			stopReceiverTransport();
			throw new NotificationServiceException("Failed to call AddMatch: '" + matchRuleStr + "'");
		}
		
	}//startReceiverTransp
	
	/**
	 * ReceiverTransport cleanups
	 * @param logger
	 */
	public void stopReceiverTransport() {
		
		GenericLogger logger        = nativePlatform.getNativeLogger();
		BusAttachment busAttachment = Transport.getInstance().getBusAttachment();
		
		logger.debug(TAG, "Stopping ReceiverTransport");
		
		Method notifConsumerMethod = getNotificationConsumerSignalMethod();
		
		if ( fromSuperAgentChannel != null ) {
			logger.debug(TAG, "Searched for a SuperAgent, cleaning up...");
			logger.debug(TAG, "Unregister SuperAgent signal handler");
			
			if ( notifConsumerMethod != null ) {
				busAttachment.unregisterSignalHandler(fromSuperAgentChannel, notifConsumerMethod);
			}
				
			busAttachment.unregisterBusObject(fromSuperAgentChannel);
			fromSuperAgentChannel = null;
			isSuperAgentFound     = null;
		}
		
		logger.debug(TAG, "Remove the AnnouncementReceiver");
		AboutServiceImpl.getInstance().removeAnnouncementHandler(this);
		
		if ( fromProducerChannel != null ) {
			logger.debug(TAG, "Unregister Producer signal handler");
			
			if ( notifConsumerMethod != null ) {
				busAttachment.unregisterSignalHandler(fromProducerChannel, notifConsumerMethod);
			}
			
			busAttachment.unregisterBusObject(fromProducerChannel);
			fromProducerChannel = null;
		}
		
		if ( dismissSignalHandler != null ) {
			logger.debug(TAG, "Unregister Dismiss signal handler");
			
			Method dismisSignalMethod = getDismissSignalMethod();
			if ( dismisSignalMethod == null ) {
				busAttachment.unregisterSignalHandler(dismissSignalHandler, dismisSignalMethod);
			}
			
			busAttachment.unregisterBusObject(dismissSignalHandler);
			dismissSignalHandler = null;
		}
		
		if ( signalReceiverMatchRule != null  ) {
			String currentMatchRule = signalReceiverMatchRule.toString();
			logger.debug(TAG, "Removing signal MatchRule: " + currentMatchRule);
			busAttachment.removeMatch(currentMatchRule);
			signalReceiverMatchRule   = null;
		}
		
	}//stopReceiverTransport
	
	/**
	 * Received notification, call the notification receiver callback to pass the notification
	 * @param notificaion
	 */
	public void onReceivedNotification(final Notification notification) {
		
		GenericLogger logger = nativePlatform.getNativeLogger();
		
		if (stopReceiving) {
			logger.debug(TAG, "In stopSending mode NOT delivering notifications!!!");
			return;
		}

		try {
			Transport.getInstance().dispatchTask(
					new Runnable() {
						@Override
						public void run() {
							notificationReceiver.receive(notification);
						}
					}//runnable
			);
		}
		catch(RejectedExecutionException ree) {
			logger.error(TAG, "Failed to return a received notification, id: '" + notification.getMessageId() + "', Error: '" + ree.getMessage() + "'");
		}
		
	}//onReceivedNotification
	
	/**
	 * Handle the received Dismiss signal
	 * @param msgId The message id of the {@link Notification} that should be dismissed
	 * @param appId The appId of the Notification sender service
	 */
	public void onReceivedNotificationDismiss(final int msgId, final UUID appId) {
		GenericLogger logger = nativePlatform.getNativeLogger();
		logger.debug(TAG, "Received the Dismiss signal notifId: '" + msgId + "', from the appId: '" + appId + "', delivering to the NotificationReceiver");
		
		try {
			Transport.getInstance().dispatchTask(
					new Runnable() {
						@Override
						public void run() {
							notificationReceiver.dismiss(msgId, appId);
						}
					}//runnable
			);
		}
		catch(RejectedExecutionException ree) {
			logger.error(TAG, "Failed to deliver the Dismiss event of the notifId: '" + msgId + "', from the appId:'" + appId + "', Error: '" + ree.getMessage() + "'");
		}
	}//onReceivedNotificationDismiss
	
	/**
	 * Called on received an Announcement signal
	 */
	@Override
	public void onAnnouncement(String serviceName, short port, BusObjectDescription[] objectDescriptions, Map<String, Variant> serviceMetadata) {
		GenericLogger logger 			= nativePlatform.getNativeLogger();
		BusAttachment busAttachment 	= Transport.getInstance().getBusAttachment();
		
		busAttachment.enableConcurrentCallbacks();
		
		//if SA is already found no need to parse this announcement		
		if ( isSuperAgentFound.get() ) {
		    return;	
		}
		
		logger.debug(TAG, "Received announcement signal");
		
		//Iterate over the BusDescription objects and search for those from SA
		for (BusObjectDescription bod: objectDescriptions) {
			for (String ifName: bod.interfaces){
				if ( NotificationTransportSuperAgent.IF_NAME.equals(ifName) ) {
					logger.debug(TAG, "Received announcement signal from SA, call onReceivedFirstSuperAgentNotification");
					onReceivedFirstSuperAgentNotification(busAttachment.getMessageContext().sender);
					return;
				}
			}//for::interfaces
		}//for::BusObjectDescription
	}//onReceivedAnnouncement

	
	/**
	 * When first Notification from SuperAgent is received:
	 * 1. Unregister producer signal handler
	 * 2. remove existing match rule
	 * 3. add match rule only of this SuperAgent 
	 */
	public void onReceivedFirstSuperAgentNotification(String superAgentUniqueName) {
		
		GenericLogger logger 		= nativePlatform.getNativeLogger();
		BusAttachment busAttachment = Transport.getInstance().getBusAttachment();
		
		if ( isSuperAgentFound.get() ) {
			logger.info(TAG, "The SuperAgent has already been found, returning");
			return;
		}
		
		String previousRule = signalReceiverMatchRule.toString();
		signalReceiverMatchRule.setLength(0);
		
		//Build the new signal receiving match rule
		signalReceiverMatchRule.append(NOTIF_SLS_BASIC_RULE)
							   .append(",sender='")
							   .append(superAgentUniqueName)
							   .append("'");
		
		String newRule = signalReceiverMatchRule.toString();
		logger.debug(TAG, "Adding the new addMatch rule: '" + newRule + "'");
		
		Status status = busAttachment.addMatch(newRule);
		if ( status != Status.OK ) {
			logger.error(TAG, "Failed to set the new addMatch rule: '" + newRule + "', not listening to a SuperAgent");
			signalReceiverMatchRule.setLength(0);
			signalReceiverMatchRule.append(previousRule);
			return;
		}
	 	
		logger.debug(TAG, "Remove the previous addMatch rule: '" + previousRule + "'");
		status = busAttachment.removeMatch(previousRule);
		if ( status != Status.OK ) {
			logger.warn(TAG, "Failed to remove the previous addMatch rule: '" + previousRule + "'");
		}
		
		logger.debug(TAG, "Set SuperAgent found as TRUE");
		isSuperAgentFound.set(true);
		
		logger.debug(TAG, "Unregister fromProducer signal handler");
		busAttachment.unregisterSignalHandler(fromProducerChannel, getNotificationConsumerSignalMethod());
		
		logger.debug(TAG, "Unregister fromProducer bus object");
		busAttachment.unregisterBusObject(fromProducerChannel);
		fromProducerChannel = null;
		
		logger.debug(TAG, "Unregister Announcment signal handler");
		AboutServiceImpl.getInstance().removeAnnouncementHandler(this);
	}//onReceivedFirstSuperAgentNotification

	
	/**
	 * @see org.alljoyn.services.common.AnnouncementHandler#onDeviceLost(java.lang.String)
	 */
	@Override
	public void onDeviceLost(String arg0) {}
	
	/**
	 * Register channel object to receive Notification signals 
	 * @param receiverChannel Receiver channel object
	 * @param receiverChannelServicePath The service path of the receiver channel object
	 * @param signalHandlerIfName The interface name the receiver channel is listening
	 * @param signalName The signal name that belongs to the interface name
	 * @return TRUE on success or FALSE on fail
	 */
	private boolean registerNotificationSignalHandlerChannel (NotificationTransport receiverChannel, String receiverChannelServicePath, String signalHandlerIfName) {
		GenericLogger logger = nativePlatform.getNativeLogger();
		
		logger.debug(TAG, "Registering signal handler for interface: '" + signalHandlerIfName + "' servicePath: " + receiverChannelServicePath);

		Method handlerMethod = getNotificationConsumerSignalMethod();
		if ( handlerMethod == null ) {
			return false;
		}
		
		boolean regRes = Transport.getInstance().registerObjectAndSetSignalHandler(logger, signalHandlerIfName, NOTIF_SIGNAL_NAME, handlerMethod, receiverChannel, receiverChannelServicePath);

		if ( !regRes ) {
			stopReceiverTransport();       // Stop receiver transport to allow later recovery
			return false;
		}
			
		return true;
	}//registerSignalHandlerChannel
	
	
	/**
	 * Registers Dismiss signal receiver 
	 * @param dismissConsumer
	 * @return TRUE on success or FALSE on fail
	 */
	private boolean registerDismissSignalHandler(DismissConsumer dismissConsumer) {
		GenericLogger logger = nativePlatform.getNativeLogger();
		
		logger.debug(TAG, "Registering signal handler for interface: '" + DismissConsumer.IFNAME + "' servicePath: '" + DismissConsumer.OBJ_PATH + "'");
		
		Method handlerMethod = getDismissSignalMethod();
		if ( handlerMethod == null ) {
			return false;
		}
		
		String allJoynName = handlerMethod.getAnnotation(BusSignal.class).name();
		boolean regRes = Transport.getInstance().registerObjectAndSetSignalHandler(logger, DismissConsumer.IFNAME, allJoynName, handlerMethod, dismissConsumer, DismissConsumer.OBJ_PATH);

		if ( !regRes ) {
			stopReceiverTransport();       // Stop receiver transport to allow later recovery
			return false;
		}
			
		return true;
	}//registerDismissSignalReceiver
	
	/**
	 * Returns reflection of {@link NotificationTransport#notify} method
	 * Used to register signal handler dynamically (without signal annotation on the method) 
	 * @return Method object or NULL if failed to retrieve
	 */
	private Method getNotificationConsumerSignalMethod() {
		Method  retMethod;
		try {
			retMethod = NotificationTransport.class.getMethod(NOTIF_SIGNAL_NAME,
														      Integer.TYPE,   				      // version
														      Integer.TYPE,   			          // msgId,														      
														      Short.TYPE,	 				      // messageType,
														      String.class,                       // deviceId,
														      String.class,                       // deviceName,
														      byte[].class,   				      // appId,
														      String.class,   				      // appName,														
														      Map.class,		 				  // attributes
														      Map.class,      				  	  // customAttributes
														      TransportNotificationText[].class); // text

		}
		catch(Exception ex) {
			nativePlatform.getNativeLogger().error(TAG, "Failed to get a reflection of the signal method: '" + NOTIF_SIGNAL_NAME + "', Error: " + ex.getMessage());
			retMethod = null;
		}
		
		return retMethod;
	}//getNotificationTransportSignalMethod


	
	/**
	 * Returns reflection of the {@link DismissConsumer#dismiss(int, byte[])}
	 * @return Method object or NULL if failed to retrieve
	 */
	private Method getDismissSignalMethod() {
		Method retMethod;
		
		try {
			retMethod = NotificationDismisser.class.getMethod(DISMISS_SIGNAL_NAME,
															  int.class,
															  byte[].class);
		}
		catch(Exception ex) {
			nativePlatform.getNativeLogger().error(TAG, "Failed to get a reflection of the signal method: '" + DISMISS_SIGNAL_NAME + "', Error: " + ex.getMessage());
			retMethod = null;
		}
		
		return retMethod;
	}//getDismissSignalMethod
	
}
