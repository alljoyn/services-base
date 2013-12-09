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

#include <stdio.h>
#include <alljoyn/onboarding/OnboardingService.h>
#include <alljoyn/BusAttachment.h>
#include <qcc/Debug.h>

#define CALLBACKTAG "AllJoynInternal"
#define TAG "ALLJOYN_ONBOARDING_SERVICE"
#define CHECK_RETURN(x) if ((status = x) != ER_OK) return status;
#define CHECK_BREAK(x) if ((status = x) != ER_OK) break;

using namespace ajn;
using namespace services;

static const char* ONBOARDING_INTERFACE_NAME = "org.alljoyn.Onboarding";
static const char* const ERROR_INVALID = "org.alljoyn.Error.InvalidValue";
static const char* const ERROR_OUTOFRANGE = "org.alljoyn.Error.OutOfRange";
static const char* const ERROR_SSID_EMPTY = "SSID was empty";
static const char* const ERROR_SSID_LONG = "Maximum SSID length is 32 characters";
static const char* const ERROR_AUTHTYPE_INVALID = "Authentication protocol was invalid";
static const char* const ERROR_PASSPHRASE_EMPTY = "Passphrase was empty";
static const char* const ERROR_PASSPHRASE_NOTEMPTY = "Passphrase should be empty for open authentication";
static const char* const ERROR_PASSPHRASE_SHORT = "Minimum passphrase length is 8 characters";
static const char* const ERROR_PASSPHRASE_LONG = "Maximum passphrase length is 64 characters";
static const char* const ERROR_WEPKEY_INVALID = "Invalid WEP key length";

OnboardingService::OnboardingService(ajn::BusAttachment& bus, OnboardingControllerAPI& pOnboardingControllerAPI) :
    BusObject("/Onboarding"), m_BusAttachment(&bus), m_OnboardingController(pOnboardingControllerAPI), logger(0)
{
    setLogger(&onboardingLogger);
    if (logger)
        logger->debug(TAG, "In OnboardingService Constructor");
}

QStatus OnboardingService::Register()
{
    if (logger)
        logger->debug(TAG, "In OnboardingService Register");
    QStatus status = ER_OK;

    InterfaceDescription* intf = NULL;
    intf = const_cast<InterfaceDescription*>(m_BusAttachment->GetInterface(ONBOARDING_INTERFACE_NAME));
    if (!intf) {
        CHECK_RETURN(m_BusAttachment->CreateInterface(ONBOARDING_INTERFACE_NAME, intf, AJ_IFC_SECURITY_REQUIRED));
        if (!intf)
            return ER_FAIL;

        CHECK_RETURN(intf->AddMethod("ConfigureWiFi", "ssn", "n", "SSID,passphrase,authType,status", 0));
        CHECK_RETURN(intf->AddMethod("Connect", NULL, NULL, NULL, 0));
        CHECK_RETURN(intf->AddMemberAnnotation("Connect", org::freedesktop::DBus::AnnotateNoReply, "true"));
        CHECK_RETURN(intf->AddMethod("Offboard", NULL, NULL, NULL, 0));
        CHECK_RETURN(intf->AddMemberAnnotation("Offboard", org::freedesktop::DBus::AnnotateNoReply, "true"));
        CHECK_RETURN(intf->AddMethod("GetScanInfo", NULL, "qa(sn)", "age,scanList", 0));
        CHECK_RETURN(intf->AddProperty("LastError", "(ns)", PROP_ACCESS_READ));
        CHECK_RETURN(intf->AddProperty("State", "n", PROP_ACCESS_READ));
        CHECK_RETURN(intf->AddProperty("Version", "q", PROP_ACCESS_READ));
        CHECK_RETURN(intf->AddSignal("ConnectionResult", "(ns)", "signalArg0", 0));
        intf->Activate();
    } //if (!intf)

    //Add the handlers
    CHECK_RETURN(AddInterface(*intf));
    CHECK_RETURN(AddMethodHandler(intf->GetMember("ConfigureWiFi"),
                                  static_cast<MessageReceiver::MethodHandler>(&OnboardingService::ConfigureWiFiHandler)));
    CHECK_RETURN(AddMethodHandler(intf->GetMember("Connect"),
                                  static_cast<MessageReceiver::MethodHandler>(&OnboardingService::ConnectHandler)));
    CHECK_RETURN(AddMethodHandler(intf->GetMember("Offboard"),
                                  static_cast<MessageReceiver::MethodHandler>(&OnboardingService::OffboardHandler)));
    CHECK_RETURN(AddMethodHandler(intf->GetMember("GetScanInfo"),
                                  static_cast<MessageReceiver::MethodHandler>(&OnboardingService::GetScanInfoHandler)));

    return status;
}

