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

#include <alljoyn/controlpanel/Label.h>
#include "../ControlPanelConstants.h"
#include "../BusObjects/LabelBusObject.h"

namespace ajn {
namespace services {
using namespace cpsConsts;

Label::Label(qcc::String const& name, Widget* rootWidget) :
    Widget(name, rootWidget, WIDGET_TYPE_LABEL), m_LabelWidgetGetLabels(0)
{
}

Label::Label(qcc::String const& name, Widget* rootWidget, ControlPanelDevice* device) :
    Widget(name, rootWidget, device, WIDGET_TYPE_LABEL), m_LabelWidgetGetLabels(0)
{
}

Label::~Label()
{
}

WidgetBusObject* Label::createWidgetBusObject(BusAttachment* bus, qcc::String const& objectPath,
                                              uint16_t langIndx, QStatus& status)
{
    return new LabelBusObject(bus, objectPath, langIndx, status, this);
}

const std::vector<qcc::String>& Label::getLabels() const
{
    return m_LabelWidgetLabels;
}

void Label::setLabels(const std::vector<qcc::String>& labels)
{
    m_LabelWidgetLabels = labels;
}

GetStringFptr Label::getGetLabels() const
{
    return m_LabelWidgetGetLabels;
}

void Label::setGetLabels(GetStringFptr getLabels)
{
    m_LabelWidgetGetLabels = getLabels;
}

QStatus Label::fillLabelArg(MsgArg& val, uint16_t languageIndx)
{
    if (!(m_LabelWidgetLabels.size() > languageIndx) && !m_LabelWidgetGetLabels) {
        return ER_BUS_PROPERTY_VALUE_NOT_SET;
    }

    return val.Set(AJPARAM_STR.c_str(), m_LabelWidgetGetLabels ? m_LabelWidgetGetLabels(languageIndx) :
                   m_LabelWidgetLabels[languageIndx].c_str());
}

QStatus Label::readLabelArg(MsgArg* val)
{
    QStatus status = ER_OK;
    char* label;
    CHECK_AND_RETURN(val->Get(AJPARAM_STR.c_str(), &label))
    m_Label = label;
    return status;
}

} /* namespace services */
} /* namespace ajn */