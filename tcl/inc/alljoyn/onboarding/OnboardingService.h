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

#ifndef _ONBOARDINGSERVICE_H_
#define _ONBOARDINGSERVICE_H_

/** @defgroup Onboarding Service
 *
 *  @{
 */

#include <alljoyn/services_common/Services_Common.h>

/**
 * published Onboarding Service objects and interfaces
 */
extern const AJ_InterfaceDescription AJSVC_OnboardingInterfaces[];

/*
 * Following definitions are read by the application.
 */

#define NUM_ONBOARDING_OBJECTS 1

#define ONBOARDING_APPOBJECTS   \
    { "/Onboarding",         AJSVC_OnboardingInterfaces },

#ifndef ONBOARDING_ANNOUNCEOBJECTS
#define ONBOARDING_ANNOUNCEOBJECTS  ONBOARDING_APPOBJECTS
#endif

/*
 * Message identifiers for the method calls this service implements
 */

#define OBS_OBJECT_INDEX                                        NUM_PRE_ONBOARDING_OBJECTS

#define OBS_GET_PROP                                            AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 0, AJ_PROP_GET)
#define OBS_SET_PROP                                            AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 0, AJ_PROP_SET)

#define OBS_VERSION_PROP                                        AJ_APP_PROPERTY_ID(OBS_OBJECT_INDEX, 1, 0)
#define OBS_STATE_PROP                                          AJ_APP_PROPERTY_ID(OBS_OBJECT_INDEX, 1, 1)
#define OBS_LASTERROR_PROP                                      AJ_APP_PROPERTY_ID(OBS_OBJECT_INDEX, 1, 2)
#define OBS_CONFIGURE_WIFI                                      AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 3)
#define OBS_CONNECT                                             AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 4)
#define OBS_OFFBOARD                                            AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 5)
#define OBS_GET_SCAN_INFO                                       AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 6)
#define OBS_CONNECTION_RESULT                                   AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 7)

#define AJOBS_SSID_MAX_LENGTH 32

/*
 * Onboarding Service API
 */

/**
 * handler for property getters associated with org.alljoyn.Onboarding
 * @param replyMsg
 * @param propId
 * @param context
 * @return aj_status
 */
AJ_Status AJOBS_PropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * handler for property setters associated with org.alljoyn.Onboarding
 * @param replyMsg
 * @param propId
 * @param context
 * @return aj_status
 */
AJ_Status AJOBS_PropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * handler for ConfigureWiFi request in org.alljoyn.Onboarding
 * @param msg
 * @return aj_status
 */
AJ_Status AJOBS_ConfigureWiFiHandler(AJ_Message* msg);

/**
 * handler for ConnectWiFi request in org.alljoyn.Onboarding
 * @param msg
 * @return aj_status
 */
AJ_Status AJOBS_ConnectWiFiHandler(AJ_Message* msg);

/**
 * handler for OffboardWiFi request in org.alljoyn.Onboarding
 * @param msg
 * @return aj_status
 */
AJ_Status AJOBS_OffboardWiFiHandler(AJ_Message* msg);

/**
 * handler for GetScanInfo request in org.alljoyn.Onboarding
 * @param msg
 * @return aj_status
 */
AJ_Status AJOBS_GetScanInfoHandler(AJ_Message* msg);

/*
   //will be used in future versions
   AJ_Status OBS_SendConnectionResult(AJ_BusAttachment* bus);
 */

/** @} */
 #endif // _ONBOARDINGSERVICE_H_
