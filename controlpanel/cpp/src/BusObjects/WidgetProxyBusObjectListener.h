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

#ifndef WIDGETPROXYBUSOBJECTLISTENER_H_
#define WIDGETPROXYBUSOBJECTLISTENER_H_

#include <alljoyn/BusAttachment.h>
#include "WidgetBusObject.h"
#include <alljoyn/controlpanel/Widget.h>

namespace ajn {
namespace services {

/**
 * WidgetProxyBusObjectListener class - used as listener for GetAllProperties Async Call
 */
class WidgetProxyBusObjectListener : public ProxyBusObject::Listener {

  public:

    /**
     * Constructor of WidgetProxyBusObjectListener class
     * @param widget - widget getallproperties is being executed for
     * @param busObject - busObject being used for call
     */
    WidgetProxyBusObjectListener(Widget* widget, WidgetBusObject* busObject);

    /**
     * Destructor of WidgetProxyBusObjectListener class
     */
    virtual ~WidgetProxyBusObjectListener();

    /**
     * GetAllProperties callback
     * @param status - status of call: success/failure
     * @param obj - proxybusobject used for call
     * @param values - response MsgArg
     * @param context - context passed in to call
     */
    void GetAllPropertiesCallBack(QStatus status, ProxyBusObject* obj, const MsgArg& values, void* context);

  private:

    /**
     * The widget getAllProperties was being executed for
     */
    Widget* m_Widget;

    /**
     * The BusObject getAllProperties was being executed from
     */
    WidgetBusObject* m_BusObject;
};

} /* namespace services */
} /* namespace ajn */

#endif /* WIDGETPROXYBUSOBJECTLISTENER_H_ */