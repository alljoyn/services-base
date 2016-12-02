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

#ifdef _WIN32
/* Disable deprecation warnings */
#pragma warning(disable: 4996)
#endif

#include "WidgetProxyBusObjectListener.h"
#include "../ControlPanelConstants.h"
#include "WidgetBusObject.h"
#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/LogModule.h>

namespace ajn {
namespace services {
using namespace cpsConsts;

WidgetProxyBusObjectListener::WidgetProxyBusObjectListener(Widget* widget, WidgetBusObject* busObject) :
    m_Widget(widget), m_BusObject(busObject)
{


}

WidgetProxyBusObjectListener::~WidgetProxyBusObjectListener()
{

}

void WidgetProxyBusObjectListener::GetAllPropertiesCallBack(QStatus status, ProxyBusObject* obj, const MsgArg& values, void* context)
{
    QCC_UNUSED(obj);
    QCC_UNUSED(context);
    if (!m_Widget || !m_BusObject) {
        QCC_DbgHLPrintf(("WidgetProxyBusObjectListener does not have widget or BusObject set"));
        delete this;     //Finished using listener - needs to be deleted
        return;
    }

    ControlPanelDevice* device = m_Widget->getDevice();
    ControlPanelListener* listener = device ? device->getListener() : NULL;
    if (status != ER_OK) {
        QCC_DbgHLPrintf(("Something went wrong reloading properties"));
        if (listener) {
            listener->errorOccured(device, status, REFRESH_PROPERTIES, "Something went wrong reloading properties");
        }
        delete this;     //Finished using listener - needs to be deleted
        return;
    }

    status = m_BusObject->fillAllProperties(values);
    if (status != ER_OK) {
        QCC_LogError(status, ("Something went wrong reloading properties"));
        if (listener) {
            listener->errorOccured(device, status, REFRESH_PROPERTIES, "Something went wrong reloading properties");
        }
        delete this;     //Finished using listener - needs to be deleted
        return;
    }

    if (listener) {
        listener->signalPropertiesChanged(device, m_Widget);
    }

    delete this;     //Finished using listener - needs to be deleted
}

} /* namespace services */
} /* namespace ajn */