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

#include "TimeClientAnnouncementHandler.h"
#include <qcc/StringUtil.h>
#include <alljoyn/AboutData.h>

using namespace ajn;
using namespace services;

//Constructor
TimeClientAnnouncementHandler::TimeClientAnnouncementHandler(OnAnnouncementCallback onAnnounceCb) :
    m_OnAnnouncementCb(onAnnounceCb)
{
}

//Destructor
TimeClientAnnouncementHandler::~TimeClientAnnouncementHandler()
{
}

//Handle received Announcement signal
void TimeClientAnnouncementHandler::Announced(const char* busName, uint16_t version, SessionPort port, const MsgArg& objectDescriptionArg, const MsgArg& aboutDataArg)
{
    printf("Received Announcement from '%s', handling... \n", busName);

    AboutData aboutData;
    aboutData.CreatefromMsgArg(aboutDataArg);
    AboutObjectDescription aboutObjectDescription;
    aboutObjectDescription.CreateFromMsgArg(objectDescriptionArg);

    qcc::String deviceId;
    unmarshalDeviceId(busName, &deviceId, aboutData);

    if (deviceId.length() == 0) {

        return;
    }

    qcc::String appId;
    unmarshalAppId(busName, &appId, aboutData);

    if (appId.length() == 0) {

        return;
    }

    qcc::String uniqueKey = createUniqueKey(deviceId, appId);

    m_OnAnnouncementCb(busName, deviceId, appId, uniqueKey, aboutObjectDescription);
}

//Unmarshal DeviceId
void TimeClientAnnouncementHandler::unmarshalDeviceId(const char* busName, qcc::String* deviceId, const AboutData& aboutData)
{
    char* tmpDeviceId = NULL;
    QStatus status = const_cast<AboutData&>(aboutData).GetDeviceId(&tmpDeviceId);
    if (status != ER_OK) {

        printf("Received corrupted Announcement from '%s', no deviceId \n", busName);
        return;
    }

    deviceId->assign(tmpDeviceId);

    return;
}

//Unmarshal AppId
void TimeClientAnnouncementHandler::unmarshalAppId(const char* busName, qcc::String* appId, const AboutData& aboutData)
{

    size_t numElements        = 0;
    uint8_t* appIdBuffer      = NULL;
    QStatus status = const_cast<AboutData&>(aboutData).GetAppId(&appIdBuffer, &numElements);

    if (status != ER_OK || numElements < 16) {

        printf("Received corrupted Announcement from '%s', failed to unmarshal AppId \n", busName);
        return;
    }

    appId->assign(qcc::BytesToHexString(appIdBuffer, numElements).c_str());
}

//Create UniqueKey
qcc::String TimeClientAnnouncementHandler::createUniqueKey(const qcc::String& deviceId, const qcc::String& appId)
{

    qcc::String retValue = deviceId + "_" + appId;
    return retValue;
}