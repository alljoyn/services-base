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


package org.alljoyn.ns;

import java.lang.reflect.Method;
import java.util.EnumMap;
import java.util.Map;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.atomic.AtomicBoolean;

import org.alljoyn.about.AboutService;
import org.alljoyn.about.AboutServiceImpl;
import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.Status;
import org.alljoyn.bus.Variant;
import org.alljoyn.ns.commons.GenericLogger;
import org.alljoyn.ns.commons.NativePlatform;
import org.alljoyn.ns.commons.NativePlatformFactory;
import org.alljoyn.ns.commons.NativePlatformFactoryException;
import org.alljoyn.services.common.AnnouncementHandler;
import org.alljoyn.services.common.BusObjectDescription;


class Transport implements AnnouncementHandler {
	
	private static final String TAG = "ioe" + Transport.class.getSimpleName(); 
	
	/**
	 * Reference to Transport object
	 */
	private static Transport transport = new Transport();
	
		
	/**
	 * The interface name of the Notification Service
	 * Used to receive session less signals directly from producer (without Super Agent) 
	 */
	private static final String NOTIF_PRODUCER_IFNAME 	    = "org.alljoyn.Notification";
	
	/**
	 * The name of the notification signal
	 */
	private static final String NOTIF_SIGNAL_NAME           = "notify";

	
	/**
	 * The basic addMatch rule to receive session less signals(SLS) 
	 */
	private static final String NOTIF_SLS_BASIC_RULE        = "type='signal'";
	
	/**
	 * Reference to native platform object
	 */
	private NativePlatform nativePlatform;
	
	/**
	 * Reference to BusAttachment object
	 */
	private BusAttachment busAttachment;
	
	/**
	 * Reference to NotificationReceiver object
	 */
	private NotificationReceiver notificationReceiver;
	
	/**
	 * TRUE if we need to look for the SuperAgent
	 */
	private boolean isNeedSearchSA    = true;
	
	/**
	 * TRUE means received message from SuperAgent, possible to stop receiving message from a regular consumers
	 */
	private AtomicBoolean isSuperAgentFound;
	
	/**
	 * TRUE means to stop sending notification messages
	 */
	private boolean stopSending       = false;
	
	/**
	 * TRUE means stop forwarding notification messages to notificationReceiver
	 */
	private boolean stopReceiving     = false;
	
	/**
	 * Received TRUE if sender transport was already called
	 */
	private boolean isSenderTransportCalled   = false;
	
	/**
	 * Receives TRUE if receiver transport was called
	 */
	private boolean isReceiverTransportCalled = false;
	
	/**
	 * Map NotificationMessageType to transport object
	 */
	private Map<NotificationMessageType,TransportChannelObject> transportProducerChannels;
	
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
	 * The addMatch rule we used to receive signals
	 */
	private StringBuilder signalReceiverMatchRule;
	
	/**
	 * Pool of threads that is used to return the received notifications to a {@link NotificationReceiver} 
	 */
	private TransportReceiverPoolManager receiverPool;
	
	/**
	 * Return the Transport object
	 * @return
	 */
	public static Transport getInstance() {
		return transport;
	}//getInstance

	/**
	 * Constructor
	 * private constructor - to avoid creation of Transport object by other classes
	 */
	private Transport() {}//Constructor
	
	/**
	 * Returns reference to the BusAttachment
	 * @return
	 */
	public BusAttachment getBusAttachment() {
		return busAttachment;
	}
	
	/**
	 * @return Returns TRUE if SuperAgent was found
	 */
	public synchronized boolean getIsSuperAgentFound() {
		return isSuperAgentFound.get();
	}//getSuperAgentFound
	
	/**
	 * Set notificationReceiver object
	 * This will be used to call back the receiver once, we received a notification 
	 * @param notificationReceiver
	 */
	public void setNotificationReceiver(NotificationReceiver notificationReceiver){
		this.notificationReceiver = notificationReceiver;
	}//setNotificationReceiver
	
