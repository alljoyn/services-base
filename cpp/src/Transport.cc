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

#include <alljoyn/about/AboutServiceApi.h>
#include <alljoyn/about/AnnouncementRegistrar.h>
#include <alljoyn/notification/NotificationService.h>

#include "NotificationConstants.h"
#include "PayloadAdapter.h"
#include "Transport.h"

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

Transport* Transport::s_Instance(0);

Transport::Transport() : m_Bus(0), m_Receiver(0), m_Consumer(0), m_SuperAgent(0), m_AnnounceListener(0),
    m_IsSendingDisabled(false), m_IsReceivingDisabled(false), m_IsSuperAgentDisabled(false),
    m_IsListeningToSuperAgent(false), TAG(TAG_TRANSPORT)
{
    for (int32_t indx = 0; indx < MESSAGE_TYPE_CNT; indx++)
        m_Producers[indx] = 0;
}

Transport::~Transport()
{
    if (m_Bus)
        cleanup();
}

Transport* Transport::getInstance()
{
    if (!s_Instance)
        s_Instance = new Transport();
    return s_Instance;
}

QStatus Transport::setBusAttachment(ajn::BusAttachment* bus)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (m_Bus && bus->GetUniqueName().compare(m_Bus->GetUniqueName()) != 0) {
        if (logger)
            logger->warn(TAG, "Could not accept this BusAttachment, a different bus attachment already exists");
        return ER_BUS_LISTENER_ALREADY_SET;
    }

    if (!bus->IsStarted() || !bus->IsConnected()) {
        if (logger)
            logger->warn(TAG, "Could not acccept this BusAttachment, bus attachment not started or not connected");
        return ER_BAD_ARG_1;
    }

    m_Bus = bus;
    return ER_OK;
}

QStatus Transport::disableSuperAgent()
{
    if (m_Consumer || m_SuperAgent)
        return ER_BUS_ALREADY_CONNECTED;         // Receiver already started

    m_IsSuperAgentDisabled = true;
    return ER_OK;
}

void Transport::setNotificationReceiver(NotificationReceiver* notificationReceiver)
{
    m_Receiver = notificationReceiver;
}

void Transport::onReceivedNotification(Notification const& notification)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if (m_Receiver == 0) {
        if (logger)
            logger->warn(TAG, "Could not receive message, Receiver is not initialized");
        return;
    }

    if (m_IsReceivingDisabled) {
        if (logger)
            logger->info(TAG, "Could not receive message, Receiving is Disabled");
        return;
    }

    m_Bus->EnableConcurrentCallbacks();
    m_Receiver->receive(notification);
}

QStatus Transport::sendNotification(NotificationMessageType messageType,
                                    MsgArg const notificationArgs[AJ_NOTIFY_NUM_PARAMS], uint16_t ttl)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if (m_Producers[messageType] == 0) {
        if (logger)
            logger->warn(TAG, "Could not send message, Sender is not initialized");
        return ER_BUS_OBJECT_NOT_REGISTERED;
    }

    if (m_IsSendingDisabled) {
        logger->info(TAG, "Could not send message, Sending is Disabled");
        return ER_BUS_NOT_ALLOWED;
    }

    return m_Producers[messageType]->sendSignal(notificationArgs, ttl);
}


QStatus Transport::deleteLastMsg(NotificationMessageType messageType)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if (m_Producers[messageType] == 0) {
        if (logger)
            logger->warn(TAG, "Could not delete message, Sender is not initialized");
        return ER_BUS_OBJECT_NOT_REGISTERED;
    }

    return m_Producers[messageType]->deleteLastMsg(messageType);
}

