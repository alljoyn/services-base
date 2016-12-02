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

#include "HandleOnboardingSignals.h"
#include "alljoyn/onboarding/LogModule.h"

using namespace ajn::services;


HandleOnboardingSignals::HandleOnboardingSignals(ajn::BusAttachment* bus, OnboardingClientListener* listener) :
    m_bus(bus), m_userListener(listener), m_asyncTaskQueue(NULL)
{
    QCC_DbgTrace(("ConnectionResultHandler::%s", __FUNCTION__));
}

HandleOnboardingSignals::~HandleOnboardingSignals()
{
    m_asyncTaskQueue->Stop();
    delete m_asyncTaskQueue;
}

HandleOnboardingSignals::HandleOnboardingSignals(const HandleOnboardingSignals& handler) :
    m_bus(NULL), m_userListener(NULL), m_asyncTaskQueue(NULL)
{
    QCC_UNUSED(handler);
}

HandleOnboardingSignals& HandleOnboardingSignals::operator=(const HandleOnboardingSignals& handler)
{
    QCC_UNUSED(handler);
    return *this;
}

QStatus HandleOnboardingSignals::RegisterConnectionResultSignalHandler(const ajn::InterfaceDescription::Member* connectionResultSignalMethod)
{
    // the incoming signals are placed in a queue so that the alljoyn thread
    // is released right away
    m_asyncTaskQueue = new AsyncTaskQueue(this);
    m_asyncTaskQueue->Start();

    QStatus status = m_bus->RegisterSignalHandler(this,
                                                  static_cast<MessageReceiver::SignalHandler>(&HandleOnboardingSignals::ConnectionResultSignal),
                                                  connectionResultSignalMethod,
                                                  0);

    return status;
}


QStatus HandleOnboardingSignals::UnregisterConnectionResultSignalHandler(const ajn::InterfaceDescription::Member* connectionResultSignalMethod)
{
    QStatus status = m_bus->UnregisterSignalHandler(this,
                                                    static_cast<MessageReceiver::SignalHandler>(&HandleOnboardingSignals::ConnectionResultSignal),
                                                    connectionResultSignalMethod,
                                                    NULL);

    m_asyncTaskQueue->Stop();
    delete m_asyncTaskQueue;

    return status;
}


void HandleOnboardingSignals::ConnectionResultSignal(const ajn::InterfaceDescription::Member* member,
                                                     const char* srcPath, ajn::Message& message)
{
    QCC_UNUSED(member);
    QCC_UNUSED(srcPath);
    QCC_DbgTrace(("ConnectionResultHandler::%s", __FUNCTION__));

    QCC_DbgPrintf(("received signal interface=%s method=%s", message->GetInterface(), message->GetMemberName()));

    if (strcmp(message->GetInterface(), "org.alljoyn.Onboarding") != 0 || strcmp(message->GetMemberName(), "ConnectionResult") != 0) {
        QCC_DbgPrintf(("This is not the signal we are looking for"));
        return;
    }

    SignalMesssage* originalMessage = new SignalMesssage(message);
    m_asyncTaskQueue->Enqueue(originalMessage);
}

///////////////// AsyncTask Methods ////////////////////////
void HandleOnboardingSignals::OnEmptyQueue()
{
}

void HandleOnboardingSignals::OnTask(TaskData const* taskdata)
{
    SignalMesssage const* wrappedMessage = static_cast<SignalMesssage const*>(taskdata);
    ajn::Message* message = (ajn::Message*)&wrappedMessage->m_signalMessage;

    const ajn::MsgArg* args = 0;
    size_t numArgs = 0;
    QStatus status;

    short connectionResultCode;
    char* connectionResultMessage;

    message->unwrap()->GetArgs(numArgs, args);

    if (numArgs == 1) {
        status = args[0].Get("(ns)", &connectionResultCode, &connectionResultMessage);
    } else {
        status = ER_BAD_ARG_COUNT;
    }

    if (status != ER_OK) {
        QCC_DbgPrintf(("unmarshalling ConnectionResult Signal, failed"));
        return;
    }

    qcc::String resultMessage(connectionResultMessage);
    m_userListener->ConnectionResultSignalReceived(connectionResultCode, resultMessage);
}