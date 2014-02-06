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

#ifndef _CONFIGSERVICE_H_
#define _CONFIGSERVICE_H_

/** @defgroup Config Service
 *
 *  @{
 */

#include <alljoyn.h>
#include <alljoyn/services_common/Services_Common.h>

/**
 * published Config Service objects and interfaces
 */
extern const AJ_InterfaceDescription AJSVC_ConfigInterfaces[];

/*
 * Following definitions are read by the application.
 */

#define NUM_CONFIG_OBJECTS 1

#define CONFIG_APPOBJECTS \
    { "/Config",             AJSVC_ConfigInterfaces },

#ifndef CONFIG_ANNOUNCEOBJECTS
#define CONFIG_ANNOUNCEOBJECTS CONFIG_APPOBJECTS
#endif

/**
 * actions to perform when factory reset is requested
 * @return status
 */
typedef AJ_Status (*AJCFG_FactoryReset)(void);

/**
 * actions to perform when a device restart is requested
 * @return status
 */
typedef AJ_Status (*AJCFG_Restart)(void);

/**
 * actions to perform when a new device passcode is set
 * @param daemonRealm
 * @param newStringPasscode
 * @return status
 */
typedef AJ_Status (*AJCFG_SetPasscode)(const char* daemonRealm, const char* newStringPasscode);

/**
 * check whether the given value is valid for the given key
 * @param key
 * @param value
 * @return isValid
 */
typedef uint8_t (*AJCFG_IsValueValid)(const char* key, const char* value);

/*
 * Config Service API
 */

/**
 * Start Config service framework passing callbacks
 * @param factoryReset
 * @param restart
 * @param setPasscode
 * @param isValueValid
 * @return status
 */
AJ_Status AJCFG_Start(AJCFG_FactoryReset factoryReset, AJCFG_Restart restart, AJCFG_SetPasscode setPasscode, AJCFG_IsValueValid isValueValid);

/**
 * on connected service
 * @param bus
 * @return aj_status
 */
AJ_Status AJCFG_ConnectedHandler(AJ_BusAttachment* busAttachment);

/**
 * on processing message
 * @param bus
 * @param msg
 * @return aj_status
 */
AJSVC_ServiceStatus AJCFG_MessageProcessor(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJ_Status* msgStatus);

/**
 * on disconnect service
 * @param bus
 * @return aj_status
 */
AJ_Status AJCFG_DisconnectHandler(AJ_BusAttachment* busAttachment);

/** @} */
 #endif // _CONFIGSERVICE_H_
