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

#ifndef ROOTWIDGET_H_
#define ROOTWIDGET_H_

#include <alljoyn/controlpanel/Widget.h>

namespace ajn {
namespace services {

/**
 * RootWidget class. Used as base for widgets that function as a
 * rootContainer or rootDialog
 */
class RootWidget : public Widget {
  public:

    /**
     * Constructor for RootWidget
     * @param name - name of Widget
     * @param rootWidget - the RootWidget of the widget
     * @param widgetType - the type of the widget
     */
    RootWidget(qcc::String const& name, Widget* rootWidget, WidgetType widgetType);

    /**
     * Constructor for RootWidget
     * @param name - name of Widget
     * @param rootWidget - the RootWidget of the widget
     * @param objectPath the objectPath of the widget
     * @param device - the device that contains the widget
     * @param widgetType - the type of Widget created
     */
    RootWidget(qcc::String const& name, Widget* rootWidget, qcc::String const& objectPath,
               ControlPanelDevice* device, WidgetType widgetType);

    /**
     * Destructor of RootWidget
     */
    virtual ~RootWidget();

    /**
     * Send a Signal to Dismiss the widget
     * @return stats - success/failure
     */
    QStatus SendDismissSignal();

    /**
     * Set the BusObject if RootWidget is a NotificationAction
     * @param notificationActionBusObject - the busObject to store
     * @return status - success/failure
     */
    QStatus setNotificationActionBusObject(BusObject* notificationActionBusObject);

    /**
     * Unregister the BusObjects
     * @param bus - bus used to unregister
     * @return status - success/failure
     */
    virtual QStatus unregisterObjects(BusAttachment* bus);

    /**
     * Register the BusObjects for this Widget
     * @param bus - the bus to be used
     * @return status - success/failure
     */
    virtual QStatus registerObjects(BusAttachment* bus);

    /**
     * Register the BusObjects for this Widget
     * @param bus - the bus used to register the busObject
     * @param objectPath - the objectPath of the busObject
     * @return
     */
    virtual QStatus registerObjects(BusAttachment* bus, qcc::String const& objectPath);

    /**
     * Register the BusObjects for this Widget
     * @param bus - the bus used to register the busObjects
     * @param languageSet -  the languageSet to register the busObjects for
     * @param objectPathPrefix - the objectPathPrefix of the busObject
     * @param objectPathSuffix - the objectPathSuffix of the busObject
     * @param isRoot - is this a rootWidget
     * @return status - success/failure
     */
    virtual QStatus registerObjects(BusAttachment* bus, LanguageSet const& languageSet,
                                    qcc::String const& objectPathPrefix, qcc::String const& objectPathSuffix, bool isRoot = false);

  protected:

    /**
     * The BusObject if this Widget is a NotificationAction
     */
    BusObject* m_NotificationActionBusObject;

    /**
     * The ObjectPath of the Widget
     */
    qcc::String m_ObjectPath;

};
} //namespace services
} //namespace ajn

#endif /* ROOTWIDGET_H_ */