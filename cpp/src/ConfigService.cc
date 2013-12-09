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

#include <alljoyn/config/ConfigService.h>
#include <alljoyn/BusAttachment.h>
#include <qcc/Debug.h>

#define TAG "ALLJOYN_CONFIG_SERVICE"
#define CALLBACKTAG "AllJoynInternal"
#define CHECK_RETURN(x) if ((status = x) != ER_OK) return status;
#define CHECK_BREAK(x) if ((status = x) != ER_OK) break;

using namespace ajn;
using namespace services;

static const char* CONFIG_INTERFACE_NAME = "org.alljoyn.Config";

ConfigService::ConfigService(ajn::BusAttachment& bus, PropertyStore& store, Listener& listener) :
    BusObject("/Config"), m_BusAttachment(&bus), m_PropertyStore(&store), m_Listener(&listener), logger(0)
{
    setLogger(&configLogger);
    if (logger)
        logger->debug(TAG, "In ConfigService Constructor");
}

ConfigService::~ConfigService()
{

}

QStatus ConfigService::Register()
{
    QStatus status = ER_OK;
    if (logger)
        logger->debug(TAG, "In ConfigService Register");

    InterfaceDescription* intf = const_cast<InterfaceDescription*>(m_BusAttachment->GetInterface(CONFIG_INTERFACE_NAME));
    if (!intf) {
        CHECK_RETURN(m_BusAttachment->CreateInterface(CONFIG_INTERFACE_NAME, intf,
                                                      AJ_IFC_SECURITY_REQUIRED))
        if (!intf)
            return ER_BUS_CANNOT_ADD_INTERFACE;

        CHECK_RETURN(intf->AddMethod("FactoryReset", NULL, NULL, NULL))
        CHECK_RETURN(intf->AddMemberAnnotation("FactoryReset", org::freedesktop::DBus::AnnotateNoReply, "true"))
        CHECK_RETURN(intf->AddMethod("Restart", NULL, NULL, NULL))
        CHECK_RETURN(intf->AddMemberAnnotation("Restart", org::freedesktop::DBus::AnnotateNoReply, "true"))
        CHECK_RETURN(intf->AddMethod("SetPasscode", "say", NULL, "DaemonRealm,newPasscode"))
        CHECK_RETURN(intf->AddMethod("GetConfigurations", "s", "a{sv}", "languageTag,languages"))
        CHECK_RETURN(intf->AddMethod("UpdateConfigurations", "sa{sv}", NULL, "languageTag,configMap"))
        CHECK_RETURN(intf->AddMethod("ResetConfigurations", "sas", NULL, "languageTag,fieldList"))
        CHECK_RETURN(intf->AddProperty("Version", "q", (uint8_t) PROP_ACCESS_READ))
        intf->Activate();
    } //if (!intf)

    CHECK_RETURN(AddInterface(*intf))
    CHECK_RETURN(AddMethodHandler(intf->GetMember("FactoryReset"),
                                  static_cast<MessageReceiver::MethodHandler>(&ConfigService::FactoryResetHandler)))
    CHECK_RETURN(AddMethodHandler(intf->GetMember("Restart"),
                                  static_cast<MessageReceiver::MethodHandler>(&ConfigService::RestartHandler)))
    CHECK_RETURN(AddMethodHandler(intf->GetMember("SetPasscode"),
                                  static_cast<MessageReceiver::MethodHandler>(&ConfigService::SetPasscodeHandler)))
    CHECK_RETURN(AddMethodHandler(intf->GetMember("GetConfigurations"),
                                  static_cast<MessageReceiver::MethodHandler>(&ConfigService::GetConfigurationsHandler)))
    CHECK_RETURN(AddMethodHandler(intf->GetMember("UpdateConfigurations"),
                                  static_cast<MessageReceiver::MethodHandler>(&ConfigService::UpdateConfigurationsHandler)))
    CHECK_RETURN(AddMethodHandler(intf->GetMember("ResetConfigurations"),
                                  static_cast<MessageReceiver::MethodHandler>(&ConfigService::ResetConfigurationsHandler)))
    return status;
}

void ConfigService::SetPasscodeHandler(const InterfaceDescription::Member* member, Message& msg)
{
    if (logger)
        logger->debug(TAG, "In ConfigService SetPassCodeHandler");

    const ajn::MsgArg* args = 0;
    size_t numArgs = 0;
    QStatus status;
    msg->GetArgs(numArgs, args);
    do {
        if (numArgs != 2)
            break;

        char* newPasscode;
        size_t newPasscodeNumElements;
        CHECK_BREAK(args[1].Get("ay", &newPasscodeNumElements, &newPasscode))

        if (args[0].typeId != ALLJOYN_STRING)
            break;

        if (newPasscodeNumElements == 0) {
            if (logger)
                logger->warn(TAG, "Password can not be empty");
            MethodReply(msg, ER_INVALID_DATA);
            return;
        }

        m_Listener->SetPassphrase(args[0].v_string.str, newPasscodeNumElements, newPasscode);
        MethodReply(msg, args, 0);
        return;
    } while (0);
    MethodReply(msg, ER_INVALID_DATA);
}

