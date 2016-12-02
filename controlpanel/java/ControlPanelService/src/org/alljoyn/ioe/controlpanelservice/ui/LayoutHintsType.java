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
 * Possible types of layout hints
 */
public enum LayoutHintsType {
    VERTICAL_LINEAR((short)1),
    HORIZONTAL_LINEAR((short)2),
    ;

    /**
     * The key number
     */
    public final short ID;

    /**
     * Constructor
     * @param id
     */
    private LayoutHintsType(short id) {
        ID = id;
    }

    /**
     * Search for the enum by the given id
     * If not found returns NULL
     * @param id
     * @return Enum type by the given id
     */
    public static LayoutHintsType getEnumById(short id) {
        LayoutHintsType retType = null;
        for (LayoutHintsType type : LayoutHintsType.values()) {
            if (id == type.ID) {
                retType = type;
                break;
            }
        }
        return retType;
    }//getEnumById
}