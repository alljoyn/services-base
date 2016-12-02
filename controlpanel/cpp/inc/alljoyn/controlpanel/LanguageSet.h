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

#ifndef LANGUAGESET_H_
#define LANGUAGESET_H_

#include <qcc/String.h>
#include <vector>

namespace ajn {
namespace services {

/**
 * Class used to define a LanguageSet
 */
class LanguageSet {

  public:

    /**
     * Constructor for LanguageSet class
     * @param languageSetName - name of LanguageSet
     */
    LanguageSet(qcc::String const& languageSetName);

    /**
     * Destructor for LanguageSet class
     */
    virtual ~LanguageSet();

    /**
     * Get the LanguageSetName
     * @return LanguageSetName
     */
    const qcc::String& getLanguageSetName() const;

    /**
     * Get the number of Languages defined
     * @return number of Languages
     */
    const size_t getNumLanguages() const;

    /**
     * Add a language to the LanguageSet
     * @param language - language to Add
     */
    void addLanguage(qcc::String const& language);

    /**
     *
     * Get the Languages defined in the LanguageSet
     * @return languages vector
     */
    const std::vector<qcc::String>& getLanguages() const;

  private:

    /**
     * Name of LanguageSet
     */
    qcc::String m_LanguageSetName;

    /**
     * vector to store Languages of LanguageSet
     */
    std::vector<qcc::String> m_Languages;
};

} /* namespace services */
} /* namespace ajn */
#endif /* LANGUAGESET_H_ */