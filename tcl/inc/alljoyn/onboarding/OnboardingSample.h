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

#ifndef _ONBOARDINGSAMPLE_H_
#define _ONBOARDINGSAMPLE_H_

/** @defgroup Onboarding Sample
 *
 *  @{
 */

#include <Apps_Common.h>

/*
 * Onboarding Sample API
 */

/**
 * on init
 */
void Onboarding_Init();

/**
 * on finish
 * @param bus
 */
void Onboarding_Finish(AJ_BusAttachment* bus);

/**
 * on connected
 * @param bus
 * @return aj_status
 */
AJ_Status Onboarding_ConnectedHandler(AJ_BusAttachment* bus);

/**
 * on idle connected
 * @param bus
 */
void Onboarding_IdleConnectedHandler(AJ_BusAttachment* bus);

/**
 * Switch to retry state
 */
void Onboarding_SwitchToRetry();

/**
 * on idle disconnected
 * @param bus
 */
AJ_Status Onboarding_IdleDisconnectedHandler(AJ_BusAttachment* bus);

/**
 * on messge to process
 * @param bus
 * @param msg
 * @return processor status
 */
Service_Status Onboarding_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status* processorStatus);

/**
 * on disconnect
 * @param bus
 * @return aj_status
 */
AJ_Status Onboarding_DisconnectedHandler(AJ_BusAttachment* bus);

/** @} */
 #endif // _ONBOARDINGSAMPLE_H_
