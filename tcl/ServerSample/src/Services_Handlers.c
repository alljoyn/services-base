/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
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

#include <aj_link_timeout.h>

#include "Services_Handlers.h"
#include <alljoyn/services_common/Services_Common.h>
#include <alljoyn/services_common/PropertyStore.h>

#include <alljoyn/about/AboutSample.h>
#ifdef CONFIG_SERVICE
    #include <alljoyn/config/ConfigSample.h>
#endif
#ifdef ONBOARDING_SERVICE
    #include <alljoyn/onboarding/OnboardingSample.h>
#endif
#ifdef NOTIFICATION_SERVICE_PRODUCER
    #include <NotificationProducerSample.h>
#endif
#ifdef CONTROLPANEL_SERVICE
    #include <ControlPanelSample.h>
#endif
#ifdef NOTIFICATION_SERVICE_CONSUMER
    #include <NotificationConsumerSample.h>
#endif

const char UpdateNotAllowed[] =         "org.alljoyn.Error.UpdateNotAllowed";           //Update not allowed for given field
const char InvalidValue[] =             "org.alljoyn.Error.InvalidValue";               //Invalid value
const char FeatureNotAvailable[] =      "org.alljoyn.Error.FeatureNotAvailable";        //Feature not available
const char MaxSizeExceeded[] =          "org.alljoyn.Error.MaxSizeExceeded";            //Maximum size exceeded
const char LanguageNotSupported[] =     "org.alljoyn.Error.LanguageNotSupported";       //The language specified is not supported

const char SESSIONLESS_MATCH[] = "sessionless='t',type='error'"; //AddMatch to allow sessionless messages coming in
const char NAMEOWNER_MATCH[] = "type='signal',member='NameOwnerChanged',interface='org.freedesktop.DBus'"; //AddMatch to receive Nameowner changed signals

#ifdef NOTIFICATION_SERVICE_CONSUMER
uint8_t addSessionLessMatch = TRUE;
#else
uint8_t addSessionLessMatch = FALSE;
#endif

typedef enum {
    INIT_START = 0,
    INIT_SERVICES_PORT = INIT_START,
    INIT_ADVERTISE_NAME,
    INIT_ADDSLMATCH,
    INIT_FINISHED
} enum_init_state_t;

static const uint8_t MAX_INIT_RETRIES = 5;
static uint8_t init_retries = 0;

void Service_Init()
{
    PropertyStore_Init();
    About_Init();
#ifdef CONFIG_SERVICE
    Config_Init();
#endif
#ifdef ONBOARDING_SERVICE
    Onboarding_Init();
#endif
#ifdef NOTIFICATION_SERVICE_PRODUCER
    Producer_Init();
#endif
#ifdef NOTIFICATION_SERVICE_CONSUMER
    Consumer_Init();
#endif
#ifdef CONTROLPANEL_SERVICE
    CPS_Init();
#endif
}

uint8_t Daemon_Connect(char* daemonName)
{
    while (TRUE) {
        AJ_Status status = AJ_OK;
#ifdef ONBOARDING_SERVICE
        status = Onboarding_IdleDisconnectedHandler(&busAttachment);
        if (status != AJ_OK) {
            AJ_Printf("Failed to establish WiFi connectivity\n");
            AJ_Sleep(AJ_CONNECT_PAUSE);
            return FALSE;
        }
#endif
        AJ_Printf("Attempting to connect to bus '%s'\n", daemonName);
        status = AJ_Connect(&busAttachment, daemonName, AJ_CONNECT_TIMEOUT);
        if (status != AJ_OK) {
            AJ_Printf("Failed to connect to bus sleeping for %d seconds\n", AJ_CONNECT_PAUSE / 1000);
            AJ_Sleep(AJ_CONNECT_PAUSE);
#ifdef ONBOARDING_SERVICE
            if (status == AJ_ERR_DHCP) {
                Onboarding_SwitchToRetry();
            }
#endif
            continue;
        }
        const char* busUniqueName = AJ_GetUniqueName(&busAttachment);
        if (busUniqueName == NULL) {
            AJ_Printf("Failed to GetUniqueName() from newly connected bus, retrying\n");
            continue;
        }
        AJ_Printf("Connected to daemon with BusUniqueName=%s\n", busUniqueName);
        break;
    }
    return TRUE;
}

