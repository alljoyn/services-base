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

#ifndef ONBOARDINGCLIENT_H_
#define ONBOARDINGCLIENT_H_

#include <vector>
#include <alljoyn/Message.h>
#include <alljoyn/BusListener.h>
#include <alljoyn/BusObject.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/MsgArg.h>
#include <qcc/String.h>
#include <alljoyn/onboarding/Onboarding.h>
#include <alljoyn/services_common/GenericLogger.h>
#include <alljoyn/services_common/ServicesLoggerImpl.h>

namespace ajn {
namespace services {

/**
 * OnboardingClient is a helper class used by an AllJoyn IoE client application to onboard/offboard
 * nearby AllJoyn IoE service application devices.
 */
class OnboardingClient {

  public:

    /**
     * Constructor of OnboardingClient
     * @param[in] bus is a reference to BusAttachment
     */
    OnboardingClient(ajn::BusAttachment& bus);

    /**
     * Destruct OnboardingClient
     */
    ~OnboardingClient();

    /**
     *	map of AboutData using qcc::String as key and ajn::MsgArg as value.
     */
    typedef std::vector<OBScanInfo> ScanInfos;

    /**
     * Get the ScanInfo array for specified bus name.
     * @param[in] busName Unique or well-known name of AllJoyn node to retrieve Onboarding data from.
     * @param[out] age
     * @param[out] scanInfos
     * @param[in] sessionId sessionIdthe session received  after joining alljoyn session
     * @return Status.OK if successful.
     */
    QStatus GetScanInfo(const char* busName, unsigned short& age, ScanInfos& scanInfos, ajn::SessionId sessionId = 0);


    /**
     * Configure WiFi a specified bus name.
     * @param[in] busName busName Unique or well-known name of AllJoyn node to retrieve Onboarding data from.
     * @param[in] oBInfo reference to  OBInfo
     * @param[out] resultStatus the status of the request
     * @param[in] sessionId the session received  after joining alljoyn session
     * @return Status.OK if successful.
     */
    QStatus ConfigureWiFi(const char* busName, const OBInfo& oBInfo, short& resultStatus,  ajn::SessionId sessionId = 0);

    /**
     * Connect WiFi a specified bus name.
     *
     * @param[in] busName    Unique or well-known name of AllJoyn node to retrieve Onboarding data from.
     * @param[in] sessionId the session received  after joining alljoyn session
     * @return Status.OK if successful.
     */
    QStatus ConnectTo(const char* busName, ajn::SessionId sessionId = 0);

    /**
     * Offboard WiFi a specified bus name.
     *
     * @param[in] busName   Unique or well-known name of AllJoyn node to retrieve Onboarding data from.
     * @param[in] sessionId the session received  after joining alljoyn session
     * @return Status.OK if successful.
     */
    QStatus OffboardFrom(const char* busName, ajn::SessionId sessionId = 0);
    /**
     * GetVersion retrieves the Onboarding version
     * @param[in] busName  Unique or well-known name of AllJoyn node to retrieve Onboarding data from.
     * @param[out] version
     * @param[in] sessionId the session received  after joining alljoyn session
     * @return Status.OK if successful.
     */
    QStatus GetVersion(const char* busName, int& version, ajn::SessionId sessionId = 0);


    /**
     * GetState retrieves  the last state of the WIFI connection attempt
     * @param[in] busName Unique or well-known name of AllJoyn node to retrieve Onboarding data from.
     * @param[out] state
     * @param[in] sessionId the session received  after joining alljoyn session
     * @return Status.OK if successful.
     */
    QStatus GetState(const char* busName, short& state, ajn::SessionId sessionId = 0);

    /**
     * GetLastError retrieves the last error of WIFI connection attempt
     * @param[in] busName Unique or well-known name of AllJoyn node to retrieve Onboarding data from.
     * @param[out] lastError
     * @param[in] sessionId the session received  after joining alljoyn session
     * @return Status.OK if successful.
     */
    QStatus GetLastError(const char* busName, OBLastError& lastError, ajn::SessionId sessionId = 0);

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
     *	pointer to  BusAttachment
     */
    ajn::BusAttachment* m_BusAttachment;

    /**
     * Logger that is used in library
     */
    GenericLogger* logger;

    /**
     * Default Logger that is used in library
     */
    ServicesLoggerImpl onboardingLogger;

};

}
}

#endif /* ONBOARDINGCLIENT_H_ */
