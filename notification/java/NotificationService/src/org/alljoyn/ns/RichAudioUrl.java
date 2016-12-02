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
 * RichNotificationAudioUrl. Creates object of the RichNotificationAudioUrl class
 */
public class RichAudioUrl {

	/**
	 * Reference to language string <br>
	 * The language must comply IETF standard 
	 */
	private String language;
	
	/**
	 * RichNotification Audio Url
	 */
	private String url;
	
	/**
	 * Constructor
	 */
	public RichAudioUrl() {}
	
	/**
	 * Constructor
	 * @param language - The language must comply IETF standard 
	 * @param url - rich notification audio url
	 * @throws NotificationServiceException Thrown if language is null url is null or empty
	 */
	public RichAudioUrl(String language, String url) throws NotificationServiceException {
		setLanguage(language);
		setUrl(url);
	}

	/**
	 * @return String Returns audio url language. The language must comply IETF standard 
	 */
	public String getLanguage() {
		return language;
	}
	
	/**
	 * Set the language
	 * @param language The language must comply IETF standard 
	 * @throws NotificationServiceException Thrown if language is null
	 */
	public void setLanguage(String language) throws NotificationServiceException {
		if (language == null || language.length() == 0 ) {
			throw new NotificationServiceException("Language is undefined");
		}
		this.language = language;
	}//setLanguage
	
	/**
	 * @return the rich notification audio url
	 */
	public String getUrl(){
		return url;
	}
	
	/**
	 * Set the notification url
	 * @param url the notification url
	 * @throws NotificationServiceException Thrown if url is null or is empty
	 */
	public void setUrl(String url) throws NotificationServiceException {
		if ( url == null || url.length() == 0 ) {
			throw new NotificationServiceException("audio url is undefined");
		}
		this.url = url;
	}//setUrl

	
	/**
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return "RichAudioUrl [language= '" + language + "', url='" + url + "']";
	}
	
}//RichNotificationAudioUrl
