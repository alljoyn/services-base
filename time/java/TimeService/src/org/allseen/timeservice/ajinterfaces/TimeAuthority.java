 /******************************************************************************
  * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
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

package org.allseen.timeservice.ajinterfaces;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusProperty;
import org.alljoyn.bus.annotation.BusSignal;
import org.allseen.timeservice.TimeServiceConst;

/**
 * Time Service Time Authority interface.
 * {@link BusObject} implementing this interface declares that it is a Time Authority.
 */
@BusInterface(name=TimeAuthority.IFNAME)
public interface TimeAuthority {

    /**
     * The name of the AllJoyn interface
     */
    public static final String IFNAME = TimeServiceConst.IFNAME_PREFIX + ".TimeAuthority";

    /**
     * This interface version
     */
    public static final short VERSION = 1;

    /**
     * Returns the interface version
     * @return interface version number
     */
    @BusProperty(signature="q")
    short getVersion() throws BusException;

    /**
     * Enum value of the different options of where this time authority synchronizes its time with.
     * @return Enum value of this time authority
     * @throws BusException
     */
    @BusProperty(signature="y")
    byte getAuthorityType() throws BusException;

    /**
     * Session-Less signal to suggest to synchronize time with the time authority clock,
     * sending this signal
     */
    @BusSignal(name="TimeSync", sessionless=true)
    void timeSync() throws BusException;
}
