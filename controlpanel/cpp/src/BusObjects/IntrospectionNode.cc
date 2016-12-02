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

#include "IntrospectionNode.h"

namespace ajn {
namespace services {

IntrospectionNode::IntrospectionNode(qcc::String const& objectPath, WidgetType widgetType, bool secured) :
    m_ObjectPath(objectPath), m_WidgetType(widgetType), m_Secured(secured)
{

}

IntrospectionNode::~IntrospectionNode()
{

}

const qcc::String& IntrospectionNode::getObjectPath() const
{
    return m_ObjectPath;
}

void IntrospectionNode::setObjectPath(const qcc::String& objectPath)
{
    m_ObjectPath = objectPath;
}

bool IntrospectionNode::isSecured() const
{
    return m_Secured;
}

void IntrospectionNode::setSecured(bool secured)
{
    m_Secured = secured;
}

WidgetType IntrospectionNode::getWidgetType() const
{
    return m_WidgetType;
}

void IntrospectionNode::setWidgetType(WidgetType widgetType)
{
    m_WidgetType = widgetType;
}

} /* namespace services */
} /* namespace ajn */