/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

package org.alljoyn.login.dashboard.security;

/**
 * A handler for AllJoyn security callbacks.
 * The OnboardingServer accesses secure AllJoyn interfaces like Config and Onboarding
 * This handler addresses the callbacks that AllJoyn security generates
 */
public interface AuthPasswordHandler 
{
	/**
	 * When a client tries to access one of the board's secure interfaces, AllJoyn Bus will ask the board
	 * for a password and match it with the one it receives from the client.
	 * @param peerName the AllJoyn bus name of the peer
	 * @return the password
	 */
	public char[] getPassword(String peerName);
	
	/**
	 * AllJoyn Bus communicates the result of a security check, and lets the board handle it.
	 * On the board's side this is pretty much FYI, not as important as on the client's side where failures mean
	 * denial of service and require user's attention.
	 * @param mechanism the Authentication mechanism
	 * @param authPeer the Bus name of the peer
	 * @param authenticated the result: failure/success
	 */
	public void completed(String mechanism, String authPeer, boolean authenticated);
}
