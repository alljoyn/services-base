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

#include <alljoyn/notification/NotificationService.h>

#include "NotificationConstants.h"
#include "NotificationTransportSuperAgent.h"
#include "PayloadAdapter.h"
#include "Transport.h"
#include <alljoyn/notification/LogModule.h>

using namespace qcc;
using namespace ajn;
using namespace services;
using namespace nsConsts;

ReceiveNotificationTaskData::ReceiveNotificationTaskData(Message& msg) : message(msg)
{
}

void ReceiveNotificationAsyncTask::OnEmptyQueue()
{
}

void ReceiveNotificationAsyncTask::OnTask(TaskData const* taskdata)
{
    QCC_DbgPrintf(("Handling notification from superagent"));
    const ReceiveNotificationTaskData* receiveNotificationTaskData = static_cast<const ReceiveNotificationTaskData*>(taskdata);
    if (receiveNotificationTaskData) {
        PayloadAdapter::receivePayload(const_cast<ReceiveNotificationTaskData*>(receiveNotificationTaskData)->message);
    }
}

NotificationTransportSuperAgent::NotificationTransportSuperAgent(
    BusAttachment* bus, String const& servicePath, QStatus& status, bool isFirstSuperAgent)  :
    NotificationTransport(bus, servicePath, status, AJ_SA_INTERFACE_NAME),
    m_IsFirstSuperAgent(isFirstSuperAgent)
{
    if (status != ER_OK) {
        return;
    }

    SetAnnounceFlag(bus->GetInterface(AJ_SA_INTERFACE_NAME.c_str()), BusObject::UNANNOUNCED);
    m_receiveNotificationTaskQueue = new AsyncTaskQueue(&m_receiveNotificationAsyncTask);
    m_receiveNotificationTaskQueue->Start();

    status =  bus->RegisterSignalHandler(this,
                                         static_cast<MessageReceiver::SignalHandler>(&NotificationTransportSuperAgent::handleSignal),
                                         m_SignalMethod,
                                         NULL);

    if (status != ER_OK) {
        QCC_LogError(status, ("Could not register the SignalHandler"));
    } else {
        QCC_DbgPrintf(("Registered the SignalHandler successfully"));
    }

}


void NotificationTransportSuperAgent::handleSignal(const InterfaceDescription::Member* member, const char* srcPath, Message& msg)
{
    QCC_DbgPrintf(("Got notification from super agent"));
    Transport* transport = Transport::getInstance();
    // We must enable concurrent callbacks since some of the calls below are blocking
    transport->getBusAttachment()->EnableConcurrentCallbacks();

    if (m_IsFirstSuperAgent) {
        m_IsFirstSuperAgent = false;
        QCC_DbgPrintf(("Found first super agent."));

        const char* messageSender = msg->GetSender();
        if (transport->FindSuperAgent(messageSender) != ER_OK) {
            QCC_DbgHLPrintf(("Could not listen to SuperAgent."));
            m_IsFirstSuperAgent = true;     //so that we try again
            return;
        }
    }

    String sender = msg->GetSender();
    QCC_DbgPrintf(("Received Message from super agent: %s", sender.c_str()));
    if (m_receiveNotificationTaskQueue) {
        m_receiveNotificationTaskQueue->Enqueue(new ReceiveNotificationTaskData(msg));
    }
}

void NotificationTransportSuperAgent::unregisterHandler(BusAttachment* bus)
{
    bus->UnregisterSignalHandler(this,
                                 static_cast<MessageReceiver::SignalHandler>(&NotificationTransportSuperAgent::handleSignal),
                                 m_SignalMethod,
                                 NULL);

    if (m_receiveNotificationTaskQueue) {
        m_receiveNotificationTaskQueue->Stop();
        delete m_receiveNotificationTaskQueue;
        m_receiveNotificationTaskQueue = NULL;
    }
}

void NotificationTransportSuperAgent::setIsFirstSuperAgent(bool isFirstSuperAgent)
{
    m_IsFirstSuperAgent = isFirstSuperAgent;
}