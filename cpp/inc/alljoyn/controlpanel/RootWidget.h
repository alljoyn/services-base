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
     * @param tag - Tag for logging
     */
    RootWidget(qcc::String const& name, WidgetType widgetType, qcc::String const& tag);

    /**
     * Constructor for RootWidget
     * @param name - name of Widget
     * @param objectPath the objectPath of the widget
     * @param device - the device that contains the widget
     * @param widgetType - the type of Widget created
     * @param tag - Tag for logging
     */
    RootWidget(qcc::String const& name, qcc::String const& objectPath, ControlPanelDevice* device, WidgetType widgetType, qcc::String const& tag);

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
