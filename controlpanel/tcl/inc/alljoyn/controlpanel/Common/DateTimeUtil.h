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

#ifndef DATETIMEUTIL_H_
#define DATETIMEUTIL_H_

/** @defgroup DateTimeUtil Date and Time Utilities
 * details Structs and Functions that assist in Un/Marshaling
 * Date and Time Properties from/to a message.
 *  @{
 */

#include <alljoyn.h>

/**
 * DatePropertyValue structure
 */
typedef struct {
    uint16_t mDay;     //!< The day of the month value i.e. 1 for 1st of the month
    uint16_t month;    //!< The month value i.e. 3 for March
    uint16_t fullYear; //!< The full year value i.e. 2014 and not 14
} DatePropertyValue;

/**
 * TimePropertyValue structure
 */
typedef struct {
    uint16_t hour;   //!< The hour value as 24 hours
    uint16_t minute; //!< The minute value
    uint16_t second; //!< The second valur
} TimePropertyValue;

/**
 * Marshal DatePropertyValue into given reply message
 * @param datePropertyValue
 * @param reply
 * @return aj_status
 */
AJ_Status marshalDatePropertyValue(DatePropertyValue* datePropertyValue, AJ_Message* reply);

/**
 * Marshal TimePropertyValue into given reply message
 * @param timePropertyValue
 * @param reply
 * @return aj_status
 */
AJ_Status marshalTimePropertyValue(TimePropertyValue* timePropertyValue, AJ_Message* reply);

/**
 * Unmarshal DatePropertyValue from given message
 * @param datePropertyValue
 * @param message
 * @return aj_status
 */
AJ_Status unmarshalDatePropertyValue(DatePropertyValue* datePropertyValue, AJ_Message* message);

/**
 * Unmarshal TimePropertyValue from given message
 * @param timePropertyValue
 * @param message
 * @return aj_status
 */
AJ_Status unmarshalTimePropertyValue(TimePropertyValue* timePropertyValue, AJ_Message* message);

/** @} */
#endif /* DATETIMEUTIL_H_ */
