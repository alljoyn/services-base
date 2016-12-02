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

#ifndef TIMESERVICESERVERCLOCK_H_
#define TIMESERVICESERVERCLOCK_H_

#include <alljoyn/time/TimeServiceDateTime.h>
#include <qcc/String.h>
#include <alljoyn/Status.h>


namespace ajn {
namespace services {

class TimeServiceServer;
class TimeServiceClockBusObj;

/**
 * Extend this class to receive events related to this Clock object.
 */
class TimeServiceServerClock {

    /**
     * This Clock object is a Friend of the TimeServiceServer
     */
    friend class TimeServiceServer;

  public:

    /**
     * Constructor
     */
    TimeServiceServerClock();

    /**
     * Destructor
     */
    virtual ~TimeServiceServerClock();

    /**
     * Releases object resources
     */
    void release();

    /**
     * Returns DateTime object
     *
     * @return DateTime object
     */
    virtual const TimeServiceDateTime& getDateTime() const = 0;

    /**
     * Set the DateTime object
     *
     * @param dateTime DateTime
     */
    virtual void setDateTime(TimeServiceDateTime const& dateTime) = 0;

    /**
     * Whether or not this clock has been set.
     *
     * @return Returns TRUE if the Clock has been set since the last reboot
     */
    virtual bool isSet() const = 0;

    /**
     * Returns object path of this Clock
     *
     * @return Returns object path of this Clock
     */
    qcc::String const& getObjectPath() const;

  protected:

    /**
     * Returns Clock BusObject
     */
    const TimeServiceClockBusObj* getClockBusObj();

  private:

    /**
     * Private Copy constructor of TimeServiceServerClock.
     * TimeServiceServerClock is not copy-able
     *
     * @param serverClock TimeServiceServerClock to copy
     */
    TimeServiceServerClock(const TimeServiceServerClock& serverClock);

    /**
     * Private assignment operator of TimeServiceServerClock.
     * TimeServiceServerClock is not assignable
     *
     * @param serverClock
     *
     * @return TimeServiceServerClock
     */
    TimeServiceServerClock& operator=(const TimeServiceServerClock& serverClock);

    /**
     * TimeServiceClock BusObject implementing AllJoyn functionality
     */
    TimeServiceClockBusObj* m_ClockBusObj;

    /**
     * Associate this Clock with the ClockBusObj which implements AllJoyn functionality
     *
     * @param clockBusObj
     * @return status if Succeeded to set the clockBusObj
     */
    QStatus setClockBusObj(TimeServiceClockBusObj* clockBusObj);
};

} /* namespace services */
} /* namespace ajn */

#endif /* TIMESERVICESERVERCLOCK_H_ */