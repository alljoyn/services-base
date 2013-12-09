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

#ifndef _SERVICES_HANDLERS_H_
#define _SERVICES_HANDLERS_H_

#include <alljoyn.h>
#include <Apps_Common.h>

/**
 * Initialize the Services. called at the start of the application
 */
void Service_Init();

/**
 * Connect to Daemon
 * @param daemonName - daemonName to connect to
 * @return true/false if connected successfully
 */
uint8_t Daemon_Connect(char* daemonName);

/**
 * Functions to call after the Service Connected
 * @return aj_status - status of last request to daemon
 */
AJ_Status Service_ConnectedHandler();

/**
 * Process an incoming message
 * @param msg
 * @param status
 * @return service_status - shows if the message was processed or not
 */
Service_Status Service_MessageProcessor(AJ_Message* msg, AJ_Status* status);

/**
 * Run when there is a timeout reading off the bus -
 * application is idle
 */
void Service_IdleConnectedHandler();

/**
 * Shutdown services. Should be called on bus disconnect
 */
void Service_DisconnectHandler();

/**
 * Callback to set the password for the daemon
 * @param buffer
 * @param bufLen
 * @return length of data but in buffer
 */
uint32_t PasswordCallback(uint8_t* buffer, uint32_t bufLen);

/**
 * Run when the bus is connected to the daemon
 * application is idle
 * @return aj_status for last request to daemon
 */
AJ_Status Application_ConnectedHandler();

/**
 * Run when the bus is disconnecting from the daemon
 * Connection to daemon is either restarting or was stopped
 * @param restart
 * @return aj_status for last request to daemon
 */
AJ_Status Application_DisconnectHandler(uint8_t restart);

/**
 * Disconnect from Daemon
 * @param disconnectWiFi
 * @return true/false if disconnected successfully
 */
uint8_t Daemon_Disconnect(uint8_t disconnectWiFi);

#endif /* _SERVICES_HANDLERS_H_ */
