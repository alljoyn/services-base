/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include "TimeClientAnnouncementHandler.h"
#include <alljoyn/about/AboutPropertyStoreImpl.h>
#include <qcc/StringUtil.h>

using namespace ajn;
using namespace services;

//Constructor
TimeClientAnnouncementHandler::TimeClientAnnouncementHandler(OnAnnouncementCallback onAnnounceCb) :
    AnnounceHandler(), m_OnAnnouncementCb(onAnnounceCb)
{
}

//Destructor
TimeClientAnnouncementHandler::~TimeClientAnnouncementHandler()
{
}

//Handle received Announcement signal
void TimeClientAnnouncementHandler::Announce(unsigned short version, unsigned short port, const char* busName,
                                             const ObjectDescriptions& objectDescs, const AboutData& aboutData)
{

    printf("Received Announcement from '%s', handling... \n", busName);

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

    m_OnAnnouncementCb(busName, deviceId, appId, uniqueKey, objectDescs);
}

//Unmarshal DeviceId
void TimeClientAnnouncementHandler::unmarshalDeviceId(const char* busName, qcc::String* deviceId, const AboutData& aboutData)
{

    qcc::String deviceIdKey        = AboutPropertyStoreImpl::getPropertyStoreName(DEVICE_ID);
    AboutData::const_iterator iter = aboutData.find(deviceIdKey);

    if (iter == aboutData.end()) {

        printf("Received corrupted Announcement from '%s', no DeviceId \n", busName);
        return;
    }

    const MsgArg& deviceIdMsgArg = iter->second;
    char* tmpDeviceId;

    QStatus status               = deviceIdMsgArg.Get("s", &tmpDeviceId);

    if (status != ER_OK) {

        printf("Received corrupted Announcement from '%s', failed to unmarshal deviceId \n", busName);
        return;
    }

    deviceId->assign(tmpDeviceId);

    return;
}

//Unmarshal AppId
void TimeClientAnnouncementHandler::unmarshalAppId(const char* busName, qcc::String* appId, const AboutData& aboutData)
{

    qcc::String appIdKey           = AboutPropertyStoreImpl::getPropertyStoreName(APP_ID);
    AboutData::const_iterator iter = aboutData.find(appIdKey);

    if (iter == aboutData.end()) {

        printf("Received corrupted Announcement from '%s', no AppId \n", busName);
        return;
    }

    const MsgArg& appIdMsgArg = iter->second;
    size_t numElements        = 0;
    uint8_t* appIdBuffer      = NULL;

    QStatus status = appIdMsgArg.Get("ay", &numElements, &appIdBuffer);

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
