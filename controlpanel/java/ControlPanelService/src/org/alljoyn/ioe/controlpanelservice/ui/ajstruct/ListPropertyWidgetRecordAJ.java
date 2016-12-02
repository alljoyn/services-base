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

package org.alljoyn.ioe.controlpanelservice.ui.ajstruct;

import org.alljoyn.bus.annotation.Position;
import org.alljoyn.ioe.controlpanelservice.ui.ListPropertyWidget;

/**
 * The value of the {@link ListPropertyWidget}
 */
public class ListPropertyWidgetRecordAJ {
	@Position(0)
	public short recordId;
		
	@Position(1)
	public String label;
}