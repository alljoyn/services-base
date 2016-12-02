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

package org.alljoyn.onboarding.test;

/**
 * This class provides all the intent actions and
 * extras that the onboarding application can send.
 */
public class Keys {

    public static final String actionPrefix = new Keys().getClass().getPackage().getName() + ".";

    static class Actions {

        /**
         * An action indicating an error has occurred.
         * Has an extra string called Keys.Extras.EXTRA_ERROR provides the error msg.
         */
        public static final String ACTION_ERROR = actionPrefix + "ACTION_ERROR";

        /**
         * An action indicating a new device was found.
         * Has an extra string called Keys.Extras.EXTRA_DEVICE_ID provides the device id.
         */
        public static final String ACTION_DEVICE_FOUND = actionPrefix + "ACTION_DEVICE_FOUND";

        /**
         * An action indicating a device was lost.
         * Has an extra string called Keys.Extras.EXTRA_BUS_NAME provides the device bus name.
         */
        public static final String ACTION_DEVICE_LOST = actionPrefix + "ACTION_DEVICE_LOST";

        /**
         * An action indicating we are connected to a network. Has an extra string
         * called Keys.Extras.EXTRA_NETWORK_SSID provides the network ssid.
         * Id we are connecting/connected to a network, the EXTRA_NETWORK_SSID indicates that.
         */
        public static final String ACTION_CONNECTED_TO_NETWORK = actionPrefix + "ACTION_CONNECTED_TO_NETWORK";

        /**
         * An action indicating the password we gave do not match to the device password.
         */
        public static final String ACTION_PASSWORD_IS_INCORRECT = actionPrefix + "ACTION_PASSWORD_IS_INCORRECT";
    }

    class Extras {

        /**
         * The lookup key for a String object indicating the error message that occurred.
         * Retrieve with getStringExtra(String).
         */
        public static final String EXTRA_ERROR = "extra_error";

        /**
         * The lookup key for a String object indicating a device id.
         * Retrieve with intent.getExtras().getString(String).
         * To get the device itself call getApplication().getDevice(String);
         */
        public static final String EXTRA_DEVICE_ID = "extra_deviceId";

        /**
         * The lookup key for a String object indicating a device bus name.
         * Retrieve with getExtras().getString(String).
         */
        public static final String EXTRA_BUS_NAME = "extra_BusName";

        /**
         * The lookup key for a String object indicating the network ssid to which we are connected.
         * Retrieve with intent.getStringExtra(String).
         */
        public static final String EXTRA_NETWORK_SSID = "extra_network_ssid";

    }

}