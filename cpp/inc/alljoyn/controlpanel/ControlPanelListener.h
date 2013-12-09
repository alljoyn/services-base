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


#ifndef CONTROLPANELLISTENER_H_
#define CONTROLPANELLISTENER_H_

#include <alljoyn/controlpanel/ControlPanelDevice.h>

namespace ajn {
namespace services {

/**
 * An Abstract class with function receive. The implementation of this class
 * can be passed in to the initReceive function and will be the callback for
 * when notifications are received
 */
class ControlPanelListener {
  public:

    /**
     * Constructor for DeviceEventsListener
     */
    ControlPanelListener() { };

    /**
     * Destructor for DeviceEventsListener
     */
    virtual ~ControlPanelListener() { };

    /**
     * sessionEstablished
     * @param device
     */
    virtual void sessionEstablished(ControlPanelDevice* device) = 0;

    /**
     * sessionLost
     * @param device
     */
    virtual void sessionLost(ControlPanelDevice* device) = 0;

    /**
     * signalPropertiesChanged
     * @param device
     * @param widget
     */
    virtual void signalPropertiesChanged(ControlPanelDevice* device, Widget* widget) = 0;

    /**
     * signalPropertiesChanged
     * @param device
     * @param widget
     */
    virtual void signalValueChanged(ControlPanelDevice* device, Widget* widget) = 0;

    /**
     * errorOccured
     * @param device
     * @param error
     */
    virtual void errorOccured(ControlPanelDevice* device, qcc::String const& error) = 0;
};
} //namespace services
} //namespace ajn

#endif /* CONTROLPANELLISTENER_H_ */


