/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
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

#include "ControlPanelListenerImpl.h"
#include <alljoyn/controlpanel/ControlPanel.h>
#include <alljoyn/controlpanel/ActionWithDialog.h>
#include <iostream>

#ifndef UINT32_MAX
#define UINT32_MAX      (4294967295U)
#endif

using namespace ajn;
using namespace services;

ControlPanelListenerImpl::ControlPanelListenerImpl()
{
}

ControlPanelListenerImpl::~ControlPanelListenerImpl()
{
}

void ControlPanelListenerImpl::sessionEstablished(ControlPanelDevice* device)
{
    std::map<qcc::String, ControlPanelUnit*> units = device->getDeviceUnits();
    std::map<qcc::String, ControlPanelUnit*>::iterator iter;
    std::map<qcc::String, ControlPanel*>::iterator it;

    for (iter = units.begin(); iter != units.end(); iter++) {
        std::cout << "Now parsing unit: " << iter->first.c_str() << std::endl;
        HttpControl* httpControl = iter->second->getHttpControl();
        if (httpControl) {
            std::cout << "Unit has a HttpControl: " << std::endl;
            std::cout << "  HttpControl version: " << httpControl->getInterfaceVersion() << std::endl;
            std::cout << "  HttpControl url: " << httpControl->getUrl().c_str() << std::endl;
        }
        std::map<qcc::String, ControlPanel*> controlPanels = iter->second->getControlPanels();
        for (it = controlPanels.begin(); it != controlPanels.end(); it++) {
            std::cout << "Now parsing panelName: " << it->first.c_str() << std::endl;
            std::vector<qcc::String> languages = it->second->getLanguageSet().getLanguages();
            for (size_t i = 0; i < languages.size(); i++) {

                std::vector<Action*> actionsToExecute;
                std::vector<Property*> propertiesToChange;
                std::vector<Dialog*> dialogsToExecute;

                std::cout << "Now parsing language: " << languages[i].c_str() << std::endl;
                Container* rootContainer = it->second->getRootWidget(languages[i]);
                std::cout << "Finished loading widget: " << rootContainer->getWidgetName().c_str() << std::endl;
                printContainer(rootContainer, actionsToExecute, dialogsToExecute, propertiesToChange);

                std::cout << "Going to execute all Actions now" << std::endl;
                executeActions(actionsToExecute);
                std::cout << std::endl;

                std::cout << "Going to execute all Dialog Actions now" << std::endl;
                executeDialogActions(dialogsToExecute);
                std::cout << std::endl;
            }
        }
    }
}

void ControlPanelListenerImpl::printContainer(Container* container, std::vector<Action*>& actionsToExecute, std::vector<Dialog*>& dialogsToExecute,
                                              std::vector<Property*>& propertiesToChange, qcc::String const& indent)
{
    printBasicWidget(container, "Container", indent);

    std::vector<Widget*> childWidgets = container->getChildWidgets();
    std::cout << indent.c_str() << "Print ChildWidgets: " << std::endl;
    for (size_t i = 0; i < childWidgets.size(); i++) {
        WidgetType widgetType = childWidgets[i]->getWidgetType();
        switch (widgetType) {
        case ACTION:
            printBasicWidget(childWidgets[i], "Action", indent + "  ");
            actionsToExecute.push_back((Action*)childWidgets[i]);
            break;

        case ACTION_WITH_DIALOG:
            printBasicWidget(childWidgets[i], "Action", indent + "  ");
            std::cout << indent.c_str() << "  Printing ChildDialog: " << std::endl;
            printDialog(((ActionWithDialog*)childWidgets[i])->getChildDialog(), indent + "    ");
            dialogsToExecute.push_back(((ActionWithDialog*)childWidgets[i])->getChildDialog());
            break;

        case LABEL:
            printBasicWidget(childWidgets[i], "Label", indent + "  ");
            break;

        case PROPERTY:
            printProperty(((Property*)childWidgets[i]), indent + "  ");
            propertiesToChange.push_back((Property*)childWidgets[i]);
            break;

        case CONTAINER:
            printContainer(((Container*)childWidgets[i]), actionsToExecute, dialogsToExecute, propertiesToChange, indent + "  ");
            break;

        case DIALOG:
            printDialog(((Dialog*)childWidgets[i]), indent + "  ");
            break;

        }

        std::cout << std::endl;
    }
}

void ControlPanelListenerImpl::printProperty(Property* property, qcc::String const& indent)
{
    printBasicWidget(property, "Property", indent);
    printPropertyValue(property->getPropertyValue(), property->getPropertyType(), indent);
    std::cout << indent.c_str() << "Property is " << (property->getIsWritable() ? "writable" : "not writable") << std::endl;

    if (property->getUnitOfMeasure().size())
        std::cout << indent.c_str() << "Property unitOfMeasure: " << property->getUnitOfMeasure().c_str() << std::endl;
    if (property->getConstraintRange()) {
        std::cout << indent.c_str() << "Property has ConstraintRange: " << std::endl;
        printConstraintRange(property->getConstraintRange(), property->getPropertyType(), indent + "  ");
    }
    if (property->getConstraintList().size()) {
        std::cout << indent.c_str() << "Property has ConstraintList: " << std::endl;
        printConstraintList(property->getConstraintList(), property->getPropertyType(), indent + "  ");
    }
}

void ControlPanelListenerImpl::printDialog(Dialog* dialog, qcc::String const& indent)
{
    printBasicWidget(dialog, "Dialog", indent);
    std::cout << indent.c_str() << "Dialog message: " << dialog->getMessage().c_str() << std::endl;
    std::cout << indent.c_str() << "Dialog numActions: " << dialog->getNumActions() << std::endl;
    if (dialog->getLabelAction1().size())
        std::cout << indent.c_str() << "Dialog Label for Action1: " << dialog->getLabelAction1().c_str() << std::endl;
    if (dialog->getLabelAction2().size())
        std::cout << indent.c_str() << "Dialog Label for Action2: " << dialog->getLabelAction2().c_str() << std::endl;
    if (dialog->getLabelAction3().size())
        std::cout << indent.c_str() << "Dialog Label for Action3: " << dialog->getLabelAction3().c_str() << std::endl;
}

void ControlPanelListenerImpl::printBasicWidget(Widget* widget, qcc::String const& widgetType, qcc::String const& indent)
{
    std::cout << indent.c_str() << widgetType.c_str() << " name: " << widget->getWidgetName().c_str() << std::endl;
    std::cout << indent.c_str() << widgetType.c_str() << " version: " << widget->getInterfaceVersion() << std::endl;
    std::cout << indent.c_str() << widgetType.c_str() << " is " << (widget->getIsSecured() ? "secured" : "not secured") << std::endl;
    std::cout << indent.c_str() << widgetType.c_str() << " is " << (widget->getIsEnabled() ? "enabled" : "not enabled") << std::endl;
    if (widget->getLabel().size())
        std::cout << indent.c_str() << widgetType.c_str() << " label: " << widget->getLabel().c_str() << std::endl;
    if (widget->getBgColor() != UINT32_MAX)
        std::cout << indent.c_str() << widgetType.c_str() << " bgColor: " << widget->getBgColor() << std::endl;
    std::vector<uint16_t> hints = widget->getHints();
    if (hints.size()) {
        std::cout << indent.c_str() << widgetType.c_str() << " hints: ";
        for (size_t i = 0; i < hints.size(); i++) {
            std::cout << hints[i] << (i == hints.size() - 1 ? "\n" : ", ");
        }
    }
}

