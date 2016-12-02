/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#ifndef NOTIFICATIONENUMS_H_
#define NOTIFICATIONENUMS_H_

#include <qcc/String.h>

namespace ajn {
namespace services {

/**
 * Enum to describe Message type levels
 */
enum NotificationMessageType {
    EMERGENCY = 0,            //!< EMERGENCY - Urgent Message
    WARNING = 1,              //!< WARNING  - Warning Message
    INFO = 2,                     //!< INFO - Informational Message
    MESSAGE_TYPE_CNT = 3,     //!< MESSAGE_TYPE_CNT  - Number of Message Types Defined
    UNSET = 4                 //!< UNSET  - No message Type set - used for testing
};

/**
 * A Util class that convers MessageType to String
 *
 */
class MessageTypeUtil {
  public:

    /**
     * Get MessageType String back based on int received
     * @param messageType - MessageType to convert
     * @return String that represents MessageType Enum
     */
    static qcc::String const& getMessageTypeString(int32_t messageType);

    /**
     * Get MessageType int and convert to enum
     * To prevent ugly casting in many places
     * @param messageType - MessageType to convert
     * @return Enum MessageType
     */
    static NotificationMessageType getMessageType(int32_t messageType);

    /**
     * Get the number of MessageType
     * Same as using enum MESSAGE_TYPE_CNT
     * @return amount of MessageType defined
     */
    static int32_t getNumMessageTypes();

  private:

    /**
     * Array that holds message type Strings
     */
    static const qcc::String MESSAGE_TYPE_STRINGS[];

    /**
     * private Constructor
     */
    MessageTypeUtil() { };
};
} //namespace services
} //namespace ajn

#endif /* NOTIFICATIONENUMS_H_ */