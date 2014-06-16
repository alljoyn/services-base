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

package org.alljoyn.ioe.controlpanelservice;

import java.util.Map;
import java.util.UUID;

import org.alljoyn.about.AboutService;
import org.alljoyn.about.AboutServiceImpl;
import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.Status;
import org.alljoyn.ioe.controlpanelservice.communication.AnnouncementReceiver;
import org.alljoyn.ioe.controlpanelservice.communication.CommunicationUtil;
import org.alljoyn.ioe.controlpanelservice.communication.ConnManagerEventType;
import org.alljoyn.ioe.controlpanelservice.communication.ConnManagerEventsListener;
import org.alljoyn.ioe.controlpanelservice.communication.ConnectionManager;
import org.alljoyn.ioe.controlpanelservice.communication.TaskManager;
import org.alljoyn.ioe.controlpanelservice.communication.interfaces.ControlPanel;
import org.alljoyn.ioe.controlpanelservice.communication.interfaces.HTTPControl;
import org.alljoyn.ioe.controlpanelservice.ui.DeviceControlPanel;
import org.alljoyn.services.common.BusObjectDescription;

import android.util.Log;

/**
 * Provides the main methods for control panel service 
 */
public class ControlPanelService  implements ConnManagerEventsListener {
	private static final String TAG = "cpan" + ControlPanelService.class.getSimpleName();
	
	private static final ControlPanelService SELF     = new ControlPanelService();
	
	public static final String INTERFACE_PREFIX       = "org.alljoyn.ControlPanel";
	
	private static final String[] ANNOUNCEMENT_IFACES = new String[]{ControlPanel.IFNAME, HTTPControl.IFNAME};
	
	/**
	 * Reference to connection manager object
	 */
	private ConnectionManager connMgr;
	
	/**
	 * Listens to changes of the devices in the control panel proximity 
	 */
	private DeviceRegistry deviceRegistry;
	
	/**
	 * Announcement receiver to receive announcement signals from the {@link AboutService}
	 */
	private AnnouncementReceiver announcementReceiver; 
	
	/**
	 * Creates instance of the control panel service
	 * @return {@link ControlPanelService}
	 */
	public static ControlPanelService getInstance() {
		return SELF;
	}//getInstance
	
	/**
	 * Constructor
	 */
	private ControlPanelService() {
		connMgr = ConnectionManager.getInstance();
	}
	
	/**
	 * Starts {@link ControlPanelService} which discovers controllable devices in its proximity. <br>
	 * The ControlPanelService user is informed about the devices in proximity via the {@link DeviceRegistry} interface. <br>
	 * The discovery mechanism is implemented by receiving Announcement signals. <br>
	 * @param bus BusAttachment that the service should use
	 * @param deviceRegistry Holds the information about the devices in proximity<br>
	 * {@link DefaultDeviceRegistry} may be passed in to receive information about the devices
	 * @throws ControlPanelException if failed to initialize the control panel service
	 */
	public void init(BusAttachment bus, DeviceRegistry deviceRegistry) throws ControlPanelException {
		if (deviceRegistry == null) {
			throw new ControlPanelException("deviceRegistry can't be NULL");
		}
		
		AboutService aboutService = AboutServiceImpl.getInstance();
		if ( !aboutService.isClientRunning() ) {
			throw new ControlPanelException("The AboutService is not running, impossible to receive Announcement signals");
		}	
		
		//Perform the basic service initialization
		init(bus);
		
		this.deviceRegistry = deviceRegistry;
		
		Log.d(TAG, "Start listening for Announcement signals");
		connMgr.registerEventListener(ConnManagerEventType.ANNOUNCEMENT_RECEIVED, this);
		
		//Add an announcement handler
		announcementReceiver = new AnnouncementReceiver();
		for (String iface : ANNOUNCEMENT_IFACES) {
			aboutService.addAnnouncementHandler(announcementReceiver, new String[]{iface});
		}
	}//init

