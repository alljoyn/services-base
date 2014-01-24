/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
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

#include <alljoyn/onboarding/OnboardingClient.h>
#include <alljoyn/AllJoynStd.h>
#include <qcc/Debug.h>

#define CALLBACKTAG "AllJoynInternal"
#define TAG "ALLJOYN_ONBOARDING_CLIENT"
#define CHECK_RETURN_STATUS(x) if ((status = x) != ER_OK) { return status; }
#define CHECK_RETURN(x) if ((status = x) != ER_OK) { return; }
#define CHECK_BREAK(x) if ((status = x) != ER_OK) { break; }

using namespace ajn;
using namespace services;

static const char* ONBOARDING_OBJECT_PATH = "/Onboarding";
static const char* ONBOARDING_INTERFACE_NAME = "org.alljoyn.Onboarding";

OnboardingClient::OnboardingClient(BusAttachment& bus) :
    m_BusAttachment(&bus), logger(0)
{
    setLogger(&onboardingLogger);
    if (logger) {
        logger->debug(TAG, "In OnboardingClient Constructor");
    }

    QStatus status = ER_OK;

    const InterfaceDescription* getIface = NULL;
    getIface = m_BusAttachment->GetInterface(ONBOARDING_INTERFACE_NAME);
    if (!getIface) {
        InterfaceDescription* createIface = NULL;
        status = m_BusAttachment->CreateInterface(ONBOARDING_INTERFACE_NAME, createIface, AJ_IFC_SECURITY_REQUIRED);
        if (createIface) {
            CHECK_RETURN(createIface->AddMethod("ConfigureWiFi", "ssn", "n", "SSID,passphrase,authType,status", 0));
            CHECK_RETURN(createIface->AddMethod("Connect", NULL, NULL, NULL, 0));
            CHECK_RETURN(createIface->AddMemberAnnotation("Connect", org::freedesktop::DBus::AnnotateNoReply, "true"));
            CHECK_RETURN(createIface->AddMethod("Offboard", NULL, NULL, NULL, 0));
            CHECK_RETURN(createIface->AddMemberAnnotation("Offboard", org::freedesktop::DBus::AnnotateNoReply, "true"));
            CHECK_RETURN(createIface->AddMethod("GetScanInfo", NULL, "qa(sn)", "age,scanList", 0));
            CHECK_RETURN(createIface->AddProperty("LastError", "(ns)", PROP_ACCESS_READ));
            CHECK_RETURN(createIface->AddProperty("State", "n", PROP_ACCESS_READ));
            CHECK_RETURN(createIface->AddProperty("Version", "q", PROP_ACCESS_READ));
            CHECK_RETURN(createIface->AddSignal("ConnectionResult", "(ns)", "signalArg0", 0));
            createIface->Activate();
        }
    }

}

OnboardingClient::~OnboardingClient()
{
    if (logger) {
        logger->debug(TAG, "In OnboardingClient Destructor");
    }
}

QStatus OnboardingClient::GetScanInfo(const char* busName, unsigned short& age, ScanInfos& scanInfos,
                                      ajn::SessionId sessionId)
{
    if (logger) {
        logger->debug(TAG, "In OnboardingClient GetScanInfo");
    }
    QStatus status = ER_OK;

    const InterfaceDescription* ifc = m_BusAttachment->GetInterface(ONBOARDING_INTERFACE_NAME);
    if (!ifc) {
        return ER_FAIL;
    }

    ProxyBusObject*proxyBusObj = new ProxyBusObject(*m_BusAttachment, busName, ONBOARDING_OBJECT_PATH, sessionId);
    if (!proxyBusObj) {
        return ER_FAIL;
    }
    status = proxyBusObj->AddInterface(*ifc);
    if (status != ER_OK) {
        delete proxyBusObj;
        return status;
    }

    Message replyMsg(*m_BusAttachment);
    status = proxyBusObj->MethodCall(ONBOARDING_INTERFACE_NAME, "GetScanInfo", NULL, 0, replyMsg);
    if (status != ER_OK) {
        delete proxyBusObj;
        proxyBusObj = NULL;
        if (status == ER_BUS_REPLY_IS_ERROR_MESSAGE) {
            qcc::String errorMessage;
            const char* errorName = replyMsg->GetErrorName(&errorMessage);
            if (logger) {
                logger->warn(TAG, "GetScanInfo errorName: " + qcc::String(errorName) + " errorMessage: " + errorMessage);
            }
        }
        return status;
    }

    const ajn::MsgArg* returnArgs;
    size_t numArgs;
    replyMsg->GetArgs(numArgs, returnArgs);
    if (numArgs == 2) {
        status = returnArgs[0].Get("q", &age);
        MsgArg* scanInfoEntries;
        size_t scanInfoNum;
        status = returnArgs[1].Get("a(sn)", &scanInfoNum, &scanInfoEntries);
        if (status == ER_OK && scanInfoNum > 0) {
            for (unsigned int i = 0; i < scanInfoNum; i++) {
                char* SSID;
                short authType;
                status = scanInfoEntries[i].Get("(sn)", &SSID, &authType);
                if (status != ER_OK) {
                    break;
                }
                OBScanInfo info;
                info.authType = (OBAuthType) authType;
                info.SSID.assign(SSID);
                scanInfos.push_back(info);
            }            //end of for
        }
    } else {
        status = ER_BAD_ARG_COUNT;
    }

    delete proxyBusObj;
    return status;
}

