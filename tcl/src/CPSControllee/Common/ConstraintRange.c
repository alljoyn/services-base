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

#include <alljoyn/controlpanel/Common/ConstraintRange.h>
#include <alljoyn/controlpanel/Common/ControlMarshalUtil.h>

void initializeConstraintRange(ConstraintRange* constraints)
{
    constraints->minValue = NULL;
    constraints->maxValue = NULL;
    constraints->increment = NULL;
}

AJ_Status marshalConstraintRange(ConstraintRange* constraints, AJ_Message* reply)
{
    AJ_Status status;
    AJ_Arg constraintRange;

    status = StartComplexOptionalParam(reply, &constraintRange, PROPERTY_CONSTRAINT_RANGE, PROPERTY_CONSTRAINT_RANGE_SIG);
    if (status != AJ_OK) {
        return status;
    }

    status = AddConstraintRange(reply, *constraints->signature, constraints->minValue, constraints->maxValue, constraints->increment);
    if (status != AJ_OK) {
        return status;
    }

    return AJ_MarshalCloseContainer(reply, &constraintRange);
}
