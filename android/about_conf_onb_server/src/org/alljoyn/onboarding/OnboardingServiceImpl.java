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

package org.alljoyn.onboarding;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import org.alljoyn.about.AboutServiceImpl;
import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusException;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.ErrorReplyBusException;
import org.alljoyn.bus.Status;
import org.alljoyn.onboarding.OnboardingService.AuthType;
import org.alljoyn.onboarding.client.OnboardingClient;
import org.alljoyn.onboarding.client.OnboardingClientImpl;
import org.alljoyn.onboarding.transport.MyScanResult;
import org.alljoyn.onboarding.transport.OBLastError;
import org.alljoyn.onboarding.transport.OnboardingTransport;
import org.alljoyn.onboarding.transport.ScanInfo;
import org.alljoyn.services.common.BusObjectDescription;
import org.alljoyn.services.common.ServiceAvailabilityListener;
import org.alljoyn.services.common.ServiceCommonImpl;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.util.Log;

/**
 * Impements the Server side of OnboardingService
 */
public class OnboardingServiceImpl extends ServiceCommonImpl implements OnboardingService{
	
	private static final String TAG = "ioe"
			+ OnboardingServiceImpl.class.getSimpleName();
	
	// State. This service is implemented as a state machine. See HLD
	private OnboardingState m_state = OnboardingState.PERSONAL_AP_NOT_CONFIGURED;
	
	// The AllJoyn onboarding interface
	private OnboardingInterface m_onboardingInterface;

	// Android Wi-Fi Manager
	private WifiManager m_wifi;
	
	// listener to Wi-Fi intents
	private BroadcastReceiver m_wifiConnectionStateReceiver;
	
	// Android preferences
	private static SharedPreferences m_preferences;
	private final static String ONBOARDING_PREFERENCES = "OnboardingService";
	private static final String ONBAORDING_STATE_KEY = "State";
	private static final String ONBAORDING_SSID_KEY = "SSID";
	
	// Singelton instance
	private static OnboardingServiceImpl m_instance;
	
	// The networks that the board "sees". to be delivered to the client so that users can choose
	// @see org.alljoyn.onboarding.transport.OnboardingTransport#GetScanInfo()
	private ScanInfo m_scanInfo = new ScanInfo();
	private long m_lastScanTime;
	
	// the board's last onboarding error, if the client asks.
	// @see org.alljoyn.onboarding.client.OnboardingClient#GetLastError()
	private OBLastError m_lastError = new OBLastError(); 

	// Context.. for Android necessities like WiFi manager, Preferences.
	private Context m_context;
	
	// the personal AP id in Android list of networks
	private int m_networkId;
	
	// the personal AP SSID
	public String m_ssid;
	
	// the personal AP Password
	public String m_passphrase;
	
	// the Personal AP authentication type
	public short m_authType;
	
	
	public static enum MODE {
		BASIC ((short)1),
		FAST_CHANNEL_SWITCHING ((short)2);
		
		public short id;
		
		private MODE(short id) {
			this.id = id;
		}
				
		public static MODE getModeById(short typeId) {
			MODE retType = null;
			for (MODE type : MODE.values()) {
				if (typeId == type.id ) {
					retType = type;
					break;
				}
			}
			return retType;
		}
	}
	
	/** 
	 * Singleton instance
	 * @return
	 */
	public static OnboardingService getInstance()
	{
		if (m_instance == null)
			m_instance = new OnboardingServiceImpl();
		return m_instance;
			
	}
	
	private OnboardingServiceImpl()
	{}
	
	/**
	 * Pass application context. Required for accessing Android WiFi manager
	 * @param context
	 */
	public void initContext(Context context)
	{
		if (m_context == null)
		{
			m_context = context;
		}
	}
	
	 // ------------------------  Client ------------------------------
	
	/* (non-Javadoc)
	 * @see org.alljoyn.onboarding.OnboardingService#createOnboardingClient(java.lang.String, org.alljoyn.services.common.ServiceAvailabilityListener, short)
	 */
	@Override
	public OnboardingClient createOnboardingClient(String deviceName, ServiceAvailabilityListener serviceAvailabilityListener, short port) throws Exception
	{
		return  new OnboardingClientImpl(deviceName, getBus(), serviceAvailabilityListener, port);
	}
	
