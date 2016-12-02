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

package org.alljoyn.ns.transport;

import org.alljoyn.bus.annotation.Position;
import org.alljoyn.ns.RichAudioUrl;

/* 
 * The utility class used to wrap RichAudioUrl object into format of TransportRichAudioUrl that 
 * is sent over AJ network
 */
public class TransportRichAudioUrl {
	
	/**
	 * Url language
	 */
	@Position(0)
	public String language;
	
	/**
	 * Message url
	 */
	@Position(1)
	public String url;

	/**
	 * Constructor
	 */
	public TransportRichAudioUrl() {}
	
	/**
	 * Constructor
	 * @param language
	 * @param url
	 */
	public TransportRichAudioUrl(String language, String url) {
		this.language = language;
		this.url      = url;
	}

	/**
	 * Creates object of TransportRichAudioUrl from RichAudioUrl object
	 * @param richAudioUrl reference to TransportRichAudioUrl object
	 * @return TransportRichAudioUrl
	 */
	public static TransportRichAudioUrl buildInstance(RichAudioUrl richAudioUrl) {
		return new TransportRichAudioUrl(richAudioUrl.getLanguage(),
		     							 richAudioUrl.getUrl()
		);		
	}//buildInstance

}