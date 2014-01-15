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

#ifndef CONSUMERSAMPLE_H_
#define CONSUMERSAMPLE_H_

#include <Apps_Common.h>

#define CONSUMER_ACTION_NOTHING         0
#define CONSUMER_ACTION_ACKNOWLEDGE     1
#define CONSUMER_ACTION_DISMISS         2

/*
 * Functions that the application needs to implement
 */

/**
 * Initialize the Notification Service Consumer
 */
void Consumer_Init();

/**
 * Function called after service connects
 * @param bus
 * @return status success/failure
 */
AJ_Status Consumer_ConnectedHandler(AJ_BusAttachment* bus);


/**
 * If idle, do work
 * @param bus
 */
void Consumer_IdleConnectedHandler(AJ_BusAttachment* bus);

/**
 * Process the message received
 * @param bus
 * @param msg
 * @param msgStatus
 * @return status - was message handled
 */
Service_Status Consumer_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status*msgStatus);

/**
 * Handle Session state changed
 * @param bus
 * @param sessionId
 * @param sessionJoined
 * @param replySerialNum
 * @return status - was message handled
 */
Service_Status Consumer_HandleSessionStateChanged(AJ_BusAttachment* bus, uint32_t sessionId, uint8_t sessionJoined, uint32_t replySerialNum);

/**
 * Finish Consumer - called when bus disconnects
 * @param bus
 */
void Consumer_Finish(AJ_BusAttachment* bus);

#endif /* CONSUMERSAMPLE_H_ */

