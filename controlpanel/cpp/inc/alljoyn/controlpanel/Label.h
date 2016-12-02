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

#ifndef LABEL_H_
#define LABEL_H_

#include <alljoyn/controlpanel/Widget.h>
#include <map>

namespace ajn {
namespace services {

/**
 * Label class used to display a Label
 */
class Label : public Widget {
  public:

    /**
     * Constructor for Label class
     * @param name - name of Widget
     * @param rootWidget - the RootWidget of the widget
     */
    Label(qcc::String const& name, Widget* rootWidget);

    /**
     * Constructor for Label class
     * @param name - name of Widget
     * @param rootWidget - the RootWidget of the widget
     * @param device - the device that contains this Widget
     */
    Label(qcc::String const& name, Widget* rootWidget, ControlPanelDevice* device);

    /**
     * Destructor for Label class
     */
    virtual ~Label();

    /**
     * creates and returns the appropriate BusObject for this Widget
     * @param bus - the bus used to create the widget
     * @param objectPath - the objectPath of the widget
     * @param langIndx - the language Indx
     * @param status - the status indicating success or failure
     * @return a newly created WidgetBusObject
     */
    WidgetBusObject* createWidgetBusObject(BusAttachment* bus, qcc::String const& objectPath,
                                           uint16_t langIndx, QStatus& status);

    /**
     * Get the Labels vector of the widget
     * @return - label
     */
    const std::vector<qcc::String>& getLabels() const;

    /**
     * Set the labels vector of the widget
     * @param labels - vector of labels
     */
    void setLabels(const std::vector<qcc::String>& labels);

    /**
     * Get the GetLabel function pointer
     * @return GetLabel function pointer
     */
    GetStringFptr getGetLabels() const;

    /**
     * Set the GetLabel function pointer
     * @param getLabel - getLabel function pointer
     */
    void setGetLabels(GetStringFptr getLabel);

    /**
     * Fill MsgArg passed in with the Label
     * @param val - msgArg to fill
     * @param languageIndx - language of the label
     * @return status - success/failure
     */
    QStatus fillLabelArg(MsgArg& val, uint16_t languageIndx);

    /**
     * Read MsgArg passed in to fill the Label property
     * @param val - MsgArg passed in
     * @return status - success/failure
     */
    QStatus readLabelArg(MsgArg* val);

  private:

    /**
     * Vector of Labels of Label Widget
     */
    std::vector<qcc::String> m_LabelWidgetLabels;

    /**
     * GetLabel functionPointer of Label Widget
     */
    GetStringFptr m_LabelWidgetGetLabels;
};
} //namespace services
} //namespace ajn

#endif /* LABEL_H_ */