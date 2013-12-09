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

#ifndef CONTROL_GENERATED_H_
#define CONTROL_GENERATED_H_

#include <alljoyn.h>
#include <alljoyn/controlpanel/Widgets/ActionWidget.h>
#include <alljoyn/controlpanel/Widgets/PropertyWidget.h>
#include <alljoyn/controlpanel/Widgets/ContainerWidget.h>
#include <alljoyn/controlpanel/Widgets/LabelWidget.h>
#include <alljoyn/controlpanel/Widgets/DialogWidget.h>
#include <alljoyn/controlpanel/Common/HttpControl.h>

/**
 * Defines and objectPaths
 */
#define NUM_PRECEDING_OBJECTS NUM_PRE_CONTROLPANEL_OBJECTS

#define UNIQUE_DEVICE_ID "MyOven"

#define GET_WIDGET_VALUE_CASES \
case EN_OVENCONTROLLER_GET_VALUE: \
case EN_PROGRAM_GET_VALUE: \
case EN_TEMPERATURE_GET_VALUE: \
case EN_START_GET_VALUE: \
case EN_STOP_GET_VALUE: \


#define GET_WIDGET_ALL_VALUE_CASES \
case EN_OVENCONTROLLER_GET_ALL_VALUES: \
case EN_PROGRAM_GET_ALL_VALUES: \
case EN_TEMPERATURE_GET_ALL_VALUES: \
case EN_START_GET_ALL_VALUES: \
case EN_STOP_GET_ALL_VALUES: \


#define GET_ROOT_VALUE_CASES \
case HTTP_CONTROL_GET_VALUE: \
case ROOT_CONTROLPANEL_OVENCONTROLLER_GET_VALUE: \


#define GET_ROOT_ALL_VALUE_CASES \
case HTTP_CONTROL_GET_ALL_VALUES: \
case ROOT_CONTROLPANEL_OVENCONTROLLER_GET_ALL_VALUES: \


#define SET_VALUE_CASES \
case HTTP_CONTROL_SET_VALUE: \
case ROOT_CONTROLPANEL_OVENCONTROLLER_SET_VALUE: \
case EN_OVENCONTROLLER_SET_VALUE: \
case EN_PROGRAM_SET_VALUE: \
case EN_TEMPERATURE_SET_VALUE: \
case EN_START_SET_VALUE: \
case EN_STOP_SET_VALUE: \


#define ACTION_CASES \
case EN_START_EXEC: \
case EN_STOP_EXEC: \


#define GET_URL_CASES \
case HTTP_CONTROL_GET_ROOT_URL: \


#define CONTROLPANELAPPOBJECTS     {  httpControlObjectPath, HttpControlInterfaces  }, \
    {  OvenControllerObjectPath, ControlPanelInterfaces  }, \
    {  enOvencontrollerObjectPath, ContainerInterfaces  }, \
    {  enProgramObjectPath, PropertyInterfaces  }, \
    {  enTemperatureObjectPath, PropertyInterfaces  }, \
    {  enStartObjectPath, ActionInterfaces  }, \
    {  enStopObjectPath, ActionInterfaces  }, \


#define CONTROLPANELANNOUNCEOBJECTS     {  httpControlObjectPath, HttpControlInterfaces  }, \
    {  OvenControllerObjectPath, ControlPanelInterfaces  }, \


#define MAX_NUM_LANGUAGES 1

#define NUM_CONTROLPANEL_OBJECTS 7

#define MYLANGUAGES_EN 0

#define HTTP_CONTROL_GET_VALUE             AJ_PRX_MESSAGE_ID(0 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define HTTP_CONTROL_SET_VALUE             AJ_PRX_MESSAGE_ID(0 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define HTTP_CONTROL_GET_ALL_VALUES        AJ_PRX_MESSAGE_ID(0 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define HTTP_CONTROL_VERSION_PROPERTY      AJ_PRX_PROPERTY_ID(0 + NUM_PRECEDING_OBJECTS, 1, 0)
#define HTTP_CONTROL_GET_ROOT_URL          AJ_PRX_MESSAGE_ID(0 + NUM_PRECEDING_OBJECTS, 1, 1)

