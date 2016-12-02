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

#include <alljoyn/controlpanel/LanguageSet.h>
#include <algorithm>

namespace ajn {
namespace services {

LanguageSet::LanguageSet(qcc::String const& languageSetName) :
    m_LanguageSetName(languageSetName)
{
}

LanguageSet::~LanguageSet()
{
}

const qcc::String& LanguageSet::getLanguageSetName() const
{
    return m_LanguageSetName;
}

const size_t LanguageSet::getNumLanguages() const
{
    return m_Languages.size();
}

void LanguageSet::addLanguage(qcc::String const& language)
{
    if (std::find(m_Languages.begin(), m_Languages.end(), language) == m_Languages.end()) {
        m_Languages.push_back(language);
    }
}

const std::vector<qcc::String>& LanguageSet::getLanguages() const
{
    return m_Languages;
}
} /* namespace services */
} /* namespace ajn */