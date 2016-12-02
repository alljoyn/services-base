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

#ifndef NOTIFICATIONPRODUCERRECEIVER_H_
#define NOTIFICATIONPRODUCERRECEIVER_H_

#include "NotificationProducer.h"
#include <queue>
#ifdef _WIN32
#include <Windows.h>
#define pthread_mutex_t CRITICAL_SECTION
#define pthread_cond_t CONDITION_VARIABLE
#else
#include <pthread.h>
#endif

namespace ajn {
namespace services {

/**
 * class MsgQueueContent
 *
 */
class MsgQueueContent {
  public:
    MsgQueueContent(uint32_t msgId) : m_MsgId(msgId) { }

    uint32_t m_MsgId;
};

/**
 * class NotificationProducerReceiver
 * Implements NotificationProducer interface at producer side
 */
class NotificationProducerReceiver : public ajn::services::NotificationProducer {

  public:
    /**
     * constructor of NotificationProducerReceiver
     * @param bus attachment
     * @param status
     */
    NotificationProducerReceiver(ajn::BusAttachment* bus, QStatus& status);

    /**
     * destructor of NotificationProducerReceiver
     */
    ~NotificationProducerReceiver();

    /**
     *  Handles  Dismiss method
     * @param[in]  member
     * @param[in]  msg reference of AllJoyn Message
     */
    void Dismiss(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * SetAppIdArg
     * @param application id argument
     */
    void SetAppIdArg(MsgArg appIdArg) {
        m_AppIdArg = appIdArg;
    }
    /**
     * unregisterHandler
     * @param bus attachment
     */
    void unregisterHandler(BusAttachment* bus);
  private:
    /**
     * implement method calls from notification producer interface
     */
    void HandleMethodCall(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

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
    std::queue<MsgQueueContent> m_MessageQueue;

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
    void Receiver();

    /**
     * sendDismissSignal
     */
    QStatus sendDismissSignal(int32_t msgId);

    /**
     * appIdArg
     */
    MsgArg m_AppIdArg;

};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONPRODUCERRECEIVER_H_ */