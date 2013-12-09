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

#ifndef _CONFIGOEMPROVISIONING_H_
#define _CONFIGOEMPROVISIONING_H_

/** @defgroup ConfigOEMProvisioning
 *
 *  @{
 */

#include <alljoyn.h>

/**
 * actions to perform when factory reset is requested
 * @return status
 */
AJ_Status App_FactoryReset();

/**
 * actions to perform when a device restart is requested
 * @return status
 */
AJ_Status App_Restart();

/**
 * actions to perform when a new device passcode is set
 * @param daemonRealm
 * @param newStringPasscode
 * @return status
 */
AJ_Status App_SetPasscode(const char* daemonRealm, const char* newStringPasscode);

/**
 * check whether the given value is valid for the given key
 * @param key
 * @param value
 * @return isValid
 */
uint8_t App_IsValueValid(const char* key, const char* value);

/** @} */
 #endif /* _CONFIGOEMPROVISIONING_H_ */
