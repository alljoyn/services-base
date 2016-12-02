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

#ifndef NOTIFICATIONPRODUCER_H_
#define NOTIFICATIONPRODUCER_H_

#include <alljoyn/BusObject.h>

namespace ajn {
namespace services {

/**
 * NotificationProducer
 * This class has method to dismiss the notification.
 * The producer will send dismiss signal to notify other entities.
 */
class NotificationProducer : public ajn::BusObject {

  public:

    /**
     * Constructor for NotificationProducer. Creates Interface and prepares
     * infrastructure to be able to send Signal
     * @param bus         - BusAttachment that is used
     * @param status      - success/failure
     */
    NotificationProducer(ajn::BusAttachment* bus, QStatus& status);

    /**
     * Destructor for NotificationTransport
     */
    virtual ~NotificationProducer() = 0;
    /**
     * Callback for GetProperty
     * @param ifcName - interface name
     * @param propName - property name to get
     * @param val - value requested
     * @return status
     */
    QStatus Get(const char* ifcName, const char* propName, MsgArg& val);
    /**
     * Callback for SetProperty
     * @param ifcName - interface name
     * @param propName - property name to set
     * @param val - value to set
     * @return status
     */
    QStatus Set(const char* ifcName, const char* propName, MsgArg& val);
  protected:
    /**
     *  pointer to InterfaceDescription
     */
    InterfaceDescription* m_InterfaceDescription;
    /**
     *  pointer to BusAttachment
     */
    ajn::BusAttachment* m_BusAttachment;
  private:

};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONPRODUCER_H_ */