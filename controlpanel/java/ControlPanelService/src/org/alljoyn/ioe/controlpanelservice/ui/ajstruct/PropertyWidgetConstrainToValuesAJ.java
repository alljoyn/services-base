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

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.Variant;
import org.alljoyn.bus.annotation.Position;
import org.alljoyn.ioe.controlpanelservice.ui.PropertyWidget.ConstrainToValues;
import org.alljoyn.ioe.controlpanelservice.ui.PropertyWidget.ValueType;

import android.util.Log;

public class PropertyWidgetConstrainToValuesAJ {
	
	@Position(0)
	public Variant value;
	
	@Position(1)
	public String label;
	
	/**
	 * Returns single object of PropertyWidget list of values
	 * @param valueType The property type of value
	 * @return
	 */
	public ConstrainToValues<?> getPropertyWidgetConstrainToValues(ValueType valueType) {
		Log.v("cpan" + PropertyWidgetConstrainToValuesAJ.class.getSimpleName(), "Unmarshalling PropertyWidget LOV constraint, label: '" + label + "'");
		try {
			switch (valueType) {
				case BOOLEAN: {
					Boolean valueCast = value.getObject(boolean.class);
					return new ConstrainToValues<Boolean>(valueCast, label);
				}
				case BYTE: {
					Byte valueCast    = value.getObject(byte.class);
					return new ConstrainToValues<Byte>(valueCast, label);
				}
				case DOUBLE: {
					Double valueCast  = value.getObject(double.class);
					return new ConstrainToValues<Double>(valueCast, label);					
				}
				case INT: {
					Integer valueCast = value.getObject(int.class);
					return new ConstrainToValues<Integer>(valueCast, label);					
				}
				case LONG: {
					Long valueCast    = value.getObject(long.class);
					return new ConstrainToValues<Long>(valueCast, label);					
				}
				case SHORT: {
					Short valueCast   = value.getObject(short.class);
					return new ConstrainToValues<Short>(valueCast, label);					
				}
				case STRING: {
					String valueCast  = value.getObject(String.class);
					return new ConstrainToValues<String>(valueCast, label);					
				}
				default: {
					break;
				}
			}//SWITCH
		}//TRY
		catch (BusException be) {
			 Log.e("cpan" + PropertyWidgetConstrainToValuesAJ.class.getSimpleName(), "Failed to unmarshal PropertyWidget LOV - Error: '" + be.getMessage() + "'" );
			return null;
		}
		Log.e("cpan" + PropertyWidgetConstrainToValuesAJ.class.getSimpleName(), "Failed to unmarshal PropertyWidget LOV" );
		return null;
	}//getPropertyWidgetRangeConstraint
	
}