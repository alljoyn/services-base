/******************************************************************************
 * Copyright (c) 2016 Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright 2016 Open Connectivity Foundation and Contributors to
 *    AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

/**
 * This class is experimental, and as such has not been tested.
 * Please help make it more robust by contributing fixes if you find issues
 **/

#include "AJTMTimeServiceTimerBusObjAdapter.h"
#import "AJNBusAttachment.h"
#import "AJNInterfaceDescription.h"


AJTMTimeServiceTimerBusObjAdapter::AJTMTimeServiceTimerBusObjAdapter(id<AJTMTimeServiceTimerBusObj> timeServiceTimerBusObj) : TimeServiceTimerBusObj("")
{
    handle = timeServiceTimerBusObj;
}

AJTMTimeServiceTimerBusObjAdapter::AJTMTimeServiceTimerBusObjAdapter(id<AJTMTimeServiceTimerBusObj> timeServiceTimerBusObj, qcc::String const& objectPath) : TimeServiceTimerBusObj(objectPath)
{
    handle = timeServiceTimerBusObj;
}

AJTMTimeServiceTimerBusObjAdapter::~AJTMTimeServiceTimerBusObjAdapter()
{
}

QStatus AJTMTimeServiceTimerBusObjAdapter::createCustomInterfaceHook(ajn::BusAttachment* bus)
{
    AJNBusAttachment *ajnBus = [[AJNBusAttachment alloc]initWithHandle:bus];
    return [handle createCustomInterfaceHook:ajnBus];
}

QStatus AJTMTimeServiceTimerBusObjAdapter::addTimerInterface(const ajn::InterfaceDescription& iface)
{
    AJNInterfaceDescription *ajnInt = [[AJNInterfaceDescription alloc]initWithHandle:(AJNHandle)&iface];
    return [handle addTimerInterface:ajnInt];
}

QStatus AJTMTimeServiceTimerBusObjAdapter::Get(const char* interfaceName, const char* propName, ajn::MsgArg& msgArg)
{
    AJNMessageArgument *ajnMsg = [[AJNMessageArgument alloc] initWithHandle:(AJNHandle)&msgArg];

    return [handle get:[NSString stringWithUTF8String:interfaceName] propName:[NSString stringWithUTF8String:propName] msgArg:ajnMsg];
}

QStatus AJTMTimeServiceTimerBusObjAdapter::Set(const char* interfaceName, const char* propName, ajn::MsgArg& msgArg)
{
    AJNMessageArgument *ajnMsg = [[AJNMessageArgument alloc] initWithHandle:(AJNHandle)&msgArg];

    return [handle set:[NSString stringWithUTF8String:interfaceName] propName:[NSString stringWithUTF8String:propName] msgArg:ajnMsg];
}