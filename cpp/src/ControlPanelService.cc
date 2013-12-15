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

#include <qcc/Debug.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include "ControlPanelConstants.h"

namespace ajn {
namespace services {

using namespace qcc;
using namespace cpsConsts;

#define TAG TAG_CONTROLPANELSERVICE
#define CALLBACKTAG "AllJoynInternal"

ControlPanelService* ControlPanelService::s_Instance(0);
uint16_t const ControlPanelService::CONTROLPANEL_SERVICE_VERSION = 1;

ControlPanelService* ControlPanelService::getInstance()
{
    if (!s_Instance)
        s_Instance = new ControlPanelService();

    return s_Instance;
}

ControlPanelService::ControlPanelService() :
    m_Bus(0), m_BusListener(0), m_ControlPanelControllee(0),
    m_ControlPanelController(0), m_ControlPanelListener(0), logger(0)
{
    setLogger(&cpsLogger);
}

ControlPanelService::~ControlPanelService()
{
    if (logger) logger->info(TAG, "Shutting down");

    if (m_BusListener) {
        if (m_Bus)
            m_Bus->UnregisterBusListener(*m_BusListener);
        delete m_BusListener;
        m_BusListener = 0;
    }

    if (this == s_Instance)
        s_Instance = 0;
}

uint16_t ControlPanelService::getVersion()
{
    return CONTROLPANEL_SERVICE_VERSION;
}

QStatus ControlPanelService::initControllee(BusAttachment* bus, ControlPanelControllee* controlPanelControllee)
{
    if (logger)
        logger->debug(TAG, "Initializing Controllee");

    if (!bus) {
        if (logger)
            logger->warn(TAG, "Bus cannot be NULL");
        return ER_BAD_ARG_1;
    }

    if (!bus->IsStarted()) {
        if (logger)
            logger->warn(TAG, "Bus is not started");
        return ER_BAD_ARG_1;
    }

    if (!bus->IsConnected()) {
        if (logger)
            logger->warn(TAG, "Bus is not connected");
        return ER_BAD_ARG_1;
    }

    if (m_Bus && m_Bus->GetUniqueName().compare(bus->GetUniqueName()) != 0) {
        if (logger)
            logger->warn(TAG, "Bus is already set to different BusAttachment");
        return ER_BAD_ARG_1;
    }

    if (!controlPanelControllee) {
        if (logger)
            logger->warn(TAG, "ControlPanelControllee cannot be null");
        return ER_BAD_ARG_2;
    }

    if (m_ControlPanelControllee) {
        if (logger)
            logger->warn(TAG, "ControlPanelControllee already initialized");
        return ER_BUS_OBJ_ALREADY_EXISTS;
    }

    if (m_BusListener) {
        if (logger)
            logger->warn(TAG, "BusListener already initialized");
        return ER_BUS_OBJ_ALREADY_EXISTS;
    }

    m_Bus = bus;
    m_ControlPanelControllee = controlPanelControllee;

    QStatus status = controlPanelControllee->registerObjects(bus);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not register the BusObjects");
        return status;
    }

    m_BusListener = new ControlPanelBusListener();
    m_BusListener->setSessionPort(CONTROLPANELSERVICE_PORT);
    m_Bus->RegisterBusListener(*m_BusListener);

    SessionPort servicePort = CONTROLPANELSERVICE_PORT;
    SessionOpts sessionOpts(SessionOpts::TRAFFIC_MESSAGES, true, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);

    status = m_Bus->BindSessionPort(servicePort, sessionOpts, *m_BusListener);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not bind Session Port successfully");
        return status;
    }
    logger->info(TAG, "Initialized Controllee successfully");
    return status;
}

QStatus ControlPanelService::shutdownControllee()
{
    if (!m_ControlPanelControllee) {
        if (logger)
            logger->info(TAG, "ControlPanelControllee not initialized. Returning");
        return ER_OK;
    }

    if (!m_Bus) {
        if (logger)
            logger->info(TAG, "Bus not set.");
        return ER_BUS_BUS_NOT_STARTED;
    }

    if (m_BusListener) {
        m_Bus->UnregisterBusListener(*m_BusListener);
        delete m_BusListener;
        m_BusListener = 0;
    }

    TransportMask transportMask = TRANSPORT_ANY;
    SessionPort sp = CONTROLPANELSERVICE_PORT;
    SessionOpts opts(SessionOpts::TRAFFIC_MESSAGES, false, SessionOpts::PROXIMITY_ANY, transportMask);

    QStatus returnStatus = ER_OK;
    QStatus status = m_Bus->UnbindSessionPort(sp);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not unbind the SessionPort");
        returnStatus = status;
    }

    status = m_ControlPanelControllee->unregisterObjects(m_Bus);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not unregister the BusObjects");
        returnStatus = status;
    }

    m_ControlPanelControllee = 0;
    return returnStatus;
}