	/**
	 * @param bus The {@link BusAttachment} to be used by this {@link Transport} object
	 * @throws NotificationServiceException Is thrown if failed to start the SenderTransport
	 */
	public void startSenderTransport(BusAttachment bus) throws NotificationServiceException {
		GenericLogger logger;
		logger = getLogger();

		if ( isSenderTransportCalled ) {
			logger.debug(TAG, "Sender transport was started before, returning");
			return;
		}
		
		//Store the received busAttachment or verify if already exists
		saveBus(logger, bus);
		
		logger.debug(TAG, "Starting a sender transport");
		
		//Creating transportChannel objects
		transportProducerChannels = new EnumMap<NotificationMessageType, TransportChannelObject>(NotificationMessageType.class);
		try {
			for ( NotificationMessageType messageType : NotificationMessageType.values() ) {
				transportProducerChannels.put(messageType,  new TransportChannelObject(messageType, busAttachment, nativePlatform));
			}
		}
		catch (NotificationServiceException nse) {
			logger.error(TAG, nse.getMessage());
			stopSenderTransport(logger);            // We failed to create a channel, stopSenderTransport to allow recovery
			throw nse;
		}
		
		//announce the producer via the About service
		AboutService aboutService = AboutServiceImpl.getInstance();
		if ( !aboutService.isServerRunning() ) {
			logger.error(TAG, "The AboutServer wasn't started, unable to send Announcement signals");
			stopSenderTransport(logger);
	        throw new NotificationServiceException("The AboutServer wasn't started");
		}
		
		for (String objPath : NotificationTransportProducer.getServicePath().values()) {
			aboutService.addObjectDescription(objPath, new String[] {NotificationTransport.IF_NAME});
		}	
	    aboutService.announce();
		
		isSenderTransportCalled = true;
	}//startSenderTransport
	
	/**
	 * @param bus The {@link BusAttachment} to be used by this {@link Transport} object
	 * @throws NotificationServiceException Is thrown if failed to start the ReceiverTransport
	 */
	public void startReceiverTransport(BusAttachment bus) throws NotificationServiceException {
		GenericLogger logger;
		logger = getLogger();
		
		if ( isReceiverTransportCalled ) {
			logger.debug(TAG, "Receiver transport was started before, returning");
			return;
		}
		
		saveBus(logger, bus);
		
		logger.debug(TAG, "Starting receiver transport");

		//Save the basic rule of receiving session less signals
		signalReceiverMatchRule = new StringBuilder(NOTIF_SLS_BASIC_RULE);
		
		isSuperAgentFound       = new AtomicBoolean(false);
		
		if ( isNeedSearchSA ) {
			logger.debug(TAG, "Need to search for SuperAgent, register SuperAgent signal receiver, announcement receiver and then Producer signal receiver");
			
			//Register to receive signals directly from SA
			fromSuperAgentChannel = new NotificationTransportConsumer(NotificationTransportConsumer.FROM_SUPERAGENT_RECEIVER_PATH); 
			if ( !registerSignalHandlerChannel(logger, fromSuperAgentChannel, NotificationTransportConsumer.FROM_SUPERAGENT_RECEIVER_PATH, NotificationTransportSuperAgent.IF_NAME) ) {
				logger.error(TAG, "Failed to register a SuperAgent signal handler");
				throw new NotificationServiceException("Failed to register a SuperAgent signal handler");
			}
			
			AboutService aboutService = AboutServiceImpl.getInstance();
			if ( !aboutService.isClientRunning() ) {
				logger.error(TAG, "The AboutClient wasn't started, unable to register to receive Announcement signals");
				stopReceiverTransport(logger);
		        throw new NotificationServiceException("The AboutClient wasn't started");
			}
			
			//Register to receive announcements from SA
			logger.debug(TAG, "Registering AnnouncementReceiver");
			aboutService.addAnnouncementHandler(this);
		}//if :: isNeedSearchSA
		
		//Register to receive signals directly from Producers
		logger.debug(TAG, "Registering to receive signals from producers");
		fromProducerChannel = new NotificationTransportConsumer(NotificationTransportConsumer.FROM_PRODUCER_RECEIVER_PATH);
		if ( !registerSignalHandlerChannel(logger, fromProducerChannel, NotificationTransportConsumer.FROM_PRODUCER_RECEIVER_PATH, NOTIF_PRODUCER_IFNAME) ) {
			logger.error(TAG, "Failed to register a Producer signal handler");
			throw new NotificationServiceException("Failed to register a Producer signal handler");
		}
		
		String matchRuleStr = signalReceiverMatchRule.toString();
		Status status       = busAttachment.addMatch(matchRuleStr);
		if ( status == Status.OK ) {
			logger.debug(TAG, "The signal handler match rule: '" + matchRuleStr + "' added successfully, signal handler established");
		}
		else {			
			logger.error(TAG, "Failed to add match rule: '" + matchRuleStr + "', signal handler not established");
			stopReceiverTransport(logger);
			throw new NotificationServiceException("Failed to call AddMatch: '" + matchRuleStr + "'");
		}
		
		//Initialize the receiver pool of threads 
		receiverPool = new TransportReceiverPoolManager();
		
		isReceiverTransportCalled = true;
	}//startReceiverTransport

