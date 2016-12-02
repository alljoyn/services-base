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

#ifndef ONBOARDINGCLIENTLISTENER_H_
#define ONBOARDINGCLIENTLISTENER_H_

#include <qcc/String.h>

namespace ajn {
namespace services {
/**
 * OnboardingClientListener
 *
 *  is a helper class used by an AllJoyn IoE client application to receive OnboardingService signal notification.
 * The user of the class need to implement virtual void ConnectionResultSignalReceived(...) function
 */

class OnboardingClientListener {
  public:
    /**
     * Destructor
     */
    virtual ~OnboardingClientListener() { }

    /**
     * ConnectionResultSignalReceived
     * Needs to be implemented by the user for the desired behaviour when receiving the ConnectResult Signal
     * @param connectionResultCode
     * @param connectionResultMessage
     */
    virtual void ConnectionResultSignalReceived(short connectionResultCode, const qcc::String& connectionResultMessage) = 0;
};
}
}



#endif /* ONBOARDINGCLIENTLISTENER_H_ */