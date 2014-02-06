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

#ifndef CONTROLPANELRESPONSES_H_
#define CONTROLPANELRESPONSES_H_

#include <alljoyn/controlpanel/ControlPanelInterfaces.h>
#include <alljoyn/controlpanel/Definitions.h>

#ifdef CONTROLPANEL_SERVICE
#include <alljoyn/services_common/Services_Common.h>
#else
#define NUM_PRE_CONTROLPANEL_OBJECTS 0
#include "ControlPanelClientGenerated.h"
#endif

/**
 * Send Signal if a property of a widget has changed.
 * @param busAttachment
 * @param propSignal
 * @param sessionId
 * @return status
 */
AJ_Status AJCPS_SendPropertyChangedSignal(AJ_BusAttachment* busAttachment, uint32_t propSignal, uint32_t sessionId);

/**
 * Send a signal to dismiss the displayed NotificationAction
 * @param busAttachment
 * @param propSignal
 * @param sessionId
 * @return status
 */
AJ_Status AJCPS_SendDismissSignal(AJ_BusAttachment* busAttachment, uint32_t propSignal, uint32_t sessionId);

/**
 * Function used to identify what kind of request we're dealing with. Defined in Generated code.
 * @param identifier
 * @param widgetType
 * @param propType
 * @param language
 * @return widget
 */
typedef void* (*AJCPS_IdentifyMsgOrPropId)(uint32_t identifier, uint16_t* widgetType, uint16_t* propType, uint16_t* language);

/**
 * Function used to identify what kind of signal we're sending. Defined in Generated code.
 * @param identifier
 * @param isProperty
 * @param language
 * @return widget
 */
typedef void* (*AJCPS_IdentifyMsgOrPropIdForSignal)(uint32_t identifier, uint8_t* isProperty);

/**
 * Function used to identify what kind of request we're dealing with. Defined in Generated code.
 * @param identifier
 * @return true/false
 */
typedef uint8_t (*AJCPS_IdentifyRootMsgOrPropId)(uint32_t identifier);

/**
 * Start ControlPanel service framework passing callbacks from generated code
 * @param generatedMessageProcessor
 * @param identifyMsgOrPropId
 * @param identifyMsgOrPropIdForSignal
 * @param identifyRootMsgOrPropId
 * @return status
 */
AJ_Status AJCPS_Start(AJSVC_MessageProcessor generatedMessageProcessor, AJCPS_IdentifyMsgOrPropId identifyMsgOrPropId, AJCPS_IdentifyMsgOrPropIdForSignal identifyMsgOrPropIdForSignal, AJCPS_IdentifyRootMsgOrPropId identifyRootMsgOrPropId);

/**
 * return the current session id
 * @return sessionId
 */
uint32_t AJCPS_GetCurrentSessionId();

uint8_t AJCPS_CheckSessionAccepted(uint16_t port, uint32_t sessionId, char* joiner);

AJ_Status AJCPS_ConnectedHandler(AJ_BusAttachment* busAttachment);

AJSVC_ServiceStatus AJCPS_MessageProcessor(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJ_Status* msgStatus);

AJ_Status AJCPS_DisconnectHandler(AJ_BusAttachment* busAttachment);

AJ_Status AJCPS_SendRootUrl(AJ_Message* msg, uint32_t msgId);
AJ_Status AJCPS_GetWidgetProperty(AJ_Message* replyMsg, uint32_t propId, void* context);
AJ_Status AJCPS_GetRootProperty(AJ_Message* replyMsg, uint32_t propId, void* context);
AJ_Status AJCPS_GetAllRootProperties(AJ_Message* msg, uint32_t msgId);
AJ_Status AJCPS_GetAllWidgetProperties(AJ_Message* msg, uint32_t msgId);

#endif /* CONTROLPANELRESPONSES_H_ */
