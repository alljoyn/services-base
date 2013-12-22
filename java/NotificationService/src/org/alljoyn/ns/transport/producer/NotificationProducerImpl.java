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

package org.alljoyn.ns.transport.producer;

import java.util.concurrent.RejectedExecutionException;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.Status;
import org.alljoyn.ns.NotificationServiceException;
import org.alljoyn.ns.commons.GenericLogger;
import org.alljoyn.ns.commons.NativePlatform;
import org.alljoyn.ns.transport.Transport;
import org.alljoyn.ns.transport.interfaces.NotificationProducer;

/**
 * The class implements the {@link NotificationProducer} interface and by this realizes the Notification producer
 * proprietary logic
 */
class NotificationProducerImpl implements NotificationProducer {
	private static final String TAG = "ioe" + NotificationProducerImpl.class.getSimpleName();
	
	public static final String OBJ_PATH = "/notificationProducer";
	
	/**
	 * The Sender transport
	 */
	private SenderTransport senderTransport;
	
	/**
	 * The reference to the platform dependent object
	 */
	private NativePlatform nativePlatform;
	
	/**
	 * Constructor
	 * @param senderTransport The Sender transport
	 * @param nativePlatform The reference to the platform dependent object
	 */
	public NotificationProducerImpl(SenderTransport senderTransport, NativePlatform nativePlatform) {
		this.senderTransport = senderTransport;
		this.nativePlatform  = nativePlatform;
	}

	/**
	 * Initializes the object <br>
	 * Register {@link BusObject}, if failed to register the {@link NotificationServiceException} is thrown
	 * @throws NotificationServiceException
	 */
	public void init() throws NotificationServiceException {
		Status status = Transport.getInstance().getBusAttachment().registerBusObject(this, OBJ_PATH);
		if ( status != Status.OK ) {
			throw new NotificationServiceException("Failed to register BusObject: '" + OBJ_PATH + "', Error: '" + status + "'");
		}
	}//init

	/**
	 * @see org.alljoyn.ns.transport.interfaces.NotificationProducer#acknowledge(int)
	 */
	@Override
	public void acknowledge(final int msgId) throws BusException {
		GenericLogger logger = nativePlatform.getNativeLogger();
		logger.debug(TAG, "Received Acknowledgement for: '" + msgId + "', delegating to be executed");
		
		try {
			
			Transport.getInstance().dispatchTask(new Runnable() {
				@Override
				public void run() {
					senderTransport.acknowledge(msgId);
				}
			});
			
		}
		catch (RejectedExecutionException ree) {
			logger.error(TAG, "Failed to execute the Acknowledgement task for: '" + msgId + "', no available workers were found, Error: '" + ree.getMessage() + "'");
		}
	}//acknowledge

	/**
	 * @see org.alljoyn.ns.transport.interfaces.NotificationProducer#dismiss(int)
	 */
	@Override
	public void dismiss(final int msgId) throws BusException {
		GenericLogger logger = nativePlatform.getNativeLogger();
		logger.debug(TAG, "Received Dismiss for: '" + msgId + "', delegating to be executed");
		
		try {
		
			Transport.getInstance().dispatchTask(new Runnable() {
				@Override
				public void run() {
					senderTransport.dismiss(msgId);
				}
			});
			
		}
		catch (RejectedExecutionException ree) {
			logger.error(TAG, "Failed to execute the Dismiss task for: '" + msgId + "', no available workers were found, Error: '" + ree.getMessage() + "'");
		}
	}//dismiss

	/**
	 * @see org.alljoyn.ns.transport.interfaces.NotificationProducer#getVersion()
	 */
	@Override
	public short getVersion() throws BusException {
		return VERSION;
	}//getVersion

	
	/**
	 * Cleans the object resources 
	 */
	public void clean() {
		
		nativePlatform.getNativeLogger().debug(TAG, "Cleaning the NotificationProducerImpl");
		
		Transport.getInstance().getBusAttachment().unregisterBusObject(this);
		senderTransport = null;
		nativePlatform  = null;
	}//clean
}
