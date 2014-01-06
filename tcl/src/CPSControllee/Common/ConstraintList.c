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

#include <alljoyn/controlpanel/Common/ConstraintList.h>
#include <alljoyn/controlpanel/Common/ControlMarshalUtil.h>

AJ_Status marshalConstraintList(ConstraintList* constraints, AJ_Message* reply, uint16_t numConstraints,
                                const char* signature, uint16_t language)
{
    AJ_Status status;
    AJ_Arg arrayArg, opParams;

    if (status = StartComplexOptionalParam(reply, &opParams, PROPERTY_CONSTRAINT_LIST, PROPERTY_CONSTRAINT_LIST_SIG) != AJ_OK)
        return status;

    if (status = AJ_MarshalContainer(reply, &arrayArg, AJ_ARG_ARRAY) != AJ_OK)
        return status;

    uint16_t cnt;
    for (cnt = 0; cnt < numConstraints; cnt++) {
        if (constraints[cnt].getDisplay != 0) {
            if (status = AddConstraintValue(reply, signature, constraints[cnt].value, constraints[cnt].getDisplay(language)) != AJ_OK)
                return status;
        } else {
            if (status = AddConstraintValue(reply, signature, constraints[cnt].value, constraints[cnt].display[language]) != AJ_OK)
                return status;
        }
    }

    if (status = AJ_MarshalCloseContainer(reply, &arrayArg) != AJ_OK)
        return status;

    return AJ_MarshalCloseContainer(reply, &opParams);
}
