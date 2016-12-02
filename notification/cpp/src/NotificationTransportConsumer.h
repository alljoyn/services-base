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

#ifndef NOTIFICATIONTRANSPORTCONSUMER_H_
#define NOTIFICATIONTRANSPORTCONSUMER_H_

#include <queue>
#ifdef _WIN32
#include <Windows.h>
#define pthread_mutex_t CRITICAL_SECTION
#define pthread_cond_t CONDITION_VARIABLE
#else
#include <pthread.h>
#endif

#include "NotificationTransport.h"

namespace ajn {
namespace services {

/**
 * Notification Transport Consumer. Used to Create the Notification Interface
 * on the receiver side and register a signal handler to receive the notify signals
 */
class NotificationTransportConsumer : public NotificationTransport {
  public:

    /**
     * Constructor for TransportConsumer
     * @param bus         - BusAttachment that is used
     * @param servicePath - servicePath of BusObject
     * @param status      - success/failure
     */
    NotificationTransportConsumer(ajn::BusAttachment* bus,
                                  qcc::String const& servicePath, QStatus& status);

    /**
     * Destructor of TransportConsumer
     */
    ~NotificationTransportConsumer() { };

    /**
     * Callback when Signal arrives
     * @param member    Method or signal interface member entry.
     * @param srcPath   Object path of signal emitter.
     * @param message   The received message.
     */
    void handleSignal(const ajn::InterfaceDescription::Member* member,
                      const char* srcPath, ajn::Message& msg);

    void unregisterHandler(ajn::BusAttachment* bus);

  private:

    /**
     * The thread responsible for receiving the notification
     */
#ifdef _WIN32
    HANDLE m_handle;
#else
    pthread_t m_ReceiverThread;
#endif

    /**
     * A Queue that holds the messages
     */
    std::queue<ajn::Message> m_MessageQueue;

    /**
     * The mutex Lock
     */
    pthread_mutex_t m_Lock;

    /**
     * The Queue Changed thread condition
     */
    pthread_cond_t m_QueueChanged;

    /**
     * is the thread in the process of shutting down
     */
    bool m_IsStopping;

    /**
     * A wrapper for the receiver Thread
     * @param context
     */
    static void* ReceiverThreadWrapper(void* context);

    /**
     * The function run in the ReceiverThread
     */
    void ReceiverThread();
};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONTRANSPORTCONSUMER_H_ */