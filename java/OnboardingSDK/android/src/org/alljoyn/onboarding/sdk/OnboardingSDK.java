/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
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

package org.alljoyn.onboarding.sdk;

import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;

import org.alljoyn.about.AboutKeys;
import org.alljoyn.about.AboutService;
import org.alljoyn.about.transport.AboutTransport;
import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusException;
import org.alljoyn.bus.Status;
import org.alljoyn.bus.Variant;
import org.alljoyn.onboarding.OnboardingService.AuthType;
import org.alljoyn.onboarding.client.OnboardingClient;
import org.alljoyn.onboarding.client.OnboardingClientImpl;
import org.alljoyn.onboarding.sdk.DeviceResponse.ResponseCode;
import org.alljoyn.onboarding.transport.OnboardingTransport;
import org.alljoyn.services.common.AnnouncementHandler;
import org.alljoyn.services.common.BusObjectDescription;
import org.alljoyn.services.common.ClientBase;
import org.alljoyn.services.common.ServiceAvailabilityListener;
import org.alljoyn.services.common.utils.TransportUtil;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiConfiguration;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.Log;
import android.util.Pair;

/**
 * Streamlines the process of onboarding for Android application developers.<br>
 * The SDK encapsulates the Wi-Fi and AllJoyn sessions that are part of the process.<br>
 *  The Onboarding SDK API provides the following :
 *
 * <ul>
 *      <li>Discovery of potential target networks,Discovery of potential onboardees,by calling {@link #scanWiFi()}
 *      <li>A single call, {@link #runOnboarding(OnboardingConfiguration)}, for start the flow of onboarding process
 *      <li>A single call ,{@link #runOffboarding(OffboardingConfiguration)}, for starting the flow of offboarding process
 * </ul>
 *
 * <P>
 * The onboarding flow works as follows:
 * <P>
 * The SDK uses a state machine handled by Andorid's handler
 * {@link #stateHandler} using the {@link #onHandleCommandMessage} function to
 * handle state changes.
 *  <ul>
 *      <li> IDLE state moves to  CONNECTING_TO_ONBOARDEE when starting the onboarding process
 *      <li> CONNECTING_TO_ONBOARDEE state moves to WAITING_FOR_ONBOARDEE_ANNOUNCEMENT after Wi-Fi connection has been established with onboardee device.
 *      <li> WAITING_FOR_ONBOARDEE_ANNOUNCEMENT state moves to state ONBOARDEE_ANNOUNCEMENT_RECEIVED after a valid announce message has been received.
 *      <li> ONBOARDEE_ANNOUNCEMENT_RECEIVED state moves to CONFIGURING_ONBOARDEE if the onboardee supports onboarding service.
 *      <li> CONFIGURING_ONBOARDEE state moves to  CONNECTING_TO_TARGET_WIFI_AP if passing the target credentials to the onboardee was successful.
 *      <li> CONNECTING_TO_TARGET_WIFI_AP moves to WAITING_FOR_TARGET_ANNOUNCE after Wi-Fi connection has been established with target .
 *      <li> WAITING_FOR_TARGET_ANNOUNCE moves to TARGET_ANNOUNCEMENT_RECEIVED after after a valid announce message has been received.
 *  </ul>
 *
 *  <P>The SDK receives events from external resources
 *     <ul>
 *      <li>  OnboardingSDKWifiManager {@link #onboardingClient} that broadcasts Wi-Fi success and failure to connect to a desired Wi-Fi device.
 *      <li>  onAnnouncement callback which handles AllJoyn AboutService announcements.
 *    </ul>
 *
 * <P>
 *   The SDK uses intents to report status changes and errors during onboarding/offboarding process.
 *   <ul>
 *      <li> {@link #STATE_CHANGE_ACTION} action with extra
 *         <ul>
 *            <li>{@link #EXTRA_ONBOARDING_STATE} extra information of enum  {@link OnboardingState}
 *            <li>{@link #EXTRA_WIFI_AP_TYPE} extra information if enum  {@link WifiApType}
 *         </ul>
 *      <li>{@link #ERROR} action with extra
 *          <ul>
 *            <li>{@link #EXTRA_ERROR_DETAILS} extra information of enum {@link OnboardingErrorType}
 *            <li>{@link #EXTRA_WIFI_AP_TYPE}  extra information of enum {@link WifiApType}
 *         </ul>
 *    </ul>
 *
 *<P>View sample code for SDK usage
 */

public class OnboardingSDK {

    /**
     * Activity Action:WIFI has been connected
     */
    static final String WIFI_CONNECTED_BY_REQUEST_ACTION = "org.alljoyn.onboardingsdk.wifi.connection_by_request";

    /**
     * Activity Action:WIFI connection has timed out
     */
    static final String WIFI_TIMEOUT_ACTION = "org.alljoyn.onboardingsdk.wifi.time_out";

    /**
     * Activity Action:WIFI authentication has occurred
     */
    static final String WIFI_AUTHENTICATION_ERROR = "org.alljoyn.onboardingsdk.wifi.authentication_error";

    /**
     * The lookup key for WifiConfiguration details after connection request.
     */
    static final String EXTRA_WIFI_WIFICONFIGURATION = "org.alljoyn.intent_keys.WifiConfiguration";

    /**
     * The lookup key for list of onboardee access points
     */
    public static final String EXTRA_ONBOARDEES = "org.alljoyn.onboardingsdk.intent_keys.ONBOARDEES";

    /**
     * The lookup key for list of target access points
     */
    public static final String EXTRA_TARGETS = "org.alljoyn.onboardingsdk.intent_keys.TARGETS";

    /**
     * The lookup key for Onboarding state reported by the SDK
     */
    public static final String EXTRA_ONBOARDING_STATE = "org.alljoyn.onboardingsdk.intent_keys.onboardingState";

    /**
     * The lookup key for WIFI details reported by the SDK
     */
    public static final String EXTRA_WIFI_AP_TYPE = "org.alljoyn.onboardingsdk.intent_keys.wifiApType";

    /**
     * The lookup key for ERROR details reported by the SDK
     */
    public static final String EXTRA_ERROR_DETAILS = "org.alljoyn.onboardingsdk.intent_keys.error";

    /**
     * The lookup key for DEVICE_INFORMATION reported by the SDK
     */
    public static final String EXTRA_DEVICE_INFORMATION = "org.alljoyn.onboardingsdk.intent_keys.deviceInfo";

    /**
     * Activity Action: indicates that the WIFI scan has been completed
     */
    public static final String WIFI_SCAN_RESULTS_AVAILABLE_ACTION = "org.alljoyn.onboardingsdk.scan_result_available";

    /**
     * Activity Action: indicates state changes in the SDK
     */
    public static final String STATE_CHANGE_ACTION = "org.alljoyn.onboardingsdk.state_change";

    /**
     * Activity Action: indicates error encountered by the SDK
     */
    public static final String ERROR = "org.alljoyn.onboardingsdk.error";

    /**
     *These enumeration values are used to indicate possible errors
     *
     * see also
     * {@link #EXTRA_ERROR_DETAILS}
     */
    public static enum OnboardingErrorType {

        /**
         * WIFI authentication error
         */
        WIFI_AUTH(0),

        /**
         * WIFI connection request timed out
         */
        WIFI_TIMEOUT(1),

        /**
         * Announcement failed to arrive in due time from onboardee while
         * connected to onboardee WIFI
         */
        FIND_ONBOARDEE_TIMEOUT(2),

        /**
         * Failed to establish connection
         */
        CONNECTION_FAILED(3),

        /**
         * Failed to send WIFI credentials to onboardee
         */
        CONFIGURATION_FAILED(4),

        /**
         * Announcement failed to arrive in due time from onboardee while
         * connected to target WIFI
         */
        VERIFICATION_TIMEOUT(5),

        /**
         * Device doesn't support onboarding interface
         */
        ONBOARDING_NOT_SUPPORTED(6),

        /**
         * Failed to offboard the device
         */
        OFFBOARDING_FAILED(7),

        /**
         * Announce data is invald.
         */
        INVALID_ANNOUNCE_DATA(8),

        /**
         * SDK has encountered interanl error
         */
        INTERNAL_ERROR(9);

        private int value;

        private static String strings[] = { "WIFI_AUTH", "WIFI_TIMEOUT", "FIND_ONBOARDEE_TIMEOUT", "CONNECTION_FAILED", "CONFIGURATION_FAILED", "VERIFICATION_TIMEOUT", "ONBOARDING_NOT_SUPPORTED",
                "OFFBOARDING_FAILED", "INVALID_ANNOUNCE_DATA", "INTERNAL_ERROR" };

        private OnboardingErrorType(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public static OnboardingErrorType getOnboardingErrorTypeByValue(int value) {
            OnboardingErrorType retType = null;
            for (OnboardingErrorType type : OnboardingErrorType.values()) {
                if (value == type.getValue()) {
                    retType = type;
                    break;
                }
            }
            return retType;
        }

        public static OnboardingErrorType getOnboardingErrorTypeByString(String str) {
            OnboardingErrorType retType = null;
            for (int i = 0; i < strings.length; i++) {
                if (str.equals(strings[i])) {
                    return OnboardingErrorType.values()[i];
                }
            }
            return retType;
        }

        @Override
        public String toString() {
            return strings[value];
        }

    };


    /**
     * These enumeration values are used to indicate the type of the access
     * point used
     *
     * see also
     * {@link #EXTRA_WIFI_AP_TYPE}
     */
    public static enum WifiApType {

        /**
         * Onboardee device
         */
        ONBOARDEE(0),

        /**
         * Target device
         */
        TARGET(1),

        /**
         * Original device
         */
        ORIGINAL(2),

        /**
         * Other
         */
        OTHER(3);

        private int value;

        private static String strings[] = { "ONBOARDEE", "TARGET", "ORIGINAL", "OTHER", };

        private WifiApType(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public static WifiApType getWifiApTypeByValue(int value) {
            WifiApType retType = null;
            for (WifiApType type : WifiApType.values()) {
                if (value == type.getValue()) {
                    retType = type;
                    break;
                }
            }
            return retType;
        }

        public static WifiApType getWifiApTypeByString(String str) {
            WifiApType retType = null;
            for (int i = 0; i < strings.length; i++) {
                if (str.equals(strings[i])) {
                    return WifiApType.values()[i];
                }
            }
            return retType;
        }

        @Override
        public String toString() {
            return strings[value];
        }

    };


    /**
     * These enumeration values are used to indicate the current onboarding
     * state
     *
     * see also
     * {@link #EXTRA_ONBOARDING_STATE}
     * {@link #STATE_CHANGE_ACTION}
     */
    public static enum OnboardingState {

        /**
         * Connecting to onboardee Wi-Fi
         */
        CONNECTING_WIFI(0),

        /**
         * Connected to onboardee Wi-Fi
         */
        CONNECTED_WIFI(1),

        /**
         * Waiting for announcement from onboardee
         */
        FINDING_ONBOARDEE(2),

        /**
         * Announcement received from onboardee
         */
        FOUND_ONBOARDEE(3),

        /**
         * Creating AllJoyn session with onboardee
         */
        CONNECTING_ONBOARDEE(4),

        /**
         * AllJoyn session established with onboardee
         */
        CONNECTED_ONBOARDEE(5),

        /**
         * Sending target credentials to onboardee
         */
        CONFIGURING_ONBOARDEE(6),

        /**
         * Onboardee received target credentials
         */
        CONFIGURED_ONBOARDEE(7),

        /**
         * Connecting to WIFI target
         */
        CONNECTING_TARGET(8),

        /**
         * Wi-Fi connection with target established
         */
        CONNECTED_TARGET(9),

        /**
         * Wait for announcement from onboardee over target WIFI
         */
        VERIFYING_ONBOARDED(10),

        /**
         * Announcement from onboardee over target WIFI has been received
         */
        VERIFIED_ONBOARDED(11),

        /**
         * Abort has been completed
         */
        ABORT_COMPLETED(12);

        private int value;

        private static String strings[] = { "CONNECTING_WIFI", "CONNECTED_WIFI", "FINDING_ONBOARDEE", "FOUND_ONBOARDEE", "CONNECTING_ONBOARDEE", "CONNECTED_ONBOARDEE", "CONFIGURING_ONBOARDEE",
                "CONFIGURED_ONBOARDEE", "CONNECTING_TARGET", "CONNECTED_TARGET", "VERIFYING_ONBOARDED", "VERIFIED_ONBOARDED", "ABORT_COMPLETED" };

        private OnboardingState(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public static OnboardingState getOnboardingStateByValue(int value) {
            OnboardingState retType = null;
            for (OnboardingState type : OnboardingState.values()) {
                if (value == type.getValue()) {
                    retType = type;
                    break;
                }
            }
            return retType;
        }

        public static OnboardingState getOnboardingStateByString(String str) {
            OnboardingState retType = null;
            for (int i = 0; i < strings.length; i++) {
                if (str.equals(strings[i])) {
                    return OnboardingState.values()[i];
                }
            }
            return retType;
        }

        @Override
        public String toString() {
            return strings[value];
        }
    }


    /**
     * TAG for debug information
     */
    private final static String TAG = "OnBoardingSDK";

    /**
     * Default timeout for Wi-Fi connection {@value #DEFAULT_WIFI_CONNECTION_TIMEOUT} msec
     */
    public static final int DEFAULT_WIFI_CONNECTION_TIMEOUT = 20000;

    /**
     * Default timeout for waiting for
     * {@link AboutTransport#Announce(short, short, org.alljoyn.services.common.BusObjectDescription[], java.util.Map)}
     */
    public static final int DEFAULT_ANNOUNCEMENT_TIMEOUT = 25000;

    /**
     * OnboardingSDK singleton
     */
    private static OnboardingSDK onboardingSDK = null;

    /**
     * Application context
     */
    private Context context = null;

    /**
     * HandlerThread for the state machine looper
     */
    private static HandlerThread stateHandlerThread = new HandlerThread("OnboardingSDKLooper");

    /**
     * Handler for OnboardingSDK state changing messages.
     */
    private static Handler stateHandler = null;

    /**
     * Stores the AllJoynManagmentCallback object
     */
    private AllJoynManagmentCallback alljoynManagmentCallback = null;

    /**
     * Stores the OnboardingsdkWifiManager object
     */
    private OnboardingSDKWifiManager onboardingSDKWifiManager = null;

    /**
     * Stores the OnboardingConfiguration object
     */
    private OnboardingConfiguration onboardingConfiguration = null;

    /**
     * IntentFilter used to filter out intents of WIFI messages received from
     * OnboardingsdkWifiManager
     */
    private final IntentFilter wifiIntentFilter = new IntentFilter();

    /**
     * BroadcastReceiver for intents from OnboardingSDKWifiManager while running
     * the onboarding process.
     */
    private BroadcastReceiver onboardingWifiBroadcastReceiver = null;

    /**
     * BroadcastReceiver for intents from OnboardingsdkWifiManager while running
     * {@link #connectToNetwork(WiFiNetworkConfiguration, int)}.
     */
    private BroadcastReceiver connectToNetworkWifiBroadcastReceiver = null;

    /**
     * Stores the OnboardingClient object used to communicate with
     * OnboardingService.
     */
    private OnboardingClient onboardingClient = null;

    /**
     * Stores the BusAttachment needed for accessing Alljoyn framework.
     */
    private BusAttachment bus = null;

    /**
     * Timer used for managing announcement timeout
     */
    private Timer announcementTimeout = new Timer();

