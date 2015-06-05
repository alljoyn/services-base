/******************************************************************************
 * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

/**
 * This class is experimental, and as such has not been tested.
 * Please help make it more robust by contributing fixes if you find issues
 **/

#ifndef TIMESERVICESESSIONLISTENERADAPTER_H_
#define TIMESERVICESESSIONLISTENERADAPTER_H_

#include "alljoyn/time/TimeServiceSessionListener.h"
#include "AJTMTimeServiceSessionListener.h"

class AJTMTimeServiceSessionListenerAdapter : public ajn::services::TimeServiceSessionListener {
public:

    id<AJTMTimeServiceSessionListener> handle;

    /**
     * Constructor
     */
    AJTMTimeServiceSessionListenerAdapter(id<AJTMTimeServiceSessionListener> timeServiceServerClock);

    /**
     * Destructor
     */
    ~AJTMTimeServiceSessionListenerAdapter();

    /**
     * The method is called when the existing session is lost.
     * Avoid blocking this thread with long running tasks.
     *
     * @param timeServiceClient TimeServiceClient that was used to create the session
     * @param reason Session lost reason
     */
    void sessionLost(ajn::services::TimeServiceClient* timeServiceClient, ajn::SessionListener::SessionLostReason reason);

    /**
     * The method is called when a session was established following the call to
     * TimeServiceClient.joinSessionAsync().
     * Avoid blocking this thread with long running tasks.
     *
     * @param timeServiceClient TimeServiceClient that was used to create the session
     * @param status Check this QStatus to ensure that the session was created successfully
     */
    void sessionJoined(ajn::services::TimeServiceClient* timeServiceClient, QStatus status);
};

#endif