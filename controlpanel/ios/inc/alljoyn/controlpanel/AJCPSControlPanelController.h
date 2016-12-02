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
#import "alljoyn/controlpanel/ControlPanelController.h"
#import "AJCPSControlPanelDevice.h"
#import "AJNAboutObjectDescription.h"

/**
 * AJCPSControlPanelController facilitated controlling of remote ControlPanels
 */
@interface AJCPSControlPanelController : NSObject

- (id)init;

- (id)initWithHandle:(ajn::services::ControlPanelController *)handle;

/**
 * create a controllable device by parsing announce descriptions.
 * @deprecated Deprecated in October 2016 for 16.10
 * @param deviceBusName - BusName of device received in announce
 * @param objectDescs - ObjectDescriptions received in announce
 * @return a ControlPanelDevice
 */
- (AJCPSControlPanelDevice *)createControllableDevice:(NSString *)deviceBusName ObjectDescs:(NSDictionary *)objectDescs __deprecated;

/**
 * create a controllable device by parsing announce descriptions.
 * @param deviceBusName - BusName of device received in announce
 * @param objectDescs - AboutObjectDescriptions received as an AJNMessageArgument in announce
 * @return a ControlPanelDevice
 */
- (AJCPSControlPanelDevice *)createControllableDevice:(NSString *)deviceBusName withObjectDesc:(AJNAboutObjectDescription *)objectDesc;

/**
 * GetControllableDevice - get a device using the busName - creates it if it doesn't exist
 * @param deviceBusName - deviceName to get
 * @return ControlPanelDevice* - returns the Device
 */
- (AJCPSControlPanelDevice *)getControllableDevice:(NSString *)deviceBusName;

/**
 * deleteControllableDevice - shutdown a controllable device and delete it from the Controller
 * @param deviceBusName - deviceName to delete
 * @return status - success-failure
 */
- (QStatus)deleteControllableDevice:(NSString *)deviceBusName;

/**
 * deleteAllControllableDevices - shutdown and delete all controllable devices from the controller
 * @return status - success-failure
 */
- (QStatus)deleteAllControllableDevices;

/**
 * Get map of All Controllable Devices
 * @return controllable Devices map const std::map<qcc::String, ControlPanelDevice*>&
 */
- (NSDictionary *)getControllableDevices;

@property (nonatomic, readonly) ajn::services::ControlPanelController *handle;

@end