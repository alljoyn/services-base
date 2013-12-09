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

#ifndef WIDGETENUMS_H_
#define WIDGETENUMS_H_

namespace ajn {
namespace services {

#ifndef UINT32_MAX
#define UINT32_MAX      (4294967295U)
#endif

/*
 * Typedefs for functionPointers used
 */
typedef bool (*GetBoolFptr)();
typedef uint32_t (*GetUint32Fptr)();
typedef const char* (*GetStringFptr)(uint16_t);

/**
 * Mode Widget is in
 */
typedef enum {
    CONTROLLEE_WIDGET, //!< CONTROLLEE_WIDGET
    CONTROLLER_WIDGET //!< CONTROLLER_WIDGET
} WidgetMode;

typedef enum {
    CONTAINER = 0,
    ACTION = 1,
    ACTION_WITH_DIALOG = 2,
    LABEL = 3,
    PROPERTY = 4,
    DIALOG = 5
} WidgetType;

} //namespace services
} //namespace ajn

#endif /* WIDGETENUMS_H_ */


