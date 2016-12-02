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

#ifndef NOTIFICATIONTESTERIMPL_H_
#define NOTIFICATIONTESTERIMPL_H_

#include <alljoyn/notification/NotificationService.h>
#include "NotificationReceiverImpl.h"
#include <AboutDataStore.h>

/**
 * Class for testing notification functionality based on runtime configuration.
 * First call Initialize and ten call LoopHandler periodically based on the number of seconds returned.
 */
class NotificationTesterImpl {
  public:

    /**
     * Constructor
     */
    NotificationTesterImpl();

    /**
     * Destructor
     */
    ~NotificationTesterImpl();

    /**
     * Initialize - initializes the testing functionality
     * @param bus
     * @param store
     * @return true if initialization was successful
     */
    bool Initialize(ajn::BusAttachment* bus, AboutDataStore* store);

    /**
     * LoopHandler - function that executes the test functionality
     * @return maximum amount of time in seconds to wait before calling LoopHandler again
     */
    int32_t LoopHandler();

  private:

    enum NotificationRateType {
        RATE_FIXED, RATE_RANDOM
    };
    enum NotificationPriorityType {
        PRIORITY_FIXED, PRIORITY_RANDOM
    };

    void LoadConfiguration();
    void Cleanup();

    uint64_t nextMessageTime;
    uint64_t intervalOffset;
    ajn::services::NotificationService* prodService;
    NotificationReceiverImpl* receiver;
    ajn::services::NotificationSender* sender;

    bool notifProdEnabled;
    uint32_t notifInterval;
    uint32_t notifIntervalOffset;
    NotificationRateType notifRateType;
    uint16_t notifRatePriority;
    NotificationPriorityType notifRatePriorityType;
    qcc::String notifMsgText;
    qcc::String notifMsgLang;
    uint32_t notifTTL;
    bool notifCustEnabled;
};

#endif /* NOTIFICATIONTESTERIMPL_H_ */