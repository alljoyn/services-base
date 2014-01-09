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

#include <alljoyn/about/AboutServiceApi.h>
#include <alljoyn/about/AnnouncementRegistrar.h>
#include <alljoyn/notification/NotificationService.h>
#include <alljoyn/notification/NotificationReceiver.h>

#include "NotificationConstants.h"
#include "PayloadAdapter.h"
#include "Transport.h"
#include "NotificationProducerReceiver.h"
#include "NotificationProducerSender.h"
#include "NotificationProducerListener.h"
#include "NotificationDismisserSender.h"
#include "NotificationDismisserReceiver.h"
#include "SuperAgentBusListener.h"
#include "NotificationAnnounceListener.h"
#include "NotificationConstants.h"
#include "NotificationTransportConsumer.h"
#include "NotificationTransportProducer.h"
#include "NotificationTransportSuperAgent.h"

using namespace ajn;
using namespace services;
using namespace nsConsts;
using namespace qcc;

Transport* Transport::s_Instance(NULL);

Transport::Transport() : m_Bus(0), m_Receiver(0), m_Consumer(0), m_SuperAgent(0), m_AnnounceListener(0), m_SuperAgentBusListener(0),
    m_IsSendingDisabled(false), m_IsReceivingDisabled(false), m_IsSuperAgentDisabled(false),
    m_IsListeningToSuperAgent(false), TAG(TAG_TRANSPORT), m_NotificationProducerSender(0), m_NotificationProducerReceiver(0),
    m_NotificationProducerListener(0), m_NotificationDismisserSender(0), m_NotificationDismisserReceiver(0)
{
    Notification::m_feedbackAsyncTask.Start();

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
        if (logger) {
            logger->warn(TAG, "Could not accept this BusAttachment, a different bus attachment already exists");
        }
        return ER_BUS_LISTENER_ALREADY_SET;
    }

    if (!bus->IsStarted() || !bus->IsConnected()) {
        if (logger) {
            logger->warn(TAG, "Could not acccept this BusAttachment, bus attachment not started or not connected");
        }
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
        if (logger) {
            logger->warn(TAG, "Could not receive message, Receiver is not initialized");
        }
        return;
    }

    if (m_IsReceivingDisabled) {
        if (logger) {
            logger->info(TAG, "Could not receive message, Receiving is Disabled");
        }
        return;
    }

    m_Bus->EnableConcurrentCallbacks();
    m_Receiver->Receive(notification);
}

QStatus Transport::sendNotification(NotificationMessageType messageType,
                                    MsgArg const notificationArgs[AJ_NOTIFY_NUM_PARAMS], uint16_t ttl)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if (m_Producers[messageType] == 0) {
        if (logger) {
            logger->warn(TAG, "Could not send message, Sender is not initialized");
        }
        return ER_BUS_OBJECT_NOT_REGISTERED;
    }

    if (m_IsSendingDisabled) {
        if (logger) {
            logger->info(TAG, "Could not send message, Sending is Disabled");
        }
        return ER_BUS_NOT_ALLOWED;
    }

    return m_Producers[messageType]->sendSignal(notificationArgs, ttl);
}

QStatus Transport::deleteLastMsg(NotificationMessageType messageType)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if (m_Producers[messageType] == 0) {
        if (logger) {
            logger->warn(TAG, "Could not delete message, Sender is not initialized");
        }
        return ER_BUS_OBJECT_NOT_REGISTERED;
    }

    return m_Producers[messageType]->deleteLastMsg(messageType);
}

QStatus Transport::deleteMsg(int32_t msgId)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "Transport::deleteMsg()");
    }
    QStatus ret = ER_OK;

    for (size_t i  = 0; i < NotificationMessageType::MESSAGE_TYPE_CNT; i++) {
        if (m_Producers[i] == 0) {
            if (logger) {
                logger->warn(TAG, "Could not delete message, Sender is not initialized");
            }
            //return ER_BUS_OBJECT_NOT_REGISTERED;
            continue;
        } else {
            if (logger) {
                logger->warn(TAG, "deleting message");
            }
        }

        ret = m_Producers[i]->deleteMsg(msgId);
        if (ret == ER_OK)
            break;
    }
    return ret;
}

