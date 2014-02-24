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

#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/Container.h>
#include <alljoyn/controlpanel/Action.h>
#include <alljoyn/controlpanel/ActionWithDialog.h>
#include <alljoyn/controlpanel/Label.h>
#include <alljoyn/controlpanel/Property.h>
#include <alljoyn/controlpanel/ErrorWidget.h>
#include "../ControlPanelConstants.h"
#include "../BusObjects/ContainerBusObject.h"
#include "../BusObjects/NotificationActionBusObject.h"

namespace ajn {
namespace services {
using namespace cpsConsts;

Container::Container(qcc::String const& name, Widget* rootWidget) : RootWidget(name, rootWidget, CONTAINER, TAG_CONTAINER_WIDGET),
    m_IsDismissable(false)
{
}

Container::Container(qcc::String const& name, Widget* rootWidget, qcc::String const& objectPath, ControlPanelDevice* device) :
    RootWidget(name, rootWidget, objectPath, device, CONTAINER, TAG_CONTAINER_WIDGET), m_IsDismissable(false)
{
}

Container::Container(qcc::String const& name, Widget* rootWidget, ControlPanelDevice* device) :
    RootWidget(name, rootWidget, "", device, CONTAINER, TAG_CONTAINER_WIDGET), m_IsDismissable(false)
{
}

Container::~Container()
{
    if (m_ControlPanelMode == CONTROLLER_MODE) {
        for (size_t i = 0; i < m_ChildWidgets.size(); i++) {
            delete m_ChildWidgets[i];
        }
    }
}

WidgetBusObject* Container::createWidgetBusObject(BusAttachment* bus, qcc::String const& objectPath,
                                                  uint16_t langIndx, QStatus& status)
{
    return new ContainerBusObject(bus, objectPath, langIndx, status, this);
}

QStatus Container::registerObjects(BusAttachment* bus, LanguageSet const& languageSet,
                                   qcc::String const& objectPathPrefix, qcc::String const& objectPathSuffix, bool isRoot)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();

    QStatus status = Widget::registerObjects(bus, languageSet, objectPathPrefix, objectPathSuffix, isRoot);
    if (status != ER_OK) {
        return status;
    }

    qcc::String newObjectPathSuffix = isRoot ? objectPathSuffix : objectPathSuffix + "/" + m_Name;

    if (m_IsDismissable) {
        NotificationActionBusObject* NaBusObject = new NotificationActionBusObject(bus, newObjectPathSuffix, status);

        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not create NotificationActionBusObjects");
            }
            delete NaBusObject;
            return status;
        }

        status = setNotificationActionBusObject(NaBusObject);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not set NotificationActionBusObjects");
            }
            delete NaBusObject;
            return status;
        }
    }

    for (size_t indx = 0; indx < m_ChildWidgets.size(); indx++) {
        status = m_ChildWidgets[indx]->registerObjects(bus, languageSet, objectPathPrefix, newObjectPathSuffix);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not register childWidgets objects");
            }
            return status;
        }
    }
    return status;
}

QStatus Container::unregisterObjects(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus returnStatus = ER_OK;
    QStatus status = RootWidget::unregisterObjects(bus);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Could not unregister BusObjects");
        }
        returnStatus = status;
    }

    for (size_t indx = 0; indx < m_ChildWidgets.size(); indx++) {
        status = m_ChildWidgets[indx]->unregisterObjects(bus);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Could not unregister Objects for the childWidget");
            }
            returnStatus = status;
        }
    }
    return returnStatus;
}

QStatus Container::registerObjects(BusAttachment* bus)
{
    return Widget::registerObjects(bus, m_ObjectPath);
}

QStatus Container::addChildren(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!m_BusObjects.size()) {
        if (logger) {
            logger->warn(TAG, "BusObject is not set");
        }
        return ER_BUS_BUS_NOT_STARTED;
    }

    std::vector<IntrospectionNode> childNodes;
    QStatus status = m_BusObjects[0]->Introspect(childNodes);
    if (status != ER_OK) {
        if (logger) {
            logger->warn(TAG, "Introspection failed");
        }
        return status;
    }

    for (size_t i = 0; i < childNodes.size(); i++) {
        qcc::String const& objectPath = childNodes[i].getObjectPath();
        std::vector<qcc::String> splitObjectPath = ControlPanelService::SplitObjectPath(objectPath.c_str());
        qcc::String name = splitObjectPath.back();
        Widget* widget = createWidget(name, this, m_Device, childNodes[i].getWidgetType());
        if (widget == NULL) {
            continue;
        }
        widget->setIsSecured(childNodes[i].isSecured());
        QStatus childStatus = widget->registerObjects(bus, objectPath);
        if (childStatus == ER_OK) {
            addChildWidget(widget);
        } else {
            if (logger) {
                logger->warn(TAG, "Failed creating childWidget " + name);
            }

            ControlPanelListener* listener = m_Device->getListener();
            if (listener) {
                listener->errorOccured(m_Device, status, REGISTER_OBJECTS, "Could not register Objects for Widget: " + name);
            }

            Widget* errorWidget = new ErrorWidget(name, this, widget, m_Device);
            addChildWidget(errorWidget);
        }
    }
    return ER_OK;
}

QStatus Container::refreshChildren(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus status = ER_OK;
    for (size_t i = 0; i < m_ChildWidgets.size(); i++) {
        status = m_ChildWidgets[i]->refreshObjects(bus);
        if (status != ER_OK) {
            if (logger) {
                logger->warn(TAG, "Error refreshing Child: " + m_ChildWidgets[i]->getWidgetName());
            }
            return status;
        }
    }
    return status;
}

QStatus Container::addChildWidget(Widget* childWidget)
{
    if (!childWidget) {
        return ER_BAD_ARG_1;
    }

    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (logger) {
        logger->info(TAG, "Adding childWidget named: " + childWidget->getWidgetName());
    }
    m_ChildWidgets.push_back(childWidget);
    return ER_OK;
}

const std::vector<Widget*>& Container::getChildWidgets() const
{
    return m_ChildWidgets;
}

bool Container::getIsDismissable() const
{
    return m_IsDismissable;
}

void Container::setIsDismissable(bool isDismissable)
{
    m_IsDismissable = isDismissable;
}

Widget* Container::createWidget(qcc::String const& name, Widget* rootWidget, ControlPanelDevice* device, WidgetType widgetType)
{
    switch (widgetType) {
    case ACTION:
        return new Action(name, rootWidget, device);
        break;

    case ACTION_WITH_DIALOG:
        return new ActionWithDialog(name, rootWidget, device);
        break;

    case CONTAINER:
        return new Container(name, rootWidget, device);
        break;

    case DIALOG:
        return new Dialog(name, rootWidget, device);
        break;

    case LABEL:
        return new Label(name, rootWidget, device);
        break;

    case PROPERTY:
        return new Property(name, rootWidget, device);
        break;

    case ERROR:
        return NULL;
        break;
    }
    return NULL;
}

} /* namespace services */
} /* namespace ajn */