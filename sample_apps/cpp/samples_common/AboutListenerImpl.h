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

#ifndef ABOUTLISTENERSAMPLE_H_
#define ABOUTLISTENERSAMPLE_H_
#include <alljoyn/AboutListener.h>
#include <alljoyn/AboutData.h>

#if defined(QCC_OS_GROUP_WINDOWS)
/* Disabling warning C 4100. Function doesnt use all passed in parameters */
#pragma warning(push)
#pragma warning(disable: 4100)
#endif


typedef void (*BasicAboutListenerCallback)(qcc::String const& busName, unsigned short port);
typedef void (*FullAboutListenerCallback)(qcc::String const& busName, unsigned short version,
                                           unsigned short port, const ajn::AboutObjectDescription& objectDescription,
                                           const ajn::AboutData& aboutData);
/**
 * class AboutListenerImpl
 */
class AboutListenerImpl : public ajn::AboutListener {

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
     * AboutListenerImpl
     * @param basicCallback
     * @param fullCallback
     */
    AboutListenerImpl(BasicAboutListenerCallback basicCallback = 0, FullAboutListenerCallback fullCallback = 0);

    /**
     * ~AboutListenerImpl
     */
    ~AboutListenerImpl();

  private:

    BasicAboutListenerCallback m_BasicCallback;

    FullAboutListenerCallback m_FullCallback;
};

#endif /* ABOUTLISTENERSAMPLE_H_ */