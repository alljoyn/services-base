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

#ifndef NOTIFICATIONSENDER_H_
#define NOTIFICATIONSENDER_H_

#include <vector>
#include <map>
#include <alljoyn/Status.h>
#include <alljoyn/AboutData.h>
#include <alljoyn/notification/NotificationEnums.h>

#include <alljoyn/about/PropertyStore.h>

namespace ajn {
namespace services {

class Notification;

/**
 * The class used to send Notifications or delete the last sent Notification
 */
class NotificationSender {
  public:

    /**
     * Constructor for NotificationSenderImpl
     * @param aboutdata - AboutData that includes entries
     * for deviceId, deviceName, appId and appName
     */
    NotificationSender(ajn::AboutData* aboutdata);

    /**
     * Constructor for NotificationSenderImpl
     * @param propertyStore - propertyStoreImplementation that includes entries
     * for deviceId, deviceName, appId and appName
     */
    NotificationSender(ajn::services::PropertyStore* propertyStore);

    /**
     * Destructor for NotificationSenderImpl
     */
    ~NotificationSender() { };

    /**
     * Send notification
     * @param notification
     * @param ttl
     * @return
     */
    QStatus send(Notification const& notification, uint16_t ttl);

    /**
     * Delete last message that was sent with given MessageType
     * @param messageType MessageType of message to be deleted
     * @return success/failure
     */
    QStatus deleteLastMsg(NotificationMessageType messageType);


  private:

    /**
     * Pointer to AboutData implementing the storage.
     */
    ajn::AboutData* m_aboutdata;

    /**
     * Pointer to PropertyStore implementing the storage.
     */
    ajn::services::PropertyStore* m_PropertyStore;

};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONSENDER_H_ */