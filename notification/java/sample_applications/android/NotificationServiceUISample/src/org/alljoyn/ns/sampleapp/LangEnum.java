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

public enum LangEnum {
    English("en"),
    Hebrew("he"),
    Russian("ru");

    /**
     * Internal name representation
     */
    public final String INT_NAME;

    /**
     * Constructor
     * @param name
     */
    private LangEnum(String name) {
        INT_NAME = name;
    }

    /**
     * Returns string representation of the enum
     * @return
     */
    public static String[] stringValues() {
        LangEnum[] enumValues = LangEnum.values();
        String[] arr = new String[enumValues.length];
        int i = 0;
        for (LangEnum le : enumValues) {
            arr[i] = le.toString();
            ++i;
        }
        return arr;
    }//stringValues
}