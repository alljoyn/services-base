/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifndef NOTIFICATIONRECEIVERTESTIMPL_H_
#define NOTIFICATIONRECEIVERTESTIMPL_H_

#include <vector>
#include <alljoyn/notification/NotificationReceiver.h>
#include <alljoyn/notification/Notification.h>



/**
 * Class that will receive Notifications. Implements NotificationReceiver
 * Receives list of applications to filter by and will only display notifications
 * from those applications
 */
class NotificationReceiverTestImpl : public ajn::services::NotificationReceiver {
  public:
    enum NotificationAction {
        ACTION_NOTHING,
        ACTION_ACKNOWLEDGE,
        ACTION_DISMISS
    };
    /**
     * Constructor
     */
    NotificationReceiverTestImpl(NotificationAction notificationAction);

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

  private:

    /**
     * vector of applications to filter by
     */
    std::vector<qcc::String> m_Applications;
    /**
     * action to do after getting notification
     */
    NotificationAction m_NotificationAction;
};

#endif /* NOTIFICATIONRECEIVERTESTIMPL_H_ */
