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

#ifndef CONTROLPANELCLIENT_H_
#define CONTROLPANELCLIENT_H_

#include "ControlPanelClientGenerated.h"
#include "alljoyn/controlpanel/ControlPanelService.h"
#include "alljoyn/controlpanel/Definitions.h"
#include "aj_debug.h"

/**
 * Static non consts.
 */
char announceSender[16];

/**
 * Objects implemented by the application. The first member in the AJ_Object structure is the path.
 * The second is the collection of all interfaces at that path.
 */

#define CONTROLPANEL_APPOBJECTS \
    CONTROLPANELAPPOBJECTS
/*
   typedef enum _AJSVC_ServiceStatus {
    AJSVC_SERVICE_STATUS_HANDLED,       //!< SERVICE_STATUS_HANDLED
    AJSVC_SERVICE_STATUS_NOT_HANDLED,   //!< SERVICE_STATUS_NOT_HANDLED
   } AJSVC_ServiceStatus;
 */
void CPS_Init();

AJ_Status CPS_StartService(AJ_BusAttachment* bus, const char* daemonName, uint32_t timeout, uint8_t connected);

void MakeMethodCall(uint32_t sessionId);

void CPS_IdleConnectedHandler(AJ_BusAttachment*bus);

AJSVC_ServiceStatus CPS_NotifySessionAccepted(uint32_t sessionId, const char*sender);

AJSVC_ServiceStatus CPS_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status*msgStatus);

void CPS_Finish();

#endif //end CONTROLPANELCLIENT_H_
