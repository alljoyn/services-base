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

#include <alljoyn/notification/NotificationText.h>

using namespace ajn;
using namespace services;
using namespace qcc;

NotificationText::NotificationText(String const& language, String const& text)
{
    m_Language = language;
    m_Text = text;
}

void NotificationText::setLanguage(String const& language)
{
    m_Language = language;
}

String const& NotificationText::getLanguage() const
{
    return m_Language;
}

void NotificationText::setText(String const& text)
{
    m_Text = text;
}

String const& NotificationText::getText() const
{
    return m_Text;
}