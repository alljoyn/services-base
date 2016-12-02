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

#import "alljoyn/time/TimeServiceServerAlarmFactory.h"
#import "AJTMTimeServiceServerAlarmFactory.h"

class AJTMTimeServiceServerAlarmFactoryAdapter : public ajn::services::TimeServiceServerAlarmFactory {
public:
    id<AJTMTimeServiceServerAlarmFactory> handle;

    /**
     * Constructor
     */
    AJTMTimeServiceServerAlarmFactoryAdapter(id<AJTMTimeServiceServerAlarmFactory> timeServiceServerAlarmFactory);

    /**
     * Destructor
     */
    ~AJTMTimeServiceServerAlarmFactoryAdapter();

    /**
     * Returns object path of this AlarmFactory
     *
     * @return Returns object path of this AlarmFactory
     */
    qcc::String const& getObjectPath() const;

    /**
     * Create new TimeServiceServerAlarm
     *
     * @param ptrAlarm Out variable. Pointer to the TimeServiceServerAlarm
     * @param errorInfo Out variable. If create new alarm fails due to max permitted number of Alarm objects
     * reached, or another reason. Populate ErrorInfo::errorName with tsConsts::OUT_OF_RANGE and
     * the errorInfo::errorDesc with the error description.
     *
     * @return ER_OK if creation of the TimeServiceServerAlarm succeeded, or return a failure status and
     * populate the ErrorInfo
     */
    QStatus newAlarm(ajn::services::TimeServiceServerAlarm** ptrAlarm, ajn::services::tsConsts::ErrorInfo* errorInfo);

    /**
     * Delete TimeServiceServerAlarm identified by the given objectPath.
     * To delete TimeServiceServerAlarm, call its release() method.
     *
     * @param objectPath of the TimeServiceServerAlarm to be deleted.
     * @param errorInfo To be populated if for any reason failed to delete the TimeServiceServerAlarm
     *
     * @return ER_OK if succeeded to delete TimeServiceServerAlarm or a failure status with population
     * of the ErrorInfo
     */
    QStatus deleteAlarm(const qcc::String& objectPath, ajn::services::tsConsts::ErrorInfo* errorInfo);

    /**
     * The method is invoked by the TimeService, when there was a failure to create NewAlarm,
     * or to construct a response to the client that has invoked the NewAlarm method.
     * Application that implementing this method is expected to invoke TimeServiceServerAlarm.release()
     * to release object resources.
     *
     * @param alarm TimeServiceServerAlarm that failed to be created
     */
    void handleError(ajn::services::TimeServiceServerAlarm* alarm);

};