QStatus Transport::startSenderTransport(BusAttachment* bus, bool startSuperAgent)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "Transport::startSenderTransport");
    }
    std::vector<String> interfaces;
    QStatus status;

    if ((status = setBusAttachment(bus)) != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not start Sender Transport. BusAttachment is not valid");
        }
        goto exit;
    }

    if (m_Producers[0]) {
        if (logger) {
            logger->info(TAG, "Sender Transport already started. Ignoring request");
        }
        goto exit;
    }

    { //Handling AboutService
        AboutServiceApi* aboutService = AboutServiceApi::getInstance();
        if (!aboutService) {
            if (logger) {
                logger->info(TAG, "AboutService is not defined");
            }
            status = ER_FAIL;
            goto exit;
        }


        for (int32_t messageTypeIndx = 0; messageTypeIndx < MESSAGE_TYPE_CNT; messageTypeIndx++) {
            status = ER_OK;
            if (startSuperAgent) {
                m_Producers[messageTypeIndx] = new NotificationTransportProducer(m_Bus,
                                                                                 AJ_PRODUCER_SERVICE_PATH_PREFIX + MessageTypeUtil::getMessageTypeString(messageTypeIndx),
                                                                                 status, AJ_SA_INTERFACE_NAME);
                if (status != ER_OK)
                    goto exit;

                if (!interfaces.size())
                    interfaces.push_back(AJ_SA_INTERFACE_NAME);

                status = aboutService->AddObjectDescription(AJ_PRODUCER_SERVICE_PATH_PREFIX +
                                                            MessageTypeUtil::getMessageTypeString(messageTypeIndx), interfaces);
            } else {
                m_Producers[messageTypeIndx] = new NotificationTransportProducer(m_Bus,
                                                                                 AJ_PRODUCER_SERVICE_PATH_PREFIX + MessageTypeUtil::getMessageTypeString(messageTypeIndx), status);

                if (status != ER_OK)
                    goto exit;

                if (!interfaces.size())
                    interfaces.push_back(AJ_NOTIFICATION_INTERFACE_NAME);

                status = aboutService->AddObjectDescription(AJ_PRODUCER_SERVICE_PATH_PREFIX +
                                                            MessageTypeUtil::getMessageTypeString(messageTypeIndx), interfaces);
            }

            if (status != ER_OK) {
                if (logger) {
                    logger->warn(TAG, "Could not create BusObject. Status: " + String(QCC_StatusText(status)));
                }
                goto exit;
            }

            status = m_Bus->RegisterBusObject(*m_Producers[messageTypeIndx]);
            if (status != ER_OK) {
                if (logger) {
                    logger->warn(TAG, "Could not register BusObject. Status: " + String(QCC_StatusText(status)));
                }
                goto exit;
            }
        }
    } //Handling AboutService

    //Code handles NotificationProducerReceiver - Start
    m_NotificationProducerReceiver = new NotificationProducerReceiver(m_Bus, status);
    if (status != ER_OK)
        goto exit;
    status = m_Bus->RegisterBusObject(*m_NotificationProducerReceiver);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not register BusObject. Status: " + String(QCC_StatusText(status)));
        }
        goto exit;
    } else {
        if (logger) {
            logger->debug(TAG, "Transport::startSenderTransport - registered NotificationProducerReceiver successfully.");
        }
    }

    m_NotificationProducerListener = new NotificationProducerListener();
    m_NotificationProducerListener->setSessionPort(AJ_NOTIFICATION_PRODUCER_SERVICE_PORT);

    { //Handling Bind session
        SessionPort servicePort = AJ_NOTIFICATION_PRODUCER_SERVICE_PORT;
        SessionOpts sessionOpts(SessionOpts::TRAFFIC_MESSAGES, true, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);

        status = m_Bus->BindSessionPort(servicePort, sessionOpts, *m_NotificationProducerListener);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not bind Session Port successfully");
            }
            goto exit;
        } else {
            if (logger) {
                logger->warn(TAG, "bind Session Port successfully for notification producer service");
            }
        }
    }
    //Code handles NotificationProducerReceiver - End

    //Handling NotificationDismisserSender - start here
    m_NotificationDismisserSender = new NotificationDismisserSender(m_Bus, AJ_NOTIFICATION_DISMISSER_PATH, status);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not create NotificationDismisserSender. Status: " + String(QCC_StatusText(status)));
        }
        goto exit;
    }

    status = m_Bus->RegisterBusObject(*m_NotificationDismisserSender);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not register NotificationDismisserSender. Status: " + String(QCC_StatusText(status)));
        }
        goto exit;
    }
    //Handling NotificationDismisserSender - end here

