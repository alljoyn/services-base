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

#ifndef CONTROLLERNOTIFICATIONRECEIVER_H_
#define CONTROLLERNOTIFICATIONRECEIVER_H_

#include <vector>
#include <alljoyn/controlpanel/ControlPanelController.h>
#include <alljoyn/notification/NotificationReceiver.h>
#include <alljoyn/notification/Notification.h>

/**
 * Class that will receive Notifications. Implements NotificationReceiver
 * Receives list of applications to filter by and will only display notifications
 * from those applications
 */
class ControllerNotificationReceiver : public ajn::services::NotificationReceiver {
  public:

    /**
     * Constructor
     */
    ControllerNotificationReceiver(ajn::services::ControlPanelController* controlPanelController);

    /**
     * Destructor
     */
    ~ControllerNotificationReceiver();

    /**
     * Receive - function that receives a notification
     * @param notification
     */
    void Receive(ajn::services::Notification const& notification);

    /**
     * receive Dismiss signal
     * @param message id
     * @param application id
     */
    void Dismiss(const int32_t msgId, const qcc::String appId);
  private:

    ajn::services::ControlPanelController* m_Controller;

};

#endif /* CONTROLLERNOTIFICATIONRECEIVER_H_ */