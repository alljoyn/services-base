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

#ifndef CONTROLPANELPROPERTYTYPE_H_
#define CONTROLPANELPROPERTYTYPE_H_

#include <qcc/String.h>

namespace ajn {
namespace services {

/**
 * Enum to define the type of Property
 */
enum PropertyType {
    BOOL_PROPERTY = 0,  //!< BOOL_PROPERTY
    UINT16_PROPERTY = 1, //!< UINT16_PROPERTY
    INT16_PROPERTY = 2, //!< INT16_PROPERTY
    UINT32_PROPERTY = 3, //!< UINT32_PROPERTY
    INT32_PROPERTY = 4, //!< INT32_PROPERTY
    UINT64_PROPERTY = 5, //!< UINT64_PROPERTY
    INT64_PROPERTY = 6, //!< INT64_PROPERTY
    DOUBLE_PROPERTY = 7, //!< DOUBLE_PROPERTY
    STRING_PROPERTY = 8, //!< STRING_PROPERTY
    DATE_PROPERTY = 9,  //!< DATE_PROPERTY
    TIME_PROPERTY = 10, //!< TIME_PROPERTY
    UNDEFINED = 11      //!< UNDEFINED
};

} //namespace services
} //namespace ajn

#endif /* CONTROLPANELPROPERTYTYPE_H_ */
