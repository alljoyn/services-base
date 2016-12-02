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

package org.alljoyn.bus;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.Variant;

/**
 * The temporary utility class to retrieve {@link Variant) signature 
 */
public class VariantUtil {
	public static String getSignature(Variant aVariant) throws BusException {
		return MsgArg.getSignature( new long [] {aVariant.getMsgArg()} );
	}
}