	/**
	 * Called when we need to send a signal
	 * @param version The version of the message signature
	 * @param msgId notification Id the id of the sent signal
	 * @param messageType  The message type of the sent message
	 * @param deviceId Device id
	 * @param deviceName Device name
	 * @param appId App id
	 * @param appName App name
	 * @param attributes All the notification metadata
	 * @param customAttributes The customAttributes
	 * @param text Array of texts to be sent
	 * @param ttl Notification message TTL 
	 * @throws NotificationServiceException
	 */
	public void sendNotification(int version, int msgId, NotificationMessageType messageType, String deviceId, String deviceName, byte[] appId, String appName, Map<Integer, Variant> attributes, Map<String, String> customAttributes,TransportNotificationText[] text, int ttl) throws NotificationServiceException {
		GenericLogger logger;
		try {
			logger = getLogger();
		}
		catch (NotificationServiceException nse) {
		    throw nse;
		}
		if (!isSenderTransportCalled){
			logger.error(TAG,"Notification service is not running, can't send message");
			throw new NotificationServiceException("Notification service is not running, can't send message");
		}
		if (stopSending){
			logger.debug(TAG, "In stopSending mode NOT sending notification!!!");
			return;
		}
		
		TransportChannelObject transportChannel = transportProducerChannels.get(messageType);
		if ( transportChannel == null ) {
			throw new NotificationServiceException("Received an unknown message type: '" + messageType + "', can't find a transport channel to send the notification");
		}
		
		//send this notification to the correct object based on messageType
		transportChannel.sendNotification(version,
									      msgId,																	
									      messageType.getTypeId(),
										  deviceId,
										  deviceName,
										  appId,
										  appName,												
										  attributes,
										  customAttributes,
										  text,
										  ttl);
	}//sendNotification

	/**
	 * Cancel the last message sent for the given messageType
	 * @param messageType
	 * @throws NotificationServiceException 
	 */
	public void deleteLastMessage(NotificationMessageType messageType) throws NotificationServiceException {
		GenericLogger logger;
		try {
			logger = getLogger();
		}
		catch (NotificationServiceException nse) {
		    throw nse;
		}

		if ( !isSenderTransportCalled ) {
			logger.error(TAG,"Notification service is not running, can't delete message");
			throw new NotificationServiceException("Notification service is not running, can't delete message");
		}
		
		TransportChannelObject transportChannel = transportProducerChannels.get(messageType);
		if ( transportChannel == null ) {
			throw new NotificationServiceException("Received an unknown message type: '" + messageType + "', can't find a transport channel to delete the notification");
		}
		
  		//delete the last message of this messageType
		transportChannel.deleteNotification();
	}//deleteLastMessage