	/* (non-Javadoc)
	 * @see org.alljoyn.onboarding.OnboardingService#startOnboardingClient(org.alljoyn.bus.BusAttachment)
	 */
	@Override
	public void startOnboardingClient(BusAttachment bus) throws Exception 
	{
		super.startClient();
		setBus(bus);
	}

	 // ------------------------  Server ------------------------------

	/* (non-Javadoc)
	 * @see org.alljoyn.onboarding.OnboardingService#startOnboardingServer(org.alljoyn.bus.BusAttachment)
	 */
	@Override
	public void startOnboardingServer(BusAttachment busAttachment) throws Exception {
		
		super.startServer();
		m_preferences = m_context.getSharedPreferences(ONBOARDING_PREFERENCES,Context.MODE_PRIVATE);
		m_wifi = (WifiManager) m_context.getSystemService(Context.WIFI_SERVICE);
		m_onboardingInterface = new OnboardingInterface();
		setBus(busAttachment);
		m_scanInfo.setScanResult(new MyScanResult[]{});

		// read the stored state
		loadState();

		// load the network id: the id of personal AP in the list of visible networks
		loadNetworkId();
		
		// stop tethering mode, enable wifi, get scan results, than enter tethering mode and disable wifi again.
		stopSoftAp();
		
		// wait... Android brings it down...
		Log.d(TAG, "validate: waiting for 2 seconds");
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		m_wifi.setWifiEnabled(true);
		
		// wait... Android brings it up...
		Thread.sleep(2000);

		// register for WiFi events
		registerWifiReceiver(m_context);

		// start scanning for AP
		scan();

		// publish the onboarding service on AllJoyn Bus
		registerOnboardingService();

		// set initial state
		initialize();
		
		// initialize error to none
		m_lastError.setErrorCode((short) -1);
		m_lastError.setErrorMessage("unKnown");

	}

	/**
	 * Initialize onboardee state. 
	 */
	private void initialize() {
		Log.d(TAG, "initial state: " + getState());
		switch (getState()) {
		case PERSONAL_AP_NOT_CONFIGURED: {
			// if not conifugred to personal AP, start soft AP and wait for onboarder
			startSoftAp();
			break;
		}
		case PERSONAL_AP_CONFIGURED_NOT_VALIDATED: {
			if (m_networkId > -1) {
				// We added personal AP to WiFi manager and received an id
				// however we haven't connected yet so the personal AP isn't validated
				setState(OnboardingState.PERSONAL_AP_CONFIGURED_VALIDATING);
				validate();
			} else {
				// probably invalid network credentials
				startSoftAp();
			}
			break;
		}
		case PERSONAL_AP_CONFIGURED_VALIDATING: {
			validate();
			break;
		}
		case PERSONAL_AP_CONFIGURED_VALIDATED: {
			// Already have a valid personal AP. So at initialization we reconnect to it
			reconnect();
			break;
		}
		case PERSONAL_AP_CONFIGURED_RETRY: {
			// We were unable to reach the given personal AP. Retry
			reconnect();
			break;
		}
		case PERSONAL_AP_CONFIGURED_ERROR: {
			// Probably bad personal AP credentials. Start soft AP so onbarder can reconnect to us
			startSoftAp();
			break;
		}
		}
	}

	/**
	 * Register for Android Wi-Fi intents
	 * @param context
	 */
	public void registerWifiReceiver(Context context) {

		IntentFilter wifiStateIntentFilter = new IntentFilter();
		wifiStateIntentFilter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
		wifiStateIntentFilter.addAction(WifiManager.NETWORK_IDS_CHANGED_ACTION);
		wifiStateIntentFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
		wifiStateIntentFilter.addAction(WifiManager.SUPPLICANT_CONNECTION_CHANGE_ACTION);
		wifiStateIntentFilter.addAction(WifiManager.SUPPLICANT_STATE_CHANGED_ACTION);
		wifiStateIntentFilter.addAction(WifiManager.RSSI_CHANGED_ACTION);
		wifiStateIntentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);

