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

#ifndef TIMESERVICECLOCKBUSOBJ_H_
#define TIMESERVICECLOCKBUSOBJ_H_

#include <alljoyn/BusAttachment.h>
#include <alljoyn/BusObject.h>
#include <alljoyn/InterfaceDescription.h>
#include <alljoyn/time/TimeServiceServerClock.h>
#include <alljoyn/Status.h>
#include <vector>

namespace ajn {
namespace services {

/**
 * This class implements org.allseen.Time.Clock interface and realizes AllJoyn communication
 * with this clock.
 */
class TimeServiceClockBusObj : public ajn::BusObject {

  public:

    /**
     * Constructor
     *
     * @param objectPath
     */
    TimeServiceClockBusObj(qcc::String const& objectPath);

    /**
     * Destructor
     */
    ~TimeServiceClockBusObj();

    /**
     * Initialize the Bus Object. Register it on the BusAttachment and in the AboutService for Announcement
     *
     * @param clock Events delegate. This clock receives all the Clock related events.
     * @return QStatus of the Clock initialization
     */
    virtual QStatus init(TimeServiceServerClock* clock);

    /**
     * Returns object path of this Clock
     *
     * @return Returns object path of this Clock
     */
    qcc::String const& getObjectPath() const;

    /**
     * Releases object resources
     */
    void release();

  protected:

    /**
     * Creates Clock interface if hasn't been created before and adds it to the BusObject
     *
     * @param bus
     * @return QStatus of creating and interface
     */
    virtual QStatus addClockInterface(BusAttachment* bus);

    /**
     * Callback for Alljoyn when GetProperty is called on this BusObject
     *
     * @param interfaceName - the name of the Interface
     * @param propName - the name of the Property
     * @param val - the MsgArg to fill
     * @return status - success/failure
     */
    virtual QStatus Get(const char* interfaceName, const char* propName, MsgArg& msgArg);

    /**
     * Callback for Alljoyn when SetProperty is called on this BusObject
     *
     * @param interfaceName - the name of the Interface
     * @param propName - the name of the Property
     * @param val - the MsgArg that contains the new Value
     * @return status - success/failure
     */
    virtual QStatus Set(const char* interfaceName, const char* propName, MsgArg& msgArg);

  private:

    /**
     * Clock events delegate
     */
    TimeServiceServerClock* m_Clock;

    /**
     * Object path of this Clock object
     */
    const qcc::String m_ObjectPath;

    /**
     * Handles Get Version property request
     *
     * @param val Is the message argument
     */
    QStatus handleGetVersion(MsgArg& msgArg);

    /**
     * Handles Get DateTime property request
     *
     * @param val Is the message argument
     */
    QStatus handleGetDateTime(MsgArg& val);

    /**
     * Handles Get IsSet property request
     *
     * @param val Is the message argument
     */
    QStatus handleGetIsSet(MsgArg& val);

    /**
     * Handles Set DateTime
     *
     * @param val Is the message argument
     */
    QStatus handleSetDateTime(MsgArg& val);
};

} /* namespace services */
} /* namespace ajn */

#endif /* TIMESERVICECLOCKBUSOBJ_H_ */