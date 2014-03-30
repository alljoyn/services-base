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

#ifndef CONSTRAINTRANGE_H_
#define CONSTRAINTRANGE_H_

/** @defgroup ConstraintRange
 *
 *  @{
 */

#include <alljoyn/controlpanel/Definitions.h>
#include <alljoyn.h>

/**
 * ConstraintRange structure
 */
typedef struct {
    const char* const* signature; //!< The Constraint Range value signature
    const void* minValue;         //!< The Minimum constraint value
    const void* maxValue;         //!< The Maximum constraint value
    const void* increment;        //!< The value increment

} ConstraintRange;

/**
 * Initialize ConstraintRange
 * @param constraints
 */
void initializeConstraintRange(ConstraintRange* constraints);

/**
 * Marshal ConstraintRange into given reply message
 * @param constraints
 * @param reply
 * @return aj_status
 */
AJ_Status marshalConstraintRange(ConstraintRange* constraints, AJ_Message* reply);

/** @} */
#endif /* CONSTRAINTRANGE_H_ */
