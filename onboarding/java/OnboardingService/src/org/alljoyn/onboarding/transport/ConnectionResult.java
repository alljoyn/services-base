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

package org.alljoyn.onboarding.transport;

/**
 * Wraps the signal that is emitted when the connection attempt against the personal AP is completed.
 * This signal will be received only if the fast channel switching feature is supported by the device.
 */
public class ConnectionResult {

    /**
     * ConnectionResult message
     */
    private final String message;

    /**
     * ConnectionResult response code
     */
    private final ConnectionResponseType connectionResponseType;

    /**
     * These enumeration values are returned from a device that supports fast channel switching .It is reported after the device tries to verify
     * the validity of the Wi-Fi parameters received via {@link OnboardingTransport#ConfigureWiFi(String, String, short)}
     */
    public static enum ConnectionResponseType {

        /**
         * Wi-Fi validated
         */
        VALIDATED((short) 0),

        /**
         * Wi-Fi unreachable
         */
        UNREACHABLE((short) 1),

        /**
         * Wi-Fi AP doesn't support the authentication received
         */
        UNSUPPORTED_PROTOCOL((short) 2),

        /**
         * Wi-Fi authentication error
         */
        UNAUTHORIZED((short) 3),

        /**
         * Misc error
         */
        ERROR_MESSAGE((short) 4);

        private short value;

        /**
         *
         * @param value
         */
        private ConnectionResponseType(short value) {
            this.value = value;
        }

        /**
         * Returns the id of authentication type
         *
         * @return id of authentication type
         */
        public short getValue() {
            return value;
        }

        public static ConnectionResponseType getConnectionResponseTypeByValue(short value) {
            ConnectionResponseType ret = null;
            for (ConnectionResponseType responseType : ConnectionResponseType.values()) {
                if (value == responseType.getValue()) {
                    ret = responseType;
                    break;
                }
            }
            return ret;
        }
    }

    public ConnectionResult(ConnectionResponseType connectionResponseType, String message) {
        this.message = message;
        this.connectionResponseType = connectionResponseType;
    }

    public String getMessage() {
        return message;
    }

    public ConnectionResponseType getConnectionResponseType() {
        return connectionResponseType;
    }
}