/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/
#ifndef PROPERTYSTOREIMPL_H_
#define PROPERTYSTOREIMPL_H_

#include <stdio.h>
#include <iostream>

#include <alljoyn/about/AboutServiceApi.h>
#include <alljoyn/about/AboutPropertyStoreImpl.h>


class PropertyStoreImpl : public ajn::services::AboutPropertyStoreImpl {

  public:
    PropertyStoreImpl(const char* factoryConfigFile, const char* configFile);

    void FactoryReset();

    const qcc::String& GetConfigFileName();

    virtual ~PropertyStoreImpl();

    virtual QStatus ReadAll(const char* languageTag, Filter filter, ajn::MsgArg& all);

    virtual QStatus Update(const char* name, const char* languageTag, const ajn::MsgArg* value);

    virtual QStatus Delete(const char* name, const char* languageTag);

    void Initialize();

  private:

    ajn::services::PropertyMap m_PropertiesReadOnly;

    bool m_IsInitialized;

    qcc::String m_configFileName;

    qcc::String m_factoryConfigFileName;

    bool persistUpdate(const char* key, const char* value);

    ajn::services::PropertyStoreKey getPropertyStoreKeyFromName(qcc::String const& propertyStoreName);
};

#endif /* PROPERTYSTOREIMPL_H_ */
