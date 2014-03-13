/******************************************************************************
 * Copyright (c) 2013 - 2014, AllSeen Alliance. All rights reserved.
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

#ifndef PROPERTYWIDGET_H_
#define PROPERTYWIDGET_H_

#include <alljoyn/controlpanel/Common/BaseWidget.h>
#include <alljoyn/controlpanel/Common/ConstraintList.h>
#include <alljoyn/controlpanel/Common/ConstraintRange.h>

typedef enum {
    SINGLE_VALUE_PROPERTY = 0,
    DATE_VALUE_PROPERTY = 1,
    TIME_VALUE_PROPERTY = 2,
} PropertyType;
/////////////////////////* PropertyWidget OptParams *//////////////////////////////////////

typedef struct {
    const char* const* unitOfMeasure;
    const char* (*getUnitOfMeasure)(uint16_t);

    ConstraintList* constraintList;
    uint16_t numConstraints;

    ConstraintRange constraintRange;
    uint8_t constraintRangeDefined;

} PropertyOptParams;

void initializePropertyOptParam(PropertyOptParams* widget);

/////////////////////////*     PropertyWidget     *//////////////////////////////////////

typedef struct PropertyWidget {
    BaseWidget base;
    PropertyType propertyType;

    const char* signature;
    void* (*getValue)(struct PropertyWidget* thisWidget);

    PropertyOptParams optParams;

} PropertyWidget;

void initializePropertyWidget(PropertyWidget* widget);

AJ_Status marshalPropertyValue(PropertyWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status unmarshalPropertyValue(PropertyWidget* widget, AJ_Message* message, void* newValue, const char* lockerId);

AJ_Status marshalPropertyOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language);

AJ_Status marshalAllPropertyProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language);

#endif /* PROPERTYWIDGET_H_ */
