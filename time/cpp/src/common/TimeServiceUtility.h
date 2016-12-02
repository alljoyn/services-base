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

#ifndef TIMESERVICEUTILITY_H_
#define TIMESERVICEUTILITY_H_

#include <vector>
#include <qcc/String.h>
#include <alljoyn/BusObject.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/Status.h>
#include <alljoyn/InterfaceDescription.h>
#include <alljoyn/time/TimeServiceConstants.h>



namespace ajn {
namespace services {
namespace tsUtility {

/**
 * No language to be used for AllSeenIntrospectable
 */
const qcc::String NO_LANG   = "";

/**
 * IntrospectWithDescription member type
 */
const uint8_t INTROSPECT    = 1;

/**
 * GetDescriptionLanguages member type
 */
const uint8_t GET_LANGUAGES  = 2;

/**
 * Create AJ interface
 *
 * @param ifaceName AllJoyn name of the interface
 * @param isSecure Whether created interface should be secured
 * @param version Name of the version property
 * @param bus BusAttachment the interface is created
 * @param ifaceDesc InterfaceDescription
 *
 * @return ER_OK if the interface is already exists or was created successfully, otherwise appropriate
 * error is returned
 */
QStatus createAJInterface(const qcc::String& ifaceName, bool isSecure, const qcc::String& version,
                          BusAttachment* bus, InterfaceDescription** ptrIfaceDesc);

/**
 * Set interface descriptions of the given bus object to be not announced if its name exists in the substruct vector
 * @param busObject
 * @param bus
 * @param notAnnounced - interfaces that shouldn't be announced.
 */
void subtract(BusObject* busObject, BusAttachment* bus, const std::vector<qcc::String>& notAnnounced);

/**
 * Adds allseen introspectable interface to be announced.
 * @param busObject
 * @param bus
 * @param ifaceName
 * @param isAnnounced
 * @return QStatus
 */
QStatus setInterfaceAnnounce(BusObject* busObject, BusAttachment* bus, const qcc::String& ifaceName, bool isAnnounced);

/**
 * Set Interface description to the BusObject
 *
 * @param busObj Bus Object to set the interface description
 * @param description Events&Actions description
 * @param language Events&Actions description language
 * @param translator Events&Actions Translator
 */
void setIntrospectionDescription(BusObject* busObj, const qcc::String& description, const qcc::String& language,
                                 Translator* translator);

/**
 * Returns member of the org::allseen::Introspectable interface, according to the given memberType
 * If the interface hasn't been added to the given proxyObj, it's been added.
 *
 * @param bus BusAttachment
 * @param proxyObj
 * @param memberType INTROSPECT or GET_LANGUAGES
 * @param status Out variable. ER_OK if the introspection member has been retrieved successfully
 *
 * @return IntrospectWithDescription method
 */
const InterfaceDescription::Member* getIntrospectionMember(BusAttachment* bus, ProxyBusObject* proxyObj, uint8_t memberType,
                                                           QStatus& status);

}  /* namespace tsUtility */
}  /* namespace services */
}  /* namespace ajn */


#endif /* TIMESERVICEUTILITY_H_ */