AJ_Status Service_ConnectedHandler()
{
    AJ_BusSetPasswordCallback(&busAttachment, PasswordCallback);
    /* Configure timeout for the link to the daemon bus */
    AJ_SetBusLinkTimeout(&busAttachment, 60);     // 60 seconds

    AJ_Status status = AJ_OK;
    do {
        CHECK(About_ConnectedHandler(&busAttachment));
#ifdef CONFIG_SERVICE
        CHECK(Config_ConnectedHandler(&busAttachment));
#endif
#ifdef ONBOARDING_SERVICE
        CHECK(Onboarding_ConnectedHandler(&busAttachment));
#endif
#ifdef NOTIFICATION_SERVICE_PRODUCER
        CHECK(Producer_ConnectedHandler(&busAttachment));
#endif
#ifdef CONTROLPANEL_SERVICE
        CHECK(ControlPanel_ConnectedHandler(&busAttachment));
#endif
#ifdef NOTIFICATION_SERVICE_CONSUMER
        CHECK(Consumer_ConnectedHandler(&busAttachment));
#endif
        return status;
    } while (0);
    AJ_Printf("Service ConnectedHandler returned an error %s\n", (AJ_StatusText(status)));
    return status;
}

static enum_init_state_t currentServicesInitializationState = INIT_START;
static enum_init_state_t nextServicesInitializationState = INIT_START;

AJ_Status Application_ConnectedHandler()
{
    AJ_Status status = AJ_OK;
    if (AJ_GetUniqueName(&busAttachment)) {
        if (currentServicesInitializationState == nextServicesInitializationState) {
            switch (currentServicesInitializationState) {
            case INIT_SERVICES_PORT:
                CHECK(AJ_BusBindSessionPort(&busAttachment, App_ServicePort, NULL));
                nextServicesInitializationState = INIT_ADVERTISE_NAME;
                break;

            case INIT_ADVERTISE_NAME:
                CHECK(AJ_BusAdvertiseName(&busAttachment, AJ_GetUniqueName(&busAttachment), AJ_TRANSPORT_ANY, AJ_BUS_START_ADVERTISING));
                if (addSessionLessMatch)
                    nextServicesInitializationState = INIT_ADDSLMATCH;
                else
                    nextServicesInitializationState = INIT_FINISHED;
                break;

            case INIT_ADDSLMATCH:
                CHECK(AJ_BusSetSignalRule(&busAttachment, SESSIONLESS_MATCH, AJ_BUS_SIGNAL_ALLOW));
                nextServicesInitializationState = INIT_FINISHED;
                break;

            case INIT_FINISHED:
                if (IsShouldAnnounce()) {
                    CHECK(AboutAnnounce(&busAttachment));
                    SetShouldAnnounce(FALSE);
                }
#ifdef ONBOARDING_SERVICE
                if (!OBCAPI_IsWiFiClient() && !OBCAPI_IsWiFiSoftAP()) {
                    status = AJ_ERR_RESTART;
                }
#endif
            }
        }
    }

    if (status == AJ_ERR_RESOURCES) {
        init_retries++;
        if (init_retries > MAX_INIT_RETRIES) {
            status = AJ_ERR_READ; // Force disconnect
        } else {
            AJ_Sleep(AJ_SLEEP_TIME);
        }
    }
    return status;
}

static Service_Status Application_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status* status)
{
    Service_Status serviceStatus = SERVICE_STATUS_NOT_HANDLED;

    switch (currentServicesInitializationState) {
    case INIT_SERVICES_PORT:
        if (msg->msgId == AJ_REPLY_ID(AJ_METHOD_BIND_SESSION_PORT)) {
            currentServicesInitializationState = nextServicesInitializationState;
        }
        break;

    case INIT_ADVERTISE_NAME:
        if (msg->msgId == AJ_REPLY_ID(AJ_METHOD_ADVERTISE_NAME)) {
            currentServicesInitializationState = nextServicesInitializationState;
        }
        break;

    case INIT_ADDSLMATCH:
        if (msg->msgId == AJ_REPLY_ID(AJ_METHOD_ADD_MATCH)) {
            currentServicesInitializationState = nextServicesInitializationState;
        }
        break;

    default:
        break;
    }

    return serviceStatus;
}

static uint8_t Services_CheckSessionAccepted(uint16_t port, uint32_t sessionId, char*joiner)
{
    uint8_t session_accepted = FALSE;
    session_accepted |= (port == App_ServicePort);

#ifdef CONTROLPANEL_SERVICE
    session_accepted |= CPS_CheckSessionAccepted(port, sessionId, joiner);
#endif

    return session_accepted;
}

