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

#include <alljoyn/notification/RichAudioUrl.h>

using namespace ajn;
using namespace services;
using namespace qcc;

RichAudioUrl::RichAudioUrl(String const& language, String const& url)
{
    m_Language = language;
    m_Url = url;
}

void RichAudioUrl::setLanguage(String const& language)
{
    m_Language = language;
}

String const& RichAudioUrl::getLanguage() const
{
    return m_Language;
}

void RichAudioUrl::setUrl(String const& url)
{
    m_Url = url;
}

String const& RichAudioUrl::getUrl() const
{
    return m_Url;
}