void ConfigService::GetConfigurationsHandler(const InterfaceDescription::Member* member, Message& msg)
{
    if (logger)
        logger->debug(TAG, "In ConfigService GetConfigurationsHandler");

    const ajn::MsgArg* args;
    size_t numArgs;
    QStatus status = ER_OK;
    msg->GetArgs(numArgs, args);
    do {
        if (numArgs != 1)
            break;

        if (args[0].typeId != ALLJOYN_STRING)
            break;

        ajn::MsgArg writeData[1];
        CHECK_BREAK(m_PropertyStore->ReadAll(args[0].v_string.str, PropertyStore::WRITE, writeData[0]))

        if (logger) {
            logger->debug(TAG, ("ReadAll called with PropertyStore::WRITE for language: " + qcc::String(args[0].v_string.str)
                                + " status= " + QCC_StatusText(status)));
            logger->debug(TAG, ("data = " + writeData[0].ToString()));
        }

        MethodReply(msg, writeData, 1);
        return;
    } while (0);

    if (status == ER_LANGUAGE_NOT_SUPPORTED)
        MethodReply(msg, "org.alljoyn.Error.LanguageNotSupported", "The language specified is not supported");
    else if (status != ER_OK)
        MethodReply(msg, status);
    else
        MethodReply(msg, ER_INVALID_DATA);
}

void ConfigService::UpdateConfigurationsHandler(const InterfaceDescription::Member* member, Message& msg)
{
    if (logger)
        logger->debug(TAG, "In ConfigService UpdateConfigurationsHandler");

    const ajn::MsgArg* args;
    size_t numArgs;
    QStatus status = ER_OK;
    msg->GetArgs(numArgs, args);
    do {
        if (numArgs != 2)
            break;

        const MsgArg* configMapDictEntries;
        size_t configMapNumElements;
        char* languageTag;
        CHECK_BREAK(args[0].Get("s", &languageTag))
        CHECK_BREAK(args[1].Get("a{sv}", &configMapNumElements, &configMapDictEntries))
        if (languageTag[0] == 0) {
            languageTag = NULL;
        }
        for (unsigned int i = 0; i < configMapNumElements; i++) {
            char* tempKey;
            MsgArg* tempValue;
            CHECK_BREAK(configMapDictEntries[i].Get("{sv}", &tempKey, &tempValue))
            CHECK_BREAK(m_PropertyStore->Update(tempKey, languageTag, tempValue))
            if (logger) {
                qcc::String logline = "Calling update for " + qcc::String(tempKey) + " with lang: " + languageTag + " Value: " +
                                      tempValue->ToString().c_str() + " status: " + QCC_StatusText(status);
                logger->debug(TAG, logline);
            }
        }
        MsgArg args[0];
        MethodReply(msg, args, 0);
        return;

    } while (0);

    if (logger)
        logger->warn(TAG, "UpdateConfigurationsHandler Failed");

    if (status == ER_MAX_SIZE_EXCEEDED) {
        MethodReply(msg, "org.alljoyn.Error.MaxSizeExceeded", "Maximum size exceeded");
    } else if (status == ER_INVALID_VALUE) {
        MethodReply(msg, "org.alljoyn.Error.InvalidValue", "Invalid value");
    } else if (status == ER_FEATURE_NOT_AVAILABLE) {
        MethodReply(msg, "org.alljoyn.Error.FeatureNotAvailable", "Feature not available");
        return;
    } else if (status == ER_LANGUAGE_NOT_SUPPORTED) {
        MethodReply(msg, "org.alljoyn.Error.LanguageNotSupported",
                    "The language specified is not supported");
    } else if (status != ER_OK) {
        MethodReply(msg, status);
    } else {
        //default error such as when num params does not match
        MethodReply(msg, ER_INVALID_DATA);
    }
}

