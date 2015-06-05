 /******************************************************************************
  *   *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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

package org.allseen.timeservice.test;

import org.allseen.timeservice.DateTime;
import org.allseen.timeservice.TimeServiceException;
import org.allseen.timeservice.server.TimeAuthorityClock;

/**
 * Test Server Time Authority Clock
 */
public class TestServerAuthorityClock extends TimeAuthorityClock {


    private final TestServerClock clock;

    /**
     * Constructor
     * @param clock
     */
    public TestServerAuthorityClock(TestServerClock clock) {

        this.clock = clock;
    }

    @Override
    public DateTime getDateTime() {

        return clock.getDateTime();
    }

    @Override
    public void setDateTime(DateTime dateTime) {

        clock.setDateTime(dateTime);
    }

    @Override
    public boolean getIsSet() {

        return true;
    }

    /**
     * Send TimeSync signal
     */
     @Override
    public void timeSync() throws TimeServiceException {

        super.timeSync();
    }
}