static uint8_t A2H(char hex, QStatus* status)
{
    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    }

    hex |= 0x20;
    if (hex >= 'a' && hex <= 'f') {
        return 10 + hex - 'a';
    } else if (hex >= 'A' && hex <= 'F') {
        return 10 + hex - 'A';
    } else {
        *status = ER_FAIL;
        return 0;
    }
}

QStatus HexToRaw(const char* hex, size_t hexLen, char* raw, size_t rawLen)
{
    QStatus status = ER_OK;
    char* p = (char*)raw;
    size_t sz = hexLen ? hexLen : strlen(hex);
    size_t i;

    /*
     * Length of encoded hex must be an even number
     */
    if (sz & 1) {
        return ER_FAIL;
    }
    if (rawLen < (sz / 2)) {
        return ER_FAIL;
    }
    for (i = 0; (i < sz) && (status == ER_OK); i += 2, hex += 2) {
        *p++ = (A2H(hex[0], &status) << 4) | A2H(hex[1], &status);
    }
    return status;
}

void OnboardingService::ConfigureWiFiHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    if (logger)
        logger->debug(TAG, "In OnboardingService ConfigureWiFiHandler");
    const ajn::MsgArg* args;
    size_t numArgs;
    QStatus status = ER_OK;
    msg->GetArgs(numArgs, args);
    do {
        if (numArgs != 3)
            break;

        char* SSID;
        char*passphrase;
        short authType;
        CHECK_BREAK(args[0].Get("s", &SSID));
        CHECK_BREAK(args[1].Get("s", &passphrase));
        CHECK_BREAK(args[2].Get("n", &authType));

        short configureWifiStatus = 0;
        qcc::String error;
        qcc::String errorMessage;

        qcc::String strSSID(SSID);
        qcc::String strPass(passphrase);

        // Platform independent validation and logic
        if (strSSID.empty()) {
            error = ERROR_INVALID;
            errorMessage = ERROR_SSID_EMPTY;
        } else if (strSSID.size() > MAX_SSID_SIZE) {
            error = ERROR_INVALID;
            errorMessage = ERROR_SSID_LONG;
        }

        if (authType < WPA2_AUTO || authType > WPS) {
            error = ERROR_OUTOFRANGE;
            errorMessage = ERROR_AUTHTYPE_INVALID;
        }

        if (OPEN == authType) {
            if (!strPass.empty()) {
                error = ERROR_INVALID;
                errorMessage = ERROR_PASSPHRASE_NOTEMPTY;
            }
        } else if (WEP == authType) {
            if (strPass.empty()) {
                error = ERROR_INVALID;
                errorMessage = ERROR_PASSPHRASE_EMPTY;
            } else {
                if (!isValidWEPKey(strPass)) {
                    error = ERROR_INVALID;
                    errorMessage = ERROR_WEPKEY_INVALID;
                }
            }
        } else if (ANY != authType) {
            if (strPass.empty()) {
                error = ERROR_INVALID;
                errorMessage = ERROR_PASSPHRASE_EMPTY;
            } else if (strPass.size() < MIN_PSK_SIZE) {
                error = ERROR_INVALID;
                errorMessage = ERROR_PASSPHRASE_SHORT;
            } else if (strPass.size() / 2 > MAX_PSK_SIZE) {
                error = ERROR_INVALID;
                errorMessage = ERROR_PASSPHRASE_LONG;
            }
        }

        if (error.empty() && errorMessage.empty()) {
            if (WEP != authType) {
                size_t rawLength = strPass.length() / 2 + 1;
                char raw[rawLength];
                CHECK_BREAK(HexToRaw(strPass.c_str(), strPass.length(), raw, rawLength));
                raw[strPass.length() / 2] = '\0';
                m_OnboardingController.ConfigureWiFi(SSID, raw, authType, configureWifiStatus, error, errorMessage);
            } else {
                m_OnboardingController.ConfigureWiFi(SSID, strPass, authType, configureWifiStatus, error, errorMessage);
            }

            MsgArg retArgs[1];
            CHECK_BREAK(retArgs[0].Set("n", configureWifiStatus));
            MethodReply(msg, retArgs, 1);
        } else {
            MethodReply(msg, error.c_str(), errorMessage.c_str());
        }
        return;
    } while (0);

    MethodReply(msg, ER_INVALID_DATA);

}