exit:    //label exit
    if (status == ER_OK) {
        if (logger) {
            logger->info(TAG, "Started Sender successfully");
        }
    } else {   //if (status != ER_OK)
        if (logger) {
            logger->info(TAG, "Started Sender with error. Status:" + String(QCC_StatusText(status)));
        }
        cleanupSenderTransport();
    }

    return status;
}

QStatus Transport::listenForAnnouncements()
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    m_AnnounceListener = new NotificationAnnounceListener();

    QStatus status;
    status = AnnouncementRegistrar::RegisterAnnounceHandler(*m_Bus, *m_AnnounceListener);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not create AnnouncementListener. AnnounceHandlerApi not initialized");
        }
        cleanupAnnouncementListener();
    }
    return status;
}

QStatus Transport::startReceiverTransport(BusAttachment* bus)
{
    QStatus status = ER_OK;
    GenericLogger* logger = NotificationService::getInstance()->getLogger();

    if ((status = setBusAttachment(bus)) != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not start Receiver Transport. BusAttachment is not valid");
        }
        goto exit;
    }

    if (m_Consumer == NULL) {
        m_Consumer = new NotificationTransportConsumer(m_Bus, AJ_CONSUMER_SERVICE_PATH, status);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not create Consumer BusObject. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }

        status = m_Bus->RegisterBusObject(*m_Consumer);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not register Consumer BusObject. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }

        String AJ_NOTIFICATION_INTERFACE_MATCH = "interface='" + AJ_NOTIFICATION_INTERFACE_NAME + "'";

        if (logger) {
            logger->debug(TAG, "Match String is: " + AJ_NOTIFICATION_INTERFACE_MATCH);
        }

        status = m_Bus->AddMatch(AJ_NOTIFICATION_INTERFACE_MATCH.c_str());

        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not add filter match for notification service. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }
    }
    if (!m_IsSuperAgentDisabled && !m_SuperAgent) {
        m_SuperAgent = new NotificationTransportSuperAgent(m_Bus, AJ_CONSUMER_SERVICE_PATH, status);

        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not create SuperAgent BusObject. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }

        status = m_Bus->RegisterBusObject(*m_SuperAgent);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not register SuperAgent BusObject. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }

        String AJ_SUPERAGENT_INTERFACE_MATCH = "interface='" + AJ_SA_INTERFACE_NAME + "'";

        if (logger) {
            logger->debug(TAG, "SuperAgent Match String is: " + AJ_SUPERAGENT_INTERFACE_MATCH);
        }

        status = m_Bus->AddMatch(AJ_SUPERAGENT_INTERFACE_MATCH.c_str());
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not add filter match. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }
    }

    //Handling NotificationProducerSender - Start
    if (m_NotificationProducerSender == NULL) {
        m_NotificationProducerSender = new NotificationProducerSender(m_Bus, status);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not create NotificationProducerSender BusObject. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }
        status = m_Bus->RegisterBusObject(*m_NotificationProducerSender);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not register NotificationProducerSender BusObject. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }
    }
    //Handling NotificationProducerSender - End

    //Handling NotificationDismisserReceiver - Start
    if (m_NotificationDismisserReceiver == NULL) {
        m_NotificationDismisserReceiver = new NotificationDismisserReceiver(m_Bus, status);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not create NotificationDismisserReceiver BusObject. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }
        status = m_Bus->RegisterBusObject(*m_NotificationDismisserReceiver);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not register m_NotificationDismisserReceiver BusObject. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }

        String AJ_DISMISSER_INTERFACE_MATCH = "interface='" + AJ_NOTIFICATION_DISMISSER_INTERFACE + "'";

        if (logger) {
            logger->debug(TAG, "NotificationDismisserReceiver Match String is: " + AJ_DISMISSER_INTERFACE_MATCH);
        }

        status = m_Bus->AddMatch(AJ_DISMISSER_INTERFACE_MATCH.c_str());
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not add filter match. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }
    }
    //Handling NotificationDismisserReceiver - End

    //Handling NotificationDismisserSender - Start
    if (m_NotificationDismisserSender == NULL) {
        m_NotificationDismisserSender = new NotificationDismisserSender(m_Bus, AJ_NOTIFICATION_DISMISSER_PATH, status);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not create NotificationDismisserSender. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }

        status = m_Bus->RegisterBusObject(*m_NotificationDismisserSender);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not register BusObject. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }
    }
    //Handling NotificationDismisserSender - End

    status = listenForAnnouncements();
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not listen for announcements.");
        }
        goto exit;
    } else {
        if (logger) {
            logger->warn(TAG, "successfully listens announcements.");
        }
    }

    if (!m_SuperAgentBusListener) {
        m_SuperAgentBusListener = new SuperAgentBusListener(m_Bus);
        m_Bus->RegisterBusListener(*m_SuperAgentBusListener);
    }

