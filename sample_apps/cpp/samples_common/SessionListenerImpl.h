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

#ifndef SESSIONLISTENERIMPL_H_
#define SESSIONLISTENERIMPL_H_

#include <alljoyn/SessionListener.h>
#include <qcc/String.h>

/**
 * SessionListenerImpl
 */
class SessionListenerImpl : public ajn::SessionListener {
  public:
    /**
     * SessionListenerImpl
     * @param inServiceNAme
     */
    SessionListenerImpl(qcc::String const& inServiceNAme);

    /**
     * destructor
     */
    virtual ~SessionListenerImpl();

    /**
     * SessionLost
     * @param sessionId
     */
    void SessionLost(ajn::SessionId sessionId);

  private:

    qcc::String serviceName;

};

#endif /* SESSIONLISTENERIMPL_H_ */

