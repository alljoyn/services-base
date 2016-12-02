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

/**
 * This class is experimental, and as such has not been tested.
 * Please help make it more robust by contributing fixes if you find issues
 **/

#import "alljoyn/time/TimeServiceServerAuthorityClock.h"
#import "AJTMTimeServiceServerClock.h"
#import "AJTMTimeServiceServerClockAdapter.h"

class AJTMTimeServiceServerTimeAuthorityAdapter : public ajn::services::TimeServiceServerAuthorityClock {
public:
    id<AJTMTimeServiceServerClock> handle;



    /**
     * Returns DateTime object
     *
     * @return DateTime object
     */
    const ajn::services::TimeServiceDateTime& getDateTime() const;

    /**
     * Set the DateTime object
     *
     * @param dateTime DateTime
     */
    void setDateTime(ajn::services::TimeServiceDateTime const& dateTime);

    /**
     * Whether or not this clock has been set.
     *
     * @return Returns TRUE if the Clock has been set since the last reboot
     */
    bool isSet() const;
    /**
     * Constructor
     */
    AJTMTimeServiceServerTimeAuthorityAdapter(id<AJTMTimeServiceServerClock> timeServiceServerClock);

    /**
     * Destructor
     */
    ~AJTMTimeServiceServerTimeAuthorityAdapter();

};