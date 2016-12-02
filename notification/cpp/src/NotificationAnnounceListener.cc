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
#include "NotificationAnnounceListener.h"
#include "NotificationConstants.h"
#include "Transport.h"
#include <alljoyn/notification/LogModule.h>

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

NotificationAnnounceListener::NotificationAnnounceListener() :
    AnnounceHandler()
{
}

NotificationAnnounceListener::~NotificationAnnounceListener()
{
}

void NotificationAnnounceListener::Announced(const char* busName, uint16_t version, ajn::SessionPort port, const ajn::MsgArg& objectDescriptionArg, const ajn::MsgArg& aboutDataArg)
{

    QCC_DbgPrintf(("Received Announce Signal"));

    Transport::getInstance()->getBusAttachment()->EnableConcurrentCallbacks();
    AboutObjectDescription aboutObjectDescription;
    aboutObjectDescription.CreateFromMsgArg(objectDescriptionArg);

    if (aboutObjectDescription.HasInterface(AJ_SA_INTERFACE_NAME.c_str())) {
        QCC_DbgPrintf((qcc::String("Received announce of superAgent interface").c_str()));


        QStatus status = Transport::getInstance()->FindSuperAgent(busName);
        if (status != ER_OK) {
            QCC_DbgPrintf((qcc::String("FindAdvertisedName failed").c_str()));
        }

        return;
    }
}

void NotificationAnnounceListener::Announce(uint16_t version, uint16_t port, const char* busName, const ObjectDescriptions& objectDescs, const AboutData& aboutData)
{

    QCC_DbgPrintf(("Received Announce Signal"));

    Transport::getInstance()->getBusAttachment()->EnableConcurrentCallbacks();

    for (AboutClient::ObjectDescriptions::const_iterator it = objectDescs.begin(); it != objectDescs.end(); ++it) {
        std::vector<qcc::String> interfaces = it->second;
        std::vector<qcc::String>::const_iterator interfaceIter;

        for (interfaceIter = interfaces.begin(); interfaceIter != interfaces.end(); ++interfaceIter) {

            QCC_DbgPrintf((qcc::String("Received announce of interface " + *interfaceIter + " for objectpath " + it->first).c_str()));

            if (0 == interfaceIter->compare(AJ_SA_INTERFACE_NAME)) {
                QCC_DbgPrintf((qcc::String("Received announce of superAgent interface").c_str()));


                QStatus status = Transport::getInstance()->FindSuperAgent(busName);
                if (status != ER_OK) {
                    QCC_DbgPrintf((qcc::String("FindAdvertisedName failed").c_str()));
                }

                return;
            }
        }
    }
}
