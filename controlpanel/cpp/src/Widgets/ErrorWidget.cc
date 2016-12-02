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

#include "alljoyn/controlpanel/ErrorWidget.h"
#include "../ControlPanelConstants.h"

#define ERROR_WIDGET_LABEL "NOT AVAILABLE"

namespace ajn {
namespace services {
using namespace cpsConsts;

ErrorWidget::ErrorWidget(qcc::String const& name, Widget* rootWidget, Widget* originalWidget, ControlPanelDevice* device) :
    Widget(name, rootWidget, device, WIDGET_TYPE_ERROR), m_OriginalWidget(originalWidget)
{
    m_Label.assign(ERROR_WIDGET_LABEL);
}

ErrorWidget::~ErrorWidget()
{
    if (m_OriginalWidget) {
        delete m_OriginalWidget;
    }
}

WidgetBusObject* ErrorWidget::createWidgetBusObject(BusAttachment* bus, qcc::String const& objectPath,
                                                    uint16_t langIndx, QStatus& status)
{
    return NULL;
}

QStatus ErrorWidget::registerObjects(BusAttachment* bus, qcc::String const& objectPath)
{
    return ER_OK;
}

QStatus ErrorWidget::unregisterObjects(BusAttachment* bus)
{
    if (m_OriginalWidget) {
        return m_OriginalWidget->unregisterObjects(bus);
    }
    return ER_OK;
}

Widget* ErrorWidget::getOriginalWidget() const
{
    return m_OriginalWidget;
}

} /* namespace services */
} /* namespace ajn */