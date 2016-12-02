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

package org.alljoyn.ns.sampleapp;

public enum MessageTypeEnum {
    Emergency("EMERGENCY"),
    Warning("WARNING"),
    Info("INFO");

    /**
     * Internal name representation
     */
    public final String INT_NAME;

    /**
     * Constructor
     * @param name
     */
    private MessageTypeEnum(String name) {
        INT_NAME = name;
    }

    /**
     * Returns string representation of the enum
     * @return
     */
    public static String[] stringValues() {
        MessageTypeEnum[] enumValues = MessageTypeEnum.values();
        String[] arr = new String[enumValues.length];
        int i = 0;
        for (MessageTypeEnum te : enumValues) {
            arr[i] = te.toString();
            ++i;
        }
        return arr;
    }//stringValues

}