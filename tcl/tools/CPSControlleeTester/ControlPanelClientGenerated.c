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

#include <alljoyn/controlpanel/Common/ConstraintList.h>
#include <alljoyn/controlpanel/ControlPanelService.h>


const char httpControlObjectPath[] = "/ControlPanel/MyOven/HTTPControl";
const char OvenControllerObjectPath[] = "/ControlPanel/MyOven/OvenController";
const char enOvencontrollerObjectPath[] = "/ControlPanel/MyOven/OvenController/en";
const char enProgramObjectPath[] = "/ControlPanel/MyOven/OvenController/en/program";
const char enTemperatureObjectPath[] = "/ControlPanel/MyOven/OvenController/en/temperature";
const char enStartObjectPath[] = "/ControlPanel/MyOven/OvenController/en/start";
const char enStopObjectPath[] = "/ControlPanel/MyOven/OvenController/en/stop";


/*
 * Static variables used to fill widgets
 */


void WidgetsInit()
{

    return;
}

void* identifyMsgOrPropId(uint32_t identifier, uint16_t* widgetType, uint16_t* propType, uint16_t* language)
{
    switch (identifier) {
    default:
        return FALSE;
    }
}

void* identifyMsgOrPropIdForSignal(uint32_t identifier, uint8_t* isProperty)
{
    switch (identifier) {
    default:
        return FALSE;
    }
}

uint8_t identifyRootMsgOrPropId(uint32_t identifier)
{
    switch (identifier) {
    default:
        return FALSE;
    }
}

AJ_Status SetValueProperty(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    const char* variantSig;
    AJ_UnmarshalVariant(replyMsg, &variantSig);

    switch (propId) {

    }

    return status;
}

AJ_Status ExecuteAction(AJ_Message* msg, uint32_t msgId, ExecuteActionContext* context)
{
    AJ_Message reply;
    AJ_MarshalReplyMsg(msg, &reply);

    switch (msgId) {

    }

    return AJ_DeliverMsg(&reply);
}

void TestsInit(CPSTest* testsToRun)
{
    testsToRun[0].msgId = HTTP_CONTROL_GET_ROOT_URL;
    testsToRun[0].numParams = 0;
    testsToRun[1].msgId = EN_OVENCONTROLLER_GET_ALL_VALUES;
    testsToRun[1].numParams = 0;
    testsToRun[2].msgId = EN_PROGRAM_GET_ALL_VALUES;
    testsToRun[2].numParams = 0;
    testsToRun[3].msgId = EN_TEMPERATURE_GET_ALL_VALUES;
    testsToRun[3].numParams = 0;
    testsToRun[4].msgId = EN_START_GET_ALL_VALUES;
    testsToRun[4].numParams = 0;
    testsToRun[5].msgId = EN_START_EXEC;
    testsToRun[5].numParams = 0;
    testsToRun[6].msgId = EN_STOP_GET_ALL_VALUES;
    testsToRun[6].numParams = 0;
    testsToRun[7].msgId = EN_STOP_EXEC;
    testsToRun[7].numParams = 0;

}
