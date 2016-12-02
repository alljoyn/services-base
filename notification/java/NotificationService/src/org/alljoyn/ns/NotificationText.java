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

package org.alljoyn.ns;


/**
 * NotificationText. Creates object of the NotificationText class
 */
public class NotificationText {

	/**
	 * Reference to language string <br>
	 * The language must comply with the IETF standard 
	 */
	private String language;
	
	/**
	 * Notification text
	 */
	private String text;
	
	/**
	 * Constructor
	 */
	public NotificationText() {}
	
	/**
	 * Constructor
	 * @param language The language must comply with the IETF standard 
	 * @param text the notification text
	 * @throws NotificationServiceException Thrown if language is null text is null or empty
	 */
	public NotificationText(String language, String text) throws NotificationServiceException {
		setLanguage(language);
		setText(text);
	}

	/**
	 * @return String Returns notification language. The language must comply with the IETF standard 
	 */
	public String getLanguage() {
		return language;
	}
	
	/**
	 * Set the language
	 * @param language The language must comply with the IETF standard 
	 * @throws NotificationServiceException Thrown if language is null
	 */
	public void setLanguage(String language) throws NotificationServiceException {
		if ( language == null || language.length() == 0 ) {
			throw new NotificationServiceException("Language is undefined");
		}
		this.language = language;
	}//setLanguage
	
	/**
	 * @return the notification text
	 */
	public String getText(){
		return text;
	}
	
	/**
	 * Set the notification text
	 * @param text the notification text
	 * @throws NotificationServiceException Thrown if text is null or is empty
	 */
	public void setText(String text) throws NotificationServiceException {
		if ( text == null || text.length() == 0 ) {
			throw new NotificationServiceException("Text is undefined");
		}
		this.text = text;
	}//setText

	
	/**
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return "NotificationText [language='" + language + "', text='" + text + "']";
	}
	
}//NotificationText