Service_Status Service_MessageProcessor(AJ_Message* msg, AJ_Status* status)
{
    Service_Status serviceStatus = SERVICE_STATUS_NOT_HANDLED;
    if  (msg->msgId == AJ_METHOD_ACCEPT_SESSION) {
        uint16_t port;
        char* joiner;
        uint32_t sessionId = 0;
        AJ_UnmarshalArgs(msg, "qus", &port, &sessionId, &joiner);
        uint8_t session_accepted = FALSE;

        session_accepted |= Services_CheckSessionAccepted(port, sessionId, joiner);

        *status = AJ_BusReplyAcceptSession(msg, session_accepted);
        AJ_Printf("%s session session_id=%u joiner=%s for port %u\n", (session_accepted ? "Accepted" : "Rejected"), sessionId, joiner, port);

        serviceStatus = SERVICE_STATUS_HANDLED;
    } else {
        if (serviceStatus == SERVICE_STATUS_NOT_HANDLED)
            serviceStatus = Application_MessageProcessor(&busAttachment, msg, status);
        if (serviceStatus == SERVICE_STATUS_NOT_HANDLED)
            serviceStatus = About_MessageProcessor(&busAttachment, msg, status);
#ifdef CONFIG_SERVICE
        if (serviceStatus == SERVICE_STATUS_NOT_HANDLED)
            serviceStatus = Config_MessageProcessor(&busAttachment, msg, status);
#endif
#ifdef ONBOARDING_SERVICE
        if (serviceStatus == SERVICE_STATUS_NOT_HANDLED)
            serviceStatus = Onboarding_MessageProcessor(&busAttachment, msg, status);
#endif
#ifdef NOTIFICATION_SERVICE_PRODUCER
        if (serviceStatus == SERVICE_STATUS_NOT_HANDLED)
            serviceStatus = Producer_MessageProcessor(&busAttachment, msg, status);
#endif
#ifdef NOTIFICATION_SERVICE_CONSUMER
        if (serviceStatus == SERVICE_STATUS_NOT_HANDLED)
            serviceStatus = Consumer_MessageProcessor(&busAttachment, msg, status);
#endif
#ifdef CONTROLPANEL_SERVICE
        if (serviceStatus == SERVICE_STATUS_NOT_HANDLED)
            serviceStatus = CPS_MessageProcessor(&busAttachment, msg, status);
#endif
    }
    return serviceStatus;
}

void Service_IdleConnectedHandler()
{
    About_IdleConnectedHandler(&busAttachment);
#ifdef CONFIG_SERVICE
    Config_IdleConnectedHandler(&busAttachment);
#endif
#ifdef ONBOARDING_SERVICE
    Onboarding_IdleConnectedHandler(&busAttachment);
#endif
#ifdef NOTIFICATION_SERVICE_PRODUCER
    Producer_DoWork(&busAttachment);
#endif
#ifdef NOTIFICATION_SERVICE_CONSUMER
    Consumer_IdleConnectedHandler(&busAttachment);
#endif
#ifdef CONTROLPANEL_SERVICE
    Controllee_DoWork(&busAttachment);
#endif
}

AJ_Status Application_DisconnectHandler(uint8_t restart)
{
    AJ_Status status = AJ_OK;

    if (restart) {
        do {
            CHECK(AJ_BusAdvertiseName(&busAttachment, AJ_GetUniqueName(&busAttachment), AJ_TRANSPORT_ANY, AJ_BUS_STOP_ADVERTISING));
            CHECK(AJ_BusUnbindSession(&busAttachment, App_ServicePort));
        } while (0);
    }

    SetShouldAnnounce(TRUE);
    currentServicesInitializationState = nextServicesInitializationState = INIT_START;
    init_retries = 0;

    return status;
}

void Service_DisconnectHandler()
{
    About_Finish(&busAttachment);
#ifdef CONFIG_SERVICE
    Config_Finish(&busAttachment);
#endif
#ifdef ONBOARDING_SERVICE
    Onboarding_Finish(&busAttachment);
#endif
#ifdef NOTIFICATION_SERVICE_PRODUCER
    Producer_Finish(&busAttachment);
#endif
#ifdef CONTROLPANEL_SERVICE
    CPS_Finish(&busAttachment);
#endif
}

uint8_t Daemon_Disconnect(uint8_t disconnectWiFi)
{
    AJ_Printf("AllJoyn disconnect\n");
    AJ_Sleep(AJ_SLEEP_TIME); // Sleep a little to let any pending requests to daemon to be sent
    AJ_Disconnect(&busAttachment);
#ifdef ONBOARDING_SERVICE
    if (disconnectWiFi) {
        Onboarding_DisconnectedHandler(&busAttachment);
    }
#endif

    return TRUE;
}

uint32_t PasswordCallback(uint8_t* buffer, uint32_t bufLen)
{
    const char* password = PropertyStore_GetValue(Passcode);
    if (password == NULL) {
        AJ_Printf("Password is NULL!\n");
        return 0;
    }
    AJ_Printf("Retrieved password=%s\n", password);
    size_t len = strlen(password);
    if (len > bufLen) {
        AJ_Printf("Password is too long (%lu > %lu) - Truncating!\n", len, (unsigned long)bufLen);
        len = bufLen;
    }
    memcpy(buffer, password, len);
    return len;
}
