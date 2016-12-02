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

package org.alljoyn.onboarding.transport;

import org.alljoyn.bus.annotation.Position;

/**
 * A class that represents the last onboarding error: The last error code and error message
 */
public class OBLastError {

	/** 
	 * The error code of the last validation
	 * 0 - Validated
	 * 1 - Unreachable
	 * 2 - Unsupported_protocol
	 * 3 - Unauthorized
	 * 4 - Error_message
	 */
    @Position(0) public short m_errorCode;
    
    /** 
     * The error message of the last validation
     * Error_message is the error message received from the underlying Wifi layer. 
     */
    @Position(1) public String m_errorMsg;
	
    public OBLastError()
    {
    }
    
    /**
     * Get the last validation error code
     * @return the last error code
	 * 0 - Validated
	 * 1 - Unreachable
	 * 2 - Unsupported_protocol
	 * 3 - Unauthorized
	 * 4 - Error_message
     */
	public short getErrorCode()
	{
		return m_errorCode;
	}

	/**
	 * Set the last validation error code
	 * @param errorCode
	 */
	public void setErrorCode(short errorCode)
	{
		m_errorCode = errorCode;
	}

	/**
	 * Get the last validation error message
	 * @return the error message received from the underlying Wifi layer.
	 */
	public String getErrorMessage()
	{
		return m_errorMsg;
	}

	/**
	 * Set the last validation error message
	 * @param errorMsg
	 */
	public void setErrorMessage(String errorMsg)
	{
		m_errorMsg = errorMsg;
	}
	
}