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

#include "TimeServiceSessionListenerImpl.h"

using namespace ajn;
using namespace services;

//Constructor
TimeServiceSessionListenerImpl::TimeServiceSessionListenerImpl() /*: TimeServiceSessionListener()*/
{
}

//Destructor
TimeServiceSessionListenerImpl::~TimeServiceSessionListenerImpl()
{
}

//Handle Session Lost
void TimeServiceSessionListenerImpl::sessionLost(TimeServiceClient* timeServiceClient, SessionListener::SessionLostReason reason)
{

    printf("******* SESSION LOST******* Server: '%s', Reason: '%u' \n", timeServiceClient->getServerBusName().c_str(), reason);
}

void TimeServiceSessionListenerImpl::sessionJoined(TimeServiceClient* timeServiceClient, QStatus status)
{

    const char* msg =  (status == ER_OK || status == ER_ALLJOYN_JOINSESSION_REPLY_ALREADY_JOINED) ? "SESSION JOIN SUCCEEDED"
                      : "SESSION JOIN FAILED";

    SessionId sid         = 0;
    QStatus getSidStatus  = timeServiceClient->getSessionId(sid);

    if (getSidStatus != ER_OK) {

        printf("Failed to retrieve the SessionId,\n");
    }

    printf("******* %s******* Server: '%s', Status: '%u', SessionId: '%u' \n",
           msg, timeServiceClient->getServerBusName().c_str(), status, sid);
}