#define ROOT_CONTROLPANEL_OVENCONTROLLER_GET_VALUE                  AJ_PRX_MESSAGE_ID(1 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define ROOT_CONTROLPANEL_OVENCONTROLLER_SET_VALUE                  AJ_PRX_MESSAGE_ID(1 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define ROOT_CONTROLPANEL_OVENCONTROLLER_GET_ALL_VALUES             AJ_PRX_MESSAGE_ID(1 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define ROOT_CONTROLPANEL_OVENCONTROLLER_VERSION_PROPERTY           AJ_PRX_PROPERTY_ID(1 + NUM_PRECEDING_OBJECTS, 1, 0)

#define EN_OVENCONTROLLER_GET_VALUE                  AJ_PRX_MESSAGE_ID(2 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define EN_OVENCONTROLLER_SET_VALUE                  AJ_PRX_MESSAGE_ID(2 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define EN_OVENCONTROLLER_GET_ALL_VALUES             AJ_PRX_MESSAGE_ID(2 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define EN_OVENCONTROLLER_VERSION_PROPERTY           AJ_PRX_PROPERTY_ID(2 + NUM_PRECEDING_OBJECTS, 1, 0)
#define EN_OVENCONTROLLER_STATES_PROPERTY            AJ_PRX_PROPERTY_ID(2 + NUM_PRECEDING_OBJECTS, 1, 1)
#define EN_OVENCONTROLLER_OPTPARAMS_PROPERTY         AJ_PRX_PROPERTY_ID(2 + NUM_PRECEDING_OBJECTS, 1, 2)
#define EN_OVENCONTROLLER_SIGNAL_PROPERTIES_CHANGED  AJ_PRX_MESSAGE_ID(2 + NUM_PRECEDING_OBJECTS, 1, 3)

#define EN_PROGRAM_GET_VALUE                  AJ_PRX_MESSAGE_ID(3 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define EN_PROGRAM_SET_VALUE                  AJ_PRX_MESSAGE_ID(3 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define EN_PROGRAM_GET_ALL_VALUES             AJ_PRX_MESSAGE_ID(3 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define EN_PROGRAM_VERSION_PROPERTY           AJ_PRX_PROPERTY_ID(3 + NUM_PRECEDING_OBJECTS, 1, 0)
#define EN_PROGRAM_STATES_PROPERTY            AJ_PRX_PROPERTY_ID(3 + NUM_PRECEDING_OBJECTS, 1, 1)
#define EN_PROGRAM_OPTPARAMS_PROPERTY         AJ_PRX_PROPERTY_ID(3 + NUM_PRECEDING_OBJECTS, 1, 2)
#define EN_PROGRAM_SIGNAL_PROPERTIES_CHANGED  AJ_PRX_MESSAGE_ID(3 + NUM_PRECEDING_OBJECTS, 1, 3)
#define EN_PROGRAM_VALUE_PROPERTY             AJ_PRX_PROPERTY_ID(3 + NUM_PRECEDING_OBJECTS, 1, 4)
#define EN_PROGRAM_SIGNAL_VALUE_CHANGED       AJ_PRX_MESSAGE_ID(3 + NUM_PRECEDING_OBJECTS, 1, 5)

#define EN_TEMPERATURE_GET_VALUE                  AJ_PRX_MESSAGE_ID(4 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define EN_TEMPERATURE_SET_VALUE                  AJ_PRX_MESSAGE_ID(4 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define EN_TEMPERATURE_GET_ALL_VALUES             AJ_PRX_MESSAGE_ID(4 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define EN_TEMPERATURE_VERSION_PROPERTY           AJ_PRX_PROPERTY_ID(4 + NUM_PRECEDING_OBJECTS, 1, 0)
#define EN_TEMPERATURE_STATES_PROPERTY            AJ_PRX_PROPERTY_ID(4 + NUM_PRECEDING_OBJECTS, 1, 1)
#define EN_TEMPERATURE_OPTPARAMS_PROPERTY         AJ_PRX_PROPERTY_ID(4 + NUM_PRECEDING_OBJECTS, 1, 2)
#define EN_TEMPERATURE_SIGNAL_PROPERTIES_CHANGED  AJ_PRX_MESSAGE_ID(4 + NUM_PRECEDING_OBJECTS, 1, 3)
#define EN_TEMPERATURE_VALUE_PROPERTY             AJ_PRX_PROPERTY_ID(4 + NUM_PRECEDING_OBJECTS, 1, 4)
#define EN_TEMPERATURE_SIGNAL_VALUE_CHANGED       AJ_PRX_MESSAGE_ID(4 + NUM_PRECEDING_OBJECTS, 1, 5)

