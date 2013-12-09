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

#ifndef CONTROLPANEL_H_
#define CONTROLPANEL_H_

#include <Apps_Common.h>
#include "ControlPanelGenerated.h"

/**
 * SessionId used in controlpanelservice
 */
extern uint32_t CPSsessionId;

/**
 * A NULL terminated collection of all interfaces.
 */

void CPS_Init();

Service_Status CPS_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status*msgStatus);

uint8_t CPS_CheckSessionAccepted(uint16_t port, uint32_t sessionId, char*joiner);

AJ_Status ControlPanel_ConnectedHandler(AJ_BusAttachment*bus);

void CPS_IdleConnectedHandler(AJ_BusAttachment*bus);

void CPS_Finish();

void Controllee_DoWork(AJ_BusAttachment*bus);

#endif /* CONTROLPANEL_H_ */
