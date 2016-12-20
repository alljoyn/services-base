/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
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

