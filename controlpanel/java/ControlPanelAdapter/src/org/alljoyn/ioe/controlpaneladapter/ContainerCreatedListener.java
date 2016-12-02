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

package org.alljoyn.ioe.controlpaneladapter;

import android.view.View;

/**
 * Implement this interface to receive an event of creating a Root-Container-Element.
 */
public interface ContainerCreatedListener {

    /**
     * This method is called when the Root-Container-Element is created
     * @param container The container {@link View}.
     */
    void onContainerViewCreated(View container);
}