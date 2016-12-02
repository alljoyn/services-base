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

public class PropertyWidgetThreeShortAJ {
	
	/**
	 * The type of the received structure, either Date or Time
	 */
	@Position(0)
	public short dataType;
	
	/**
	 * Either Hour or Day
	 */
	@Position(1)
	public ThreeShortAJ data;
	
		
	public static class ThreeShortAJ {
		@Position(0)
		public short var1;
		
		@Position(1)
		public short var2;
		
		@Position(2)
		public short var3;

	}
}