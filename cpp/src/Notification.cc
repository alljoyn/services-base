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

#include <alljoyn/notification/Notification.h>
#include <alljoyn/notification/NotificationService.h>

using namespace ajn;
using namespace services;
using namespace qcc;

Notification::Notification(int32_t messageId,
                           NotificationMessageType messageType, const char* deviceId, const char* deviceName,
                           const char* appId, const char* appName, const char* sender,
                           std::map<qcc::String, qcc::String> const& customAttributes,
                           std::vector<NotificationText> const& notificationText,
                           const char* richIconUrl, std::vector<RichAudioUrl> const&  richAudioUrl,
                           const char* richIconObjectPath, const char* richAudioObjectPath,
                           const char* controlPanelServiceObjectPath) :
    m_MessageId(messageId), m_Sender(sender), m_MessageType(messageType), m_DeviceId(deviceId),
    m_DeviceName(deviceName), m_AppId(appId), m_AppName(appName), m_CustomAttributes(customAttributes),
    m_Text(notificationText), m_RichIconUrl(richIconUrl), m_RichAudioUrl(richAudioUrl), m_RichIconObjectPath(richIconObjectPath),
    m_RichAudioObjectPath(richAudioObjectPath), m_ControlPanelServiceObjectPath(controlPanelServiceObjectPath)
{

}

Notification::Notification(NotificationMessageType messageType, std::vector<NotificationText> const& notificationText) :
    m_MessageId(-1), m_Sender(0), m_MessageType(messageType), m_DeviceId(0),
    m_DeviceName(0), m_AppId(0), m_AppName(0), m_Text(notificationText), m_RichIconUrl(0), m_RichIconObjectPath(0),
    m_RichAudioObjectPath(0), m_ControlPanelServiceObjectPath(0)
{

}

const uint16_t Notification::getVersion() const
{
    return NotificationService::getVersion();
}

const char* Notification::getDeviceId() const
{
    return m_DeviceId;
}

const char* Notification::getDeviceName() const
{
    return m_DeviceName;
}

const char* Notification::getAppId() const
{
    return m_AppId;
}

const char* Notification::getAppName() const
{
    return m_AppName;
}

const std::map<qcc::String, qcc::String>& Notification::getCustomAttributes() const
{
    return m_CustomAttributes;
}

const int32_t Notification::getMessageId() const
{
    return m_MessageId;
}

const std::vector<NotificationText>& Notification::getText() const
{
    return m_Text;
}

const char* Notification::getSenderBusName() const
{
    return m_Sender;
}

const NotificationMessageType Notification::getMessageType() const
{
    return m_MessageType;
}

const char* Notification::getRichIconUrl() const
{
    return m_RichIconUrl;
}

const char* Notification::getRichIconObjectPath() const
{
    return m_RichIconObjectPath;
}

const char* Notification::getRichAudioObjectPath() const
{
    return m_RichAudioObjectPath;
}

const std::vector<RichAudioUrl>& Notification::getRichAudioUrl() const
{
    return m_RichAudioUrl;
}

const char* Notification::getControlPanelServiceObjectPath() const
{
    return m_ControlPanelServiceObjectPath;
}

void Notification::setAppId(const char* appId) {
    m_AppId = appId;
}

void Notification::setAppName(const char* appName) {
    m_AppName = appName;
}

void Notification::setControlPanelServiceObjectPath(
    const char* controlPanelServiceObjectPath) {
    m_ControlPanelServiceObjectPath = controlPanelServiceObjectPath;
}

void Notification::setCustomAttributes(
    const std::map<qcc::String, qcc::String>& customAttributes) {
    m_CustomAttributes = customAttributes;
}

void Notification::setDeviceId(const char* deviceId) {
    m_DeviceId = deviceId;
}

void Notification::setDeviceName(const char* deviceName) {
    m_DeviceName = deviceName;
}

void Notification::setMessageId(int32_t messageId) {
    m_MessageId = messageId;
}

void Notification::setRichAudioUrl(
    const std::vector<RichAudioUrl>& richAudioUrl) {
    m_RichAudioUrl = richAudioUrl;
}

void Notification::setRichIconUrl(const char* richIconUrl) {
    m_RichIconUrl = richIconUrl;
}

void Notification::setRichIconObjectPath(const char* richIconObjectPath) {
    m_RichIconObjectPath = richIconObjectPath;
}

void Notification::setRichAudioObjectPath(const char* richAudioObjectPath) {
    m_RichAudioObjectPath = richAudioObjectPath;
}

void Notification::setSender(const char* sender) {
    m_Sender = sender;
}