		m_wifiConnectionStateReceiver = new WifiConnectionStateReceiver();
		context.registerReceiver(m_wifiConnectionStateReceiver, wifiStateIntentFilter);
	}

	/** 
	 * Start WiFi scanning. Onboardee application should call it so that onboarder can retrieve the 
	 * list of AP that the are visible to the onboardee
	 */
	public void scan() {
		m_wifi.startScan();
	}

	/* (non-Javadoc)
	 * @see org.alljoyn.onboarding.OnboardingService#stopOnboardingClient()
	 */
	@Override
	public void stopOnboardingClient(){
		super.stopClient();
	}
	
	/* (non-Javadoc)
	 * @see org.alljoyn.onboarding.OnboardingService#stopOnboardingServer()
	 */
	@Override
	public void stopOnboardingServer() throws Exception {
		if (m_onboardingInterface != null) {
			getBus().unregisterBusObject(m_onboardingInterface);
		}
		super.stopServer();
	}

	/**
	 * Reconnect to the personal AP
	 */
	private void reconnect() {
		stopSoftAp();
		startWifi();
	}

	/**
	 * Enable the WiFi connection. 
	 */
	public void startWifi() {
		m_wifi.setWifiEnabled(true);
	}

	/**
	 * Factory reset. Return to soft AP mode
	 */
	public void reset(){
		setState(OnboardingState.PERSONAL_AP_NOT_CONFIGURED);
		initialize();
	}

	/**
	 * The board connects to personal AP for validating the personal AP credentials
	 */
	public void validate() {

		// Disconnect from caller here
		stopSoftAp();

		Log.d(TAG, "validate: waiting for 2 seconds");
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		// enable WiFi again
		m_wifi.setWifiEnabled(true);

		Log.d(TAG, "validate: waiting for 5 seconds");
		try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		// set the WiFi configuration
		
		AuthType authenticationType = AuthType.getAuthTypeById(m_authType);
		WifiConfiguration wifiConfiguration = new WifiConfiguration();
		m_networkId = -1;
		
		switch (authenticationType) {
		case OPEN: {
			wifiConfiguration.SSID = "\"" + m_ssid + "\"";
			wifiConfiguration.allowedKeyManagement
					.set(WifiConfiguration.KeyMgmt.NONE);

			m_networkId = m_wifi.addNetwork(wifiConfiguration);
			Log.d(TAG, "addNetwork returned " + m_networkId);

			break;
		}
		case WEP: {
			wifiConfiguration.SSID = "\"" + m_ssid + "\"";
			wifiConfiguration.wepKeys[0] = m_passphrase;//Password is in Hex
			wifiConfiguration.wepTxKeyIndex = 0;
			wifiConfiguration.allowedKeyManagement
					.set(WifiConfiguration.KeyMgmt.NONE);
			wifiConfiguration.allowedGroupCiphers
					.set(WifiConfiguration.GroupCipher.WEP40);

			m_networkId = m_wifi.addNetwork(wifiConfiguration);
			Log.d(TAG, "addNetwork returned " + m_networkId);

			break;
		}
		case WPA_AUTO:
		case WPA_TKIP:	
		case WPA_CCMP:	
		case WPA2_AUTO:
		case WPA2_TKIP:
		case WPA2_CCMP: {
			wifiConfiguration.SSID = "\"" + m_ssid + "\"";
			wifiConfiguration.preSharedKey = m_passphrase;//The password is given in Hex-this is why we don't need the ""
			wifiConfiguration.hiddenSSID = true;
			wifiConfiguration.status = WifiConfiguration.Status.ENABLED;
			wifiConfiguration.allowedGroupCiphers
					.set(WifiConfiguration.GroupCipher.TKIP);
			wifiConfiguration.allowedGroupCiphers
					.set(WifiConfiguration.GroupCipher.CCMP);
			wifiConfiguration.allowedKeyManagement
					.set(WifiConfiguration.KeyMgmt.WPA_PSK);
			wifiConfiguration.allowedPairwiseCiphers
					.set(WifiConfiguration.PairwiseCipher.TKIP);
			wifiConfiguration.allowedPairwiseCiphers
					.set(WifiConfiguration.PairwiseCipher.CCMP);
			wifiConfiguration.allowedProtocols
					.set(WifiConfiguration.Protocol.RSN);
			wifiConfiguration.allowedProtocols
					.set(WifiConfiguration.Protocol.WPA);

			m_networkId = m_wifi.addNetwork(wifiConfiguration);
			Log.d(TAG, "addNetwork returned " + m_networkId);

			break;
		}
		}

		boolean changeHappened = m_wifi.saveConfiguration();

		if (m_networkId != -1 && changeHappened) {
			Log.d(TAG, "Successfully configured Wifi");
			saveNetworkId();
		} else {
			Log.w(TAG, "WifiManager.saveConfiguration failed.");
		}

		// enable the network
		boolean networkEnabled = m_wifi.enableNetwork(m_networkId, true);
		if (!networkEnabled) {
			Log.w(TAG, "WifiManager.enableNetwork failed");
			setState(OnboardingState.PERSONAL_AP_CONFIGURED_ERROR);
		}
	}

	/**
	 * Start the AP mode.
	 */
	private void startSoftAp() {
		m_wifi.setWifiEnabled(false);

		Method[] methods = m_wifi.getClass().getDeclaredMethods();
		for (Method method : methods) {
			if (method.getName().equals("setWifiApEnabled")) {
				try {
					method.invoke(m_wifi, null, true);
				} catch (Exception ex) {
				}
				break;
			}
		}
	}

	/**
	 * Stop the AP mode.
	 */
	public void stopSoftAp() {
		Method[] methods = m_wifi.getClass().getDeclaredMethods();
		for (Method method : methods) {
			if (method.getName().equals("setWifiApEnabled")) {
				try {
					method.invoke(m_wifi, null, false);
				} catch (Exception ex) {
				}
				break;
			}
		}
	}

	/**
	 * Register the onboarding service on the AllJoyn Bus. Exposing it to clients
	 */
	private void registerOnboardingService() {
		// register my alljoyn interface
		Status status = getBus().registerBusObject(m_onboardingInterface, OnboardingTransport.OBJ_PATH);
		Log.d(TAG, String.format("BusAttachment.registerBusObject(m_onboardingInterface): %s",status));
	}

	/**
	 * Set the home AP credentials, so that the board can connect to them
	 * @param ssid
	 * @param passphrase
	 * @param authType
	 */
	public void setHomeApCredentials(String ssid, String passphrase,
			short authType) {
		m_ssid = ssid;
		m_passphrase = passphrase;
		m_authType = authType;
	}

	/**
	 * Get the onboarding state
	 * @return
	 */
	public OnboardingState getState() {
		return m_state;
	}

	private void setState(OnboardingState state) {
		Log.i(TAG, "setState: old=" + m_state + " new=" + state);
		this.m_state = state;
		// state persistence
		saveState();
	}

	private void saveState() {
		m_preferences.edit().putInt(ONBAORDING_STATE_KEY, m_state.getStateId())
				.commit();
	}

	private void loadState() {
		int stateId = m_preferences.getInt(ONBAORDING_STATE_KEY,
				OnboardingState.PERSONAL_AP_NOT_CONFIGURED.getStateId());
		m_state = OnboardingState.getStateById((short) stateId);
	}

	private void saveNetworkId() {
		m_preferences.edit().putInt(ONBAORDING_SSID_KEY, m_networkId);
	}

	private void loadNetworkId() {
		m_networkId = m_preferences.getInt(ONBAORDING_SSID_KEY, -1);
	}

	/**
	 * The AllJoyn BusObject that exposes theOnboarding interface of this device
	 * over the Bus.
	 */
	private class OnboardingInterface implements BusObject, OnboardingTransport {

		@Override
		public short getVersion() throws BusException {
			return PROTOCOL_VERSION;
		}

		/**
		 * Tell the device to connect to the Personal AP. The device is
		 * recommended to use channel switching feature if it is available.
		 * 
		 * @return short: 1 -- current soft AP mode will be disabled. 2 – a
		 *         separate channel is used to validate the Personal AP
		 *         connection.
		 * @throws BusException
		 */
		@Override
		public void Connect() throws BusException {
			Log.i(TAG, "CONNECT FUNCATION IN OnboardingServiceImpl!!!!!");
			setState(OnboardingState.PERSONAL_AP_CONFIGURED_VALIDATING);
			new Thread(new Runnable(){

				@Override
				public void run() {
					// 2. Executed after we returned the response to the onboarder
					initialize();
				}}).start();

			// 1. caller will get the response. we tell him we're about to hang up
		}

		/**
		 * Send the personal AP info to device. When the authType is equal to
		 * any, the device needs to try out all the possible authentication
		 * types it supports to connect to the AP. If authType parameter is
		 * invalid then the AllJoyn error code org.alljoyn.Error.OutOfRange will
		 * be returned
		 * 
		 * @param ssid
		 * @param passphrase
		 * @param authType
		 *            : 0 - any, 1 - Open, 2 - WPA, 3 - WEP, 4 - WPA2, 5 - WPS
		 * @throws BusException
		 */
		@Override
		public short ConfigureWiFi(String ssid, String passphrase, short authType)
				throws BusException {

			// validate credentials
			AuthType authenticationType = AuthType.getAuthTypeById(authType);
			switch (authenticationType) {
			case ANY:
			case OPEN:
			case WEP:
			case WPA_AUTO:
			case WPA_TKIP:
			case WPA_CCMP:	
			case WPA2_AUTO:
			case WPA2_TKIP:
			case WPA2_CCMP:
			case WPS: {
				break;
			}
			default: {
				// If authType parameter is invalid then the AllJoyn error code
				// org.alljoyn.Error.OutOfRange will be returned
				throw new ErrorReplyBusException(
						"org.alljoyn.Error.OutOfRange", "Value out of range");
			}
			}
			setState(OnboardingState.PERSONAL_AP_CONFIGURED_NOT_VALIDATED);
			setHomeApCredentials(ssid, passphrase, authType);
			return MODE.BASIC.id;
		}

		/**
		 * @return the state: 0 - Personal AP Not Configured 1 – Personal AP
		 *         Configured/Not Validated 2 – Personal AP
		 *         Configured/Validating 3 – Personal AP Configured/Validated 4
		 *         – Personal AP Configured/Error 5 – Personal AP
		 *         Configured/Retry
		 * @throws BusException
		 */
		@Override
		public short getState() throws BusException {
			return m_state.getStateId();
		}

		/**
		 * Tell the device to disconnect from the personal AP, clear the
		 * personal AP configuration fields, and start the soft AP mode.
		 */
		@Override
		public void Offboard() throws BusException {
			setState(OnboardingState.PERSONAL_AP_NOT_CONFIGURED);
			new Thread(new Runnable(){

				@Override
				public void run() {
					// 2. Executed after we returned the response to the onboarder
					initialize();
				}}).start();

			// 1. caller will get the response. we tell him we're about to hang up
		}

		/**
		 * Scan all the WiFi access points in the device’s proximity. Some
		 * device may not support this feature. In such a case, the AllJoyn
		 * error code org.alljoyn.Error.FeatureNotAvailable will be returned in
		 * the AllJoyn response.
		 * 
		 * @throws BusException
		 */
		@Override
		public ScanInfo GetScanInfo() throws BusException {
			// return m_lastScan;
			long currentTime = System.currentTimeMillis(); 
			m_scanInfo.setAge((short) ((currentTime - m_lastScanTime)/(60*1000)));
			return m_scanInfo;
		}

		@Override
		public OBLastError getLastError() throws BusException {
			return m_lastError;
		}
	}

	/**
	 * A receiver for Android Wi-Fi intents
	 */
	class WifiConnectionStateReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			
			// refresh the scan results
			List<ScanResult> scans1 = m_wifi.getScanResults();
			if (scans1 != null) {
				MyScanResult[] finalRes = new MyScanResult[scans1.size()];
				for (int i = 0; i < scans1.size(); ++i){
					MyScanResult result = new MyScanResult();
					result.m_ssid = scans1.get(i).SSID;
					result.m_authType = getScanResultSecurity(scans1.get(i).capabilities).getTypeId();
					finalRes[i] = result;
				}
				m_lastScanTime = System.currentTimeMillis();
				m_scanInfo.setScanResult(finalRes);
			}
			
			Log.d(TAG, "onReceive: " + intent.getAction());

			if (intent.getAction().equals(WifiManager.NETWORK_STATE_CHANGED_ACTION)) {
				NetworkInfo networkInfo = intent.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);
				if (networkInfo.getState().equals(State.CONNECTED)/*networkInfo.isConnected()*/) {
					Log.d(TAG, "onReceive: networkInfo.isConnected()");
					
					WifiInfo wifiInfo = intent.getParcelableExtra(WifiManager.EXTRA_WIFI_INFO);
					String currentSSID = wifiInfo.getSSID();
					Log.d(TAG, "check if "+m_ssid+" equals "+currentSSID);
					if(isSsidEquals(m_ssid, currentSSID)){
						if (getState().equals(OnboardingState.PERSONAL_AP_CONFIGURED_VALIDATING)) {
							setState(OnboardingState.PERSONAL_AP_CONFIGURED_VALIDATED);
							saveNetworkId();
							AboutServiceImpl.getInstance().announce(); // send an announcement after onboarding succeeded.
						}
					}
				}
			} else if (intent.getAction().equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
				NetworkInfo networkInfo = intent.getParcelableExtra(ConnectivityManager.EXTRA_NETWORK_INFO);
				if (networkInfo.getType() == ConnectivityManager.TYPE_WIFI
						&& !networkInfo.isConnected()) {
					Log.d(TAG, "onReceive: ! networkInfo.isConnected()");
					if (getState().equals(OnboardingState.PERSONAL_AP_CONFIGURED_VALIDATED)) {
						setState(OnboardingState.PERSONAL_AP_CONFIGURED_RETRY);
						reconnect();
					} else if (getState().equals(OnboardingState.PERSONAL_AP_CONFIGURED_VALIDATING)) {
						setState(OnboardingState.PERSONAL_AP_CONFIGURED_ERROR);
					}
				}
			} else if (intent.getAction().equals(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION)) {
			}
		}

	}

	/* (non-Javadoc)
	 * @see org.alljoyn.services.common.ServiceCommonImpl#getBusObjectDescriptions()
	 */
	@Override
	public List<BusObjectDescription> getBusObjectDescriptions() 
	{
		// add self. announcer isn't ready yet, so no announcement will go
		List<BusObjectDescription> addBusObjectDescriptions = new ArrayList<BusObjectDescription>(2);
		BusObjectDescription ajAboutBusObjectDescription = new BusObjectDescription();
		ajAboutBusObjectDescription.setPath(OnboardingTransport.OBJ_PATH);
		ajAboutBusObjectDescription.setInterfaces(new String[]{OnboardingTransport.INTERFACE_NAME});
		addBusObjectDescriptions.add(ajAboutBusObjectDescription);
		return addBusObjectDescriptions;
	}
	
	/**
	 * @param hex hexadecimal password
	 * @return an ASCII format
	 */
	public String convertHexToString(String hex){

		if(hex == null || hex.length() % 2 != 0)
			return "";

		StringBuilder sb = new StringBuilder();

		//split into two characters
		for(int i=0; i < hex.length()-1; i+=2 ){

			//grab the hex in pairs
			String output = hex.substring(i, (i + 2));
			//convert hex to decimal
			int decimal = Integer.parseInt(output, 16);
			//convert the decimal to character
			sb.append((char)decimal);
		}
		return sb.toString();
	}
	
	/**
	 * Overcomes an Android issue where sometimes network SSID are surrounded with '"', hence failing equals() 
	 * @param ssid1
	 * @param ssid2
	 * @return
	 */
	private boolean isSsidEquals(String ssid1, String ssid2)
	{
		if (ssid1 == null || ssid1.length() == 0 || ssid2 == null || ssid2.length() == 0){
			return false;
		}
		ssid1 = ssid1.replace("\"", "");
		ssid2 = ssid2.replace("\"", "");
		return ssid1.equals(ssid2);
	}
	
	/**
	 * Map a capabilities string from Android's scan to AllJoyn Onboarding service AuthType enum
	 * @param capabilities the AP's capabilities screen. Authentication
	 * @return AuthType
	 * @see AuthType
	 */
	private AuthType getScanResultSecurity(String capabilities) {
	    
		Log.i(TAG, "* getScanResultSecurity");
	    if (capabilities.contains(AuthType.WEP.name())) {
	    	return AuthType.WEP;
	    }
	    else if (capabilities.contains("WPA")) {
	    	if (capabilities.contains("WPA2"))
		    	return AuthType.WPA2_AUTO;
	    	else
	    		return AuthType.WPA_AUTO;
	    }
	    else if (capabilities.contains(AuthType.WPS.name())) {
	    	return AuthType.WPS;
	    }   
		return AuthType.OPEN;

	}



}// OnboardingServiceImpl