    /**
     * Stores the OnboardingSDK state machine state
     */
    private  State currentState = State.IDLE;

    /**
     * Stores the instance of About Service for registering Announce handler when needed.
     */
    private AboutService aboutService=null;

    /**
     *  Handles About Service announcements
     */
    private final AnnouncementHandler announcementHandler=new AnnouncementHandler() {
        @Override
        public void onAnnouncement(final String serviceName, final short port, final BusObjectDescription[] objectDescriptions, final Map<String, Variant> serviceMetadata) {
            Log.d(TAG, "onAnnouncement: received ");
            Map<String, Object> announceDataMap = null;
            try {
                announceDataMap = TransportUtil.fromVariantMap(serviceMetadata);
                if (announceDataMap == null) {
                    // ignoring error. will be handled by announcement timeout
                    Log.e(TAG, "onAnnouncement: invalid announcement");
                    return;
                }

            } catch (BusException e) {
                // ignoring error. will be handled by announcement timeout
                Log.e(TAG, "onAnnouncement: invalid announcement", e);
            }

            UUID uniqueId = (UUID) announceDataMap.get(AboutKeys.ABOUT_APP_ID);
            if (uniqueId == null) {
                Log.e(TAG, "onAnnouncement: received null device uuid!! ignoring.");
                return;
            } else {
                Log.e(TAG, "onAnnouncement: received UUID " + uniqueId);
            }

            switch (currentState) {

            case WAITING_FOR_ONBOARDEE_ANNOUNCEMENT:
                setState(State.ONBOARDEE_ANNOUNCEMENT_RECEIVED, new AnnounceData(serviceName, port, objectDescriptions, serviceMetadata));
                break;

            case WAITING_FOR_TARGET_ANNOUNCE:
                if (deviceData != null && deviceData.getAnnounceData() != null && deviceData.getAppUUID() != null) {
                    Log.e(TAG, "onAnnouncement: device  UUID " + deviceData.getAppUUID());
                    if (deviceData.getAppUUID().compareTo(uniqueId) == 0) {
                        setState(State.TARGET_ANNOUNCEMENT_RECEIVED, new AnnounceData(serviceName, port, objectDescriptions, serviceMetadata));
                    }
                }
                break;

            default:
                break;
            }
        }

        /**
         * Irrelevant to the process of onboarding
         */
        @Override
        public void onDeviceLost(String deviceName) {
            Log.d(TAG, "Received onDeviceLost for busName " + deviceName);
        }
    };


    /**
     * Stores information about the device to be onboarded
     */
    private DeviceData deviceData = null;
    /**
     * Internal class that stores information about the device to be onboarded
     * AnnounceData,appUUID
     */
    private static class DeviceData {

        public AnnounceData getAnnounceData() {
            return announceData;
        }

        public void setAnnounceData(AnnounceData announceData) throws BusException {
            this.announceData = announceData;
            Map<String, Object> announceDataMap = TransportUtil.fromVariantMap(announceData.getServiceMetadata());
            appUUID = (UUID) announceDataMap.get(AboutKeys.ABOUT_APP_ID);
        }

        private AnnounceData announceData = null;

        public UUID getAppUUID() {
            return appUUID;
        }

        private UUID appUUID = null;

    }


    /**
     * An internal class to store the Announcement received by the AboutService.
     */
    private static class AnnounceData {
        private final String serviceName;
        private final short port;
        private final BusObjectDescription[] objectDescriptions;
        private final Map<String, Variant> serviceMetadata;

        public String getServiceName() {
            return serviceName;
        }

        public BusObjectDescription[] getObjectDescriptions() {
            return objectDescriptions;
        }

        public Map<String, Variant> getServiceMetadata() {
            return serviceMetadata;
        }

        public short getPort() {
            return port;
        }

        public AnnounceData(String serviceName, short port, BusObjectDescription[] objectDescriptions, Map<String, Variant> serviceMetadata) {
            this.serviceName = serviceName;
            this.port = port;
            this.objectDescriptions = objectDescriptions;
            this.serviceMetadata = serviceMetadata;
        }
    }


    /**
     * These enumeration values are used to indicate the current internal state
     * of the OnboardingSDK state machine.
     */
    private static enum State {
        /**
         * Start state
         */
        IDLE(0),

        /**
         * Connecting to onboardee device Wi-Fi
         */
        CONNECTING_TO_ONBOARDEE(10),

        /**
         * Waiting for announcement on onboardee Wi-Fi
         */
        WAITING_FOR_ONBOARDEE_ANNOUNCEMENT(11),

        /**
         * Announcement received on onboardee Wi-Fi
         */
        ONBOARDEE_ANNOUNCEMENT_RECEIVED(12),

        /**
         * Join session with onboardee
         */
        JOINING_SESSION(13),

        /**
         * Configuring onboardee with target credentials
         */
        CONFIGURING_ONBOARDEE(14),

        /**
         * Connecting to target Wi-Fi AP
         */
        CONNECTING_TO_TARGET_WIFI_AP(20),

        /**
         * Waiting for announcement on target Wi-Fi from onboardee
         */
        WAITING_FOR_TARGET_ANNOUNCE(21),

        /**
         * Announcement received on target Wi-Fi from onboardee
         */
        TARGET_ANNOUNCEMENT_RECEIVED(22),

        /**
         * Aborting state ,temporary state used to Abort the onboarding process.
         */
        ABORTING(30),

        /**
         * Error connecting to onboardee device Wi-Fi
         */
        ERROR_CONNECTING_TO_ONBOARDEE(110),

        /**
         * Error waiting for announcement on onboardee Wi-Fi
         */
        ERROR_WAITING_FOR_ONBOARDEE_ANNOUNCEMENT(111),

        /**
         * Error announcement received on onboardee Wi-Fi
         */
        ERROR_ONBOARDEE_ANNOUNCEMENT_RECEIVED(112),

        /**
         * Error joining AllJoyn session
         */
        ERROR_JOINING_SESSION(113),

        /**
         * Error configuring onboardee with target credentials
         */
        ERROR_CONFIGURING_ONBOARDEE(114),

        /**
         * Error connecting to target Wi-Fi AP
         */
        ERROR_CONNECTING_TO_TARGET_WIFI_AP(120),

        /**
         * Error waiting for announcement on target Wi-Fi from onboardee
         */
        ERROR_WAITING_FOR_TARGET_ANNOUNCE(121);

        private int value;

        private State(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public static State getStateByValue(int value) {
            State retType = null;
            for (State type : State.values()) {
                if (value == type.getValue()) {
                    retType = type;
                    break;
                }
            }
            return retType;
        }
    }


    /**
     * @return instance of the OnBoardingSDK
     */
    public static OnboardingSDK getInstance() {
        if (onboardingSDK == null) {
            onboardingSDK = new OnboardingSDK();
        }
        return onboardingSDK;
    }


    private OnboardingSDK() {
        wifiIntentFilter.addAction(WIFI_CONNECTED_BY_REQUEST_ACTION);
        wifiIntentFilter.addAction(WIFI_TIMEOUT_ACTION);
        wifiIntentFilter.addAction(WIFI_AUTHENTICATION_ERROR);

        stateHandlerThread.start();
        stateHandler = new Handler(stateHandlerThread.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                onHandleCommandMessage(msg);
            }
        };
    }


    /**
     * Initialize the SDK singleton with the current application configuration.
     * Registers AnnouncementHandler to receive announcements
     *
     * @param context
     *            The application context
     * @param allJoynManagmentCallback
     *            a callback for invoking BusAttachment recycling.
     * @throws IllegalArgumentException
     *             if either of the parameters is null.
     * @throws IllegalStateException
     *             if already initialized.
     */
    public void init(Context context, AllJoynManagmentCallback allJoynManagmentCallback) throws IllegalArgumentException, IllegalStateException {
        if (context == null || allJoynManagmentCallback == null) {
            throw new IllegalArgumentException();
        }
        if (this.context != null || this.onboardingSDKWifiManager != null) {
            throw new IllegalStateException();
        }
        this.context = context;
        this.onboardingSDKWifiManager = new OnboardingSDKWifiManager(context);
        this.alljoynManagmentCallback = allJoynManagmentCallback;
    }


