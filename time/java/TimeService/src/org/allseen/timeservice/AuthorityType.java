 /******************************************************************************
  *   * 
  *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
  *    Source Project Contributors and others.
  *    
  *    All rights reserved. This program and the accompanying materials are
  *    made available under the terms of the Apache License, Version 2.0
  *    which accompanies this distribution, and is available at
  *    http://www.apache.org/licenses/LICENSE-2.0

  ******************************************************************************/

package org.allseen.timeservice;

import org.allseen.timeservice.server.TimeAuthorityClock;

/**
 * The different sources a {@link TimeAuthorityClock} can synchronize its time against.
 */
public enum AuthorityType {

    OTHER((byte)0),
    NTP((byte)1),
    CELLULAR((byte)2),
    GPS((byte)3),
    USER_DESIGNATED((byte)4)
    ;

    /**
     * Authority type
     */
    private final byte TYPE_VALUE;

    /**
     * Constructor
     * @param type Authority type
     */
    private AuthorityType(byte type) {

        TYPE_VALUE = type;
    }

    /**
     * Return the numeric value of the {@link AuthorityType}
     * @return authority type numeric value
     */
    public byte getValue() {

        return TYPE_VALUE;
    }

    /**
     * Looks for {@link AuthorityType} with the given type value
     * @param typeValue The value of the {@link AuthorityType} to look for
     * @return {@link AuthorityType} if found otherwise NULL is returned
     */
    public static AuthorityType getType(byte typeValue) {

        AuthorityType[] types = AuthorityType.values();
        for ( AuthorityType type : types ) {

            if ( type.TYPE_VALUE == typeValue ) {

                return type;
            }
        }

        return null;
    }
}