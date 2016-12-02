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

#ifndef NOTIFICATIONRECEIVERTESTIMPL_H_
#define NOTIFICATIONRECEIVERTESTIMPL_H_

#include <vector>
#include <alljoyn/notification/NotificationReceiver.h>
#include <alljoyn/notification/Notification.h>
#ifdef _WIN32
#include <Windows.h>
#define pthread_mutex_t CRITICAL_SECTION
#define pthread_cond_t CONDITION_VARIABLE
#else
#include <pthread.h>
#endif


/**
 * Class that will receive Notifications. Implements NotificationReceiver
 * Receives list of applications to filter by and will only display notifications
 * from those applications
 */
class NotificationReceiverTestImpl : public ajn::services::NotificationReceiver {
  public:
    enum NotificationAction {
        ACTION_NOTHING,
        ACTION_DISMISS
    };
    /**
     * Constructor
     * @param wait to external notification action
     */
    NotificationReceiverTestImpl(bool waitForExternalNotificationAction = true);

    /**
     * Destructor
     */
    ~NotificationReceiverTestImpl();

    /**
     * Receive - function that receives a notification
     * @param notification
     */
    void Receive(ajn::services::Notification const& notification);

    /**
     * receive a list of applications to filter by and set the filter
     * @param listOfApps
     */
    void setApplications(qcc::String const& listOfApps);

    /**
     * receive Dismiss signal
     * @param message id
     * @param application id
     */
    void Dismiss(const int32_t msgId, const qcc::String appId);

    /**
     * Get notification action
     * @return NotificationAction
     */
    NotificationAction GetNotificationAction();

    /**
     * Set notification action
     * This method is called from a free thread to set an action and to release the blocked thread (At NotificationReceiverTestImpl::Receive(...)),
     * that received the notification and waiting to the action decision.
     * @param NotificationAction
     */
    void SetNotificationAction(NotificationAction notificationAction);

  private:

    /**
     * vector of applications to filter by
     */
    std::vector<qcc::String> m_Applications;
    /**
     * action to do after getting notification
     */
    NotificationAction m_NotificationAction;
    /**
     * locks for the condition according to 'pthread_cond_t' declaration.
     */
    pthread_mutex_t m_Lock;

    /**
     * thread condition
     * Blocking the notification receiving thread in case m_WaitForExternalNotificationAction is true, until SetNotificationAction() will be called.
     */
    pthread_cond_t m_Condition;

    /**
     * Wait to external notification action
     * If true - external thread will need to call to SetNotificationAction() to unblock the thread that received the notification.
     * If false - a normal standard input will block the thread that received the notification until the user will decide what to do with the notification.
     */
    bool m_WaitForExternalNotificationAction;
};

#endif /* NOTIFICATIONRECEIVERTESTIMPL_H_ */