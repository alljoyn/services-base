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

/** @defgroup OnboardingService Onboarding Service Framework
 *
 *  @{
 */

#include <alljoyn.h>
#include <alljoyn/services_common/ServicesCommon.h>

/**
 * Published Onboarding BusObjects and Interfaces.
 */
extern const AJ_InterfaceDescription AJSVC_OnboardingInterfaces[];

/*
 * Following definitions are read by the application.
 */

#define NUM_ONBOARDING_OBJECTS 1                                                    /**< number of onboarding objects */

#define ONBOARDING_APPOBJECTS   \
    { "/Onboarding",           AJSVC_OnboardingInterfaces, AJ_OBJ_FLAG_ANNOUNCED }, /**< onboarding interfaces */

/*
 * Message identifiers for the method calls this service implements
 */

#define OBS_OBJECT_INDEX       NUM_PRE_ONBOARDING_OBJECTS                           /**< number of pre onboarding objects */

#define OBS_GET_PROP           AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 0, AJ_PROP_GET)  /**< property get */
#define OBS_SET_PROP           AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 0, AJ_PROP_SET)  /**< property set */

#define OBS_VERSION_PROP       AJ_APP_PROPERTY_ID(OBS_OBJECT_INDEX, 1, 0)           /**< version property index */
#define OBS_STATE_PROP         AJ_APP_PROPERTY_ID(OBS_OBJECT_INDEX, 1, 1)           /**< state property */
#define OBS_LASTERROR_PROP     AJ_APP_PROPERTY_ID(OBS_OBJECT_INDEX, 1, 2)           /**< last error property */
#define OBS_CONFIGURE_WIFI     AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 3)            /**< configure wifi */
#define OBS_CONNECT            AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 4)            /**< connect */
#define OBS_OFFBOARD           AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 5)            /**< offboard */
#define OBS_GET_SCAN_INFO      AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 6)            /**< get scan info */
#define OBS_CONNECTION_RESULT  AJ_APP_MESSAGE_ID(OBS_OBJECT_INDEX, 1, 7)            /**< connection result */

#define AJOBS_SSID_MAX_LENGTH  32                                                   /**< ssid max length */

/*
 * Onboarding Service Message Handlers
 */

/**
 * Handler for property getters associated with org.alljoyn.Onboarding.
 * @param replyMsg
 * @param propId
 * @param context
 * @return aj_status
 */
AJ_Status AJOBS_PropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * Handler for property setters associated with org.alljoyn.Onboarding.
 * @param replyMsg
 * @param propId
 * @param context
 * @return aj_status
 */
AJ_Status AJOBS_PropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * Handler for ConfigureWiFi request in org.alljoyn.Onboarding.
 * @param msg
 * @return aj_status
 */
AJ_Status AJOBS_ConfigureWiFiHandler(AJ_Message* msg);

/**
 * Handler for ConnectWiFi request in org.alljoyn.Onboarding.
 * @param msg
 * @return aj_status
 */
AJ_Status AJOBS_ConnectWiFiHandler(AJ_Message* msg);

/**
 * Handler for OffboardWiFi request in org.alljoyn.Onboarding.
 * @param msg
 * @return aj_status
 */
AJ_Status AJOBS_OffboardWiFiHandler(AJ_Message* msg);

/**
 * Handler for GetScanInfo request in org.alljoyn.Onboarding.
 * @param msg
 * @return aj_status
 */
AJ_Status AJOBS_GetScanInfoHandler(AJ_Message* msg);

/*
   //will be used in future versions
   AJ_Status AJOBS_SendConnectionResult(AJ_BusAttachment* bus);
 */

/**
 * Called when router is connected.
 * @param busAttachment
 * @return aj_status
 */
AJ_Status AJOBS_ConnectedHandler(AJ_BusAttachment* busAttachment);

/**
 * Called just before the router disconnects.
 * @param busAttachment
 * @return aj_status
 */
AJ_Status AJOBS_DisconnectHandler(AJ_BusAttachment* busAttachment);

/**
 * Called when a new incoming message requires processing.
 * @param busAttachment
 * @param msg
 * @param msgStatus
 * @return service_Status
 */
AJSVC_ServiceStatus AJOBS_MessageProcessor(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJ_Status* msgStatus);

/** @} */ //End of group 'OnboardingService'
 #endif // _ONBOARDINGSERVICE_H_
