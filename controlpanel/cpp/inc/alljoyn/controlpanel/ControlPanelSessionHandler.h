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

#ifndef CONTROLPANELSESSIONHANDLER_H_
#define CONTROLPANELSESSIONHANDLER_H_

#include <alljoyn/BusAttachment.h>
#include <alljoyn/SessionListener.h>

namespace ajn {
namespace services {

class ControlPanelDevice;

/**
 * class ControlPanelSessionHandler
 */
class ControlPanelSessionHandler : public ajn::BusAttachment::JoinSessionAsyncCB, public ajn::SessionListener {
  public:

    /**
     * Constructor for ControlPanelSessionHandler
     */
    ControlPanelSessionHandler(ControlPanelDevice* device);

    /**
     * Destructor for ControlPanelSessionHandler
     */
    virtual ~ControlPanelSessionHandler();

    /**
     * SessionLost
     * @param sessionId
     */
    void SessionLost(ajn::SessionId sessionId);

    /**
     * JoinSessionCB
     * @param status
     * @param id
     * @param opts
     * @param context
     */
    void JoinSessionCB(QStatus status, ajn::SessionId id, const ajn::SessionOpts& opts, void* context);

    /**
     * getSessionId
     * @return SessionId
     */
    ajn::SessionId getSessionId() const;

  private:

    /**
     * SessionId for this Device
     */
    ajn::SessionId m_SessionId;

    /**
     * The device of this Session Handler
     */
    ControlPanelDevice* m_Device;

};

} /* namespace services */
} /* namespace ajn */

#endif /* CONTROLPANELSESSIONHANDLER_H_ */