QStatus Transport::startSenderTransport(BusAttachment* bus, bool startSuperAgent)
{
    std::vector<String> interfaces;
    QStatus status;

    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if ((status = setBusAttachment(bus)) != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not start Sender Transport. BusAttachment is not valid");
        return status;
    }

    if (m_Producers[0]) {
        if (logger)
            logger->info(TAG, "Sender Transport already started. Ignoring request");
        return ER_OK;
    }

    AboutServiceApi* aboutService = AboutServiceApi::getInstance();
    if (!aboutService) {
        if (logger)
            logger->info(TAG, "AboutService is not defined");
        return ER_FAIL;
    }


    for (int32_t messageTypeIndx = 0; messageTypeIndx < MESSAGE_TYPE_CNT; messageTypeIndx++) {
        status = ER_OK;
        if (startSuperAgent) {
            m_Producers[messageTypeIndx] = new NotificationTransportProducer(m_Bus,
                                                                             AJ_PRODUCER_SERVICE_PATH_PREFIX + MessageTypeUtil::getMessageTypeString(messageTypeIndx),
                                                                             status, AJ_SA_INTERFACE_NAME);

            if (!interfaces.size())
                interfaces.push_back(AJ_SA_INTERFACE_NAME);

            aboutService->AddObjectDescription(AJ_PRODUCER_SERVICE_PATH_PREFIX +
                                               MessageTypeUtil::getMessageTypeString(messageTypeIndx), interfaces);
        } else {
            m_Producers[messageTypeIndx] = new NotificationTransportProducer(m_Bus,
                                                                             AJ_PRODUCER_SERVICE_PATH_PREFIX + MessageTypeUtil::getMessageTypeString(messageTypeIndx), status);

            if (!interfaces.size())
                interfaces.push_back(AJ_NOTIFICATION_INTERFACE_NAME);

            aboutService->AddObjectDescription(AJ_PRODUCER_SERVICE_PATH_PREFIX +
                                               MessageTypeUtil::getMessageTypeString(messageTypeIndx), interfaces);
        }

        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not create BusObject. Status: " + String(QCC_StatusText(status)));
            cleanupTransportProducer(messageTypeIndx);
            return status;
        }

        status = m_Bus->RegisterBusObject(*m_Producers[messageTypeIndx]);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register BusObject. Status: " + String(QCC_StatusText(status)));
            cleanupTransportProducer(messageTypeIndx);
            return status;
        }
    }

    if (logger)
        logger->info(TAG, "Started Sender successfully");



    return status;
}

QStatus Transport::listenForAnnouncements()
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    m_AnnounceListener = new NotificationAnnounceListener();

    QStatus status;
    status = AnnouncementRegistrar::RegisterAnnounceHandler(*m_Bus, *m_AnnounceListener);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not create AnnouncementListener. AnnounceHandlerApi not initialized");
        cleanupAnnouncementListener();
    }
    return status;
}

QStatus Transport::startReceiverTransport(BusAttachment* bus)
{
    QStatus status;
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if ((status = setBusAttachment(bus)) != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not start Receiver Transport. BusAttachment is not valid");
        return status;
    }

    if (m_Consumer || m_SuperAgent) {
        if (logger)
            logger->info(TAG, "Receiver Transport already started. Ignoring request");
        return ER_OK;
    }

    bool consumerRegisteredBus   = false;
    bool superagentRegisteredBus = false;

    do {
        status = ER_OK;

        m_Consumer = new NotificationTransportConsumer(m_Bus, AJ_CONSUMER_SERVICE_PATH, status);

        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not create Consumer BusObject. Status: " + String(QCC_StatusText(status)));
            break;
        }

        status = m_Bus->RegisterBusObject(*m_Consumer);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register Consumer BusObject. Status: " + String(QCC_StatusText(status)));
            break;
        }

        consumerRegisteredBus = true;

        String AJ_NOTIFICATION_INTERFACE_MATCH = "interface='" + AJ_NOTIFICATION_INTERFACE_NAME + "'";

        if (logger)
            logger->debug(TAG, "Match String is: " + AJ_NOTIFICATION_INTERFACE_MATCH);

        status = m_Bus->AddMatch(AJ_NOTIFICATION_INTERFACE_MATCH.c_str());

        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not add filter match for notification service. Status: " + String(QCC_StatusText(status)));
            break;
        }

        if (!m_IsSuperAgentDisabled) {
            m_SuperAgent = new NotificationTransportSuperAgent(m_Bus, AJ_CONSUMER_SERVICE_PATH, status);

            if (status != ER_OK) {
                if (logger)
                    logger->warn(TAG, "Could not create SuperAgent BusObject. Status: " + String(QCC_StatusText(status)));
                break;
            }

            status = m_Bus->RegisterBusObject(*m_SuperAgent);
            if (status != ER_OK) {
                if (logger)
                    logger->warn(TAG, "Could not register SuperAgent BusObject. Status: " + String(QCC_StatusText(status)));
                break;
            }
            superagentRegisteredBus = true;

            String AJ_SUPERAGENT_INTERFACE_MATCH = "interface='" + AJ_SA_INTERFACE_NAME + "'";

            if (logger)
                logger->debug(TAG, "SuperAgent Match String is: " + AJ_SUPERAGENT_INTERFACE_MATCH);

            status = m_Bus->AddMatch(AJ_SUPERAGENT_INTERFACE_MATCH.c_str());
            if (status != ER_OK) {
                if (logger)
                    logger->warn(TAG, "Could not add filter match. Status: " + String(QCC_StatusText(status)));
                break;
            }

            status = listenForAnnouncements();
            if (status != ER_OK) {
                if (logger)
                    logger->warn(TAG, "Could not listen for announcements.");
                break;
            }
        }

        if (logger)
            logger->info(TAG, "Started Receiver successfully");
        return ER_OK;
    } while (0);

    if (logger)
        logger->info(TAG, "Not listening to any producers");

    cleanupTransportConsumer(consumerRegisteredBus);
    if (!m_IsSuperAgentDisabled)
        cleanupTransportSuperAgent(superagentRegisteredBus);
    return status;
}

