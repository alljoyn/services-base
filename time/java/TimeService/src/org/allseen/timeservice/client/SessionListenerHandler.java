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

package org.allseen.timeservice.client;

import org.alljoyn.bus.SessionListener;
import org.alljoyn.bus.Status;

/**
 * Implement this interface to receive AllJoyn session related events.
 */
public interface SessionListenerHandler {

    /**
     * The method is called when the existing session is lost. <br>
     * Avoid blocking this thread with long running tasks.
     * @param reason Session lost reason
     * @param timeServiceClient {@link TimeServiceClient} that was used to create the session
     * @see SessionListener#sessionLost(int, int)
     */
    void sessionLost(int reason, TimeServiceClient timeServiceClient);

    /**
     * The method is called when a session was established following the call to
     * {@link TimeServiceClient#joinSessionAsync(SessionListenerHandler)}.
     * Avoid blocking this thread with long running tasks.
     * @param timeServiceClient {@link TimeServiceClient} that was used to create the session
     * @param status Check this {@link Status} to ensure that the session was created successfully
     * @see Status
     */
    void sessionJoined(TimeServiceClient timeServiceClient, Status status);
}