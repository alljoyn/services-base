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

#ifndef NOTIFICATIONRECEIVER_H_
#define NOTIFICATIONRECEIVER_H_

#include <alljoyn/notification/Notification.h>

namespace ajn {
namespace services {

/**
 * An Abstract class with function receive. The implementation of this class
 * can be passed in to the initReceive function and will be the callback for
 * when notifications are received
 */
class NotificationReceiver {
  public:

    /**
     * Constructor for NotificationReceiver
     */
    NotificationReceiver() { };

    /**
     * Destructor for NotificationReceiver
     */
    virtual ~NotificationReceiver() { };

    /**
     * Pure abstract function that receives a notification
     * Consumer Application must override this method
     * @param notification the notification that is received
     */
    virtual void Receive(Notification const& notification) = 0;

    /**
     *   Dismiss handler
     */
    virtual void Dismiss(const int32_t msgId, const qcc::String appId) = 0;
};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONRECEIVER_H_ */