void ControlPanelListenerImpl::printConstraintList(const std::vector<ConstraintList>& constraintList, PropertyType propertyType, qcc::String const& indent)
{
    for (size_t i = 0; i < constraintList.size(); i++) {
        std::cout << indent.c_str() << "ConstraintList " << i << " Display: " << constraintList[i].getDisplay().c_str() << std::endl;
        switch (propertyType) {
        case UINT16_PROPERTY:
            std::cout << indent.c_str() << "ConstraintList " << i << " Value: " << constraintList[i].getConstraintValue().uint16Value << std::endl;
            break;

        case INT16_PROPERTY:
            std::cout << indent.c_str() << "ConstraintList " << i << " Value: " << constraintList[i].getConstraintValue().int16Value << std::endl;
            break;

        case UINT32_PROPERTY:
            std::cout << indent.c_str() << "ConstraintList " << i << " Value: " << constraintList[i].getConstraintValue().uint32Value << std::endl;
            break;

        case INT32_PROPERTY:
            std::cout << indent.c_str() << "ConstraintList " << i << " Value: " << constraintList[i].getConstraintValue().int32Value << std::endl;
            break;

        case UINT64_PROPERTY:
            std::cout << indent.c_str() << "ConstraintList " << i << " Value: " << constraintList[i].getConstraintValue().uint64Value << std::endl;
            break;

        case INT64_PROPERTY:
            std::cout << indent.c_str() << "ConstraintList " << i << " Value: " << constraintList[i].getConstraintValue().int64Value << std::endl;
            break;

        case DOUBLE_PROPERTY:
            std::cout << indent.c_str() << "ConstraintList " << i << " Value: " << constraintList[i].getConstraintValue().doubleValue << std::endl;
            break;

        case STRING_PROPERTY:
            std::cout << indent.c_str() << "ConstraintList " << i << " Value: " << constraintList[i].getConstraintValue().charValue << std::endl;
            break;

        default:
            std::cout << indent.c_str() << "ConstraintList is unknown property type" << std::endl;
            break;
        }
    }
}

void ControlPanelListenerImpl::printConstraintRange(ConstraintRange* constraintRange, PropertyType propertyType, qcc::String const& indent)
{
    switch (propertyType) {
    case UINT16_PROPERTY:
        std::cout << indent.c_str() << "ConstraintRange MinValue: " << constraintRange->getMinValue().uint16Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange MaxValue: " << constraintRange->getMaxValue().uint16Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange IncrementValue: " << constraintRange->getIncrementValue().uint16Value << std::endl;
        break;

    case INT16_PROPERTY:
        std::cout << indent.c_str() << "ConstraintRange MinValue: " << constraintRange->getMinValue().int16Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange MaxValue: " << constraintRange->getMaxValue().int16Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange IncrementValue: " << constraintRange->getIncrementValue().int16Value << std::endl;
        break;

    case UINT32_PROPERTY:
        std::cout << indent.c_str() << "ConstraintRange MinValue: " << constraintRange->getMinValue().uint32Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange MaxValue: " << constraintRange->getMaxValue().uint32Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange IncrementValue: " << constraintRange->getIncrementValue().uint32Value << std::endl;
        break;

    case INT32_PROPERTY:
        std::cout << indent.c_str() << "ConstraintRange MinValue: " << constraintRange->getMinValue().int32Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange MaxValue: " << constraintRange->getMaxValue().int32Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange IncrementValue: " << constraintRange->getIncrementValue().int32Value << std::endl;
        break;

    case UINT64_PROPERTY:
        std::cout << indent.c_str() << "ConstraintRange MinValue: " << constraintRange->getMinValue().uint64Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange MaxValue: " << constraintRange->getMaxValue().uint64Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange IncrementValue: " << constraintRange->getIncrementValue().uint64Value << std::endl;
        break;

    case INT64_PROPERTY:
        std::cout << indent.c_str() << "ConstraintRange MinValue: " << constraintRange->getMinValue().int64Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange MaxValue: " << constraintRange->getMaxValue().int64Value << std::endl;
        std::cout << indent.c_str() << "ConstraintRange IncrementValue: " << constraintRange->getIncrementValue().int64Value << std::endl;
        break;

    case DOUBLE_PROPERTY:
        std::cout << indent.c_str() << "ConstraintRange MinValue: " << constraintRange->getMinValue().doubleValue << std::endl;
        std::cout << indent.c_str() << "ConstraintRange MaxValue: " << constraintRange->getMaxValue().doubleValue << std::endl;
        std::cout << indent.c_str() << "ConstraintRange IncrementValue: " << constraintRange->getIncrementValue().doubleValue << std::endl;
        break;

    default:
        std::cout << indent.c_str() << "ConstraintRange is unknown property type" << std::endl;
        break;
    }
}

