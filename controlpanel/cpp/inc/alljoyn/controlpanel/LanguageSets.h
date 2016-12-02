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

#ifndef LANGUAGESETS_H_
#define LANGUAGESETS_H_

#include <alljoyn/controlpanel/LanguageSet.h>
#include <map>

namespace ajn {
namespace services {

/**
 * Class to store the LanguageSets defined
 */
class LanguageSets {
  public:

    /**
     * Constructor for LanguageSets
     */
    LanguageSets();

    /**
     * Destructor for LanguageSets
     */
    virtual ~LanguageSets();

    /**
     * Add a LanguageSet
     * @param languageSetName - name of LanguageSet
     * @param languageSet - languageSet
     */
    static void add(qcc::String const& languageSetName, LanguageSet const& languageSet);

    /**
     * Get a LanguageSet
     * @param languageSetName - the name of the languageSet to get
     * @return the languageSet requested or NULL if it does not exist
     */
    static LanguageSet* get(qcc::String const& languageSetName);

  private:

    /**
     * The LanguageSets map storing the LanguageSets defined
     */
    static std::map<qcc::String, LanguageSet> languageSets;
};

} /* namespace services */
} /* namespace ajn */
#endif /* LANGUAGESETS_H_ */