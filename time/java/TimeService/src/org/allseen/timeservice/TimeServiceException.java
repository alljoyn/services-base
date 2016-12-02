 /******************************************************************************
  *   * 
  *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
  *    Source Project Contributors and others.
  *    
  *    All rights reserved. This program and the accompanying materials are
  *    made available under the terms of the Apache License, Version 2.0
  *    which accompanies this distribution, and is available at
  *    http://www.apache.org/licenses/LICENSE-2.0

  ******************************************************************************/

package org.allseen.timeservice;

/**
 * {@link TimeServiceException} is thrown when there is a problem in the Time Service functionality
 */
public class TimeServiceException extends Exception {
    private static final long serialVersionUID = 5319795501514525468L;

    public TimeServiceException() {
        super();
    }

    public TimeServiceException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public TimeServiceException(String detailMessage) {
        super(detailMessage);
    }

    public TimeServiceException(Throwable throwable) {
        super(throwable);
    }
}