QStatus OnboardingClient::ConfigureWiFi(const char* busName, const OBInfo& oBInfo, short& resultStatus,
                                        ajn::SessionId sessionId)
{
    if (logger) {
        logger->debug(TAG, "In OnboardingClient ConfigureWiFi");
    }

    QStatus status = ER_OK;

    const InterfaceDescription* ifc = m_BusAttachment->GetInterface(ONBOARDING_INTERFACE_NAME);
    if (!ifc) {
        return ER_FAIL;
    }

    ProxyBusObject*proxyBusObj = new ProxyBusObject(*m_BusAttachment, busName, ONBOARDING_OBJECT_PATH, sessionId);
    if (!proxyBusObj) {
        return ER_FAIL;
    }
    status = proxyBusObj->AddInterface(*ifc);
    if (status != ER_OK) {
        delete proxyBusObj;
        return status;
    }

    MsgArg args[3];
    Message replyMsg(*m_BusAttachment);
    do {
        CHECK_BREAK(args[0].Set("s", oBInfo.SSID.c_str()));
        CHECK_BREAK(args[1].Set("s", oBInfo.passcode.c_str()));
        CHECK_BREAK(args[2].Set("n", oBInfo.authType));

        CHECK_BREAK(proxyBusObj->MethodCall(ONBOARDING_INTERFACE_NAME, "ConfigureWiFi", args, 3, replyMsg));
    } while (0);
    if (status != ER_OK) {
        delete proxyBusObj;
        proxyBusObj = NULL;
        if (status == ER_BUS_REPLY_IS_ERROR_MESSAGE) {
            qcc::String errorMessage;
            const char* errorName = replyMsg->GetErrorName(&errorMessage);
            if (logger) {
                logger->warn(TAG, "ConfigureWiFi errorName: " + qcc::String(errorName) + " errorMessage: " + errorMessage);
            }

        }
        return status;
    }
    const ajn::MsgArg* returnArgs;
    size_t numArgs;
    replyMsg->GetArgs(numArgs, returnArgs);
    if (numArgs == 1) {
        status = returnArgs[0].Get("n", &resultStatus);
    } else {
        status = ER_BAD_ARG_COUNT;
    }

    delete proxyBusObj;
    return status;

}

QStatus OnboardingClient::ConnectTo(const char* busName, ajn::SessionId sessionId)
{
    if (logger) {
        logger->debug(TAG, "In OnboardingClient ConnectTo");
    }

    QStatus status = ER_OK;

    const InterfaceDescription* ifc = m_BusAttachment->GetInterface(ONBOARDING_INTERFACE_NAME);
    if (!ifc) {
        return ER_FAIL;
    }

    ProxyBusObject*proxyBusObj = new ProxyBusObject(*m_BusAttachment, busName, ONBOARDING_OBJECT_PATH, sessionId);
    if (!proxyBusObj) {
        return ER_FAIL;
    }
    status = proxyBusObj->AddInterface(*ifc);
    if (status != ER_OK) {
        delete proxyBusObj;
        return status;
    }
    status = proxyBusObj->MethodCall(ONBOARDING_INTERFACE_NAME, "Connect", NULL, 0);
    delete proxyBusObj;
    return status;
}

QStatus OnboardingClient::OffboardFrom(const char* busName, ajn::SessionId sessionId)
{
    if (logger) {
        logger->debug(TAG, "In OnboardingClient OffboardFrom");
    }
    QStatus status = ER_OK;

    const InterfaceDescription* ifc = m_BusAttachment->GetInterface(ONBOARDING_INTERFACE_NAME);
    if (!ifc) {
        return ER_FAIL;
    }

    ProxyBusObject*proxyBusObj = new ProxyBusObject(*m_BusAttachment, busName, ONBOARDING_OBJECT_PATH, sessionId);
    if (!proxyBusObj) {
        return ER_FAIL;
    }
    status = proxyBusObj->AddInterface(*ifc);
    if (status != ER_OK) {
        delete proxyBusObj;
        return status;
    }
    status = proxyBusObj->MethodCall(ONBOARDING_INTERFACE_NAME, "Offboard", NULL, 0);
    delete proxyBusObj;
    return status;

}

