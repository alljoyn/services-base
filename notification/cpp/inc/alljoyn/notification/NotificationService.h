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

#ifndef NOTIFICATIONSERVICE_H_
#define NOTIFICATIONSERVICE_H_

#include <qcc/Debug.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/notification/NotificationSender.h>
#include <alljoyn/AboutData.h>
#include <alljoyn/about/PropertyStore.h>

namespace ajn {
namespace services {

class NotificationReceiver;

/**
 * Notification Service class. Used to initialize and shutdown the service
 */
class NotificationService {

  public:

    /**
     * Get Instance of NotificationService - singleton implementation
     * @return instance
     */
    static NotificationService* getInstance();

    /**
     * Destructor for NotificationService
     */
    ~NotificationService();

    /**
     *  Initialize Producer side via Transport. Create and
     *  return NotificationSender.
     * @param bus
     * @param store
     * @return NotificationSender instance
     */

    NotificationSender* initSend(ajn::BusAttachment* bus, ajn::AboutData* store);

    /**
     *  Initialize Producer side via Transport. Create and
     *  return NotificationSender.
     * @param bus
     * @param store
     * @return NotificationSender instance
     */

    NotificationSender* initSend(ajn::BusAttachment* bus, ajn::services::PropertyStore* store);

    /**
     * Initialize Consumer side via Transport.
     * Set NotificationReceiver to given receiver
     * @param bus
     * @param notificationReceiver
     * @return status
     */
    QStatus initReceive(ajn::BusAttachment* bus, NotificationReceiver* notificationReceiver);

    /**
     * Stops sender but leaves bus and other objects alive
     */
    void shutdownSender();

    /**
     * Stops receiving but leaves bus and other objects alive
     */
    void shutdownReceiver();

    /**
     * Cleanup and get ready for shutdown
     */
    void shutdown();

    /**
     * Disabling superagent mode. Needs to be called before
     * starting receiver
     * @return status
     */
    QStatus disableSuperAgent();

    /**
     * Virtual method to get the busAttachment used in the service.
     */
    ajn::BusAttachment* getBusAttachment();

    /**
     * Get the Version of the NotificationService
     * @return the NotificationService version
     */
    static uint16_t getVersion();

  private:

    /**
     * Default constructor for NotificationServiceImpl
     * Private to allow for singleton implementation
     */
    NotificationService();

    /**
     * Version of the API
     */
    static uint16_t const NOTIFICATION_SERVICE_VERSION;

    /**
     * instance variable - NotificationServiceImpl is a singleton
     */
    static NotificationService* s_Instance;
};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONSERVICE_H_ */