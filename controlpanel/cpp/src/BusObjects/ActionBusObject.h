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

#ifndef ACTIONBUSOBJECT_H_
#define ACTIONBUSOBJECT_H_

#include "WidgetBusObject.h"

namespace ajn {
namespace services {

/**
 * ActionBusObject - BusObject for Actions
 */
class ActionBusObject : public WidgetBusObject {
  public:

    /**
     * Constructor for ActionBusObject class
     * @param bus - the bus to create the interface
     * @param objectPath - objectPath of BusObject
     * @param langIndx - the languageIndex of the BusObject
     * @param status - success/failure
     * @param widget - the widget associated with the BusObject
     */
    ActionBusObject(ajn::BusAttachment* bus, qcc::String const& objectPath,
                    uint16_t langIndx, QStatus& status, Widget* widget);

    /**
     * Destructor for the BusObject
     */
    virtual ~ActionBusObject();

    /**
     * Callback when Execute is called
     * @param member - the member (method) of the interface that was executed
     * @param msg - the Message of the method
     */
    void ActionExecute(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /**
     * Execute this Action's remoteAction
     * @return status - success/failure
     */
    QStatus ExecuteAction();

    /**
     * PropertyChanged - callback when a propertyChanged signal comes in
     * @param member - the member (signal) of the interface that was fired
     * @param srcPath - path of signal
     * @param msg - the arguments
     */
    void PropertyChanged(const InterfaceDescription::Member* member, const char* srcPath, Message& msg);

    /**
     * Introspect to receive childNodes
     * @param childNodes - childNodes found during introspection
     * @return status - success/failure
     */
    QStatus Introspect(std::vector<IntrospectionNode>& childNodes);
};

} /* namespace services */
} /* namespace ajn */
#endif /* ACTIONBUSOBJECT_H_ */