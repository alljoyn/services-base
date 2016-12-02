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

package org.alljoyn.services.android.security;

/**
 * An API between the {@link SrpAnonymousKeyListener} and the application.  
 * The application will implement a password handler.
 * When the bus requires authentication with a remote device, it will let the password handler (the application) handle it.
 * When the bus receives a result of an authentication attempt with a remote device, it will let the password handler (the application) handle it.
 * @see AuthPasswordHandler
 */
 public interface AuthPasswordHandler 
{
	/**
	 * Get the password for this peer. The application should store a password per peer 
	 * @param peerName the alljoyn bus name of the peer we want to authenticate with
	 * @return the password for this peer
	 */
	public char[] getPassword(String peerName);
	
	/**
	 * Once the authentication has finished the completed(...) call-back method is called.
	 * The application can then show a popup, a toast, etc.
	 * @param mechanism The authentication mechanism that was just completed
	 * @param authPeer The peerName (well-known name or unique name)
	 * @param authenticated A boolean variable indicating if the authentication attempt completed successfuly.
	 */
	public void completed(String mechanism, String authPeer, boolean authenticated);
}