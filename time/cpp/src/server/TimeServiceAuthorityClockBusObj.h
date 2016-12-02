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

#ifndef TIMESERVICEAUTHORITYCLOCKBUSOBJ_H_
#define TIMESERVICEAUTHORITYCLOCKBUSOBJ_H_

#include "TimeServiceClockBusObj.h"
#include <alljoyn/time/TimeServiceServerAuthorityClock.h>
#include <alljoyn/time/TimeServiceConstants.h>

namespace ajn {
namespace services {

class TimeServiceAuthorityClockBusObj : public TimeServiceClockBusObj {

  public:

    /**
     * Constructor
     *
     * @param objectPath
     * @param authorityType
     */
    TimeServiceAuthorityClockBusObj(qcc::String const& objectPath, tsConsts::ClockAuthorityType type);

    /**
     * Destructor
     */
    ~TimeServiceAuthorityClockBusObj();

    /**
     * Send TimeSync signal
     *
     * @return status
     */
    QStatus sendTimeSync();

  private:

    /**
     * The pointer is used to send signals
     */
    const ajn::InterfaceDescription::Member* m_TimeSyncSignalMethod;

    /**
     * Authority type
     */
    tsConsts::ClockAuthorityType m_Type;

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
     * Handles Get Version property request
     *
     * @param val Is the message argument
     * @return status
     */
    QStatus handleGetAuthVersion(MsgArg& msgArg);

    /**
     * Handles Get AuthorityType request
     *
     * @return status
     */
    QStatus handleGetAuthorityType(MsgArg& msgArg);
};

} /* namespace services */
} /* namespace ajn */

#endif /* TIMESERVICEAUTHORITYCLOCKBUSOBJ_H_ */