	/**
	 * Starts {@link ControlPanelService} without discovering new devices in proximity 
	 * @param bus
	 */
	public void init(BusAttachment bus) throws ControlPanelException {
		connMgr.setBusAttachment(bus);
		TaskManager.getInstance().initPool();
	}//init

	/**
	 * @return Returns this device registry, or NULL if not defined
	 */
	public DeviceRegistry getDeviceRegistry() {
		return deviceRegistry;
	}//getDeviceRegistry
	
	/**
	 * Creates object of controllable device. <br>
	 * The controllable device allows to join session with the remote device and to receive its {@link DeviceControlPanel}
	 * @param sender The unique name of the remote device
	 * @return Created ControllableDevice
	 * @throws ControlPanelException cpe
	 */
	public ControllableDevice getControllableDevice(String sender) throws ControlPanelException {
	    return getControllableDevice(UUID.randomUUID().toString(), sender);
	}//getControllableDevice
	
	/**
	 * Creates object of controllable device. <br>
	 * The controllable device allows to join session with the remote device and to receive its {@link DeviceControlPanel}
	 * @param deviceId The device unique identifier
	 * @param sender The unique name of the remote device
	 * @return Created ControllableDevice
	 * @throws ControlPanelException cpe
	 */
	public ControllableDevice getControllableDevice(String deviceId, String sender) throws ControlPanelException {
		
		if ( sender == null || sender.length() == 0 ) {
			throw new ControlPanelException("Received an illegal sender name, Sender: '" + sender + "'");
		}
		
		Log.i(TAG, "Creating ControllableDevice, Sender: '" + sender + "', DeviceId: '" + deviceId + "'");
	    return new ControllableDevice(deviceId, sender);
	}//getControllableDevice
	
	/**
	 * Shutdown the {@link ControlPanelService}
	 */
	public void shutdown() {
		Log.d(TAG, "Shutdown ControlPanelService");
		
		if ( announcementReceiver != null ) {
			
			AboutService aboutService = AboutServiceImpl.getInstance();
			for (String iface : ANNOUNCEMENT_IFACES) {
				aboutService.removeAnnouncementHandler(announcementReceiver, new String[]{iface});
			}
		}
		
		if ( deviceRegistry != null ) {
			Log.d(TAG, "Clear devices registry");
			for (ControllableDevice device : deviceRegistry.getDevices().values()) {
				stopControllableDevice(device);
			}
			deviceRegistry = null;
		}
		
		TaskManager taskManager = TaskManager.getInstance();
		if ( taskManager.isRunning() ) {
			taskManager.shutdown();
		}
		
		connMgr.shutdown();
	}//shutdown
	
	/**
	 * Stops activities of the passed {@link ControllableDevice}
	 * @param device {@link ControllableDevice} to be stopped
	 */
	public void stopControllableDevice(ControllableDevice device) {
		Log.d(TAG, "Stop device: '" + device.getDeviceId() + "' activities");
		device.stopDeviceActivities();
	}//shutdown
	
	/**
	 * Called when received a connection manager event 
	 * @see org.alljoyn.ioe.controlpanelservice.communication.ConnManagerEventsListener#connMgrEventOccured(org.alljoyn.ioe.controlpanelservice.communication.ConnManagerEventType, java.util.Map)
	 */
	public void connMgrEventOccured(ConnManagerEventType eventType, Map<String, Object> args) {
		Log.d(TAG, "Received event from connection manager, type: '" + eventType + "'");
		switch (eventType) {
			case ANNOUNCEMENT_RECEIVED: {
				handleAnnouncement(args);
				break;
			}//announcement
			default:
				break;
			}//switch
	}//connMgrEventOccured
	
	
	//======================================================//
		
