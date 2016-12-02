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

#ifndef NOTIFICATIONPRODUCERSENDER_H_
#define NOTIFICATIONPRODUCERSENDER_H_

#include "NotificationProducer.h"

namespace ajn {
namespace services {

/**
 * NotificationProducerSender class
 * Calls NotificationProducer interface methods
 */
class NotificationProducerSender : public ajn::services::NotificationProducer {

  public:
    /**
     * constructor of NotificationProducerSender
     * @param bus attachment
     * @param status
     */
    NotificationProducerSender(ajn::BusAttachment* bus, QStatus& status);
    /**
     * destructor of NotificationProducerSender
     */
    ~NotificationProducerSender();

    /*
     * calls method Dismiss at the producer side
     * @param bus attachment
     * @param relevant session id
     * @param elevant message id
     * @return status
     */
    QStatus Dismiss(const char* busName, ajn::SessionId sessionId, int32_t mgsId);

  private:

};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONPRODUCERSENDER_H_ */