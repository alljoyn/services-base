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

#ifndef NOTIFICATIONRECEIVERIMPL_H_
#define NOTIFICATIONRECEIVERIMPL_H_

#include <alljoyn/notification/NotificationReceiver.h>
#include <alljoyn/notification/Notification.h>

/**
 * NotificationReceiverImpl
 */
class NotificationReceiverImpl : public ajn::services::NotificationReceiver {
  public:
    /**
     * Constructor
     */
    NotificationReceiverImpl();

    /**
     * Destructor
     */
    ~NotificationReceiverImpl();

    /**
     * Receive - function that receives a notification
     * @param notification
     */
    void Receive(ajn::services::Notification const& notification);

    /**
     * receive Dismiss signal
     * @param msgId message id
     * @param appId application id
     */
    void Dismiss(const int32_t msgId, const qcc::String appId);

};

#endif /* NOTIFICATIONRECEIVERIMPL_H_ */