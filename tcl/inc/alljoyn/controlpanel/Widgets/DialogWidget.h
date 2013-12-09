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

#ifndef DIALOGWIDGET_H_
#define DIALOGWIDGET_H_

#include <alljoyn/controlpanel/Common/BaseWidget.h>

/////////////////////////* DialogWidget OptParams *//////////////////////////////////////

typedef struct {
    const char* const* labelAction1;
    const char* (*getLabelAction1)(uint16_t);
    const char* const* labelAction2;
    const char* (*getLabelAction2)(uint16_t);
    const char* const* labelAction3;
    const char* (*getLabelAction3)(uint16_t);

} DialogOptParams;

void initializeDialogOptParam(DialogOptParams* optParam);

/////////////////////////*     DialogWidget     *//////////////////////////////////////

typedef struct {
    BaseWidget base;
    const char* const* message;
    const char* (*getMessage)(uint16_t);
    uint16_t numActions;

    DialogOptParams optParams;

} DialogWidget;

void initializeDialogWidget(DialogWidget* widget);

AJ_Status marshalDialogMessage(DialogWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status marshalDialogNumActions(DialogWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status marshalDialogOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status marshalAllDialogProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language);

#endif /* DIALOGWIDGET_H_ */

