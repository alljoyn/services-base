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

#ifndef WIDGETFACTORY_H_
#define WIDGETFACTORY_H_

#include "../BusObjects/IntrospectionNode.h"
#include <alljoyn/controlpanel/Widget.h>

namespace ajn {
namespace services {

/**
 * Class used to create Widgets based on widgetType
 */
class WidgetFactory {
  public:

    /**
     * Create Widget based on WidgetType passed in
     * @param name - name of Widget
     * @param device - device that contains Widget
     * @param widgetType - type of widget to create
     * @return newly created widget or NULL if error
     */
    static Widget* createWidget(qcc::String const& name, ControlPanelDevice* device, WidgetType widgetType);
};

} /* namespace services */
} /* namespace ajn */
#endif /* WIDGETFACTORY_H_ */