void ControlPanelListenerImpl::printPropertyValue(PropertyValue propertyValue, PropertyType propertyType, qcc::String const& indent)
{
    switch (propertyType) {
    case UINT16_PROPERTY:
        std::cout << indent.c_str() << "Property is a UINT16 Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.uint16Value << std::endl;
        break;

    case INT16_PROPERTY:
        std::cout << indent.c_str() << "Property is a INT16 Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.int16Value << std::endl;
        break;

    case UINT32_PROPERTY:
        std::cout << indent.c_str() << "Property is a UINT32 Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.uint32Value << std::endl;
        break;

    case INT32_PROPERTY:
        std::cout << indent.c_str() << "Property is a INT32 Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.int32Value << std::endl;
        break;

    case UINT64_PROPERTY:
        std::cout << indent.c_str() << "Property is a UINT64 Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.uint64Value << std::endl;
        break;

    case INT64_PROPERTY:
        std::cout << indent.c_str() << "Property is a INT64 Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.int64Value << std::endl;
        break;

    case DOUBLE_PROPERTY:
        std::cout << indent.c_str() << "Property is a DOUBLE Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.doubleValue << std::endl;
        break;

    case STRING_PROPERTY:
        std::cout << indent.c_str() << "Property is a STRING Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.charValue << std::endl;
        break;

    case BOOL_PROPERTY:
        std::cout << indent.c_str() << "Property is a BOOL Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << (propertyValue.boolValue ? "true" : "false") << std::endl;
        break;

    case DATE_PROPERTY:
        std::cout << indent.c_str() << "Property is a Date Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.dateValue->getDay() << "/" <<
        propertyValue.dateValue->getMonth() << "/" <<
        propertyValue.dateValue->getYear() << std::endl;
        break;

    case TIME_PROPERTY:
        std::cout << indent.c_str() << "Property is a Time Property." << std::endl;
        std::cout << indent.c_str() << "Property Value: " << propertyValue.timeValue->getHour() << ":" <<
        propertyValue.timeValue->getMinute() << ":" <<
        propertyValue.timeValue->getSecond() << std::endl;
        break;

    default:
        std::cout << indent.c_str() << "Property is unknown property type" << std::endl;
        break;
    }
}

void ControlPanelListenerImpl::executeActions(std::vector<Action*>& actionsToExecute)
{
    for (size_t i = 0; i < actionsToExecute.size(); i++) {
        std::cout << "Execute Action for " << actionsToExecute[i]->getWidgetName().c_str() << std::endl;
        QStatus status = actionsToExecute[i]->executeAction();
        std::cout << "    Action execution " << (status == ER_OK ? "succeeded" : "failed") << std::endl;
    }
}

void ControlPanelListenerImpl::executeDialogActions(std::vector<Dialog*>& dialogsToExecute)
{
    for (size_t i = 0; i < dialogsToExecute.size(); i++) {
        std::cout << "Execute Actions for " << dialogsToExecute[i]->getWidgetName().c_str() << " which has " <<
        dialogsToExecute[i]->getNumActions() << " actions: " << std::endl;
        QStatus status = dialogsToExecute[i]->executeAction1();
        std::cout << "    Action 1 execution " << (status == ER_OK ? "succeeded" : "failed") << std::endl;
        status = dialogsToExecute[i]->executeAction2();
        std::cout << "    Action 2 execution " << (status == ER_OK ? "succeeded" : "failed") << std::endl;
        status = dialogsToExecute[i]->executeAction3();
        std::cout << "    Action 3 execution " << (status == ER_OK ? "succeeded" : "failed") << std::endl;
    }
}

void ControlPanelListenerImpl::sessionLost(ControlPanelDevice* device)
{

}

void ControlPanelListenerImpl::errorOccured(ControlPanelDevice* device, qcc::String const& error)
{

}

void ControlPanelListenerImpl::signalPropertiesChanged(ajn::services::ControlPanelDevice* device, ajn::services::Widget* widget)
{
    std::cout << "Received PropertiesChanged Signal for Widget " << widget->getWidgetName().c_str() << std::endl;
}

void ControlPanelListenerImpl::signalValueChanged(ajn::services::ControlPanelDevice* device, ajn::services::Widget* widget)
{
    std::cout << "Received ValueChanged Signal for Widget " << widget->getWidgetName().c_str() << std::endl;
}

