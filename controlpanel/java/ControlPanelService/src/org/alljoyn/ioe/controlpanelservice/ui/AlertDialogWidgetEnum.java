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
 * Optional parameters keys of {@link AlertDialogWidget}
 */
public enum AlertDialogWidgetEnum {
	LABEL((short)0),
	BG_COLOR((short)1),
	HINTS((short)2),
	LABEL_ACTION1((short)6),
	LABEL_ACTION2((short)7),
	LABEL_ACTION3((short)8)
	;
	
	  
    /**
     * The key number
     */
    public final short ID;
      
    /**
     * Constructor
     * @param id 
     */           
    private AlertDialogWidgetEnum(short id) {
        ID = id;
    }   
	
}