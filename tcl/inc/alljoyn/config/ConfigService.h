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

#ifndef _CONFIGSERVICE_H_
#define _CONFIGSERVICE_H_

/** @defgroup Config Service
 *
 *  @{
 */

#include <alljoyn/services_common/PropertyStore.h>
#include <alljoyn/config/ConfigOEMProvisioning.h>
#include <alljoyn/services_common/Services_Common.h>

/*
 * Following definitions are read by the application.
 */

#define NUM_CONFIG_OBJECTS 1

#define CONFIG_APPOBJECTS \
    { "/Config",             ConfigInterfaces },

#ifndef CONFIG_ANNOUNCEOBJECTS
#define CONFIG_ANNOUNCEOBJECTS CONFIG_APPOBJECTS
#endif

/*
 * Message identifiers for the method calls this service implements
 */

#define CONFIG_OBJECT_INDEX                                     NUM_PRE_CONFIG_OBJECTS

#define CONFIG_GET_PROP                                         AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 0, AJ_PROP_GET)
#define CONFIG_SET_PROP                                         AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 0, AJ_PROP_SET)

#define CONFIG_VERSION_PROP                                     AJ_APP_PROPERTY_ID(CONFIG_OBJECT_INDEX, 1, 0)
#define CONFIG_FACTORY_RESET                                    AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 1)
#define CONFIG_RESTART                                          AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 2)
#define CONFIG_GET_CONFIG_CONFIGURATIONS                        AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 3)
#define CONFIG_UPDATE_CONFIGURATIONS                            AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 4)
#define CONFIG_RESET_CONFIGURATIONS                             AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 5)
#define CONFIG_SET_PASSCODE                                     AJ_APP_MESSAGE_ID(CONFIG_OBJECT_INDEX, 1, 6)

/*
 * Config Service API
 */

/**
 * published Config Service objects and interfaces
 */
extern const AJ_InterfaceDescription ConfigInterfaces[];

/**
 * handler for property getters associated with org.alljoyn.Config
 * @param replyMsg
 * @param propId
 * @param context
 * @return aj_status
 */
AJ_Status ConfigPropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * handler for property setters associated with org.alljoyn.Config
 * @param replyMsg
 * @param propId
 * @param context
 * @return aj_status
 */
AJ_Status ConfigPropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * handler for FactoryReset request in org.alljoyn.Config
 * @param msg
 * @return aj_status
 */
AJ_Status ConfigFactoryReset(AJ_Message* msg);

/**
 * handler for Restart request in org.alljoyn.Config
 * @param msg
 * @return aj_status
 */
AJ_Status ConfigRestart(AJ_Message* msg);

/**
 * handler for GetConfigruations request in org.alljoyn.Config
 * @param msg
 * @return aj_status
 */
AJ_Status ConfigGetConfigurations(AJ_Message* msg);

/**
 * handler for UpdateConfigurations request in org.alljoyn.Config
 * @param msg
 * @return aj_status
 */
AJ_Status ConfigUpdateConfigurations(AJ_Message* msg);

/**
 * handler for ResetConfigurations request in org.alljoyn.Config
 * @param msg
 * @return aj_status
 */
AJ_Status ConfigResetConfigurations(AJ_Message* msg);

/**
 * handler for SetPasscode request in org.alljoyn.Config
 * @param msg
 * @return aj_status
 */
AJ_Status ConfigSetPasscode(AJ_Message* msg);

/** @} */
 #endif // _CONFIGSERVICE_H_
