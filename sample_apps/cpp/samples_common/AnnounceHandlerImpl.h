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

#ifndef ANNOUNCEHANDLERSAMPLE_H_
#define ANNOUNCEHANDLERSAMPLE_H_
#include <alljoyn/AboutListener.h>
#include <alljoyn/AboutData.h>

typedef void (*BasicAnnounceHandlerCallback)(qcc::String const& busName, unsigned short port);
typedef void (*FullAnnounceHandlerCallback)(qcc::String const& busName, unsigned short version,
                                            unsigned short port, const ajn::AboutObjectDescription& objectDescription,
                                            const ajn::AboutData& aboutData);
/**
 * class AnnounceHandlerImpl
 */
class AnnounceHandlerImpl : public ajn::AboutListener {

  public:

    /**
     * Announced - announced callback
     * @param busName
     * @param version
     * @param port
     * @param objectDescriptionArg
     * @param aboutDataArg
     */
    virtual void Announced(const char* busName, uint16_t version, ajn::SessionPort port, const ajn::MsgArg& objectDescriptionArg, const ajn::MsgArg& aboutDataArg);
    /**
     * AnnounceHandlerImpl
     * @param basicCallback
     * @param fullCallback
     */
    AnnounceHandlerImpl(BasicAnnounceHandlerCallback basicCallback = 0, FullAnnounceHandlerCallback fullCallback = 0);

    /**
     * ~AnnounceHandlerImpl
     */
    ~AnnounceHandlerImpl();

  private:

    BasicAnnounceHandlerCallback m_BasicCallback;

    FullAnnounceHandlerCallback m_FullCallback;
};

#endif /* ANNOUNCEHANDLERSAMPLE_H_ */