    /**
     * Update the SDK singleton with the current application configuration.
     *
     * @param aboutService
     * @param bus
     *            The AllJoyn BusAttachment
     *
     * @throws IllegalArgumentException
     *             if any of the parameters is null
     *
     */
    public void update(AboutService aboutService, BusAttachment bus) throws IllegalArgumentException {
        if (aboutService == null || bus == null) {
            throw new IllegalArgumentException();
        }
        this.bus = bus;
        this.aboutService=aboutService;
    }


    /**
     * Handle the CONNECT_TO_ONBOARDEE state.
     * Listen to WIFI intents from OnboardingsdkWifiManager. Requests from OnboardingsdkWifiManager to
     * connect to the Onboardee. If successful moves to the next state otherwise
     * sends an error intent and returns to IDLE state.
     */
    private void handleConnectToOnboardeeState() {
        final Bundle extras = new Bundle();
        onboardingWifiBroadcastReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {

                String action = intent.getAction();
                Log.d(TAG, "handleConnectToOnboardeeState onReceive action=" + action);

                if (action == null) {
                    return;
                }

                if (WIFI_CONNECTED_BY_REQUEST_ACTION.equals(action)) {
                    context.unregisterReceiver(onboardingWifiBroadcastReceiver);
                    extras.clear();
                    extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTED_WIFI.toString());
                    extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.ONBOARDEE.toString());
                    sendBroadcast(STATE_CHANGE_ACTION, extras);
                    setState(State.WAITING_FOR_ONBOARDEE_ANNOUNCEMENT);
                }

                if (WIFI_TIMEOUT_ACTION.equals(action)) {
                    context.unregisterReceiver(onboardingWifiBroadcastReceiver);
                    extras.clear();
                    extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.WIFI_TIMEOUT.toString());
                    extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.ONBOARDEE.toString());
                    sendBroadcast(ERROR, extras);
                    setState(State.ERROR_CONNECTING_TO_ONBOARDEE);
                }

