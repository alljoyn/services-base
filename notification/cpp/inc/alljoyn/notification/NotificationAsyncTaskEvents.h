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


#ifndef NOTIFICATION_ASYNCTASK_EVENTS_H_
#define NOTIFICATION_ASYNCTASK_EVENTS_H_

#include <queue>
#include <qcc/String.h>
#include <iostream>
#include <alljoyn/notification/NotificationService.h>
#include <alljoyn/services_common/AsyncTaskQueue.h>

namespace ajn {
namespace services {

/**
 * Notification async task events.
 * Class implementing callbacks fto handle messages.
 */
class NotificationAsyncTaskEvents : public AsyncTask {
  public:
    /**
     * constructor of NotificationAsyncTaskEvents
     */
    NotificationAsyncTaskEvents();
    /**
     * destructor of NotificationAsyncTaskEvents
     */
    virtual ~NotificationAsyncTaskEvents();
    /**
     * callback to handle the case of empty message queue.
     */
    virtual void OnEmptyQueue();
    /**
     * callback to handle the case of new message
     * @param taskdata - object to handle
     */
    virtual void OnTask(TaskData const* taskdata);

  private:
    /**
     * send dismiss signal
     * @param asyncTaskQueue - a template type of message
     */
    void sendDismissSignal(TaskData const* taskdata);

};

} //namespace services
} //namespace ajn



#endif /* NOTIFICATION_ASYNCTASK_EVENTS_H_ */