QStatus ControlPanelService::initController(BusAttachment* bus, ControlPanelController* controlPanelController,
                                            ControlPanelListener* controlPanelListener)
{
    if (logger)
        logger->debug(TAG, "Initializing Controller");

    if (!bus) {
        if (logger)
            logger->warn(TAG, "Bus cannot be NULL");
        return ER_BAD_ARG_1;
    }

    if (!bus->IsStarted()) {
        if (logger)
            logger->warn(TAG, "Bus is not started");
        return ER_BAD_ARG_1;
    }

    if (!bus->IsConnected()) {
        if (logger)
            logger->warn(TAG, "Bus is not connected");
        return ER_BAD_ARG_1;
    }

    if (m_Bus && m_Bus->GetUniqueName().compare(bus->GetUniqueName()) != 0) {
        if (logger)
            logger->warn(TAG, "Bus is already set to different BusAttachment");
        return ER_BAD_ARG_1;
    }

    if (!controlPanelController) {
        if (logger)
            logger->warn(TAG, "ControlPanelController cannot be null");
        return ER_BAD_ARG_2;
    }

    if (m_ControlPanelController) {
        if (logger)
            logger->warn(TAG, "ControlPanelController already initialized");
        return ER_BUS_OBJ_ALREADY_EXISTS;
    }

    if (!controlPanelListener) {
        if (logger)
            logger->warn(TAG, "ControlPanelListener cannot be null");
        return ER_BAD_ARG_3;
    }

    if (m_ControlPanelListener) {
        if (logger)
            logger->warn(TAG, "m_ControlPanelListener already initialized");
        return ER_BUS_OBJ_ALREADY_EXISTS;
    }

    m_Bus = bus;
    m_ControlPanelController = controlPanelController;
    m_ControlPanelListener = controlPanelListener;

    logger->info(TAG, "Initialized Controller successfully");
    return ER_OK;
}

QStatus ControlPanelService::shutdownController()
{
    if (!m_ControlPanelController) {
        if (logger)
            logger->info(TAG, "ControlPanelControllee not initialized. Returning");
        return ER_OK;
    }

    if (!m_Bus) {
        if (logger)
            logger->info(TAG, "Bus not set.");
        return ER_BUS_BUS_NOT_STARTED;
    }

    QStatus status = m_ControlPanelController->deleteAllControllableDevices();
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not stop all Controllable Devices");
    }

    m_ControlPanelController = 0;
    m_ControlPanelListener = 0;

    return status;
}

void ControlPanelService::GenericLoggerCallBack(DbgMsgType type, const char* module, const char* msg, void* context)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
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

GenericLogger* ControlPanelService::setLogger(GenericLogger* newLogger)
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

GenericLogger* ControlPanelService::getLogger()
{
    return logger;
}

Log::LogLevel ControlPanelService::setLogLevel(Log::LogLevel newLogLevel)
{
    return logger ? logger->setLogLevel(newLogLevel) : Log::LogLevel::LEVEL_INFO;
}

Log::LogLevel ControlPanelService::getLogLevel()
{
    return logger ? logger->getLogLevel() : Log::LogLevel::LEVEL_INFO;
}

BusAttachment* ControlPanelService::getBusAttachment()
{
    return m_Bus;
}

ControlPanelBusListener* ControlPanelService::getBusListener() const
{
    return m_BusListener;
}

ControlPanelListener* ControlPanelService::getControlPanelListener() const
{
    return m_ControlPanelListener;
}

std::vector<qcc::String> ControlPanelService::SplitObjectPath(qcc::String const& objectPath)
{
    std::vector<qcc::String> results;

    size_t prev = 0;
    size_t next = 0;

    while ((next = objectPath.find_first_of("/", prev)) != qcc::String::npos) {
        if (next - prev != 0) {
            results.push_back(objectPath.substr(prev, next - prev));
        }
        prev = next + 1;
    }

    if (prev < objectPath.size()) {
        results.push_back(objectPath.substr(prev));
    }

    return results;
}

} /* namespace services */
} /* namespace ajn */