                if (WIFI_AUTHENTICATION_ERROR.equals(action)) {
                    extras.clear();
                    extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.WIFI_AUTH.toString());
                    extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.ONBOARDEE.toString());
                    sendBroadcast(ERROR, extras);
                    context.unregisterReceiver(onboardingWifiBroadcastReceiver);
                    setState(State.ERROR_CONNECTING_TO_ONBOARDEE);
                }
            }
        };
        context.registerReceiver(onboardingWifiBroadcastReceiver, wifiIntentFilter);
        extras.clear();
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTING_WIFI.toString());
        extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.ONBOARDEE.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);
        onboardingSDKWifiManager.connectToWifiAP(onboardingConfiguration.getOnboardee().getSSID(), onboardingConfiguration.getOnboardee().getAuthType(), onboardingConfiguration.getOnboardee()
                .getPassword(), onboardingConfiguration.getOnboardeeConnectionTimeout());
    }


    /**
     * Handle the WAIT_FOR_ONBOARDEE_ANNOUNCE state.
     * Set a timer using {@link #startAnnouncementTimeout()}
     * Wait for an announcement which should arrive
     * from the onAnnouncement handler.
     */
    private void handleWaitForOnboardeeAnnounceState() {
        Bundle extras = new Bundle();
        alljoynManagmentCallback.reconnectToBus();
        extras.clear();
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.FINDING_ONBOARDEE.toString());
        extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.ONBOARDEE.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);
        if (!startAnnouncementTimeout()) {
            extras.clear();
            extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.INTERNAL_ERROR.toString());
            sendBroadcast(ERROR, extras);
            setState(State.ERROR_WAITING_FOR_ONBOARDEE_ANNOUNCEMENT);
        }
    }


    /**
     * Handle the ONBOARDEE_ANNOUNCEMENT_RECEIVED state.
     * Stop the announcement timeout
     * timer and check that the board supports the 'org.alljoyn.Onboarding'
     * interface if so move to next state otherwise return to IDLE state.
     *
     * @param announceData
     *            contains the information of the Announcement .
     */
    private void handleOnboardeeAnnouncementReceivedState(AnnounceData announceData) {
        stopAnnouncementTimeout();

        Bundle extras = new Bundle();
        extras.clear();
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.FOUND_ONBOARDEE.toString());
        extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.ONBOARDEE.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);

        if (isSeviceSupported(announceData.getObjectDescriptions(), OnboardingTransport.INTERFACE_NAME)) {
            Log.d(TAG, "handleOnboardeeAnnouncementReceivedState ONBOARDEE_ANNOUNCEMENT_RECEIVED supporting  org.alljoyn.Onboarding");
            deviceData = new DeviceData();
            try {
                deviceData.setAnnounceData(announceData);
                setState(State.JOINING_SESSION, announceData);
            } catch (BusException e) {
                Log.e(TAG, "handleOnboardeeAnnouncementReceivedState DeviceData.setAnnounceObject failed with BusException. ", e);
                extras.clear();
                extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.INVALID_ANNOUNCE_DATA.toString());
                sendBroadcast(ERROR, extras);
                setState(State.ERROR_ONBOARDEE_ANNOUNCEMENT_RECEIVED);
            }
        } else {
            extras.clear();
            extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.ONBOARDING_NOT_SUPPORTED.toString());
            sendBroadcast(ERROR, extras);
            setState(State.ERROR_ONBOARDEE_ANNOUNCEMENT_RECEIVED);
        }
    }


    /**
     * Handle the JOINING_SESSION state.
     * Handle AllJoyn session establishment with the device.
     *
     * @param announceData contains the information of the announcement
     */
    private void handleJoiningSessionState(AnnounceData announceData) {
        Bundle extras = new Bundle();
        extras.clear();
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTING_ONBOARDEE.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);

        ResponseCode response = establishSessionWithDevice(announceData).getStatus();
        if (response == ResponseCode.Status_OK) {
            extras.clear();
            extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTED_ONBOARDEE.toString());
            sendBroadcast(STATE_CHANGE_ACTION, extras);
            setState(State.CONFIGURING_ONBOARDEE);
        } else {
            extras.clear();
            extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.CONNECTION_FAILED.toString());
            sendBroadcast(ERROR, extras);
            setState(State.ERROR_JOINING_SESSION);
        }
    }


    /**
     * Handle the CONFIGURE_ONBOARDEE state.
     * Call onboardDevice to send target information to the board. in case
     * successful moves to next step else return to IDLE state.
     */
    private void handleConfigureOnboardeeState() {
        Bundle extras = new Bundle();
        if (onboardDevice().getStatus() == ResponseCode.Status_OK) {
            extras.clear();
            extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONFIGURED_ONBOARDEE.toString());
            sendBroadcast(STATE_CHANGE_ACTION, extras);
            setState(State.CONNECTING_TO_TARGET_WIFI_AP);
        } else {
            extras.clear();
            extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.CONFIGURATION_FAILED.toString());
            sendBroadcast(ERROR, extras);
            setState(State.ERROR_CONFIGURING_ONBOARDEE);
        }
    }


    /**
     * Handle the CONNECT_TO_TARGET state.
     * Listen to WIFI intents from OnboardingsdkWifiManager Requests from
     * OnboardingsdkWifiManager to connect to the Target. if successful moves to
     * the next state otherwise send error intent and returns to IDLE state.
     */
    private void handleConnectToTargetState() {
        final Bundle extras = new Bundle();
        onboardingWifiBroadcastReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {

                String action = intent.getAction();
                Log.d(TAG, "onReceive action=" + action);
                if (action != null) {

                    if (WIFI_CONNECTED_BY_REQUEST_ACTION.equals(action)) {
                        context.unregisterReceiver(onboardingWifiBroadcastReceiver);
                        extras.clear();
                        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTED_WIFI.toString());
                        extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.TARGET.toString());
                        sendBroadcast(STATE_CHANGE_ACTION, extras);
                        extras.clear();
                        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTED_TARGET.toString());
                        sendBroadcast(STATE_CHANGE_ACTION, extras);
                        setState(State.WAITING_FOR_TARGET_ANNOUNCE);
                    }

                    if (WIFI_TIMEOUT_ACTION.equals(action)) {
                        context.unregisterReceiver(onboardingWifiBroadcastReceiver);
                        extras.clear();
                        extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.WIFI_TIMEOUT.toString());
                        extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.TARGET.toString());
                        sendBroadcast(ERROR, extras);
                        setState(State.ERROR_CONNECTING_TO_TARGET_WIFI_AP);
                    }
                    if (WIFI_AUTHENTICATION_ERROR.equals(action)) {
                        context.unregisterReceiver(onboardingWifiBroadcastReceiver);
                        extras.clear();
                        extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.WIFI_AUTH.toString());
                        extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.TARGET.toString());
                        sendBroadcast(ERROR, extras);
                        setState(State.ERROR_CONNECTING_TO_TARGET_WIFI_AP);
                    }
                }
            }
        };// receiver
        context.registerReceiver(onboardingWifiBroadcastReceiver, wifiIntentFilter);
        extras.clear();
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTING_WIFI.toString());
        extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.TARGET.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);
        extras.clear();
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTING_TARGET.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);
        onboardingSDKWifiManager.connectToWifiAP(onboardingConfiguration.getTarget().getSSID(), onboardingConfiguration.getTarget().getAuthType(), onboardingConfiguration.getTarget().getPassword(),
                onboardingConfiguration.getTargetConnectionTimeout());
    }


    /**
     * Handle the WAIT_FOR_TARGET_ANNOUNCE state.
     * Set a timer with using startAnnouncementTimeout. waits for an
     * announcement which should arrive from the onAnnouncement handler.
     */
    private void handleWaitForTargetAnnounceState() {
        Bundle extras = new Bundle();
        extras.clear();
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.VERIFYING_ONBOARDED.toString());
        extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.TARGET.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);
        if (!startAnnouncementTimeout()) {
            extras.clear();
            extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.INTERNAL_ERROR.toString());
            sendBroadcast(ERROR, extras);
            setState(State.ERROR_WAITING_FOR_TARGET_ANNOUNCE);
        }
    }


    /**
     * Handls the TARGET_ANNOUNCEMENT_RECEIVED state.
     * <ul>
     *  <li>Call {@link #stopAnnouncementTimeout()}
     *  <li>Send a success broadcast.
     *  <li>Move state machine to IDLE state (onboarding process finished)
     *  <li>Enable all Wi-Fi access points that were disabled in the onboarding process.
     * </ul>
     *
     * @param announceData
     *            contains the information of the announcement .
     */
    private void handleTargetAnnouncementReceivedState(AnnounceData announceData) {
        stopAnnouncementTimeout();

        Bundle extras = new Bundle();
        extras.clear();
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.VERIFIED_ONBOARDED.toString());
        extras.putString(EXTRA_WIFI_AP_TYPE, WifiApType.TARGET.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);

        setState(State.IDLE);
        onboardingSDKWifiManager.enableAllWifiNetworks();
    }


    /**
     * Handle the the ABORTING state.
     * Due to the implementation of the state machine via hander.
     * Messages are received into the handler's queue ,and are handled serially ,
     * when ABORTING message is received handleAbortingState deletes all messages in the handlers queue .In case there are any messages there .
     * Due to the blocking nature of JOINING_SESSION state the ABORTING message can be handled only after it has completed!.
     */
    private void handleAbortingState(){
        // store current State in initalState
        State initalState=currentState;

        //set state to State.ABORTING
        currentState=State.ABORTING;

        // remove all queued up messages in the stateHandler
        for (State s : State.values()) {
            stateHandler.removeMessages(s.value);
        }

        if (initalState.getValue()>=State.ERROR_CONNECTING_TO_ONBOARDEE.getValue()){
            abortStateCleanUp();
        }


       // note ABORTING state can't be handled during JOINING_SESSION it is blocking!
        switch (initalState){

            case CONNECTING_TO_ONBOARDEE:
                context.unregisterReceiver(onboardingWifiBroadcastReceiver);
                abortStateCleanUp();
                break;

            case WAITING_FOR_ONBOARDEE_ANNOUNCEMENT:
                stopAnnouncementTimeout();
                abortStateCleanUp();
                break;

            case ONBOARDEE_ANNOUNCEMENT_RECEIVED:
                abortStateCleanUp();
                break;

            case CONFIGURING_ONBOARDEE:
                abortStateCleanUp();
                break;

            default:
                break;
        }
    }


    /**
     *Handle the state machine transition.
     *
     * @param msg
     */
    private void onHandleCommandMessage(Message msg) {
        if (msg == null)
            return;

        Log.d(TAG,"onHandleCommandMessage "+State.getStateByValue(msg.what).toString());
        switch (State.getStateByValue(msg.what)) {

        case IDLE:
            currentState = State.IDLE;
            break;

        case CONNECTING_TO_ONBOARDEE:
            currentState = State.CONNECTING_TO_ONBOARDEE;
            handleConnectToOnboardeeState();
            break;

        case WAITING_FOR_ONBOARDEE_ANNOUNCEMENT:
            currentState = State.WAITING_FOR_ONBOARDEE_ANNOUNCEMENT;
            handleWaitForOnboardeeAnnounceState();
            break;

        case ONBOARDEE_ANNOUNCEMENT_RECEIVED:
            currentState = State.ONBOARDEE_ANNOUNCEMENT_RECEIVED;
            handleOnboardeeAnnouncementReceivedState((AnnounceData) msg.obj);
            break;

        case JOINING_SESSION:
            currentState = State.JOINING_SESSION;
            handleJoiningSessionState((AnnounceData) msg.obj);
            break;

        case CONFIGURING_ONBOARDEE:
            currentState = State.CONFIGURING_ONBOARDEE;
            handleConfigureOnboardeeState();
            break;

        case CONNECTING_TO_TARGET_WIFI_AP:
            currentState = State.CONNECTING_TO_TARGET_WIFI_AP;
            handleConnectToTargetState();
            break;

        case WAITING_FOR_TARGET_ANNOUNCE:
            currentState = State.WAITING_FOR_TARGET_ANNOUNCE;
            alljoynManagmentCallback.reconnectToBus();
            handleWaitForTargetAnnounceState();
            break;

        case TARGET_ANNOUNCEMENT_RECEIVED:
            currentState = State.TARGET_ANNOUNCEMENT_RECEIVED;
            handleTargetAnnouncementReceivedState((AnnounceData) msg.obj);
            break;

        case ERROR_CONNECTING_TO_ONBOARDEE:
            currentState = State.ERROR_CONNECTING_TO_ONBOARDEE;
            break;

        case ERROR_WAITING_FOR_ONBOARDEE_ANNOUNCEMENT:
            currentState = State.ERROR_WAITING_FOR_ONBOARDEE_ANNOUNCEMENT;
            break;

        case ERROR_ONBOARDEE_ANNOUNCEMENT_RECEIVED:
            currentState = State.ERROR_ONBOARDEE_ANNOUNCEMENT_RECEIVED;
            break;

        case ERROR_JOINING_SESSION:
            currentState = State.ERROR_JOINING_SESSION;
            break;

        case ERROR_CONFIGURING_ONBOARDEE:
            currentState = State.ERROR_CONFIGURING_ONBOARDEE;
            break;

        case ERROR_CONNECTING_TO_TARGET_WIFI_AP:
            currentState = State.ERROR_CONNECTING_TO_TARGET_WIFI_AP;
            break;

        case ERROR_WAITING_FOR_TARGET_ANNOUNCE:
            currentState = State.ERROR_CONNECTING_TO_TARGET_WIFI_AP;
            break;

        case ABORTING:
            handleAbortingState();
            break;

        default:
            break;

        }
    }


    /**
     * Move the state machine to a new state.
     *
     * @param state
     */
    private void setState(State state) {
        Message msg = stateHandler.obtainMessage(state.getValue());
        stateHandler.sendMessage(msg);
    }


    /**
     * Move the state machine to a new state.
     *
     * @param state
     * @param data metadata to pass to the new state
     *
     */
    private void setState(State state, Object data) {
        Message msg = stateHandler.obtainMessage(state.getValue());
        msg.obj = data;
        stateHandler.sendMessage(msg);
    }


 /**
     * Establish an AllJoyn session with the device.
     * @param announceData the Announcement data.
     * @return status of operation.
     */
    private DeviceResponse establishSessionWithDevice(final AnnounceData announceData) {
        if (onboardingClient == null) {
            try {
                if (announceData.serviceName == null) {
                    return new DeviceResponse(ResponseCode.Status_ERROR, "announceData.serviceName == null");
                }
                if (announceData.getPort() == 0) {
                    return new DeviceResponse(ResponseCode.Status_ERROR, "announceData.getPort() == 0");
                }
                onboardingClient = new OnboardingClientImpl(announceData.getServiceName(), bus, new ServiceAvailabilityListener() {
                    @Override
                    public void connectionLost() {
                        // expected. we are onboarding the device, hence sending
                        // it the another network.
                        Log.d(TAG, "establishSessionWithDevice connectionLost");
                    }
                }, announceData.getPort());
            } catch (Exception e) {
                Log.e(TAG, "establishSessionWithDevice Exception: ", e);
                return new DeviceResponse(ResponseCode.Status_ERROR);
            }
        }
        try {
            ResponseCode connectToDeviceStatus = connectToDevice(onboardingClient).getStatus();
            if (connectToDeviceStatus != ResponseCode.Status_OK) {
                return new DeviceResponse(ResponseCode.Status_ERROR_CANT_ESTABLISH_SESSION, connectToDeviceStatus.name());
            }
            return new DeviceResponse(ResponseCode.Status_OK);
        } catch (Exception e) {
            Log.e(TAG, "establishSessionWithDevice ", e);
            return new DeviceResponse(ResponseCode.Status_ERROR);
        }
    }


    /**
     * Call the OnboardingService API for passing the onboarding configuration
     * to the device.
     *
     * @return status of operation.
     */
    private DeviceResponse onboardDevice() {
        try {
            AuthType authType = onboardingConfiguration.getTarget().getAuthType();
            boolean isPasswordHex = false;
            String passForConfigureNetwork = onboardingConfiguration.getTarget().getPassword();
            if (authType == AuthType.WEP) {
                Pair<Boolean, Boolean> wepCheckResult = OnboardingSDKWifiManager.checkWEPPassword(passForConfigureNetwork);
                isPasswordHex = wepCheckResult.second;
            }
            Log.d(TAG, "onBoardDevice OnboardingClient isPasswordHex " + isPasswordHex);
            if (!isPasswordHex) {
                passForConfigureNetwork = toHexadecimalString(onboardingConfiguration.getTarget().getPassword());
                Log.i(TAG, "convert pass to hex: from " + onboardingConfiguration.getTarget().getPassword() + " -> to " + passForConfigureNetwork);
            }
            Log.i(TAG, "before configureWiFi networkName = " + onboardingConfiguration.getTarget().getSSID() + " networkPass = " + passForConfigureNetwork + " selectedAuthType = "
                    + onboardingConfiguration.getTarget().getAuthType().getTypeId());
            onboardingClient.configureWiFi(onboardingConfiguration.getTarget().getSSID(), passForConfigureNetwork, onboardingConfiguration.getTarget().getAuthType().getTypeId());
            onboardingClient.connectWiFi();
            return new DeviceResponse(ResponseCode.Status_OK);
        } catch (BusException e) {
            Log.e(TAG, "onboarddDevice ", e);
            return new DeviceResponse(ResponseCode.Status_ERROR);
        } catch (Exception e) {
            Log.e(TAG, "onboarddDevice ", e);
            return new DeviceResponse(ResponseCode.Status_ERROR);
        }
    }


    /**
     * Call the offboardDevice API for offboarding the device.
     *
     * @param serviceName
     *            device's service name
     * @param port
     *            device's application port
     * @return result of action
     */
    private DeviceResponse offboardDevice(String serviceName, short port) {
        Bundle extras = new Bundle();
        extras.putString(EXTRA_DEVICE_INFORMATION, serviceName);
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTING_ONBOARDEE.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);

        if (onboardingClient == null) {
            try {
                onboardingClient = new OnboardingClientImpl(serviceName, bus, new ServiceAvailabilityListener() {
                    @Override
                    public void connectionLost() {
                        // expected. we are offboarding the device...
                        Log.d(TAG, "offboardDevice connectionLost");
                    }
                }, port);
            } catch (Exception e) {
                Log.e(TAG, "offboardDevice Exception: ", e);
                return new DeviceResponse(ResponseCode.Status_ERROR);
            }
        }

        extras.clear();
        extras.putString(EXTRA_DEVICE_INFORMATION, serviceName);
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTED_ONBOARDEE.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);

        try {
            ResponseCode connectToDeviceStatus = connectToDevice(onboardingClient).getStatus();
            if (connectToDeviceStatus != ResponseCode.Status_OK) {
                return new DeviceResponse(ResponseCode.Status_ERROR_CANT_ESTABLISH_SESSION, connectToDeviceStatus.name());
            }

            extras.clear();
            extras.putString(EXTRA_DEVICE_INFORMATION, serviceName);
            extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONFIGURING_ONBOARDEE.toString());
            sendBroadcast(STATE_CHANGE_ACTION, extras);

            onboardingClient.offboard();

            extras.clear();
            extras.putString(EXTRA_DEVICE_INFORMATION, serviceName);
            extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONFIGURED_ONBOARDEE.toString());
            sendBroadcast(STATE_CHANGE_ACTION, extras);

            return new DeviceResponse(ResponseCode.Status_OK);
        } catch (BusException e) {
            Log.e(TAG, "offboardDevice ", e);
            return new DeviceResponse(ResponseCode.Status_ERROR);
        } catch (Exception e) {
            Log.e(TAG, "offboardDevice ", e);
            return new DeviceResponse(ResponseCode.Status_ERROR);
        }
    }


    /**
     * Start an AllJoyn session with another Alljoyn device.
     *
     * @param client
     * @return status of operation.
     */
    private DeviceResponse connectToDevice(ClientBase client) {
        if (client == null) {
            return new DeviceResponse(ResponseCode.Status_ERROR, "fail connect to device, client == null");
        }
        if (client.isConnected()) {
            return new DeviceResponse(ResponseCode.Status_OK);
        }

        Status status = client.connect();
        switch (status) {
        case OK:
            Log.d(TAG, "connectToDevice. Join Session OK");
            return new DeviceResponse(ResponseCode.Status_OK);
        case ALLJOYN_JOINSESSION_REPLY_ALREADY_JOINED:
            Log.d(TAG, "connectToDevice: Join Session returned ALLJOYN_JOINSESSION_REPLY_ALREADY_JOINED. Ignoring");
            return new DeviceResponse(ResponseCode.Status_OK);
        case ALLJOYN_JOINSESSION_REPLY_FAILED:
        case ALLJOYN_JOINSESSION_REPLY_UNREACHABLE:
            Log.e(TAG, "connectToDevice: Join Session returned ALLJOYN_JOINSESSION_REPLY_FAILED.");
            return new DeviceResponse(ResponseCode.Status_ERROR_CANT_ESTABLISH_SESSION, "device unreachable");
        default:
            Log.e(TAG, "connectToDevice: Join session returned error: " + status.name());
            return new DeviceResponse(ResponseCode.Status_ERROR, "Failed connecting to device");
        }
    }


    /**
     * Convert a string in ASCII format to HexAscii.
     *
     * @param pass
     *            password to convert
     * @return HexAscii of the input.
     */
    private static String toHexadecimalString(String pass) {
        char[] HEX_CODE = "0123456789ABCDEF".toCharArray();
        byte[] data;
        try {
            data = pass.getBytes("UTF-8");
        } catch (UnsupportedEncodingException e) {
            Log.e(TAG, "Failed getting bytes of passcode by UTF-8", e);
            data = pass.getBytes();
        }
        StringBuilder r = new StringBuilder(data.length * 2);
        for (byte b : data) {
            r.append(HEX_CODE[(b >> 4) & 0xF]);
            r.append(HEX_CODE[(b & 0xF)]);
        }
        return r.toString();
    }


    /**
     * Start a wifi scan.
     * Sends the following possible intents
     *   <ul>
     *      <li>{@link #WIFI_SCAN_RESULTS_AVAILABLE_ACTION} action with this extra
     *         <ul>
     *            <li>{@link #EXTRA_ONBOARDEES} extra information containg ArrayList of {@link WiFiNetwork}
     *            <li>{@link #EXTRA_TARGETS} extra information containg ArrayList of {@link WiFiNetwork}
     *         </ul>
     *    <ul>
     */
    public void scanWiFi() {
        onboardingSDKWifiManager.scan();
    }


    /**
     * @return a list of Wi-Fi networks to which the SDK can onboard a device.
     */
    public List<WiFiNetwork> getCandidateTargetNetworks() {
        return onboardingSDKWifiManager.getNonOnboardableAccessPoints();
    }


    /**
     * @return a list of networks that the SDK can onboard .
     */
    public List<WiFiNetwork> getOnboardableDevices() {
        return onboardingSDKWifiManager.getOnboardableAccessPoints();
    }


    /**
     * @return the current Wi-Fi network that the Android device is connected to.
     */
    public WiFiNetwork getCurrentNetwork() {
        return onboardingSDKWifiManager.getCurrentConnectedAP();
    }


    /**
     * Connect the Android device to a WIFI network.
     * Sends the following possible intents
     *   <ul>
     *      <li>{@link #STATE_CHANGE_ACTION} action with this extra
     *         <ul>
     *            <li>{@link #EXTRA_ONBOARDING_STATE} extra information of enum  {@link OnboardingState}
     *         </ul>
     *      <li> {@link #ERROR} action with this extra
     *          <ul>
     *            <li>{@link #EXTRA_ERROR_DETAILS} extra information of enum {@link OnboardingErrorType}
     *         </ul>
     *    <ul>
     * @param network
     *            contains detailed data how to connect to the WIFI network.
     * @param connectionTimeout
     *            timeout in Msec to complete the task of connecting to a Wi-Fi
     *            network
     */
    public void connectToNetwork(final WiFiNetworkConfiguration network, int connectionTimeout) {
        connectToNetworkWifiBroadcastReceiver = new BroadcastReceiver() {

            @Override
            public void onReceive(Context arg0, Intent intent) {
                Bundle extras = new Bundle();
                String action = intent.getAction();
                Log.d(TAG, "onReceive action=" + action);

                if (WIFI_CONNECTED_BY_REQUEST_ACTION.equals(action)) {
                    context.unregisterReceiver(connectToNetworkWifiBroadcastReceiver);
                    if (intent.hasExtra(EXTRA_WIFI_WIFICONFIGURATION)) {
                        WifiConfiguration config = (WifiConfiguration) intent.getParcelableExtra(EXTRA_WIFI_WIFICONFIGURATION);

                        if (OnboardingSDKWifiManager.normalizeSSID(network.getSSID()).equals(OnboardingSDKWifiManager.normalizeSSID(config.SSID))) {
                            extras.clear();
                            extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTED_WIFI.toString());
                            sendBroadcast(STATE_CHANGE_ACTION, extras);
                            return;
                        }
                        extras.clear();
                        extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.WIFI_TIMEOUT.toString());
                        sendBroadcast(ERROR, extras);
                    }
                }

                if (WIFI_TIMEOUT_ACTION.equals(action)) {
                    context.unregisterReceiver(connectToNetworkWifiBroadcastReceiver);
                    extras.clear();
                    extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.WIFI_TIMEOUT.toString());
                    sendBroadcast(ERROR, extras);
                }
                if (WIFI_AUTHENTICATION_ERROR.equals(action)) {
                    context.unregisterReceiver(connectToNetworkWifiBroadcastReceiver);
                    extras.clear();
                    extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.WIFI_AUTH.toString());
                    sendBroadcast(ERROR, extras);
                }

            }

        };
        context.registerReceiver(connectToNetworkWifiBroadcastReceiver, wifiIntentFilter);
        Bundle extras = new Bundle();

        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.CONNECTING_WIFI.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);

        if (connectionTimeout < 0) {
            connectionTimeout = DEFAULT_WIFI_CONNECTION_TIMEOUT;
        }

        onboardingSDKWifiManager.connectToWifiAP(network.getSSID(), network.getAuthType(), network.getPassword(), connectionTimeout);
    }


    /**
     * Start and resume the onboarding process.
     * Send these possible intents
     *    <ul>
     *      <li>{@link #STATE_CHANGE_ACTION}  action with this extra
     *         <ul>
     *          <li>{@link #EXTRA_ONBOARDING_STATE} extra information of enum  {@link OnboardingState}
     *          <li>{@link #EXTRA_WIFI_AP_TYPE} extra information of  enum  {@link WifiApType}
     *        </ul>
     *      <li> {@link #ERROR} action with this extra
     *        <ul>
     *           <li>{@link #EXTRA_ERROR_DETAILS}  extra information of enum {@link OnboardingErrorType}
     *           <li>{@link #EXTRA_WIFI_AP_TYPE}   extra information of enum  {@link WifiApType}
     *        </ul>
     *    </ul>
     *
     *
     * can resume the onboarding process in case one of the internal errors has occoured
     *    <ul>
     *        <li>ERROR_CONNECTING_TO_ONBOARDEE
     *        <li>ERROR_WAITING_FOR_ONBOARDEE_ANNOUNCEMENT
     *        <li>ERROR_JOINING_SESSION
     *        <li>ERROR_CONFIGURING_ONBOARDEE
     *        <li>ERROR_CONNECTING_TO_TARGET_WIFI_AP
     *        <li>ERROR_WAITING_FOR_TARGET_ANNOUNCE
     *    </ul>
     *
     * @param config containing information about onboardee and target networks.
     * @throws IllegalStateException in case onboarding is arleady running or trying to resume from internal state ERROR_ONBOARDEE_ANNOUNCEMENT_RECEIVED.
     */
    public void runOnboarding(OnboardingConfiguration config) throws IllegalStateException {
        onboardingConfiguration = config;
        if (currentState == State.IDLE) {
            aboutService.addAnnouncementHandler(announcementHandler);
            setState(State.CONNECTING_TO_ONBOARDEE);
        } else if (currentState.getValue() >= State.ERROR_CONNECTING_TO_ONBOARDEE.getValue()) {

            switch (currentState) {

            case ERROR_CONNECTING_TO_ONBOARDEE:
                setState(State.CONNECTING_TO_ONBOARDEE);
                break;

            case ERROR_WAITING_FOR_ONBOARDEE_ANNOUNCEMENT:
                setState(State.WAITING_FOR_ONBOARDEE_ANNOUNCEMENT);
                break;

            case ERROR_ONBOARDEE_ANNOUNCEMENT_RECEIVED:
                throw new IllegalStateException("The device doesn't comply with onboarding service");

            case ERROR_JOINING_SESSION:
                setState(State.JOINING_SESSION, deviceData.getAnnounceData());
                break;

            case ERROR_CONFIGURING_ONBOARDEE:
                setState(State.CONFIGURING_ONBOARDEE);
                break;

            case ERROR_CONNECTING_TO_TARGET_WIFI_AP:
                setState(State.CONNECTING_TO_TARGET_WIFI_AP);
                break;

            case ERROR_WAITING_FOR_TARGET_ANNOUNCE:
                setState(State.WAITING_FOR_TARGET_ANNOUNCE);
                break;

            default:
                break;
            }
        } else {
            throw new IllegalStateException("onboarding process is already running");
        }
    }


  /**
   * Abort the onboarding process.
   * Send the following possible intents
     *   <ul>
     *      <li>{@link #STATE_CHANGE_ACTION} action with this extra
     *         <ul>
     *            <li>{@link #EXTRA_ONBOARDING_STATE} extra information {@link OnboardingState#ABORT_COMPLETED}
     *         </ul>
     *    </ul>
     *
   * @throws IllegalStateException in case the state machine is in state IDLE,ABORTING (No need to Abort)
   *    in case the state machine is in state CONNECTING_TO_TARGET_WIFI_AP,WAITING_FOR_TARGET_ANNOUNCE,TARGET_ANNOUNCEMENT_RECEIVED (can't abort ,in final stages of onboarding)
   */
    public void abortOnboarding() throws IllegalStateException {
        if (currentState == State.IDLE ||
            currentState == State.ABORTING ){
            throw new IllegalStateException("Can't abort ,already ABORTED");
        }

        if (currentState == State.CONNECTING_TO_TARGET_WIFI_AP ||
            currentState == State.WAITING_FOR_TARGET_ANNOUNCE ||
            currentState ==State.TARGET_ANNOUNCEMENT_RECEIVED){
            throw new IllegalStateException("Can't abort");
        }
        setState(State.ABORTING);
    }


    /**
     * Prepare the state machine for the onboarding process after abort has beeen requested.
     * Does the following :
     *  <ul>
     *      <li> Stop listening to About Service announcements.
     *      <li> Remove onboardee Wi-Fi access point from Android's Wi-Fi configured netwroks.
     *      <li> Enable  Android's Wi-Fi manager to select a suitable  Wi-Fi access point.
     *      <li> Move state maching to IDLE state.
     *      <li> Send intent that abort reqest has beeen completed
     *  <ul>
     *
     */
    private void abortStateCleanUp(){
        aboutService.removeAnnouncementHandler(announcementHandler);
        if (onboardingConfiguration!=null && onboardingConfiguration.getOnboardee()!=null && onboardingConfiguration.getOnboardee().getSSID()!=null){
            onboardingSDKWifiManager.removeWifiAP(onboardingConfiguration.getOnboardee().getSSID());
        }
        onboardingSDKWifiManager.enableAllWifiNetworks();
        setState(State.IDLE);
        Bundle extras =new Bundle();
        extras.putString(EXTRA_ONBOARDING_STATE, OnboardingState.ABORT_COMPLETED.toString());
        sendBroadcast(STATE_CHANGE_ACTION, extras);
    }


    /**
     *
     * Offboard a device that is on the current Wi-Fi network.
     *
     * @param config contains the offboarding information needed to complete the task.
     * @throws IllegalStateException is thrown when config is not valid
     * @throws IllegalArgumentException is thrown when not in internal IDLE state.
     */
    public void runOffboarding(OffboardingConfiguration config) throws IllegalStateException, IllegalArgumentException {
        // verify that the OffboardingConfiguration has valid data
        if (config == null || config.getServiceName() == null || config.getServiceName().isEmpty() || config.getPort() == 0) {
            throw new IllegalArgumentException();
        }
        // in case the SDK is in onboarding mode the runOffboarding can't
        // continue
        if (currentState != State.IDLE) {
            throw new IllegalStateException("onboarding process is already running");
        }
        DeviceResponse deviceResponse = offboardDevice(config.getServiceName(), config.getPort());
        if (deviceResponse.getStatus() != ResponseCode.Status_OK) {
            Bundle extras = new Bundle();
            extras.putString(EXTRA_DEVICE_INFORMATION, config.getServiceName());
            extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.OFFBOARDING_FAILED.toString());
            sendBroadcast(ERROR, extras);
        }
    }


    /**
     * Check if the device supports the given service.
     *
     * @param objectDescriptions
     *            the list of supported services as announced by the device.
     * @param service
     *            name of the service to check
     * @return true if supported else false
     */
    private boolean isSeviceSupported(final BusObjectDescription[] objectDescriptions, String service) {
        if (objectDescriptions != null) {
            for (int i = 0; i < objectDescriptions.length; i++) {
                String[] interfaces = objectDescriptions[i].getInterfaces();
                for (int j = 0; j < interfaces.length; j++) {
                    String currentInterface = interfaces[j];
                    if (currentInterface.startsWith(service))
                        return true;
                }
            }
        }
        return false;
    }


    /**
     * Start a timeout announcement to arrive from a device. Takes the timeout
     * interval from the {@link OnboardingConfiguration} that stores the data.
     * If timeout expires, moves the state machine to idle state and sends
     * timeout intent.
     *
     * @return true if in correct state else false.
     */
    private boolean startAnnouncementTimeout() {
        long timeout = 0;
        switch (currentState) {

        case WAITING_FOR_ONBOARDEE_ANNOUNCEMENT:
            timeout = onboardingConfiguration.getOnboardeeAnnoucementTimeout();
            break;

        case WAITING_FOR_TARGET_ANNOUNCE:
            timeout = onboardingConfiguration.getTargetAnnoucementTimeout();
            break;

        default:
            Log.e(TAG, "startAnnouncementTimeout has been intialized in bad state abort");
            return false;

        }

        announcementTimeout.schedule(new TimerTask() {

            Bundle extras = new Bundle();

            @Override
            public void run() {

                Log.e(TAG, "Time out expired  " + currentState.toString());
                switch (currentState) {

                case WAITING_FOR_ONBOARDEE_ANNOUNCEMENT:
                    extras.clear();
                    extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.FIND_ONBOARDEE_TIMEOUT.toString());
                    sendBroadcast(ERROR, extras);
                    setState(State.ERROR_WAITING_FOR_ONBOARDEE_ANNOUNCEMENT);
                    break;

                case WAITING_FOR_TARGET_ANNOUNCE:
                    extras.clear();
                    extras.putString(EXTRA_ERROR_DETAILS, OnboardingErrorType.VERIFICATION_TIMEOUT.toString());
                    sendBroadcast(ERROR, extras);
                    setState(State.ERROR_WAITING_FOR_TARGET_ANNOUNCE);
                    break;

                default:
                    break;

                }
            }
        }, timeout);
        return true;
    }


    /**
     * Stop the announcement timeout that was activated by
     * {@link #startAnnouncementTimeout()}
     */
    private void stopAnnouncementTimeout() {
        announcementTimeout.cancel();
        announcementTimeout.purge();
        announcementTimeout = new Timer();
    }


    /**
     * A wrapper method that sends intent broadcasts with extra
     * data
     *
     * @param action
     *            an action for the intent
     * @param extras
     *            extras for the intent
     */
    private void sendBroadcast(String action, Bundle extras) {
        Intent intent = new Intent(action);
        if (extras != null && !extras.isEmpty()) {
            intent.putExtras(extras);
        }
        context.sendBroadcast(intent);
    }

}