void ConfigService::ResetConfigurationsHandler(const InterfaceDescription::Member* member, Message& msg)
{
    if (logger)
        logger->debug(TAG, "In ConfigService ResetConfigurationsHandler");

    const ajn::MsgArg* args;
    size_t numArgs;
    QStatus status = ER_OK;
    msg->GetArgs(numArgs, args);
    do {
        if (numArgs != 2)
            break;

        char* languageTag;
        CHECK_BREAK(args[0].Get("s", &languageTag))

        if (languageTag[0] == 0) {
            languageTag = NULL;
        }
        const MsgArg* stringArray;
        size_t fieldListNumElements;
        CHECK_BREAK(args[1].Get("as", &fieldListNumElements, &stringArray))
        for (unsigned int i = 0; i < fieldListNumElements; i++) {
            char* tempString;
            CHECK_BREAK(stringArray[i].Get("s", &tempString))
            CHECK_BREAK(m_PropertyStore->Delete(tempString, languageTag))
            if (logger) {
                qcc::String logline = "Calling delete for " + qcc::String(tempString) + " with lang: " + languageTag +
                                      " status: " + QCC_StatusText(status);
                logger->debug(TAG, logline);
            }
        }
        CHECK_BREAK(status)

        MsgArg args[0];
        MethodReply(msg, args, 0);
        return;
    } while (0);

    if (logger)
        logger->warn(TAG, "ResetConfigurationsHandler Failed");

    if (status == ER_MAX_SIZE_EXCEEDED) {
        MethodReply(msg, "org.alljoyn.Error.MaxSizeExceeded", "Maximum size exceeded");
    } else if (status == ER_INVALID_VALUE) {
        MethodReply(msg, "org.alljoyn.Error.InvalidValue", "Invalid value");
    } else if (status == ER_FEATURE_NOT_AVAILABLE) {
        MethodReply(msg, "org.alljoyn.Error.FeatureNotAvailable", "Feature not available");
    } else if (status == ER_LANGUAGE_NOT_SUPPORTED) {
        MethodReply(msg, "org.alljoyn.Error.LanguageNotSupported",
                    "The language specified is not supported");
    } else if (status != ER_OK) {
        MethodReply(msg, status);
    } else {
        //in case number of arguments is different from the one expected
        MethodReply(msg, ER_INVALID_DATA);
    }
}

void ConfigService::FactoryResetHandler(const InterfaceDescription::Member* member, Message& msg)
{
    if (logger)
        logger->debug(TAG, "In ConfigService FactoryResetHandler");

    const ajn::MsgArg* args;
    size_t numArgs;
    msg->GetArgs(numArgs, args);
    if (numArgs == 0) {
        m_Listener->FactoryReset();
        //check it the ALLJOYN_FLAG_NO_REPLY_EXPECTED exists if so send response
        if (!(msg->GetFlags() & ALLJOYN_FLAG_NO_REPLY_EXPECTED)) {
            MsgArg args[0];
            MethodReply(msg, args, 0);
        }
    } else {
        //check it the ALLJOYN_FLAG_NO_REPLY_EXPECTED exists if so send response ER_INVALID_DATA
        if (!(msg->GetFlags() & ALLJOYN_FLAG_NO_REPLY_EXPECTED)) {
            MethodReply(msg, ER_INVALID_DATA);
        }
    }
}

void ConfigService::RestartHandler(const InterfaceDescription::Member* member, Message& msg)
{
    if (logger)
        logger->debug(TAG, "In ConfigService RestartHandler");

    const ajn::MsgArg* args;
    size_t numArgs;
    msg->GetArgs(numArgs, args);
    if (numArgs == 0) {
        m_Listener->Restart();
        //check it the ALLJOYN_FLAG_NO_REPLY_EXPECTED exists if so send response
        if (!(msg->GetFlags() & ALLJOYN_FLAG_NO_REPLY_EXPECTED)) {
            MsgArg args[0];
            MethodReply(msg, args, 0);
        }
    } else {
        //check it the ALLJOYN_FLAG_NO_REPLY_EXPECTED exists if so send response ER_INVALID_DATA
        if (!(msg->GetFlags() & ALLJOYN_FLAG_NO_REPLY_EXPECTED)) {
            MethodReply(msg, ER_INVALID_DATA);
        }
    }
}

QStatus ConfigService::Get(const char*ifcName, const char*propName, MsgArg& val)
{
    QStatus status = ER_BUS_NO_SUCH_PROPERTY;
    // Check the requested property and return the value if it exists
    if (0 == strcmp(ifcName, CONFIG_INTERFACE_NAME)) {
        if (0 == strcmp("Version", propName)) {
            status = val.Set("q", 1);
        }
    }
    return status;
}

GenericLogger* ConfigService::setLogger(GenericLogger* newLogger)
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

GenericLogger* ConfigService::getLogger()
{
    return logger;
}

Log::LogLevel ConfigService::setLogLevel(Log::LogLevel newLogLevel)
{
    return logger ? logger->setLogLevel(newLogLevel) : Log::LogLevel::LEVEL_INFO;
}

Log::LogLevel ConfigService::getLogLevel()
{
    return logger ? logger->getLogLevel() : Log::LogLevel::LEVEL_INFO;
}

void ConfigService::GenericLoggerCallBack(DbgMsgType type, const char* module, const char* msg, void* context)
{
    ConfigService* configService = (ConfigService*)context;
    GenericLogger* logger = configService->getLogger();
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
