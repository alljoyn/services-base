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

#ifndef CONTAINERBUSOBJECT_H_
#define CONTAINERBUSOBJECT_H_

#include "WidgetBusObject.h"

namespace ajn {
namespace services {

/**
 * ContainerBusObject - BusObject for Containers
 */
class ContainerBusObject : public WidgetBusObject {
  public:

    /**
     * Constructor for ContainerBusObject class
     * @param bus - the bus to create the interface
     * @param objectPath - objectPath of BusObject
     * @param langIndx - the languageIndex of the BusObject
     * @param status - success/failure
     * @param widget - the widget associated with the BusObject
     */
    ContainerBusObject(ajn::BusAttachment* bus, qcc::String const& objectPath,
                       uint16_t langIndx, QStatus& status, Widget* widget);

    /**
     * Destructor for ContainerBusObject class
     */
    virtual ~ContainerBusObject();

    /**
     * Introspect to receive childNodes
     * @param childNodes - childNodes found during introspection
     * @return status - success/failure
     */
    QStatus Introspect(std::vector<IntrospectionNode>& childNodes);
};

} /* namespace services */
} /* namespace ajn */
#endif /* CONTAINERBUSOBJECT_H_ */