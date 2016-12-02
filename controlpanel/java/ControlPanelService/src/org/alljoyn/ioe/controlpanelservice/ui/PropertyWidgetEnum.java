/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

package org.alljoyn.ioe.controlpanelservice.ui;

/**
 * Optional parameters keys of {@link PropertyWidget}
 */
public enum PropertyWidgetEnum {
    LABEL((short)0),
    BG_COLOR((short)1),
    HINTS((short)2),
    UNIT_OF_MEASURE((short)3),
    CONSTRAINT_TO_VALUES((short)4),
    RANGE((short)5)
    ;

    /**
     * The key number
     */
    public final short ID;

    /**
     * Constructor
     * @param id
     */
    private PropertyWidgetEnum(short id) {
        ID = id;
    }
}