QStatus OnboardingClient::GetVersion(const char* busName, int& version, ajn::SessionId sessionId)
{
    if (logger) {
        logger->debug(TAG, "In OnboardingClient GetVersion");
    }

    QStatus status = ER_OK;

    const InterfaceDescription* ifc = m_BusAttachment->GetInterface(ONBOARDING_INTERFACE_NAME);
    if (!ifc) {
        return ER_FAIL;
    }

    ProxyBusObject*proxyBusObj = new ProxyBusObject(*m_BusAttachment, busName, ONBOARDING_OBJECT_PATH, sessionId);
    if (!proxyBusObj) {
        return ER_FAIL;
    }
    MsgArg arg;
    if (ER_OK == proxyBusObj->IntrospectRemoteObject()) {
        status = proxyBusObj->GetProperty(ONBOARDING_INTERFACE_NAME, "Version", arg);
        if (ER_OK == status) {
            version = arg.v_variant.val->v_uint16;
        }
    }
    delete proxyBusObj;
    proxyBusObj = NULL;
    return status;
}

QStatus OnboardingClient::GetState(const char* busName, short& state, ajn::SessionId sessionId)
{
    if (logger) {
        logger->debug(TAG, "In OnboardingClient GetState");
    }

    QStatus status = ER_OK;

    const InterfaceDescription* ifc = m_BusAttachment->GetInterface(ONBOARDING_INTERFACE_NAME);
    if (!ifc) {
        return ER_FAIL;
    }

    ProxyBusObject*proxyBusObj = new ProxyBusObject(*m_BusAttachment, busName, ONBOARDING_OBJECT_PATH, sessionId);
    if (!proxyBusObj) {
        return ER_FAIL;
    }
    MsgArg arg;
    if (ER_OK == proxyBusObj->IntrospectRemoteObject()) {
        status = proxyBusObj->GetProperty(ONBOARDING_INTERFACE_NAME, "State", arg);
        if (ER_OK == status) {
            state = arg.v_variant.val->v_int16;
        }
    }
    delete proxyBusObj;
    proxyBusObj = NULL;
    return status;
}

QStatus OnboardingClient::GetLastError(const char* busName, OBLastError& lastError, ajn::SessionId sessionId)
{
    if (logger) {
        logger->debug(TAG, "In OnboardingClient GetLastError");
    }

    QStatus status = ER_OK;

    const InterfaceDescription* ifc = m_BusAttachment->GetInterface(ONBOARDING_INTERFACE_NAME);
    if (!ifc) {
        return ER_FAIL;
    }

    ProxyBusObject*proxyBusObj = new ProxyBusObject(*m_BusAttachment, busName, ONBOARDING_OBJECT_PATH, sessionId);
    if (!proxyBusObj) {
        return ER_FAIL;
    }
    MsgArg arg;
    if (ER_OK == proxyBusObj->IntrospectRemoteObject()) {
        status = proxyBusObj->GetProperty(ONBOARDING_INTERFACE_NAME, "LastError", arg);
        if (ER_OK == status) {
            char* tempMesage;
            short errorCode;
            status = arg.Get("(ns)", &errorCode, &tempMesage);
            if (status == ER_OK) {
                lastError.message.assign(tempMesage);
                lastError.validationState = errorCode;
            }
        }
    }
    delete proxyBusObj;
    proxyBusObj = NULL;
    return status;

}

GenericLogger* OnboardingClient::setLogger(GenericLogger* newLogger)
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

GenericLogger* OnboardingClient::getLogger()
{
    return logger;
}

Log::LogLevel OnboardingClient::setLogLevel(Log::LogLevel newLogLevel)
{
    return logger ? logger->setLogLevel(newLogLevel) : Log::LogLevel::LEVEL_INFO;
}

Log::LogLevel OnboardingClient::getLogLevel()
{
    return logger ? logger->getLogLevel() : Log::LogLevel::LEVEL_INFO;
}

void OnboardingClient::GenericLoggerCallBack(DbgMsgType type, const char* module, const char* msg, void* context)
{
    OnboardingClient* onboardingClient = (OnboardingClient*)context;
    GenericLogger* logger = onboardingClient->getLogger();
    if (logger) {
        Log::LogLevel currLogLevel = logger->getLogLevel();
        switch (type) {
        case DBG_LOCAL_ERROR:
        case DBG_REMOTE_ERROR:
            if (currLogLevel >= Log::LEVEL_ERROR) {
                logger->error(CALLBACKTAG, msg);
            }
            break;

        case DBG_GEN_MESSAGE:
            if (currLogLevel >= Log::LEVEL_INFO) {
                logger->info(CALLBACKTAG, msg);
            }
            break;

        case DBG_API_TRACE:
            if (currLogLevel >= Log::LEVEL_DEBUG) {
                logger->debug(CALLBACKTAG, msg);
            }
            break;

        case DBG_HIGH_LEVEL:
            if (currLogLevel >= Log::LEVEL_WARN) {
                logger->warn(CALLBACKTAG, msg);
            }
            break;

        case DBG_REMOTE_DATA:
        case DBG_LOCAL_DATA:
            if (currLogLevel >= Log::LEVEL_DEBUG) {
                logger->debug(CALLBACKTAG, msg);
            }
            break;

        default:
            if (currLogLevel >= Log::LEVEL_DEBUG) {
                logger->debug(CALLBACKTAG, msg);
            }
        }
    }
}
