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

#include <algorithm>
#include <sstream>

#include <alljoyn/notification/NotificationSender.h>
#include <alljoyn/notification/NotificationService.h>
#include <alljoyn/services_common/ServicesLoggerImpl.h>

#include "NotificationConstants.h"
#include "Transport.h"

using namespace ajn;
using namespace services;
using namespace qcc;
using namespace nsConsts;

#define CALLBACKTAG "AllJoynInternal"

NotificationService* NotificationService::s_Instance(0);
uint16_t const NotificationService::NOTIFICATION_SERVICE_VERSION = 1;

NotificationService* NotificationService::getInstance()
{
    if (!s_Instance) {
        s_Instance = new NotificationService();
    }
    return s_Instance;
}

NotificationService::NotificationService() :
    logger(0), TAG(TAG_NOTSERVICE)
{
    ServicesLoggerImpl* nsLogger = new ServicesLoggerImpl();
    setLogger(nsLogger);
}

NotificationService::~NotificationService()
{

}

uint16_t NotificationService::getVersion()
{
    return NOTIFICATION_SERVICE_VERSION;
}

NotificationSender* NotificationService::initSend(BusAttachment* bus, PropertyStore* store)
{
    if (!bus) {
        if (logger) logger->warn(TAG, "BusAttachment cannot be NULL");
        return NULL;
    }

    if (!store) {
        if (logger) logger->warn(TAG, "PropertyStore cannot be NULL");
        return NULL;
    }

    Transport* transport = Transport::getInstance();
    if (transport->startSenderTransport(bus) != ER_OK)
        return NULL;

    return new NotificationSender(store);
}

QStatus NotificationService::initReceive(ajn::BusAttachment* bus, NotificationReceiver* notificationReceiver)
{
    if (!bus) {
        if (logger) logger->warn(TAG, "BusAttachment cannot be NULL");
        return ER_BAD_ARG_1;
    }

    if (!notificationReceiver) {
        if (logger) logger->warn(TAG, "Could not set NotificationReceiver to null pointer");
        return ER_BAD_ARG_2;
    }

    if (logger) logger->debug(TAG, "Init receive");

    Transport* transport = Transport::getInstance();
    transport->setNotificationReceiver(notificationReceiver);

    QStatus status;
    if ((status = transport->startReceiverTransport(bus)) != ER_OK) {
        transport->setNotificationReceiver(0);
    }

    return status;
}

void NotificationService::shutdownSender()
{
    if (logger) logger->debug(TAG, "Stop Sender");
    Transport* transport = Transport::getInstance();
    transport->cleanupSenderTransport();
}

void NotificationService::shutdownReceiver()
{
    if (logger) logger->debug(TAG, "Stop Receiver");
    Transport* transport = Transport::getInstance();
    transport->cleanupReceiverTransport();
}

void NotificationService::shutdown()
{
    if (logger) logger->debug(TAG, "Shutdown");
    Transport* transport = Transport::getInstance();
    transport->cleanup();

    s_Instance = 0;
    delete this;
}

QStatus NotificationService::disableSuperAgent()
{
    if (logger) logger->debug(TAG, "Disabling SuperAgent");
    Transport* transport = Transport::getInstance();
    return transport->disableSuperAgent();
}


BusAttachment* NotificationService::getBusAttachment()
{
    if (logger) logger->debug(TAG, "In Get BusAttachment");
    Transport* transport = Transport::getInstance();
    return transport->getBusAttachment();
}

GenericLogger* NotificationService::setLogger(GenericLogger* newLogger)
{
    GenericLogger* prevLogger = getLogger();
    Log::LogLevel prevLogLevel = getLogLevel();
    logger = newLogger;
    if (logger) {
        setLogLevel(prevLogLevel);
        // reroute internal AJ logging to logger
        ServicesLoggerImpl::RegisterCallBack(logger, &GenericLoggerCallBack, NULL);
    }
    return prevLogger;
}

GenericLogger* NotificationService::getLogger()
{
    return logger;
}

Log::LogLevel NotificationService::setLogLevel(Log::LogLevel newLogLevel)
{
    return logger ? logger->setLogLevel(newLogLevel) : ServicesLoggerImpl::getDefaultLogLevel();
}

Log::LogLevel NotificationService::getLogLevel()
{
    return logger ? logger->getLogLevel() : ServicesLoggerImpl::getDefaultLogLevel();
}

void NotificationService::GenericLoggerCallBack(DbgMsgType type, const char* module, const char* msg, void* context)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
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
