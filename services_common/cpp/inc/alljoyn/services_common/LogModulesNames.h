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

#ifndef LOGMODULESNAMES_H_
#define LOGMODULESNAMES_H_

/*
 * Common Macros
 */

namespace ajn {
namespace services {
namespace logModules {

static char const* const NOTIFICATION_MODULE_LOG_NAME = "Notification";
static char const* const CONTROLPANEL_MODULE_LOG_NAME = "ControlPanel";
static char const* const ONBOARDING_MODULE_LOG_NAME = "Onboarding";
static char const* const CONFIG_MODULE_LOG_NAME = "Config";
static char const* const SERVICES_COMMON_MODULE_LOG_NAME = "services_common";
static char const* const TIME_MODULE_LOG_NAME = "Time";
static const uint32_t ALL_LOG_LEVELS = 15;
} //namespace logModules
} //namespace services
} //namespace ajn

#endif /* LOGMODULESNAMES_H_ */