void OnboardingService::Check_MethodReply(const Message& msg, const MsgArg* args, size_t numArgs)
{
    QStatus status = ER_OK;
    //check it the ALLJOYN_FLAG_NO_REPLY_EXPECTED exists if so send response
    if (!(msg->GetFlags() & ALLJOYN_FLAG_NO_REPLY_EXPECTED)) {
        status = MethodReply(msg, args, numArgs);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Method did not execute successfully. Status: " + qcc::String(QCC_StatusText(status)));
        }
    }
}

void OnboardingService::Check_MethodReply(const Message& msg, QStatus status)
{
    //check it the ALLJOYN_FLAG_NO_REPLY_EXPECTED exists if so send response ER_INVALID_DATA
    if (!(msg->GetFlags() & ALLJOYN_FLAG_NO_REPLY_EXPECTED)) {
        status = MethodReply(msg, status);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Method did not execute successfully. Status: " + qcc::String(QCC_StatusText(status)));
        }
    }
}

void OnboardingService::ConnectHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    if (logger)
        logger->debug(TAG, "In OnboardingService ConnectHandler");
    const ajn::MsgArg* args = 0;
    size_t numArgs = 0;
    msg->GetArgs(numArgs, args);
    do {
        if (numArgs != 0)
            break;
        MsgArg args[0];
        Check_MethodReply(msg, args, 0);
        m_OnboardingController.Connect();
        return;
    } while (0);

    Check_MethodReply(msg, ER_INVALID_DATA);
}

void OnboardingService::OffboardHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    if (logger)
        logger->debug(TAG, "In OnboardingService OffboardHandler");
    const ajn::MsgArg* args = 0;
    size_t numArgs = 0;
    do {
        msg->GetArgs(numArgs, args);
        if (numArgs != 0)
            break;
        MsgArg args[0];
        Check_MethodReply(msg, args, 0);
        m_OnboardingController.Offboard();
        return;
    } while (0);

    Check_MethodReply(msg, ER_INVALID_DATA);
}

void OnboardingService::GetScanInfoHandler(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    if (logger)
        logger->debug(TAG, "In OnboardingService GetScanInfoHandler");
    const ajn::MsgArg* args = NULL;
    size_t numArgs = 0;
    QStatus status = ER_OK;
    msg->GetArgs(numArgs, args);
    do {
        if (numArgs != 0)
            break;
        unsigned short age = 0;
        size_t numberOfElements = 0;
        OBScanInfo* scanInfoList = NULL; // no need to delete scanInfoList it will be delete by the MsgArgs
        m_OnboardingController.GetScanInfo(age, scanInfoList, numberOfElements);
        if (!(scanInfoList != NULL && numberOfElements > 0)) {
            Check_MethodReply(msg, ER_FAIL);
            return;
        }
        MsgArg scanInfoArgs[numberOfElements];
        for (size_t i = 0; i < numberOfElements; i++) {
            CHECK_BREAK(scanInfoArgs[i].Set("(sn)", scanInfoList[i].SSID.c_str(), scanInfoList[i].authType));
        }
        CHECK_BREAK(status); //In case a break came from the for loop we want to break again.
        MsgArg retArgs[2];
        CHECK_BREAK(retArgs[0].Set("q", age));
        CHECK_BREAK(retArgs[1].Set("a(sn)", numberOfElements, scanInfoArgs));
        Check_MethodReply(msg, retArgs, 2);
        return;

    } while (0);

    Check_MethodReply(msg, ER_INVALID_DATA);
}

