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

#include <alljoyn/controlpanel/LanguageSets.h>

namespace ajn {
namespace services {
using namespace qcc;

std::map<String, LanguageSet> LanguageSets::languageSets;

LanguageSets::LanguageSets()
{
}

LanguageSets::~LanguageSets()
{
}

void LanguageSets::add(String const& languageSetName, LanguageSet const& languageSet)
{
    languageSets.insert(std::pair<String, LanguageSet>(languageSetName, languageSet));
}

LanguageSet* LanguageSets::get(String const& languageSetName)
{
    std::map<String, LanguageSet>::iterator it;
    if ((it = languageSets.find(languageSetName)) != languageSets.end()) {
        return &it->second;
    }
    return 0;
}

} /* namespace services */
} /* namespace ajn */