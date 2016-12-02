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

package org.alljoyn.ioe.controlpanelservice.ui;

/**
 * Possible types of list of properties UI hints 
 */
public enum ListPropertyWidgetHintsType {
	DYNAMIC_SPINNER((short)1);
	
    /** 
     * The key number
     */  
    public final short ID; 
    
    /** 
     * Constructor
     * @param id
     */
    private ListPropertyWidgetHintsType(short id) {
        ID = id; 
    }   

    /**
     * Search for the enum by the given id
     * If not found returns NULL
     * @param id  
     * @return Enum type by the given id
     */
    public static ListPropertyWidgetHintsType getEnumById(short id) {
    	ListPropertyWidgetHintsType retType = null;
        for (ListPropertyWidgetHintsType type : ListPropertyWidgetHintsType.values()) {
            if ( id == type.ID ) {
                retType = type;
                break;
            }
        }
        return retType;
    }//getEnumById
}