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
//INCLUDES_GO_HERE

//OBJECTPATH_DEF_GO_HERE
//WIDGETS_DEF_GO_HERE
/*
 * Static variables used to fill widgets
 */
//STATICS_GO_HERE

void WidgetsInit()
{
//INITROOT_GO_HERE
    return;
}

void* identifyMsgOrPropId(uint32_t identifier, uint16_t* widgetType, uint16_t* propType, uint16_t* language)
{
    switch (identifier) {
//ADD_MAIN_CASE_HERE    default:
        return FALSE;
    }
}

void* identifyMsgOrPropIdForSignal(uint32_t identifier, uint8_t* isProperty)
{
    switch (identifier) {
//ADD_SIGNAL_CASE_HERE    default:
        return FALSE;
    }
}

uint8_t identifyRootMsgOrPropId(uint32_t identifier)
{
    switch (identifier) {
//ADD_ROOT_CASE_HERE    default:
        return FALSE;
    }
}

AJ_Status SetValueProperty(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    const char* variantSig;
    AJ_UnmarshalVariant(replyMsg, &variantSig);

    switch (propId) {
//SET_WIDGET_PROPERTY_GO_HERE
    }

    return status;
}

AJ_Status ExecuteAction(AJ_Message* msg, uint32_t msgId, ExecuteActionContext* context)
{
    AJ_Message reply;
    AJ_MarshalReplyMsg(msg, &reply);

    switch (msgId) {
//EXECUTE_ACTION_GO_HERE
    }

    return AJ_DeliverMsg(&reply);
}

void TestsInit(CPSTest* testsToRun)
{
//INITTESTS_GO_HERE
}
