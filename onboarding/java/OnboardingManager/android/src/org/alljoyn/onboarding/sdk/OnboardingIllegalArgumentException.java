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
package org.alljoyn.onboarding.sdk;

/**
 * Thrown to indicate that a method has been passed an illegal or inappropriate
 * argument.
 */
@SuppressWarnings("serial")
public class OnboardingIllegalArgumentException extends Exception {

    public OnboardingIllegalArgumentException(String message) {
        super(message);
    }

    public OnboardingIllegalArgumentException() {
        super();
    }

}