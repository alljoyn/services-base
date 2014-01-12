/******************************************************************************
 * Copyright (c) 2013 - 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifndef _SERVICES_REGISTRATION_H_
#define _SERVICES_REGISTRATION_H_

#include <alljoyn.h>
/*
 * For each service:
 * 1) Define pre objects - the amount of objects registered before the service
 * 2) If service is included:
 *    i)   include service header file(s)
 *    If service is NOT included:
 *    i)   define the default number of appobjects and number of objects
 *    ii)  define the default announce objects
 */
/*
 * Includes for all the services
 */
#define NUM_PRE_ABOUT_OBJECTS 0
#include "alljoyn/about/AboutService.h"

#define NUM_PRE_CONFIG_OBJECTS                NUM_PRE_ABOUT_OBJECTS + NUM_ABOUT_OBJECTS
#ifdef CONFIG_SERVICE
    #include "alljoyn/config/ConfigService.h"
#else
    #define NUM_CONFIG_OBJECTS 0
    #define CONFIG_APPOBJECTS
    #define CONFIG_ANNOUNCEOBJECTS
#endif

#define NUM_PRE_ONBOARDING_OBJECTS            NUM_PRE_CONFIG_OBJECTS + NUM_CONFIG_OBJECTS
#ifdef ONBOARDING_SERVICE
    #include "alljoyn/onboarding/OnboardingService.h"
#else
    #define NUM_ONBOARDING_OBJECTS 0
    #define ONBOARDING_APPOBJECTS
    #define ONBOARDING_ANNOUNCEOBJECTS
#endif

#define NUM_PRE_NOTIFICATION_COMMON_OBJECTS   NUM_PRE_ONBOARDING_OBJECTS + NUM_ONBOARDING_OBJECTS
#if defined(NOTIFICATION_SERVICE_PRODUCER) || defined(NOTIFICATION_SERVICE_CONSUMER)
    #include "alljoyn/notification/NotificationCommon.h"
#else
    #define NUM_NOTIFICATION_COMMON_OBJECTS 0
    #define NOTIFICATION_COMMON_APPOBJECTS
#endif

#define NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS NUM_PRE_NOTIFICATION_COMMON_OBJECTS + NUM_NOTIFICATION_COMMON_OBJECTS
#ifdef NOTIFICATION_SERVICE_PRODUCER
    #include "alljoyn/notification/NotificationProducer.h"
#else
    #define NUM_NOTIFICATION_PRODUCER_OBJECTS 0
    #define NOTIFICATION_PRODUCER_APPOBJECTS
    #define NOTIFICATION_PRODUCER_ANNOUNCEOBJECTS
#endif

#define NUM_PRE_CONTROLPANEL_OBJECTS          NUM_PRE_NOTIFICATION_PRODUCER_OBJECTS + NUM_NOTIFICATION_PRODUCER_OBJECTS
#ifdef CONTROLPANEL_SERVICE
    #include "alljoyn/controlpanel/ControlPanelService.h"
#else
    #define NUM_CONTROLPANEL_OBJECTS 0
    #define CONTROLPANEL_APPOBJECTS
    #define CONTROLPANEL_ANNOUNCEOBJECTS
#endif

#define NUM_PRE_APPLICATION_OBJECTS (NUM_PRE_CONTROLPANEL_OBJECTS + NUM_CONTROLPANEL_OBJECTS)

#define NUM_PRE_NOTIFICATION_CONSUMER_PROXYOBJECTS 0
#ifdef NOTIFICATION_SERVICE_CONSUMER
    #include "alljoyn/notification/NotificationConsumer.h"
#else
    #define NUM_NOTIFICATION_CONSUMER_PROXYOBJECTS 0
    #define NOTIFICATION_CONSUMER_PROXYOBJECTS
#endif

#define NUM_PRE_PROXY_OBJECTS (NUM_PRE_NOTIFICATION_CONSUMER_PROXYOBJECTS + NOTIFICATION_CONSUMER_PROXYOBJECTS)

/*
 * Define pre objects - the amount of objects registered before each service
 */

/*
 * Defines AppObjects and Proxyobjects to be registered
 * each of those are described in the service itself
 * for example NOTIFICATION_PRODUCER_APPOBJECTS is defined in
 * services/notification/tcl/inc/alljoyn/notification/NotificationProducer.h
 */
#define IOE_SERVICES_APPOBJECTS \
    ABOUT_APPOBJECTS \
    CONFIG_APPOBJECTS \
    ONBOARDING_APPOBJECTS \
    NOTIFICATION_COMMON_APPOBJECTS \
    NOTIFICATION_PRODUCER_APPOBJECTS \
    CONTROLPANEL_APPOBJECTS

#define IOE_SERVICES_PROXYOBJECTS \
    NOTIFICATION_CONSUMER_PROXYOBJECTS

/*
 * Define all objects to be announced
 */
#define IOE_SERVICES_ANNOUNCEOBJECTS \
    ABOUT_ANNOUNCEOBJECTS \
    CONFIG_ANNOUNCEOBJECTS \
    ONBOARDING_ANNOUNCEOBJECTS \
    NOTIFICATION_PRODUCER_ANNOUNCEOBJECTS \
    CONTROLPANEL_ANNOUNCEOBJECTS

#endif /* _SERVICES_REGISTRATION_H_ */
