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

#ifdef _WIN32
/* Disable deprecation warnings */
#pragma warning(disable: 4996)
#endif

#include <alljoyn/Message.h>
#include <alljoyn/notification/NotificationService.h>

#include "NotificationDismisserSender.h"
#include "NotificationConstants.h"
#include "Transport.h"
#include <alljoyn/notification/LogModule.h>

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

NotificationDismisserSender::NotificationDismisserSender(BusAttachment* bus, String const& objectPath, QStatus& status) :
    NotificationDismisser(bus, objectPath, status)
{
    /**
     * Do not add code until the status that returned from the base class is verified.
     */
    if (status != ER_OK) {
        return;
    }

    //Add code here
    QCC_DbgPrintf(("NotificationDismisserSender()  - Got objectpath=%s", objectPath.c_str()));
}

QStatus NotificationDismisserSender::sendSignal(ajn::MsgArg const dismisserArgs[AJ_DISMISSER_NUM_PARAMS],
                                                uint16_t ttl)
{
    QCC_DbgTrace(("Notification::sendSignal() called"));

    if (m_SignalMethod == 0) {
        QCC_LogError(ER_BUS_INTERFACE_NO_SUCH_MEMBER, ("signalMethod not set. Can't send signal"));
        return ER_BUS_INTERFACE_NO_SUCH_MEMBER;
    }

    uint8_t flags = ALLJOYN_FLAG_SESSIONLESS;
    QStatus status = Signal(NULL, 0, *m_SignalMethod, dismisserArgs, AJ_DISMISSER_NUM_PARAMS, ttl, flags);

    if (status != ER_OK) {
        QCC_LogError(status, ("Could not send signal."));
        return status;
    }

    QCC_DbgPrintf(("Sent signal successfully"));
    return status;
}