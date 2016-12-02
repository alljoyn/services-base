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

#ifndef NOTIFICATIONANNOUNCELISTENER_H_
#define NOTIFICATIONANNOUNCELISTENER_H_

#include <alljoyn/AboutListener.h>
#include <alljoyn/about/AnnounceHandler.h>
#include <alljoyn/about/AnnouncementRegistrar.h>

namespace ajn {
namespace services {

class SuperAgentBusListener;

/**
 * Notification announce lisener. Receives announce signal from super agent.
 */
class NotificationAnnounceListener : public ajn::AboutListener, public ajn::services::AnnounceHandler {

  public:
    /**
     * Constructor of class NotificationAnnounceListener
     */
    NotificationAnnounceListener();
    /**
     * Destructor of class NotificationAnnounceListener
     */
    ~NotificationAnnounceListener();
    /**
     * a callback for getting announce message
     * @param busName - the bus unique name of the announce sender
     * @param version
     * @param port
     * @param objectDescriptionArg - object description related to the sender
     * @param aboutDataArg
     */
    virtual void Announced(const char* busName, uint16_t version, ajn::SessionPort port, const ajn::MsgArg& objectDescriptionArg, const ajn::MsgArg& aboutDataArg);

    /**
     * a callback for getting announce message
     * @param version
     * @param port
     * @param busName - the bus unique name of the announce sender
     * @param objectDescs description related to the sender
     * @param aboutData
     */
    void Announce(uint16_t version, uint16_t port, const char* busName, const ObjectDescriptions& objectDescs, const AboutData& aboutData);

  private:
};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONANNOUNCELISTENER_H_ */