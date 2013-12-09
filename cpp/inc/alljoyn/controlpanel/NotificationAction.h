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

#ifndef NOTIFICATIONACTION_H_
#define NOTIFICATIONACTION_H_

#include <alljoyn/controlpanel/RootWidget.h>
#include <alljoyn/BusObject.h>

namespace ajn {
namespace services {

/**
 * NotificationAction class.
 */
class NotificationAction {
  public:

    /**
     * Function used to create a NotificationAction
     * @param languageSet - languageSet of NotificationAction
     * @return A newly created NotificationAction object or NULL
     */
    static NotificationAction* createNotificationAction(LanguageSet* languageSet);

    /**
     * Destructor of NotificationAction class
     */
    virtual ~NotificationAction();

    /**
     * Set the rootWidget of the NotificationAction
     * @param rootWidget - widget to set as RootWidget
     * @return status - success/failure
     */
    QStatus setRootWidget(RootWidget* rootWidget);

    /**
     * Register the BusObjects of the Notification Action
     * @param bus - bus to register the objects on
     * @param unitName - unitName to use in ObjectPath
     * @return status - success/failure
     */
    QStatus registerObjects(BusAttachment* bus, qcc::String const& unitName);

    /**
     * Unregister the BusObjects of the NotificationAction class
     * @param bus - bus used to unregister the objects
     * @return status - success/failure
     */
    QStatus unregisterObjects(BusAttachment* bus);

  private:

    /**
     * Private constructor of NotificationAction class
     * @param languageSet - languageSet of NotificationAction
     */
    NotificationAction(LanguageSet const& languageSet);

    /**
     * The LanguageSet of the NotificationAction
     */
    LanguageSet const& m_LanguageSet;

    /**
     * The RootWidget of the NotificationAction
     */
    RootWidget* m_RootWidget;

    /**
     * The BusObject of the NotificationAction
     */
    BusObject* m_NotificationActionBusObject;
};

} /* namespace services */
} /* namespace ajn */
#endif /* NOTIFICATIONACTION_H_ */