exit:
    if (status != ER_OK) {
        if (logger) {
            logger->info(TAG, "Not listening to any producers");
        }

        cleanupReceiverTransport();
    } else {
        if (logger) {
            logger->info(TAG, "Started Receiver successfully");
        }
    }
    return status;
}

QStatus Transport::FindSuperAgent(const char* busName)
{
    m_SuperAgentBusListener->SetBusUniqueName(busName);
    return m_Bus->FindAdvertisedName(busName);
}

QStatus Transport::listenToSuperAgent(const char* senderId)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    QStatus status = ER_OK;

    if (m_IsListeningToSuperAgent == true) {
        if (logger) {
            logger->warn(TAG, "Already listening to super agent");
        }
        return status;
    }

    m_SuperAgent->setIsFirstSuperAgent(false);
    m_Bus->EnableConcurrentCallbacks();

    String AJ_REMOVE_MATCH = "interface='" + AJ_NOTIFICATION_INTERFACE_NAME + "'";
    String AJ_SA_REMOVE_MATCH = "interface='" + AJ_SA_INTERFACE_NAME + "'";

    status = m_Bus->RemoveMatch(AJ_REMOVE_MATCH.c_str());
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not remove regular match from bus. Status: " + String(QCC_StatusText(status)));
        }
        goto exit;
    }

    cleanupTransportConsumer(true);
    cleanupAnnouncementListener(true);

    { //add mach sender id
        String AJ_SA_ADD_MATCH = "sender='" + String(senderId) + "'";

        if (logger) {
            logger->debug(TAG, "SuperAgent add match:" + AJ_SA_ADD_MATCH);
        }

        status = m_Bus->AddMatch(AJ_SA_ADD_MATCH.c_str());
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not add Super Agent filter match. Status: " + String(QCC_StatusText(status)));
            }
            goto exit;
        }
    }

    status = m_Bus->RemoveMatch(AJ_SA_REMOVE_MATCH.c_str());
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not remove super agent match from bus. Status: " + String(QCC_StatusText(status)));
        }
        goto exit;
    }

    m_IsListeningToSuperAgent = true;

exit:
    if (status == ER_OK) {
        if (logger) {
            logger->info(TAG, "Added Super Agent successfully");
            logger->info(TAG, "NOTE: Starting to run in SuperAgent Mode");
        }
    }

    return status;
}

QStatus Transport::cancelListenToSuperAgent(const char* senderId)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    QStatus status = ER_OK;

    if (m_IsListeningToSuperAgent == false) {
        if (logger) {
            logger->warn(TAG, "Already not listening to super agent");
        }
        return status;
    }

    m_IsListeningToSuperAgent = false;
    m_Bus->EnableConcurrentCallbacks();

    //remove match sender
    String AJ_SA_ADD_MATCH = "sender='" + String(senderId) + "'";
    status = m_Bus->RemoveMatch(AJ_SA_ADD_MATCH.c_str());
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not remove super agent match from bus. Status: " + String(QCC_StatusText(status)));
        }
        return status;
    }
    startReceiverTransport(m_Bus);

    String AJ_SUPERAGENT_INTERFACE_MATCH = "interface='" + AJ_SA_INTERFACE_NAME + "'";

    if (logger) {
        logger->debug(TAG, "SuperAgent Match String is: " + AJ_SUPERAGENT_INTERFACE_MATCH);
    }

    status = m_Bus->AddMatch(AJ_SUPERAGENT_INTERFACE_MATCH.c_str());
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not add filter match. Status: " + String(QCC_StatusText(status)));
        }
        return status;
    }

    m_SuperAgent->setIsFirstSuperAgent(true);

    return status;
}

