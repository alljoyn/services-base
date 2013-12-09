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

#ifndef BASEWIDGET_H_
#define BASEWIDGET_H_

#include <alljoyn.h>

#ifndef UINT32_MAX
#define UINT32_MAX      (4294967295U)
#endif

/////////////////////////* BaseWidget OptParams *//////////////////////////////////////

typedef struct {
    const char* const* label;
    const char* (*getLabel)(uint16_t);

    uint32_t bgColor;
    uint32_t (*getBgColor)();

    uint16_t numHints;
    const uint16_t* hints;

} BaseOptParams;

void initializeBaseOptParam(BaseOptParams* optParam);

/////////////////////////*     BaseWidget     *//////////////////////////////////////

struct BaseWidget;
typedef AJ_Status (*MarshalWidgetFptr)(struct BaseWidget*, AJ_Message*, uint16_t language);

typedef struct BaseWidget {
    uint16_t interfaceVersion;
    uint32_t states;
    uint8_t (*getEnabled)();
    uint8_t (*getWritable)();

    uint16_t numLanguages;
    BaseOptParams optParams;

    MarshalWidgetFptr marshalVersion;
    MarshalWidgetFptr marshalStates;
    MarshalWidgetFptr marshalOptParam;
    MarshalWidgetFptr marshalAllProp;

} BaseWidget;

void initializeBaseWidget(BaseWidget* widget);

void setBaseEnabled(BaseWidget* widget, uint8_t enabled);

void setBaseWritable(BaseWidget* widget, uint8_t writeable);

AJ_Status marshalBaseVersion(BaseWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status marshalBaseStates(BaseWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status marshalOnlyBaseOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status marshalBaseOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status marshalAllBaseProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status marshalAllOnlyBaseProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language);

#endif /* BASEWIDGET_H_ */