QStatus OnboardingService::Get(const char* ifcName, const char* propName, MsgArg& val)
{
    if (logger)
        logger->debug(TAG, "In OnboardingService GetProperty");
    QStatus status = ER_OK;
// Check the requested property and return the value if it exists
    if (0 == strcmp(ifcName, ONBOARDING_INTERFACE_NAME)) {
        if (0 == strcmp("Version", propName)) {
            status = val.Set("q", 1);
        } else if (0 == strcmp("State", propName)) {
            status = val.Set("q", m_OnboardingController.GetState());
        } else if (0 == strcmp("LastError", propName)) {
            OBLastError lastError = m_OnboardingController.GetLastError();
            if (logger) {
                qcc::String message = qcc::String("Last State: ") + lastError.validationState + "lastErrorMessage: " + lastError.message;
                logger->debug(TAG, message);
            }
            status = val.Set("(ns)", lastError.validationState, lastError.message.c_str());
        } else {
            status = ER_BUS_NO_SUCH_PROPERTY;
        }
    } else {
        status = ER_BUS_NO_SUCH_PROPERTY;
    }
    return status;
}

GenericLogger* OnboardingService::setLogger(GenericLogger* newLogger)
{
    GenericLogger* prevLogger = getLogger();
    Log::LogLevel prevLogLevel = getLogLevel();
    logger = newLogger;
    if (logger) {
        setLogLevel(prevLogLevel);
        // reroute internal AJ logging to logger
        ServicesLoggerImpl::RegisterCallBack(logger, &GenericLoggerCallBack, this);
    }
    return prevLogger;
}

GenericLogger* OnboardingService::getLogger()
{
    return logger;
}

Log::LogLevel OnboardingService::setLogLevel(Log::LogLevel newLogLevel)
{
    return logger ? logger->setLogLevel(newLogLevel) : Log::LogLevel::LEVEL_INFO;
}

Log::LogLevel OnboardingService::getLogLevel()
{
    return logger ? logger->getLogLevel() : Log::LogLevel::LEVEL_INFO;
}

void OnboardingService::GenericLoggerCallBack(DbgMsgType type, const char* module, const char* msg, void* context)
{
    OnboardingService* onboardingService = (OnboardingService*)context;
    GenericLogger* logger = onboardingService->getLogger();
    if (logger) {
        Log::LogLevel currLogLevel = logger->getLogLevel();
        switch (type) {
        case DBG_LOCAL_ERROR:
        case DBG_REMOTE_ERROR:
            if (currLogLevel >= Log::LEVEL_ERROR)
                logger->error(CALLBACKTAG, msg);
            break;

        case DBG_GEN_MESSAGE:
            if (currLogLevel >= Log::LEVEL_INFO)
                logger->info(CALLBACKTAG, msg);
            break;

        case DBG_API_TRACE:
            if (currLogLevel >= Log::LEVEL_DEBUG)
                logger->debug(CALLBACKTAG, msg);
            break;

        case DBG_HIGH_LEVEL:
            if (currLogLevel >= Log::LEVEL_WARN)
                logger->warn(CALLBACKTAG, msg);
            break;

        case DBG_REMOTE_DATA:
        case DBG_LOCAL_DATA:
            if (currLogLevel >= Log::LEVEL_DEBUG)
                logger->debug(CALLBACKTAG, msg);
            break;

        default:
            if (currLogLevel >= Log::LEVEL_DEBUG)
                logger->debug(CALLBACKTAG, msg);
        }
    }
}
