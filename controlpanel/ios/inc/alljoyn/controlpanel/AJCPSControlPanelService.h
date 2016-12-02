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

#import <Foundation/Foundation.h>
#import "AJNBusAttachment.h"
#import "alljoyn/controlpanel/ControlPanelService.h"
#import "AJCPSControlPanelController.h"
#import "AJCPSControlPanelListener.h"
#import "AJSVCGenericLogger.h"

/**
 * AJCPSControlPanelService - singleton implementation
 */
@interface AJCPSControlPanelService : NSObject

/**
 * Get Instance of ControlPanelServiceImpl - singleton implementation
 * @return instance A AJCPSControlPanelService instance
 */
+ (AJCPSControlPanelService *)getInstance;


/**
 * Initialize the controller to be used
 * @param bus - bus used for Controller
 * @param controlPanelController - controller to initialize
 * @param controlPanelListener AJCPSControlPanelListener that will receive session/signal events notifications.
 * @return ER_OK if successful.
 */
- (QStatus)initController:(AJNBusAttachment *)bus controlPanelController:(AJCPSControlPanelController *)controlPanelController
                                                    controlPanelListener:(id <AJCPSControlPanelListener> )controlPanelListener;

/**
 * Remove locally stored controller. Allows a new call to initController to be made
 * @return ER_OK if successful.
 */
- (QStatus)shutdownController;

#pragma mark - Logger methods
/**
 * Receive GenericLogger* to use for logging
 * @param logger Implementation of GenericLogger
 */
- (void)setLogger:(id <AJSVCGenericLogger> )logger;

/**
 * Get the currently-configured logger implementation
 * @return logger Implementation of GenericLogger
 */
- (id <AJSVCGenericLogger> )logger;

/**
 * Set log level filter for subsequent logging messages
 * @param newLogLevel enum value
 */
- (void)setLogLevel:(QLogLevel)newLogLevel;

/**
 * Get log level filter value currently in effect
 * @return logLevel enum value currently in effect
 */
- (QLogLevel)logLevel;

/**
 * Method to get the busAttachment used in the service.
 * @return AJNBusAttachment
 */
- (AJNBusAttachment *)getBusAttachment;

/**
 * Get the ControlPanelListener
 * @return ControlPanelListener
 */
- (id <AJCPSControlPanelListener> )getControlPanelListener;

/**
 * Get the Version of the ControlPanelService
 * @return the ControlPanelService version
 */
- (uint16_t)getVersion;

@end