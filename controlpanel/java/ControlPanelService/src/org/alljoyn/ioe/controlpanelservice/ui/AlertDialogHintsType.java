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
 * The hint types of the {@link AlertDialogWidget}
 */
public enum AlertDialogHintsType {
    ALERT_DIALOG((short)1)
    ;

    /**
     * The key number
     */
    public final short ID;

    /**
     * Constructor
     * @param id
     */
    private AlertDialogHintsType(short id) {
        ID = id;
    }

    /**
     * Search for the enum by the given id
     * If not found returns NULL
     * @param id
     * @return Enum type by the given id
     */
    public static AlertDialogHintsType getEnumById(short id) {
        AlertDialogHintsType retType = null;
        for (AlertDialogHintsType type : AlertDialogHintsType.values()) {
            if (id == type.ID) {
                retType = type;
                break;
            }
        }
        return retType;
    }//getEnumById
}