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

#ifndef SUPER_AGENT_BUS_LISTENER_H_
#define SUPER_AGENT_BUS_LISTENER_H_

#include <string.h>
#include <alljoyn/BusAttachment.h>

namespace ajn {
namespace services {

/**
 * SuperAgentBusListener
 */
class SuperAgentBusListener : public BusListener {

  public:

    SuperAgentBusListener(ajn::BusAttachment* bus);

    virtual ~SuperAgentBusListener();

    void FoundAdvertisedName(const char* name, TransportMask transport, const char* namePrefix);

    void LostAdvertisedName(const char* name, TransportMask transport, const char* prefix);

    void SetBusUniqueName(const char* superAgentBusUniqueName) { m_SuperAgentBusUniqueName = superAgentBusUniqueName; }
  private:
    /**
     * BusAttachment
     */
    BusAttachment* m_Bus;
    /**
     * Super agent bus unique name
     */
    qcc::String m_SuperAgentBusUniqueName;
};
} //namespace services
} //namespace ajn

#endif /* SUPER_AGENT_BUS_LISTENER_H_ */