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

#include "TestCustomAlarmBusObject.h"
#include <alljoyn/time/TimeServiceConstants.h>

using namespace ajn;
using namespace services;

TestCustomAlarmBusObject::TestCustomAlarmBusObject(qcc::String objectPath) : TimeServiceAlarmBusObj(objectPath)
{

}

QStatus TestCustomAlarmBusObject::createCustomInterfaceHook(BusAttachment* bus)
{

    QStatus status        = ER_OK;
    const char* ifaceName = (tsConsts::ALARM_IFACE + ".CoolAlarm").c_str();

    InterfaceDescription* ifaceDesc = const_cast<InterfaceDescription*>(bus->GetInterface(ifaceName));


    if (!ifaceDesc) {

        status = bus->CreateInterface(ifaceName, ifaceDesc);
        if (status != ER_OK) {

            return status;
        }

        //Add Version property
        status = ifaceDesc->AddMethod("getCoolTitle", NULL, "s", "CoolTitle");
        if (status != ER_OK) {

            return status;
        }

        ifaceDesc->Activate();
    }

    status = addAlarmInterface(*ifaceDesc);

    if (status != ER_OK) {

        return status;
    }


    const ajn::InterfaceDescription::Member* methodMember = ifaceDesc->GetMember("getCoolTitle");
    status = AddMethodHandler(methodMember, static_cast<MessageReceiver::MethodHandler>(&TestCustomAlarmBusObject::getCoolTitle));
    if (status != ER_OK) {
        std::cout << "Could not register the getCoolTitle MethodHandler\n";
        return status;
    }

    return status;
}

void TestCustomAlarmBusObject::getCoolTitle(const InterfaceDescription::Member* member, Message& msg)
{
    ajn::MsgArg replyArg[1];
    QStatus status = replyArg[0].Set("s", "This is the coolest message title");
    if (status != ER_OK) {
        QCC_LogError(status, ("Could not marshal response for getCoolTitle method"));
        MethodReply(msg, status);
        return;
    }

    status = MethodReply(msg, replyArg, 1);
    if (status != ER_OK) {
        QCC_LogError(status, ("getCoolTitle reply call failed"));
    }
}
