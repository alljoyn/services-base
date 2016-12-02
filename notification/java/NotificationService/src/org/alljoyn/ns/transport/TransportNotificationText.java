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
import org.alljoyn.ns.NotificationText;

/*
 * The utility class used to wrap NotificationText object into format of TransportNotificationText that
 * is sent over AJ network
 */
public class TransportNotificationText {

    /**
     * Text language
     */
    @Position(0)
    public String language;

    /**
     * Message text
     */
    @Position(1)
    public String text;

    /**
     * Constructor
     */
    public TransportNotificationText() { }

    /**
     * Constructor
     * @param language
     * @param text
     */
    public TransportNotificationText(String language, String text) {
        this.language = language;
        this.text = text;
    }

    /**
     * Creates object of TransportNotificationText from NotificationText object
     * @param notifText reference to NotificationText object
     * @return TransportNotificationText
     */
    public static TransportNotificationText buildInstance(NotificationText notifText) {
        TransportNotificationText trNotTxt = new TransportNotificationText(notifText.getLanguage(),
                                                                           notifText.getText()
                                                                           );
        return trNotTxt;
    }//buildInstance

}