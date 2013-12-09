/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
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

#ifndef PAYLOADADAPTER_H_
#define PAYLOADADAPTER_H_

#include <iostream>
#include <vector>
#include <map>
#include <alljoyn/Message.h>
#include <alljoyn/MsgArg.h>
#include <alljoyn/notification/NotificationText.h>
#include <alljoyn/notification/RichAudioUrl.h>
#include <alljoyn/notification/NotificationEnums.h>
#include <alljoyn/about/PropertyStore.h>

namespace ajn {
namespace services {

/**
 * PayloadAdapter - a class used to prepare the payload from variables to alljoyn message parameters
 * and unpack the alljoyn message parameters into the individual variables
 */
class PayloadAdapter {
  public:

    /**
     * Constructor of PayloadAdapter
     */
    PayloadAdapter() { };

    /**
     * Destructor of PayloadAdapter
     */
    ~PayloadAdapter() { };

    /**
     * SendPayload Marshals the Arguments to be sent
     * @param propertyStore
     * @param messageType
     * @param notificationText
     * @param customAttributes
     * @param ttl
     * @param richIconUrl
     * @param richAudioUrl
     * @param richIconObjectPath
     * @param richAudioObjectPath
     * @param controlPanelServiceObjectPath
     * @param messageId
     * @return status - success/failure
     */
    static QStatus sendPayload(ajn::services::PropertyStore* propertyStore,
                               NotificationMessageType messageType,
                               std::vector<NotificationText> const&  notificationText,
                               std::map<qcc::String, qcc::String> const& customAttributes,
                               uint16_t ttl,
                               const char* richIconUrl, std::vector<RichAudioUrl> const&  richAudioUrl,
                               const char* richIconObjectPath, const char* richAudioObjectPath,
                               const char* controlPanelServiceObjectPath);

    /**
     * SendPayload Marshals the Arguments to be sent
     * @param deviceId
     * @param deviceName
     * @param appId
     * @param appName
     * @param messageType
     * @param notificationText
     * @param customAttributes
     * @param ttl
     * @param richIconUrl
     * @param richAudioUrl
     * @param richIconObjectPath
     * @param richAudioObjectPath
     * @param controlPanelServiceObjectPath
     * @return status - success/failure
     */
    static QStatus sendPayload(const char* deviceId, const char* deviceName,
                               const char* appId, const char* appName,
                               NotificationMessageType messageType,
                               std::vector<NotificationText> const&  notificationText,
                               std::map<qcc::String, qcc::String> const& customAttributes,
                               uint16_t ttl,
                               const char* richIconUrl, std::vector<RichAudioUrl> const&  richAudioUrl,
                               const char* richIconObjectPath, const char* richAudioObjectPath,
                               const char* controlPanelServiceObjectPath, int32_t messageId);

    /**
     * ReceivePayload Unmarshals the arguments received
     * @param msg
     */
    static void receivePayload(ajn::Message& msg);

  private:

    /**
     * SendPayload Marshals the Arguments to be sent
     * @param deviceId
     * @param deviceName
     * @param appId
     * @param appName
     * @param messageType
     * @param notificationText
     * @param customAttributes
     * @param ttl
     * @param richIconUrl
     * @param richAudioUrl
     * @param richIconObjectPath
     * @param richAudioObjectPath
     * @param controlPanelServiceObjectPath
     * @param messageId
     * @return status - success/failure
     */
    static QStatus sendPayload(ajn::MsgArg deviceIdArg, ajn::MsgArg deviceNameArg,
                               ajn::MsgArg appIdArg, ajn::MsgArg appNameArg,
                               NotificationMessageType messageType,
                               std::vector<NotificationText> const&  notificationText,
                               std::map<qcc::String, qcc::String> const& customAttributes,
                               uint16_t ttl,
                               const char* richIconUrl, std::vector<RichAudioUrl> const&  richAudioUrl,
                               const char* richIconObjectPath, const char* richAudioObjectPath,
                               const char* controlPanelServiceObjectPath, int32_t messageId);

    /**
     * static MessageId
     */
    static int32_t m_MessageId;

    /**
     * Tag for Logging
     */
    static qcc::String TAG;
};
} //namespace services
} //namespace ajn

#endif /* PAYLOADADAPTER_H_ */