	/**
	 * Checks whether the received announcement has ControlPanel interface <br>
	 * If has creates ControllableObject and save it in the registry
	 * @param args
	 */
	private void handleAnnouncement(Map<String, Object> args) {
		
		String deviceId   = (String)args.get("DEVICE_ID");
		String appId      = (String)args.get("APP_ID");
		String sender     = (String)args.get("SENDER");
		
		BusObjectDescription[] objDescList = (BusObjectDescription[])args.get("OBJ_DESC");
		
		if ( deviceId == null || deviceId.length() == 0 ) {
			Log.e(TAG, "Received a bad Announcement signal, deviceId can't be NULL or empty");
			return;
		}
		if ( sender == null || sender.length() == 0 ) {
			Log.e(TAG, "Received a bad Announcement signal, sender can't be NULL or empty");
			return;
		}
		if ( objDescList == null || objDescList.length == 0 ) {
			Log.e(TAG, "Received a bad Announcement signal, BusObjectDescription array is empty");
			return;
		}
		
		// The controllable device id should be constructed from the deviceId and the appId
		deviceId = deviceId + "_" + appId;
		
		boolean newDevice      = false;    // TRUE if it's a not registered new device
		boolean handledDevice  = false;    // TRUE if for at least one received control panel object path we handled the device 
		
		ControllableDevice device = deviceRegistry.getDevices().get(deviceId);
		
		// Iterate over the BusObjectDescription objects received from an Announcement signal 
		for (BusObjectDescription  busObjDesc : objDescList) {
			Log.v(TAG, "Found objPath: '" + busObjDesc.getPath() + "'");
			
			String[] interfaces = busObjDesc.getInterfaces();
			
		    int ifaceMask       = CommunicationUtil.getInterfaceMask(interfaces);
		    // Check if found a ControlPanel or HTTPControl interfaces
		    if ( !CommunicationUtil.maskIncludes(ifaceMask, ControlPanel.ID_MASK) &&
		         !CommunicationUtil.maskIncludes(ifaceMask, HTTPControl.ID_MASK) ) {
		    	continue;
		    }
		    
		    String objPath = busObjDesc.getPath();
		    			
			Log.d(TAG, "Found ControlPanel object, path: '" + objPath + "'");
			
			if ( !handledDevice ) {
				
			    if ( device == null ) {
					Log.d(TAG, "Discovered new device, deviceId: '" + deviceId  + "', sender: '" + sender + "'");
					
					device = new ControllableDevice(deviceId, sender);
					device.subscribeOnFoundLostEvents();           // Listen to events of found | lost adv. name 
					newDevice = true;
				 }//device == null
				 else {
				     Log.d(TAG, "Device with deviceId: '" + deviceId  + "' already exists, updating sender to be: '" + sender + "'");
					 device.setSender(sender);
					 
					 try {
					     connMgr.cancelFindAdvertisedName(sender);
					 } catch (ControlPanelException cpe) {
					     Log.e(TAG, "Failed to call cancelFindAdvertisedName(), Error: '" + cpe.getMessage() + "'");
 						 return;
				     }
				 }//else :: device == null
				
				 device.setReachable(true);
				 device.startDeviceFoundVerificationService();     // Start scheduled service before call findAdvName
				
				 Log.d(TAG, "Start findAdvertisedName for sender: '" + sender + "'");
				 Status res;
				 
				 try {
					res = connMgr.findAdvertisedName(sender);
				 } catch (ControlPanelException cpe) {
					 Log.e(TAG, "Failed to call findAdvertisedName(), Error: '" + cpe.getMessage() + "'");
					 return;
				 }
				 
				 if ( res != Status.OK ) {
				     Log.d(TAG, "Failed to start findAdvertisedName for sender: '" + sender + "', Error: '" + res + "'");
					 device.stopDeviceActivities();
					 return;
			 	 }
			 
			     //We handled the discovered device for at least one of the received ControlPanel object paths 
			     handledDevice = true;
			}//if :: not handledDevice
			
			try {
				device.addControlPanel(objPath, ifaceMask);
			} catch (ControlPanelException cpe) {
				Log.w(TAG, "Received a broken object path: '" + objPath + "', Error: '" + cpe.getMessage() + "'");
			}
			
		}//for :: BusObjectDescription
		
		if (handledDevice) {
			if ( newDevice ) {
				deviceRegistry.foundNewDevice(device);
			}
			else {
				deviceRegistry.reachabilityChanged(device, true);
			}
		}
	}//handleAnnouncement
}
