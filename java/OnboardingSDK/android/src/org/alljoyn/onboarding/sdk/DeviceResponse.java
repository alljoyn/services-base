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

/**
 * DeviceResponse is a class used internally to encapsulate possible errors that
 * may occur during AllJoyn transactions carried by the SDK
 */
class DeviceResponse {

    /**
     * enumeration of possible ResponseCodes
     */
    public enum ResponseCode {
        /**
         * AllJoyn transaction successful
         */
        Status_OK,
        /**
         * AllJoyn transaction general error
         */
        Status_ERROR,
        /**
         * AllJoyn session creation error
         */
        Status_ERROR_CANT_ESTABLISH_SESSION
    }

    /**
     * holds the response code.
     */
    private final ResponseCode status;

    /**
     * holds the description of the error.
     */
    private String description = null;
    /**
     * holds the exception related to the error.
     */
    private Exception exception = null;

    /**
     * DeviceResponse Constructor
     *
     * @param status
     *            {@link #status}
     *
     */
    public DeviceResponse(ResponseCode status) {
        this.status = status;
    }

    /**
     * DeviceResponse Constructor
     *
     * @param status
     *            {@link #status}
     * @param description
     *            {@link #description}
     *
     */
    public DeviceResponse(ResponseCode status, String description) {
        this.status = status;
        this.description = description;
    }

    /**
     * DeviceResponse Constructor
     *
     * @param status
     *            {@link #status}
     * @param description
     *            {@link #description}
     * @param exception
     *            {@link #exception}
     *
     */
    public DeviceResponse(ResponseCode status, String description, Exception exception) {
        this.status = status;
        this.description = description;
        this.exception = exception;
    }

    /**
     * Get {@link #status}
     *
     * @return the status code {@link #status}
     */
    public ResponseCode getStatus() {
        return status;
    }

    /**
     * Get {@link #description}
     *
     * @return the error description {@link #description}
     */
    public String getDescription() {
        return description;
    }

    /**
     * Get {@link #exception}
     *
     * @return the error exception {@link #exception}
     */
    public Exception getException() {
        return exception;
    }

}