#define EN_START_GET_VALUE                  AJ_PRX_MESSAGE_ID(5 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define EN_START_SET_VALUE                  AJ_PRX_MESSAGE_ID(5 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define EN_START_GET_ALL_VALUES             AJ_PRX_MESSAGE_ID(5 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define EN_START_VERSION_PROPERTY           AJ_PRX_PROPERTY_ID(5 + NUM_PRECEDING_OBJECTS, 1, 0)
#define EN_START_STATES_PROPERTY            AJ_PRX_PROPERTY_ID(5 + NUM_PRECEDING_OBJECTS, 1, 1)
#define EN_START_OPTPARAMS_PROPERTY         AJ_PRX_PROPERTY_ID(5 + NUM_PRECEDING_OBJECTS, 1, 2)
#define EN_START_SIGNAL_PROPERTIES_CHANGED  AJ_PRX_MESSAGE_ID(5 + NUM_PRECEDING_OBJECTS, 1, 3)
#define EN_START_EXEC                       AJ_PRX_MESSAGE_ID(5 + NUM_PRECEDING_OBJECTS, 1, 4)

#define EN_STOP_GET_VALUE                  AJ_PRX_MESSAGE_ID(6 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define EN_STOP_SET_VALUE                  AJ_PRX_MESSAGE_ID(6 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define EN_STOP_GET_ALL_VALUES             AJ_PRX_MESSAGE_ID(6 + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define EN_STOP_VERSION_PROPERTY           AJ_PRX_PROPERTY_ID(6 + NUM_PRECEDING_OBJECTS, 1, 0)
#define EN_STOP_STATES_PROPERTY            AJ_PRX_PROPERTY_ID(6 + NUM_PRECEDING_OBJECTS, 1, 1)
#define EN_STOP_OPTPARAMS_PROPERTY         AJ_PRX_PROPERTY_ID(6 + NUM_PRECEDING_OBJECTS, 1, 2)
#define EN_STOP_SIGNAL_PROPERTIES_CHANGED  AJ_PRX_MESSAGE_ID(6 + NUM_PRECEDING_OBJECTS, 1, 3)
#define EN_STOP_EXEC                       AJ_PRX_MESSAGE_ID(6 + NUM_PRECEDING_OBJECTS, 1, 4)



extern const char httpControlObjectPath[];
extern const char OvenControllerObjectPath[];
extern const char enOvencontrollerObjectPath[];
extern const char enProgramObjectPath[];
extern const char enTemperatureObjectPath[];
extern const char enStartObjectPath[];
extern const char enStopObjectPath[];


//WIDGETS_DECL_GO_HERE

typedef struct {
    const char* sender;
    uint16_t numSignals;
    int32_t signals[MAX_NUM_LANGUAGES];
} SetValueContext;

typedef struct {
    uint8_t signalType;
    int32_t signalId;
} Signal;

typedef struct {
    uint16_t numSignals;
    Signal signals[MAX_NUM_LANGUAGES * 2];
} ExecuteActionContext;
/**
 *
 * @return
 */
void WidgetsInit();

/**
 * Set Value of a property.
 * @param replyMsg - reply message
 * @param propId - id of property being changed
 * @param context - setvaluecontext. can be used to send signals
 * @return status
 */
AJ_Status SetValueProperty(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * Execute Action
 * @param msg - the msg for the response
 * @param msgId - the Action being executed
 * @param context - ExecuteActionContext used to send signals as a result of action
 * @return status
 */
AJ_Status ExecuteAction(AJ_Message* msg, uint32_t msgId, ExecuteActionContext* context);


/* Defines and functions for Tester App */
typedef struct {
    uint32_t msgId;
    uint16_t numParams;
    uint16_t param[3];
} CPSTest;

#define NUMBER_OF_TESTS 8

#define ALL_REPLY_CASES                case AJ_REPLY_ID(HTTP_CONTROL_GET_ROOT_URL): \
case AJ_REPLY_ID(EN_OVENCONTROLLER_GET_ALL_VALUES): \
case AJ_REPLY_ID(EN_PROGRAM_GET_ALL_VALUES): \
case AJ_REPLY_ID(EN_TEMPERATURE_GET_ALL_VALUES): \
case AJ_REPLY_ID(EN_START_GET_ALL_VALUES): \
case AJ_REPLY_ID(EN_START_EXEC): \
case AJ_REPLY_ID(EN_STOP_GET_ALL_VALUES): \
case AJ_REPLY_ID(EN_STOP_EXEC): \


void TestsInit(CPSTest* testsToRun);

#endif /* CONTROL_GENERATED_H_ */