void Transport::cleanupTransportProducer(int32_t messageTypeIndx, bool unregister)
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    if (logger) {
        logger->debug(TAG, "Transport::cleanupTransportProducer - Start");
    }
    for  (; messageTypeIndx >= 0; messageTypeIndx--) {
        if (!m_Producers[messageTypeIndx])
            continue;
        if (unregister)
            m_Bus->UnregisterBusObject(*m_Producers[messageTypeIndx]);
        delete m_Producers[messageTypeIndx];
        m_Producers[messageTypeIndx] = 0;
    }
    if (logger) {
        logger->debug(TAG, "Transport::cleanupTransportProducer - End");
    }

}

void Transport::cleanupNotificationProducerReceiver()
{
    GenericLogger* logger = NotificationService::getInstance()->getLogger();
    SessionPort sp = AJ_NOTIFICATION_PRODUCER_SERVICE_PORT;
    QStatus status = m_Bus->UnbindSessionPort(sp);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not unbind the SessionPort" + String(QCC_StatusText(status)));
        }
    }

    if (logger) {
        logger->debug(TAG, "cleaning NotificationProducerListener");
    }
    if (m_NotificationProducerListener) {
        delete m_NotificationProducerListener;
        m_NotificationProducerListener = 0;
    }

    if (m_NotificationProducerReceiver) {
        if (logger) {
            logger->debug(TAG, "cleaning NotificationProducerReceiver");
        }
        m_NotificationProducerReceiver->unregisterHandler(m_Bus);
        m_Bus->UnregisterBusObject(*m_NotificationProducerReceiver);
        delete m_NotificationProducerReceiver;
        m_NotificationProducerReceiver = NULL;
    }
}

void Transport::cleanupNotificationDismisserSender()
{
    if (!m_NotificationDismisserSender)
        return;

    m_Bus->UnregisterBusObject(*m_NotificationDismisserSender);

    delete m_NotificationDismisserSender;
    m_NotificationDismisserSender = 0;
}

void Transport::cleanupNotificationDismisserReceiver()
{
    if (!m_NotificationDismisserReceiver)
        return;

    m_NotificationDismisserReceiver->unregisterHandler(m_Bus);
    m_Bus->UnregisterBusObject(*m_NotificationDismisserReceiver);

    delete m_NotificationDismisserReceiver;
    m_NotificationDismisserReceiver = 0;
}


void Transport::cleanupSenderTransport()
{
    cleanupNotificationDismisserSender();
    cleanupNotificationProducerSender();
    cleanupNotificationProducerReceiver();
    cleanupTransportProducer(MESSAGE_TYPE_CNT - 1, true);
}

void Transport::cleanupReceiverTransport()
{
    cleanupNotificationDismisserReceiver();
    cleanupNotificationProducerSender();
    cleanupTransportConsumer(true);
    cleanupTransportSuperAgent(true);
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

void Transport::cleanupNotificationProducerSender()
{
    if (!m_NotificationProducerSender)
        return;

    m_Bus->UnregisterBusObject(*m_NotificationProducerSender);

    delete m_NotificationProducerSender;
    m_NotificationProducerSender = 0;
}

void Transport::cleanup()
{
    if (m_Bus == 0)
        return;
    Notification::m_feedbackAsyncTask.Stop();
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

NotificationProducerSender* Transport::getNotificationProducerSender()
{
    return m_NotificationProducerSender;
}

NotificationProducerReceiver* Transport::getNotificationProducerReceiver()
{
    return m_NotificationProducerReceiver;
}

NotificationDismisserSender* Transport::getNotificationDismisserSender()
{
    return m_NotificationDismisserSender;
}

NotificationReceiver* Transport::getNotificationReceiver()
{
    return m_Receiver;
}
