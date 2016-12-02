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

#ifndef NOTIFICATIONTRANSPORTSUPERAGENT_H_
#define NOTIFICATIONTRANSPORTSUPERAGENT_H_

#include "NotificationTransport.h"
#include <alljoyn/services_common/AsyncTaskQueue.h>

namespace ajn {
namespace services {

/**
 *  ReceiveNotificationTaskData implements TaskData
 */
class ReceiveNotificationTaskData : public TaskData {
  public:
    /**
     * ReceiveNotificationTaskData CTOR
     * @param msg - notification to handle
     */
    ReceiveNotificationTaskData(Message& msg);
    /**
     * message - notification to handle
     */
    Message message;
};

/**
 *  ReceiveNotificationAsyncTask implements AsyncTask
 */
class ReceiveNotificationAsyncTask : public AsyncTask {
  public:
    /*
     * ReceiveNotificationAsyncTask CTOR
     */
    ReceiveNotificationAsyncTask() { }
    /*
     * ReceiveNotificationAsyncTask DTOR
     */
    virtual ~ReceiveNotificationAsyncTask() { };
    /**
     * OnEmptyQueue - handling 'queue become empty' events.
     */
    virtual void OnEmptyQueue();
    /**
     * OnEmptyQueue - handling 'queue got new message' events.
     *  @param taskdata - pointer to the data that currently processed.
     */
    virtual void OnTask(TaskData const* taskdata);
};

/**
 * Notification Transport SuperAgent. Used to Create the SuperAgent Interface
 * on the receiver side and register a signal handler to receive the notify signals
 */
class NotificationTransportSuperAgent : public NotificationTransport {
  public:

    /**
     * Constructor for TransportSuperAgent
     * @param bus         - BusAttachment that is used
     * @param servicePath - servicePath of BusObject
     * @param status      - success/failure
     */
    NotificationTransportSuperAgent(ajn::BusAttachment* bus,
                                    qcc::String const& servicePath, QStatus& status, bool isFirstSuperAgent = true);

    /**
     * Destructor of TransportSuperAgent
     */
    ~NotificationTransportSuperAgent() { };

    /**
     * Callback when Signal arrives
     * @param member    Method or signal interface member entry.
     * @param srcPath   Object path of signal emitter.
     * @param message   The received message.
     */
    void handleSignal(const ajn::InterfaceDescription::Member* member,
                      const char* srcPath, ajn::Message& msg);

    void unregisterHandler(ajn::BusAttachment* bus);

    void setIsFirstSuperAgent(bool isFirstSuperAgent);

  private:
    /*
     * implements AsyncTask
     */
    ReceiveNotificationAsyncTask m_receiveNotificationAsyncTask;
    /**
     * Boolean to indicate if this is the first time the SuperAgent called back
     */
    bool m_IsFirstSuperAgent;
    /*
     * task queue with thread - dedicated to handle notifications from the super agent.
     */
    AsyncTaskQueue* m_receiveNotificationTaskQueue;
};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONTRANSPORTSUPERAGENT_H_ */