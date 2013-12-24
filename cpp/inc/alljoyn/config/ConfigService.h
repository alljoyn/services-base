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

#ifndef CONFIGSERVICE_H
#define CONFIGSERVICE_H

#include <alljoyn/about/PropertyStore.h>
#include <alljoyn/BusObject.h>
#include <alljoyn/services_common/GenericLogger.h>
#include <alljoyn/services_common/ServicesLoggerImpl.h>

namespace ajn {
namespace services {
/**
 * ConfigService is an AllJoyn BusObject that implements the org.alljoyn.Config standard interface.
 * Applications that provide AllJoyn IoE services with config capability
 */
class ConfigService : public ajn::BusObject {
  public:

    /**
     *	Listener is a callback that is called by ConfigService implemented  by the application to provide system calls and control
     *
     */
    class Listener {
      public:
        /**
         * Application should implement Restart of the device.
         * @return status - success/failure
         */
        virtual QStatus Restart() = 0;
        /**
         * Application should implement FactoryReset  of the device ,return to default values including password !
         * @return status - success/failure
         */
        virtual QStatus FactoryReset() = 0;
        /**
         *	Application should receive Passphrase info and persist it.
         * @param[in] daemonRealm to persist
         * @param[in] passcodeSize	size in bytes of the passcode
         * @param[in] passcode content
         * @return status - success/failure
         */
        virtual QStatus SetPassphrase(const char* daemonRealm, size_t passcodeSize, const char* passcode) = 0;
        /**
         *
         */
        virtual ~Listener() = 0;
    };

    /**
     * Constructor of a ConfigService
     * @param bus reference
     * @param store reference
     * @param listener reference
     */
    ConfigService(ajn::BusAttachment& bus, PropertyStore& store, Listener& listener);

    /**
     * Destructor of ConfigService
     */
    ~ConfigService();

    /**
     * Register the ConfigService on the alljoyn bus.
     * @return status.
     */
    QStatus Register();

    /**
     * Receive GenericLogger* to use for logging
     * @param logger Implementation of GenericLogger
     * @return previous logger
     */
    GenericLogger* setLogger(GenericLogger* logger);

    /**
     * Get the currently-configured logger implementation
     * @return logger Implementation of GenericLogger
     */
    GenericLogger* getLogger();

    /**
     * Set log level filter for subsequent logging messages
     * @param newLogLevel enum value
     * @return logLevel enum value that was in effect prior to this change
     */
    Log::LogLevel setLogLevel(Log::LogLevel newLogLevel);

    /**
     * Get log level filter value currently in effect
     * @return logLevel enum value currently in effect
     */
    Log::LogLevel getLogLevel();


    /**
     * A callback passed to the Generic Logger when the default logger is replaced by a different logger
     * @param type - message type
     * @param module - module of the message
     * @param msg - message
     * @param context - context passed in by the application
     */
    static void GenericLoggerCallBack(DbgMsgType type, const char* module, const char* msg, void* context);

  private:

    /**
     * Handles the FactoryReset method
     * @param member
     * @param msg reference of alljoyn Message
     */
    void FactoryResetHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * Handles the Restart method
     * @param member
     * @param msg reference of alljoyn Message
     */
    void RestartHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * Handles the SetPasscode method
     * @param member
     * @param msg reference of alljoyn Message
     */
    void SetPasscodeHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * Handles the GetConfiguration method
     * @param member
     * @param msg reference of alljoyn Message
     */
    void GetConfigurationsHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * Handles the UpdateConfigurations method
     * @param member
     * @param msg reference of alljoyn Message
     */
    void UpdateConfigurationsHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * Handles the DeleteConfigurations method
     * @param member
     * @param msg reference of alljoyn Message
     */
    void ResetConfigurationsHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * Handles the GetLanguages method
     * @param member
     * @param msg of alljoyn received
     */
    void GetLanguagesHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * Handles the SetDefaultLanguage method
     * @param member
     * @param msg reference of alljoyn Message
     */
    void SetDefaultLanguageHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * Handles the GetPropery request
     * @param ifcName  interface name
     * @param propName the name of the propery
     * @param val reference of MsgArg out parameter.
     * @return status - success/failure
     */
    QStatus Get(const char* ifcName, const char* propName, MsgArg& val);

    /**
     * pointer of BusAttachment
     */
    ajn::BusAttachment* m_BusAttachment;

    /**
     * pointer of PropertyStore implementing the storage.
     */
    PropertyStore* m_PropertyStore;

    /**
     * pointer of Listener
     */
    Listener* m_Listener;

    /**
     * Logger that is used in library
     */
    GenericLogger* logger;

    /**
     * Default Logger that is used in library
     */
    ServicesLoggerImpl configLogger;
};
inline ConfigService::Listener::~Listener() {
}

}
}

#endif