	/**
	 * Received notification, call the notification receiver callback to pass the notification
	 * @param notificaion
	 */
	public void onReceivedNotification(final Notification notification) {
		GenericLogger logger;
		try {
			logger = getLogger();
		}
		catch (NotificationServiceException nse) {
		    System.err.println("Could not get logger in receive notification error: " + nse.getMessage());
		    return;
		}    
		if (stopReceiving) {
			logger.debug(TAG, "In stopSending mode NOT delivering notifications!!!");
			return;
		}

		try {
			receiverPool.execute(
					new Runnable() {
						@Override
						public void run() {
							notificationReceiver.receive(notification);
						}
					}//runnable
			);
		}
		catch(RejectedExecutionException ree) {
			logger.error(TAG, "Failed to return a received notification, id: '" + notification.getMessageId() + "', Error: '" + ree.getMessage());
		}
	}//onReceivedNotification
	
	/**
	 * Called on received an Announcement signal
	 */
	@Override
	public void onAnnouncement(String serviceName, short port, BusObjectDescription[] objectDescriptions, Map<String, Variant> serviceMetadata) {
		GenericLogger logger;
		try {
			logger = getLogger();
		}
		catch (NotificationServiceException nse) {
		    System.err.println("Could not get logger in onReceivedAnnouncement error: " + nse.getMessage());
		    return;
		}
		
		busAttachment.enableConcurrentCallbacks();
		
		//if SA is already found no need to parse this announcement		
		if (isSuperAgentFound.get()) {
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
	 * @see org.alljoyn.services.common.AnnouncementHandler#onDeviceLost(java.lang.String)
	 */
	@Override
	public void onDeviceLost(String arg0) {}

	
	/**
	 * When first Notification from SuperAgent is received:
	 * 1. Unregister producer signal handler
	 * 2. remove existing match rule
	 * 3. add match rule only of this SuperAgent 
	 */
	public synchronized void onReceivedFirstSuperAgentNotification(String superAgentUniqueName) {
		
		GenericLogger logger;
		try {
			logger = getLogger();
		}
		catch (NotificationServiceException nse) {
		    System.err.println("Could not get logger in onReceivedFirstSuperAgentNotification error: " + nse.getMessage());
		    return;
		}
		
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
		busAttachment.unregisterSignalHandler(fromProducerChannel, getNotificationConsumerSignalMethod(logger));
		
		logger.debug(TAG, "Unregister fromProducer bus object");
		busAttachment.unregisterBusObject(fromProducerChannel);
		fromProducerChannel = null;
		
		logger.debug(TAG, "Unregister Announcment signal handler");
		AboutServiceImpl.getInstance().removeAnnouncementHandler(this);
	}//onReceivedFirstSuperAgentNotification
	
	/**
	 * Stop Notification Service
	 * @throws NotificationServiceException
	 */
	public void shutdown() throws NotificationServiceException {
		GenericLogger logger = getLogger();
		
		if ( busAttachment == null ) {
			logger.warn(TAG,"No BusAttachment defined, sender and receiver must be down, returning");
			return;
		}
		
		logger.debug(TAG, "Shutdown called, stopping sender and receiver transports");
		stopSenderTransport(logger);
		stopReceiverTransport(logger);
	}//shutdown
	
	/**
	 * Verifies that sender transport was started, then calls stopSenderTransport() 
	 * @throws NotificationServiceException
	 */
	public void shutdownSender() throws NotificationServiceException {
		GenericLogger logger = getLogger();
		
		if ( !isSenderTransportCalled ) {
			logger.error(TAG, "Sender service wasn't started");
			throw new NotificationServiceException("Sender service wasn't started");
		}
		
		logger.debug(TAG, "Stopping sender transport");
		stopSenderTransport(logger);
	}//stopSender
	
	/**
	 * Verifies that receiver transport was started, then calls stopReceiverTransport()
	 * @throws NotificationServiceException
	 */
	public void shutdownReceiver() throws NotificationServiceException {
		GenericLogger logger = getLogger();
		
		if ( !isReceiverTransportCalled ) {
			logger.error(TAG, "Receiver service wasn't started");
			throw new NotificationServiceException("Receiver service wasn't started");
		}
		
		logger.debug(TAG, "Stopping receiver transport");
		stopReceiverTransport(logger);
	}//stopReceiver
	
	/**
	 * Uses setNativePlatform to receive the GenericLogger
	 * @return GenericLogger Returns GenericLogger
	 * @throws NotificationServiceException thrown if no native platform object is defined
	 */
	public GenericLogger getLogger() throws NotificationServiceException {
		setNativePlatform();
		return nativePlatform.getNativeLogger();
	}//getLogger
	
	
	//********************* PRIVATE METHODS *********************//

	/**
	 * Stores the {@link BusAttachment} if it's not already exists and is connected to the Bus<br>
	 * If the {@link BusAttachment} already exists, checks whether the received {@link BusAttachment} is the same
	 * as the existent one. If not the {@link NotificationServiceException} is thrown 
	 * @param logger
	 * @param bus
	 * @throws NotificationServiceException
	 */
	private void saveBus(GenericLogger logger, BusAttachment bus) throws NotificationServiceException {
		
		if ( bus == null ) {
			logger.error(TAG, "Received a NULL BusAttachment");
			throw new NotificationServiceException("Received a not initialized BusAttachment");
		}
		
		if ( this.busAttachment == null ) {
			if ( !bus.isConnected() ) {
				logger.error(TAG, "Received a BusAttachment that is not connected to the AJ Bus");
				throw new NotificationServiceException("Received a BusAttachment that is not connected to the AJ bus");
			}
			logger.info(TAG, "BusAttachment is stored successfully");
			this.busAttachment = bus;
		}
		else if ( !this.busAttachment.getUniqueName().equals(bus.getUniqueName()) ) {
			logger.error(TAG, "NotificationService received a new BusAttachment: '" + bus.getUniqueName() + "', while previously initialized with a BusAttachment: '" + busAttachment.getUniqueName() + "'");
			throw new NotificationServiceException("BusAttachment already exists");
		}
		
	}//saveBus
	
	/**
	 * Register channel object to receive Notification signals 
	 * @param logger
	 * @param receiverChannel Receiver channel object
	 * @param receiverChannelServicePath The service path of the receiver channel object
	 * @param signalHandlerIfName The interface name the receiver channel is listening
	 * @param signalName The signal name that belongs to the interface name
	 * @return TRUE on success or FALSE on fail
	 */
	private boolean registerSignalHandlerChannel (GenericLogger logger, NotificationTransport receiverChannel, String receiverChannelServicePath, String signalHandlerIfName) {
		logger.debug(TAG, "Registering signal handler for interface: '" + signalHandlerIfName + "' servicePath: " + receiverChannelServicePath);

		Method handlerMethod = getNotificationConsumerSignalMethod(logger);
		if ( handlerMethod == null ) {
			return false;
		}
		
		boolean regRes = registerObjectAndSetSignalHandler(logger, signalHandlerIfName, NOTIF_SIGNAL_NAME, handlerMethod, receiverChannel, receiverChannelServicePath);

		if ( !regRes ) {
			stopReceiverTransport(logger);       // Stop receiver transport to allow later recovery
			return false;
		}
			
		return true;
	}//registerSignalHandlerChannel
	
	/**
	 * Uses NativePlatformFactory to receive NativePlatform object
	 * @throws NotificationServiceException thrown if failed to receive native platform factory
	 */
	private void setNativePlatform() throws NotificationServiceException {
		if ( nativePlatform == null ) {
			try {
				nativePlatform = NativePlatformFactory.getPlatformObject();
			}
			catch (NativePlatformFactoryException npfe) {
				throw new NotificationServiceException(npfe.getMessage());
			}
		}
	}//setNativePlatform

	/**
	 * Register bus object that is intended to be a signal handler
	 * @param logger
	 * @param ifName Interface name that signal handler object belongs to
	 * @param signalName 
	 * @param handlerMethod The reflection of the method that is handling the signal 
	 * @param toBeRegistered The object to be registered on the bus and set to be signal handler
	 * @param servicePath The identifier of the object
	 * @return TRUE on success and FALSE on fail
	 */
	private boolean registerObjectAndSetSignalHandler(GenericLogger logger, String ifName, String signalName, Method handlerMethod, BusObject toBeRegistered, String servicePath) {
		logger.debug(TAG, "Registering BusObject and setting signal handler, IFName: '" + ifName + ", method: '" + handlerMethod.getName() + "'");
		Status status = busAttachment.registerBusObject(toBeRegistered, servicePath);
		if ( status != Status.OK ) {
			logger.error(TAG, "Failed to register bus object, status: " + status);
			return false;
		}
				
		status = busAttachment.registerSignalHandler(ifName, signalName, toBeRegistered, handlerMethod);
		if ( status != Status.OK ) {
			logger.error(TAG, "Failed to register signal handler, status: " + status);
			return false;
		}
		return true;
	}//registerObjectAndSetSignalHandler

	/**
	 * Returns reflection of {@link NotificationTransport#notify} method
	 * Used to register signal handler dynamically (without signal annotation on the method) 
	 * @return Method object or NULL if failed to retrieve
	 */
	private Method getNotificationConsumerSignalMethod(GenericLogger logger) {
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
			logger.error(TAG, "Failed to get a reflection for the signal method: '" + NOTIF_SIGNAL_NAME + "', Error: " + ex.getMessage());
			retMethod = null;
		}
		
		return retMethod;
	}//getNotificationTransportSignalMethod
	
	/**
	 * Looping through transportProducerChannels and calls clean on the TransportObjectUtil
	 * then set transportProducerChannels to NULL and isSenderTransportCalled to false
	 */
	private void stopSenderTransport(GenericLogger logger) {
		logger.debug(TAG, "Stopping SenderTransport");
		if ( transportProducerChannels != null ) {
			for (NotificationMessageType pr : transportProducerChannels.keySet()) {			
				transportProducerChannels.get(pr).clean(busAttachment);
			}
		}
		
		// clear the busAttachment only if the receiver is not running
		if (! isReceiverTransportCalled) {
			logger.debug(TAG, "Receiver not running, clearing busAttachment");
			busAttachment = null;
		}
		
		transportProducerChannels = null;
		isSenderTransportCalled   = false;
	}//cleanTransportProducerChannel
	
	/**
	 * Unregister transportConsumerChannel BusObject and SignalHandler
	 * Test isNeedSearchSA if TRUE refer.
	 * Set isReceiverTransportCalled to FALSE
	 * @param logger
	 */
	private void stopReceiverTransport(GenericLogger logger) {
		logger.debug(TAG, "Stopping ReceiverTransport");

		if ( fromSuperAgentChannel != null ) {
			logger.debug(TAG, "Searched for a SuperAgent, cleaning up...");
			logger.debug(TAG, "Unregister from SuperAgent signal handler and bus object");
			busAttachment.unregisterSignalHandler(fromSuperAgentChannel, getNotificationConsumerSignalMethod(logger));
			busAttachment.unregisterBusObject(fromSuperAgentChannel);
			fromSuperAgentChannel = null;
			isSuperAgentFound     = null;
		}
		
		logger.debug(TAG, "Remove the AnnouncementReceiver");
		AboutServiceImpl.getInstance().removeAnnouncementHandler(this);
		
		if ( fromProducerChannel != null ) {
			logger.debug(TAG, "Unregister from Producer signal handler and bus object");
			busAttachment.unregisterSignalHandler(fromProducerChannel, getNotificationConsumerSignalMethod(logger));
			busAttachment.unregisterBusObject(fromProducerChannel);
			fromProducerChannel = null;
		}
		
		if ( signalReceiverMatchRule != null  ) {
			String currentMatchRule = signalReceiverMatchRule.toString();
			logger.debug(TAG, "Removing signal MatchRule: " + currentMatchRule);
			busAttachment.removeMatch(currentMatchRule);
			signalReceiverMatchRule   = null;
		}
		
		if ( receiverPool != null ) {
			logger.debug(TAG, "Shutting down the Receiver Thread Pool");
			receiverPool.shutdown();
			receiverPool = null;
		}
		
		// clear the busAttachment only if the sender is not running
		if (! isSenderTransportCalled) {
			logger.debug(TAG, "Sender not running, clearing busAttachment");
			busAttachment = null;
		}
		isReceiverTransportCalled = false;
	}//stopReceiverTransport
	
}//Transport