QStatus Transport::listenToSuperAgent(const char* senderId)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    QStatus status = ER_OK;

    do {
        m_SuperAgent->setIsFirstSuperAgent(false);
        m_Bus->EnableConcurrentCallbacks();

        String AJ_REMOVE_MATCH = "interface='" + AJ_NOTIFICATION_INTERFACE_NAME + "'";
        String AJ_SA_REMOVE_MATCH = "interface='" + AJ_SA_INTERFACE_NAME + "'";

        status = m_Bus->RemoveMatch(AJ_REMOVE_MATCH.c_str());
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not remove regular match from bus. Status: " + String(QCC_StatusText(status)));
            break;
        }
        cleanupTransportConsumer(true);

        cleanupAnnouncementListener(true);

        String AJ_SA_ADD_MATCH = "sender='" + String(senderId) + "'";

        if (logger)
            logger->debug(TAG, "SuperAgent add match:" + AJ_SA_ADD_MATCH);

        status = m_Bus->AddMatch(AJ_SA_ADD_MATCH.c_str());
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not add Super Agent filter match. Status: " + String(QCC_StatusText(status)));
            break;
        }

        status = m_Bus->RemoveMatch(AJ_SA_REMOVE_MATCH.c_str());
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not remove super agent match from bus. Status: " + String(QCC_StatusText(status)));
            break;
        }

        m_IsListeningToSuperAgent = true;

        if (logger)
            logger->info(TAG, "Added Super Agent successfully");
        std::cout << "NOTE: Starting to run in SuperAgent Mode" << std::endl;
        return status;
    } while (0);

    return status;
}

void Transport::cleanupTransportProducer(int32_t messageTypeIndx, bool unregister)
{
    while (messageTypeIndx >= 0) {
        if (!m_Producers[messageTypeIndx])
            continue;
        if (unregister)
            m_Bus->UnregisterBusObject(*m_Producers[messageTypeIndx]);
        delete m_Producers[messageTypeIndx];
        m_Producers[messageTypeIndx] = 0;
        messageTypeIndx--;
    }
}

void Transport::cleanupSenderTransport()
{
    if (m_Producers[0] != 0)
        cleanupTransportProducer(MESSAGE_TYPE_CNT - 1, true);
}

void Transport::cleanupReceiverTransport()
{
    if (m_Consumer != 0)
        cleanupTransportConsumer(true);
    if (m_SuperAgent != 0)
        cleanupTransportSuperAgent(true);
    if (m_AnnounceListener != 0)
        cleanupAnnouncementListener(true);
}

void Transport::cleanupTransportConsumer(bool unregister)
{
    if (!m_Consumer)
        return;

    if (unregister) {
        m_Consumer->unregisterHandler(m_Bus);
        m_Bus->UnregisterBusObject(*m_Consumer);
    }
    delete m_Consumer;
    m_Consumer = 0;
}


void Transport::cleanupTransportSuperAgent(bool unregister)
{
    if (!m_SuperAgent)
        return;

    if (unregister) {
        m_SuperAgent->unregisterHandler(m_Bus);
        m_Bus->UnregisterBusObject(*m_SuperAgent);
    }
    delete m_SuperAgent;
    m_SuperAgent = 0;
}


void Transport::cleanupAnnouncementListener(bool unregister)
{
    if (!m_AnnounceListener)
        return;

    if (unregister) {
        AnnouncementRegistrar::UnRegisterAnnounceHandler(*m_Bus, *m_AnnounceListener);
    }
    delete m_AnnounceListener;
    m_AnnounceListener = 0;
}

void Transport::cleanup()
{
    if (m_Bus == 0)
        return;

    cleanupSenderTransport();
    cleanupReceiverTransport();

    m_Bus = 0;
    s_Instance = 0;

    delete this;
}

ajn::BusAttachment* Transport::getBusAttachment()
{
    return m_Bus;
}


