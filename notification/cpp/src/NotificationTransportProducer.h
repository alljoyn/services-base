/******************************************************************************
 * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifndef NOTIFICATIONTRANSPORTPRODUCER_H_
#define NOTIFICATIONTRANSPORTPRODUCER_H_

#include <alljoyn/notification/NotificationEnums.h>

#include "NotificationTransport.h"
#include "NotificationConstants.h"

namespace ajn {
namespace services {

/**
 * Notification Transport Producer. Used to Create the Notification Interface
 * on the producers side and send the notify signals
 */
class NotificationTransportProducer : public NotificationTransport {

  public:

    /**
     * Constructor for TransportProducer. Creates Interface and prepares
     * infrastructure to be able to send Signal
     * @param bus            - BusAttachment that is used
     * @param servicePath    - servicePath of BusObject
     * @param status         - success/failure
     * @param m_serialNumber - serial number of the last successful message on the bus
     */
    NotificationTransportProducer(ajn::BusAttachment* bus,
                                  qcc::String const& servicePath, QStatus& status,
                                  qcc::String const& interfaceName = nsConsts::AJ_NOTIFICATION_INTERFACE_NAME, uint32_t serialNumber = 0);

    /**
     * Destructor for TransportProducer
     */
    ~NotificationTransportProducer() { };

    /**
     * Send Signal over Bus.
     * @param notificationArgs
     * @param ttl
     * @return status
     */
    QStatus sendSignal(ajn::MsgArg const notificationArgs[nsConsts::AJ_NOTIFY_NUM_PARAMS],
                       uint16_t ttl);

    /**
     * delete last message
     * @param type of message to delete
     * @return status
     */
    QStatus deleteLastMsg(NotificationMessageType messageType);

    /**
     * Delete Signal sent off for this messageType
     * @param messageId
     * @return status
     */
    QStatus deleteMsg(int32_t msgId);

    /**
     * get the notification id of the last message sent with the given MessageType
     * @param messageType type of message
     * @param messageId pointer to hold the notification id of the last message
     * @return status
     */
    QStatus getLastMsgId(NotificationMessageType messageType, int32_t* messageId);


  private:

    /**
     * Serial Number of the last message sent with this message type
     */
    uint32_t m_SerialNumber;

    /**
     * message Id of the last message sent with this message type
     */
    int32_t m_MsgId;

};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONTRANSPORTPRODUCER_H_ */