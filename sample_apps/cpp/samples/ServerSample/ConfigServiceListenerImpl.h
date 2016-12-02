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

#ifndef CONFIGSERVICELISTENERIMPL_H_
#define CONFIGSERVICELISTENERIMPL_H_

#include <alljoyn/config/ConfigService.h>
#include <AboutDataStore.h>
#include <CommonBusListener.h>

/**
 * class ConfigServiceListenerImpl
 * Config service Listener implementation
 */
class ConfigServiceListenerImpl : public ajn::services::ConfigService::Listener {
  public:
    /**
     * Constructor ConfigServiceListenerImpl
     * @param store
     * @param bus
     * @param busListener
     */
    ConfigServiceListenerImpl(AboutDataStore& store, ajn::BusAttachment& bus, CommonBusListener& busListener);

    /**
     * Method Restart
     * @return QStatus
     */
    virtual QStatus Restart();

    /**
     * Method FactoryReset
     * @return
     */
    virtual QStatus FactoryReset();

    /**
     * Method SetPassphrase
     * @param daemonRealm
     * @param passcodeSize
     * @param passcode
     * @param sessionId
     * @return
     */
    virtual QStatus SetPassphrase(const char* daemonRealm, size_t passcodeSize, const char* passcode, ajn::SessionId sessionId);

    /**
     * Destructor
     */
    virtual ~ConfigServiceListenerImpl();

  private:

    AboutDataStore* m_AboutDataStore;

    ajn::BusAttachment* m_Bus;

    CommonBusListener* m_BusListener;

    void PersistPassword(const char* daemonRealm, const char* passcode);
};

#endif /* CONFIGSERVICELISTENERIMPL_H_ */