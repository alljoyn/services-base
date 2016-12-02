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

#ifndef TIMESERVICESESSIONLISTENER_H_
#define TIMESERVICESESSIONLISTENER_H_

#include <alljoyn/SessionListener.h>
#include <alljoyn/time/TimeServiceClient.h>

namespace ajn {
namespace services {

/**
 * Inherit from this class to receive AllJoyn session related events
 */
class TimeServiceSessionListener {

  public:

    /**
     * Constructor
     */
    TimeServiceSessionListener();

    /**
     * Destructor
     */
    virtual ~TimeServiceSessionListener();

    /**
     * The method is called when the existing session is lost.
     * Avoid blocking this thread with long running tasks.
     *
     * @param timeServiceClient TimeServiceClient that was used to create the session
     * @param reason Session lost reason
     */
    virtual void sessionLost(TimeServiceClient* timeServiceClient, SessionListener::SessionLostReason reason) = 0;

    /**
     * The method is called when a session was established following the call to
     * TimeServiceClient.joinSessionAsync().
     * Avoid blocking this thread with long running tasks.
     *
     * @param timeServiceClient TimeServiceClient that was used to create the session
     * @param status Check this QStatus to ensure that the session was created successfully
     */
    virtual void sessionJoined(TimeServiceClient* timeServiceClient, QStatus status) = 0;
};

} /* namespace services */
} /* namespace ajn */

#endif /* TIMESERVICESESSIONLISTENER_H_ */