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

#include "WidgetFactory.h"
#include <alljoyn/controlpanel/Action.h>
#include <alljoyn/controlpanel/ActionWithDialog.h>
#include <alljoyn/controlpanel/Container.h>
#include <alljoyn/controlpanel/Property.h>
#include <alljoyn/controlpanel/Label.h>

namespace ajn {
namespace services {

Widget* WidgetFactory::createWidget(qcc::String const& name, ControlPanelDevice* device, WidgetType widgetType)
{
    switch (widgetType) {
    case ACTION:
        return new Action(name, device);
        break;

    case ACTION_WITH_DIALOG:
        return new ActionWithDialog(name, device);
        break;

    case CONTAINER:
        return new Container(name, device);
        break;

    case DIALOG:
        return new Dialog(name, device);
        break;

    case LABEL:
        return new Label(name, device);
        break;

    case PROPERTY:
        return new Property(name, device);
        break;
    }
    return NULL;
}

} /* namespace services */
} /* namespace ajn */
