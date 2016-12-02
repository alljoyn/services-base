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
 * Notification message types
 */
public enum NotificationMessageType {
	EMERGENCY ((short)0), 
	WARNING((short)1), 
	INFO((short)2);
	
	/**
	 * Type Id
	 */
	private short typeId;
	
	/**
	 * Constructor
	 * @param typeId
	 */
	private NotificationMessageType(short typeId) {
		this.typeId = typeId;
	}
	
	/**
	 * @return Returns type id
	 */
	public short getTypeId() {
		return typeId;
	}
	
	/**
	 * Search for message type with the given Id.
	 * If not found returns NULL
	 * @param msgTypeId type id
	 * @return Notification message type
	 */
	public static NotificationMessageType getMsgTypeById(short msgTypeId) {
		NotificationMessageType retType = null;
		for (NotificationMessageType type : NotificationMessageType.values()) {
			if ( msgTypeId == type.getTypeId() ) {
				retType = type;
				break;
			}
		}
		return retType;
	}//getMsgTypeById
}