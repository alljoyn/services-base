/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
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

#include "TimeServiceUtility.h"
#include <alljoyn/AllJoynStd.h>

using namespace ajn;
using namespace services;

using namespace tsUtility;

//Create AllJoyn interface
QStatus tsUtility::createAJInterface(const qcc::String& ifaceName, bool isSecure, const qcc::String& version,
                                     BusAttachment* bus, InterfaceDescription** ptrIfaceDesc)
{

    QStatus status;

    //Interface has already been defined
    if (*ptrIfaceDesc) {

        return ER_OK;
    }

    status = bus->CreateInterface(ifaceName.c_str(), *ptrIfaceDesc, isSecure);
    if (status != ER_OK) {

        return status;
    }

    //InterfaceDescription object wasn't returned
    if (!*ptrIfaceDesc) {

        status = ER_FAIL;
        return status;
    }

    status = (*ptrIfaceDesc)->AddProperty(version.c_str(), "q", PROP_ACCESS_READ);
    if (status != ER_OK) {

        return status;
    }

    return ER_OK;
}

//Subtract from the subtracted vector all the elements that are in the subtracter vector, so that
void tsUtility::subtract(std::vector<qcc::String>* source, const std::vector<qcc::String>& subtract)
{

    //If subtracter is empty, copy all the elements from subtracted to result
    if (subtract.size() == 0) {

        return;
    }

    std::vector<qcc::String>::iterator sourceIter = source->begin();
    std::vector<qcc::String>::const_iterator subtractIter;

    while (sourceIter < source->end()) {

        const char* sourceVal = (*sourceIter).c_str();

        for (subtractIter = subtract.begin(); subtractIter < subtract.end(); ++subtractIter) {

            const char* subtractVal = (*subtractIter).c_str();

            //Check if the sourceVal was found in the subtract vector
            if (0 == strcmp(sourceVal, subtractVal)) {

                //Found the same value -> remove it. Iterator is forwarded by the erase method.
                sourceIter = source->erase(sourceIter);
                break;
            } else {

                ++sourceIter;
            }
        }
    }
}

//Set Interface description to the BusObject
void tsUtility::setIntrospectionDescription(BusObject* busObj, const qcc::String& description, const qcc::String& language,
                                            Translator* translator)
{

    if (description.length() ==  0 || language.length() == 0) {

        return;
    }

    busObj->SetDescription(language.c_str(), description.c_str());

    if (translator) {

        busObj->SetDescriptionTranslator(translator);
    }
}

//Returns the member of org::allseen::Introspectable interface
const InterfaceDescription::Member* tsUtility::getIntrospectionMember(BusAttachment* bus, ProxyBusObject* proxyObj,
                                                                      uint8_t memberType, QStatus& status)
{

    //AllSeenIntrospectable interface
    const char* ifaceName                 = org::allseen::Introspectable::InterfaceName;
    const InterfaceDescription* ifaceDesc = bus->GetInterface(ifaceName);

    if (proxyObj->GetInterface(ifaceName) == NULL) {

        status = proxyObj->AddInterface(*ifaceDesc);
        if (status != ER_OK) {

            return NULL;
        }
    }

    const InterfaceDescription::Member* introspectionMember = NULL;

    if (memberType == INTROSPECT) {

        introspectionMember = ifaceDesc->GetMethod("IntrospectWithDescription");
    } else if (memberType == GET_LANGUAGES) {

        introspectionMember = ifaceDesc->GetMethod("GetDescriptionLanguages");
    }

    if (!introspectionMember) {

        status = ER_BUS_OBJECT_NO_SUCH_MEMBER;
        return NULL;
    }

    status = ER_OK;
    return introspectionMember;
}
