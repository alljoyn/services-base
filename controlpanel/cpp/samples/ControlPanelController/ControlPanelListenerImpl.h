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

#ifndef CONTROLPANELLISTENERIMPL_H_
#define CONTROLPANELLISTENERIMPL_H_

#include <alljoyn/controlpanel/ControlPanelListener.h>
#include <alljoyn/controlpanel/ControlPanelController.h>

/*
 *
 */
class ControlPanelListenerImpl : public ajn::services::ControlPanelListener {
  public:

    ControlPanelListenerImpl(ajn::services::ControlPanelController* controller);

    ~ControlPanelListenerImpl();

    void sessionEstablished(ajn::services::ControlPanelDevice* device);

    void sessionLost(ajn::services::ControlPanelDevice* device);

    void errorOccured(ajn::services::ControlPanelDevice* device, QStatus status,
                      ajn::services::ControlPanelTransaction transaction, qcc::String const& error);

    void signalPropertiesChanged(ajn::services::ControlPanelDevice* device, ajn::services::Widget* widget);

    void signalPropertyValueChanged(ajn::services::ControlPanelDevice* device, ajn::services::Property* property);

    void signalDismiss(ajn::services::ControlPanelDevice* device, ajn::services::NotificationAction* notificationAction);

  private:

    ajn::services::ControlPanelController* m_Controller;

    std::vector<qcc::String> m_ConnectedDevices;

};

#endif /* CONTROLPANELLISTENERIMPL_H_ */