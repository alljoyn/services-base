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

#ifndef NOTIFICATIONTRANSPORT_H_
#define NOTIFICATIONTRANSPORT_H_

#include <alljoyn/BusAttachment.h>
#include <alljoyn/BusObject.h>
#include <alljoyn/InterfaceDescription.h>

namespace ajn {
namespace services {

/**
 * A class implementing the BusObject used to create the Notification interface
 * and register it on the bus
 */
class NotificationTransport : public ajn::BusObject {
  public:

    /**
     * Constructor for NotificationTransport. Creates Interface and prepares
     * infrastructure to be able to send Signal
     * @param bus         - BusAttachment that is used
     * @param servicePath - servicePath of BusObject
     * @param status      - success/failure
     */
    NotificationTransport(ajn::BusAttachment* bus, qcc::String const& servicePath,
                          QStatus& status, qcc::String const& interfaceName);

    /**
     * Destructor for NotificationTransport
     */
    virtual ~NotificationTransport() = 0;

    /**
     * Callback for GetProperty
     * @param ifcName
     * @param propName
     * @param val
     * @return
     */
    QStatus Get(const char* ifcName, const char* propName, ajn::MsgArg& val);

    /**
     * Callback for SetProperty
     * @param ifcName
     * @param propName
     * @param val
     * @return
     */
    QStatus Set(const char* ifcName, const char* propName, ajn::MsgArg& val);

  protected:

    /**
     * The pointer used to send signal/register Signal Handler
     */
    const ajn::InterfaceDescription::Member* m_SignalMethod;
};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONTRANSPORT_H_ */