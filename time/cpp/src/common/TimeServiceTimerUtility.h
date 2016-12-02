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

#ifndef TIMESERVICETIMERMUTILITY_H_
#define TIMESERVICETIMERMUTILITY_H_

#include <alljoyn/BusAttachment.h>
#include <alljoyn/Status.h>
#include <alljoyn/InterfaceDescription.h>
#include <alljoyn/time/TimeServiceConstants.h>
#include <alljoyn/time/TimeServicePeriod.h>

namespace ajn {
namespace services {
namespace tsTimerUtility {

//Timer interface and object
const qcc::String IFACE_PROP_VERSION                = "Version";
const qcc::String IFACE_PROP_INTERVAL               = "Interval";
const qcc::String IFACE_PROP_TITLE                  = "Title";
const qcc::String IFACE_PROP_TIMELEFT               = "TimeLeft";
const qcc::String IFACE_PROP_ISRUNNING              = "IsRunning";
const qcc::String IFACE_PROP_REPEAT                 = "Repeat";
const qcc::String IFACE_METHOD_START                = "Start";
const qcc::String IFACE_METHOD_PAUSE                = "Pause";
const qcc::String IFACE_METHOD_RESET                = "Reset";
const qcc::String IFACE_DESCRIPTION                 = "Timer";
const qcc::String IFACE_DESCRIPTION_LAGN            = "en";
const qcc::String IFACE_SIG_TIMER_EVENT             = "TimerEvent";
const qcc::String IFACE_SIG_TIMER_EVENT_DESC        = "Timer reached signal";
const qcc::String IFACE_SIG_TIMER_RUNSTATECHANGED   = "RunStateChanged";
const qcc::String OBJ_PATH_PREFIX                   = "/Timer";
const int TIMER_REACHED_TTL_SECONDS                 = 50;

const qcc::String IFACE_FAC_PROP_VERSION            = "Version";
const qcc::String IFACE_FAC_METHOD_NEW_TIMER        = "NewTimer";
const qcc::String IFACE_FAC_METHOD_DELETE_TIMER     = "DeleteTimer";
const qcc::String FAC_OBJ_PATH_PREFIX               = "/TimerFactory";



/**
 * Create Timer interface
 *
 * @param bus BusAttachment
 * @param ptrIfaceDesc InterfaceDescription
 */
QStatus createInterface(BusAttachment* bus, InterfaceDescription** ptrIfaceDesc);

/**
 * Unmarshal Period - used for Timeleft and Interval
 *
 * @param msgArg Contains period to be unmarshalled
 * @param period Out variable. Unmarshalled period
 */
QStatus unmarshalPeriod(const MsgArg&  msgArg, TimeServicePeriod* period);

/**
 * Marshal Period - used for Timeleft and Interval
 *
 * @param msgArg Out variable. MsgArg contains marshalled period.
 * @param period period to be marshalled.
 */
QStatus marshalPeriod(MsgArg& msgArg, const TimeServicePeriod& period);

/**
 * Unmarshal title
 *
 * @param msgArg Contains title to be unmarshalled
 * @param title Out variable. Unmarshalled title
 */
QStatus unmarshalTitle(const MsgArg&  msgArg, qcc::String* title);

/**
 * Marshal Title
 *
 * @param msgArg Out variable. MsgArg contains marshalled title.
 * @param title Title to be marshalled.
 */
QStatus marshalTitle(MsgArg& msgArg, const qcc::String& title);

/**
 * Marshal and Unmarshal TimeLeft property - use un/marhsalPeriod
 */

/**
 * Unmarshal bool property
 *
 * @param msgArg Contains Enabled property to be unmarshalled
 * @param ret Out variable. Unmarshalled Enabled property
 */
QStatus unmarshalBoolean(const MsgArg&  msgArg, bool* ret);

/**
 * Marshal bool property
 *
 * @param msgArg Out variable. MsgArg contains marshalled Enabled property.
 * @param input Enabled property to be marshalled.
 */
QStatus marshalBoolean(MsgArg& msgArg, const bool input);

/**
 * Unmarshal Repeat property
 *
 * @param msgArg Contains Enabled property to be unmarshalled
 * @param repeat Out variable. Unmarshalled Enabled property
 */
QStatus unmarshalRepeat(const MsgArg&  msgArg, uint16_t* repeat);

/**
 * Marshal Repeat property
 *
 * @param msgArg Out variable. MsgArg contains marshalled Enabled property.
 * @param repeat Enabled property to be marshalled.
 */
QStatus marshalRepeat(MsgArg& msgArg, const uint16_t repeat);

/**
 * Create TimerFactory interface
 *
 * @param bus BusAttachment
 * @param ptrIfaceDesc InterfaceDescription
 */
QStatus createFactoryInterface(BusAttachment* bus, InterfaceDescription** ptrIfaceDesc);

/**
 * Marshal object path
 *
 * @param msgArg Out. Contains object path to marshal
 * @param objPath Object path to marshal
 */
QStatus marshalObjectPath(MsgArg& msgArg, const qcc::String& objPath);

/**
 * Unmarshal object path
 *
 * @param msgArg Out. Contains object path to be unmarshalled
 * @param objPath Object path to unmarshal
 */
QStatus unmarshalObjectPath(const MsgArg& msgArg, qcc::String* objPath);

}  /* namespace tsTimerUtility */
}  /* namespace services */
}  /* namespace ajn */

#endif /* TIMESERVICETIMERMUTILITY_H_ */