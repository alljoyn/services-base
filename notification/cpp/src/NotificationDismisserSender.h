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

#ifndef NOTIFICATIONDISMISSERSENDER_H_
#define NOTIFICATIONDISMISSERSENDER_H_

#include <alljoyn/notification/NotificationEnums.h>
#include "NotificationDismisser.h"
#include "NotificationConstants.h"

namespace ajn {
namespace services {

/**
 * Implementing the sender side of the dismiss signal in dismisser interface
 */
class NotificationDismisserSender : public NotificationDismisser {

  public:

    /**
     * Constructor for TransportProducer. Creates Interface and prepares
     * infrastructure to be able to send Signal
     * @param bus            - BusAttachment that is used
     * @param objectPath     - Object path name of the dismisser
     * @param status         - success/failure
     */
    NotificationDismisserSender(ajn::BusAttachment* bus, qcc::String const& objectPath, QStatus& status);

    /**
     * Destructor for TransportProducer
     */
    ~NotificationDismisserSender() { };

    /**
     * Send Signal over Bus.
     * @param notificationArgs
     * @param ttl
     * @return status
     */
    QStatus sendSignal(ajn::MsgArg const dismisserArgs[nsConsts::AJ_DISMISSER_NUM_PARAMS],
                       uint16_t ttl);


};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONDISMISSERSENDER_H_ */