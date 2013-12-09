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

#ifndef _APPS_COMMON_H_
#define _APPS_COMMON_H_

#include <alljoyn.h>

/*
 * Define timeout/pause values. Values are in milli seconds.
 * The following may be tuned according to platform requirements such as battery usage.
 */
#ifndef AJ_CONNECT_TIMEOUT
#define AJ_CONNECT_TIMEOUT     (1000 * 60)
#endif

#ifndef AJ_CONNECT_PAUSE
#define AJ_CONNECT_PAUSE       (1000 * 2)
#endif

#ifndef AJ_UNMARSHAL_TIMEOUT
#define AJ_UNMARSHAL_TIMEOUT   (1000 * 1)
#endif

#ifndef AJ_SLEEP_TIME
#define AJ_SLEEP_TIME          (1000 * 2)
#endif

/**
 * Service Status is an enum that signals whether a call was handled
 * or not handled within a service_message_processor function
 */
typedef enum {
    SERVICE_STATUS_HANDLED,       //!< SERVICE_STATUS_HANDLED
    SERVICE_STATUS_NOT_HANDLED    //!< SERVICE_STATUS_NOT_HANDLED
} Service_Status;

/**
 * AppObjects that are registered on the Bus
 */
extern AJ_Object AppObjects[];

/**
 * ProxyObjects that are registered on the Bus
 */
extern AJ_Object ProxyObjects[];

/**
 * "boolean" that reflects whether the application requires an addMatch
 * Can be set to TRUE or FALSE - should default to FALSE
 */
extern uint8_t addSessionLessMatch;

#endif /* _APPS_COMMON_H_ */
