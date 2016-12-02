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

/**
 * This class is experimental, and as such has not been tested.
 * Please help make it more robust by contributing fixes if you find issues
 **/

#include "AJTMTimeServiceAlarmBusObjAdapter.h"
#import "AJNBusAttachment.h"
#import "AJNInterfaceDescription.h"


AJTMTimeServiceAlarmBusObjAdapter::AJTMTimeServiceAlarmBusObjAdapter(id<AJTMTimeServiceAlarmBusObj> timeServiceAlarmBusObj) : TimeServiceAlarmBusObj("")
{
    handle = timeServiceAlarmBusObj;
}

AJTMTimeServiceAlarmBusObjAdapter::AJTMTimeServiceAlarmBusObjAdapter(id<AJTMTimeServiceAlarmBusObj> timeServiceAlarmBusObj, qcc::String const& objectPath) : TimeServiceAlarmBusObj(objectPath) 
{
    handle = timeServiceAlarmBusObj;
}

AJTMTimeServiceAlarmBusObjAdapter::~AJTMTimeServiceAlarmBusObjAdapter()
{
}

QStatus AJTMTimeServiceAlarmBusObjAdapter::createCustomInterfaceHook(ajn::BusAttachment* bus)
{
    AJNBusAttachment *ajnBus = [[AJNBusAttachment alloc]initWithHandle:bus];
    return [handle createCustomInterfaceHook:ajnBus];
}

QStatus AJTMTimeServiceAlarmBusObjAdapter::addAlarmInterface(const ajn::InterfaceDescription& iface)
{
    AJNInterfaceDescription *ajnInt = [[AJNInterfaceDescription alloc]initWithHandle:(AJNHandle)&iface];
    return [handle addAlarmInterface:ajnInt];
}

QStatus AJTMTimeServiceAlarmBusObjAdapter::Get(const char* interfaceName, const char* propName, ajn::MsgArg& msgArg)
{
    AJNMessageArgument *ajnMsg = [[AJNMessageArgument alloc] initWithHandle:(AJNHandle)&msgArg];

    return [handle get:[NSString stringWithUTF8String:interfaceName] propName:[NSString stringWithUTF8String:propName] msgArg:ajnMsg];
}

QStatus AJTMTimeServiceAlarmBusObjAdapter::Set(const char* interfaceName, const char* propName, ajn::MsgArg& msgArg)
{
    AJNMessageArgument *ajnMsg = [[AJNMessageArgument alloc] initWithHandle:(AJNHandle)&msgArg];

    return [handle set:[NSString stringWithUTF8String:interfaceName] propName:[NSString stringWithUTF8String:propName] msgArg:ajnMsg];
}