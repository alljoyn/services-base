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

#include "AnnounceHandlerImpl.h"
#include <iostream>

using namespace ajn;

AnnounceHandlerImpl::AnnounceHandlerImpl(BasicAnnounceHandlerCallback basicCallback, FullAnnounceHandlerCallback fullCallback) :
    m_BasicCallback(basicCallback), m_FullCallback(fullCallback)
{

}

AnnounceHandlerImpl::~AnnounceHandlerImpl()
{
}

void AnnounceHandlerImpl::Announced(const char* busName, uint16_t version, SessionPort port, const MsgArg& objectDescriptionArg, const MsgArg& aboutDataArg)
{
    std::cout << "*********************************************************************************" << std::endl;
    std::cout << "Announce signal discovered" << std::endl;
    std::cout << "\tFrom bus " << busName << std::endl;
    std::cout << "\tAbout version " << version << std::endl;
    std::cout << "\tSessionPort " << port << std::endl;
    std::cout << "\tObjectDescription" << std::endl << objectDescriptionArg.ToString().c_str() << std::endl;
    std::cout << "\tAboutData:" << std::endl << aboutDataArg.ToString().c_str() << std::endl;
    std::cout << "*********************************************************************************" << std::endl;

    std::cout << "AnnounceHandlerImpl::Announced()" << std::endl;
    if (m_BasicCallback) {
        std::cout << "Calling AnnounceHandler Callback" << std::endl;
        m_BasicCallback(busName, port);
    }

    if (m_FullCallback) {
        std::cout << "Calling AnnounceHandler Callback" << std::endl;
        AboutData aboutData;
        aboutData.CreatefromMsgArg(aboutDataArg);
        AboutObjectDescription aboutObjectDescription;
        aboutObjectDescription.CreateFromMsgArg(objectDescriptionArg);
        m_FullCallback(busName, version, port, aboutObjectDescription, aboutData);
    }
}
