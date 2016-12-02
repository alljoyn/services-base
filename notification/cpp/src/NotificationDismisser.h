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

#ifndef NOTIFICATIONDISMISSER_H_
#define NOTIFICATIONDISMISSER_H_

#include <alljoyn/BusAttachment.h>
#include <alljoyn/BusObject.h>
#include <alljoyn/InterfaceDescription.h>

namespace ajn {
namespace services {

/**
 * a base class to implement the dismisser interface
 */
class NotificationDismisser : public ajn::BusObject {
  public:

    /**
     * Constructor for NotificationDismisser. Creates Interface and prepares
     * infrastructure to be able to send Signal
     * @param bus         - BusAttachment that is used
     * @param objectPath  - object path name of the dismisser
     * @param status      - success/failure
     */
    NotificationDismisser(ajn::BusAttachment* bus, qcc::String const& objectPath, QStatus& status);

    /**
     * Destructor for NotificationDismisser
     */
    virtual ~NotificationDismisser() = 0;

    /**
     * Callback for GetProperty
     * @param ifcName - interface name
     * @param propName - property name to get
     * @param val - value requested
     * @return status
     */
    QStatus Get(const char* ifcName, const char* propName, ajn::MsgArg& val);

    /**
     * Callback for SetProperty
     * @param ifcName - interface name
     * @param propName - property name to set
     * @param val - value to set
     * @return status
     */
    QStatus Set(const char* ifcName, const char* propName, ajn::MsgArg& val);

    /**
     * Get the app Id
     * @return appId
     */
    const char* getAppId() const;

  protected:

    /**
     * The pointer used to send signal/register Signal Handler
     */
    const ajn::InterfaceDescription::Member* m_SignalMethod;

    /**
     * message Id of the last message sent with this message type
     */
    int32_t m_MsgId;
    /**
     * The Notification's App Id
     */
    const char* m_AppId;
    /**
     *  pointer to InterfaceDescription
     */
    InterfaceDescription* m_InterfaceDescription;
};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONDISMISSER_H_ */