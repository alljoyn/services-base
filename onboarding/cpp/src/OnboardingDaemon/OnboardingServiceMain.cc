/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
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

#ifdef _WIN32
/* Disable deprecation warnings */
#pragma warning(disable: 4996)
#endif

#include <stdio.h>
#include <signal.h>
#include <fstream>
#include <map>
#include <stdint.h>
#include <chrono>
#include <thread>

#include <CommonBusListener.h>
#include <CommonSampleUtil.h>
#include <OptParser.h>
#include "ConfigServiceListenerImpl.h"
#include <AJInitializer.h>
#include <alljoyn/services_common/BsSecurity.h>
#include "SecurityUtil.h"

#include <alljoyn/version.h>
#include <qcc/platform.h>
#include <qcc/Log.h>
#include <qcc/Debug.h>
#include <qcc/String.h>

#include <alljoyn/BusObject.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/Status.h>

#include <alljoyn/config/ConfigService.h>

#include <alljoyn/onboarding/Onboarding.h>
#include <alljoyn/onboarding/OnboardingService.h>
#include <OnboardingControllerImpl.h>
#include <alljoyn/services_common/LogModulesNames.h>
#include <alljoyn/AboutIconObj.h>
#include <AboutDataStore.h>
#include <alljoyn/AboutObj.h>


#define QCC_MODULE "ONBOARD"

using namespace ajn;
using namespace services;

#define SERVICE_EXIT_OK 0
#define SERVICE_OPTION_ERROR 1
#define SERVICE_CONFIG_ERROR 2

static const std::chrono::seconds SIGINT_DELAY{100};

static const int BUS_ATTACHMENT_RETRY_MAX{180};
static const std::chrono::seconds BUS_ATTACHMENT_RETRY_DELAY{1};

#if defined(_OPEN_WRT_)
static const char *const KEYSTORE_PATH{"/etc/alljoyn-onboarding/service.keys"};
static const char *const SECURITY_FILE_PREFIX{"/etc/alljoyn-onboarding/security"};
#else
static const char *const KEYSTORE_PATH{".alljoyn_keys/service.keys"};
static const char *const SECURITY_FILE_PREFIX{"security"};
#endif

/** static variables need for sample */
static BusAttachment* msgBus = nullptr;

static ConfigService* configService = nullptr;
static ConfigServiceListenerImpl* configServiceListener = nullptr;

static AboutIcon* icon = nullptr;
static AboutIconObj* aboutIconObj = nullptr;
static AboutDataStore* aboutDataStore = nullptr;
static AboutObj* aboutObj = nullptr;

OnboardingService* onboardingService = nullptr;
static OnboardingControllerImpl* obController = nullptr;

static CommonBusListener* busListener = nullptr;

static SessionPort servicePort = 900;

static volatile sig_atomic_t s_interrupt = false;
static volatile sig_atomic_t s_restart = false;

static void CDECL_CALL SigIntHandler(int sig)
{
    QCC_UNUSED(sig);
    s_interrupt = true;
}

static void daemonDisconnectCB()
{
    s_restart = true;
}

static void cleanup()
{
    if (AboutObjApi::getInstance()) {
        AboutObjApi::DestroyInstance();
    }

    if (configService) {
        delete configService;
        configService = nullptr;
    }

    if (configServiceListener) {
        delete configServiceListener;
        configServiceListener = nullptr;
    }

    if (aboutIconObj) {
        delete aboutIconObj;
        aboutIconObj = nullptr;
    }

    if (icon) {
        delete icon;
        icon = nullptr;
    }

    if (aboutDataStore) {
        delete aboutDataStore;
        aboutDataStore = nullptr;
    }

    if (aboutObj) {
        delete aboutObj;
        aboutObj = nullptr;
    }

    if (onboardingService) {
        delete onboardingService;
        onboardingService = nullptr;
    }

    if (obController) {
        delete obController;
        obController = nullptr;
    }

    if (busListener) {
        if (msgBus) {
            msgBus->UnregisterBusListener(*busListener);
        }
        delete busListener;
        busListener = nullptr;
    }

    /* Clean up msg bus */
    if (msgBus) {
        delete msgBus;
        msgBus = nullptr;
    }
}

/** Advertise the service name, report the result to stdout, and return the status code. */
QStatus AdvertiseName(TransportMask mask)
{
    QStatus status = ER_BUS_ESTABLISH_FAILED;
    if (msgBus->IsConnected() && msgBus->GetUniqueName().size() > 0) {
        status = msgBus->AdvertiseName(msgBus->GetUniqueName().c_str(), mask);
        QCC_DbgHLPrintf(("AdvertiseName for %s = %s", msgBus->GetUniqueName().c_str(), QCC_StatusText(status)));
    }
    return status;
}

void WaitForSigInt(void)
{
    while (s_interrupt == false && s_restart == false) {
        std::this_thread::sleep_for(SIGINT_DELAY);
    }
}

int main(int argc, char** argv, char** envArg)
{
    QCC_UNUSED(envArg);
    // Initialize AllJoyn
    AJInitializer ajInit;
    QStatus status = ajInit.Initialize();
    if (status != ER_OK) {
        return 1;
    }

    QCC_SetLogLevels("ONBOARD=15;");
    QCC_SetDebugLevel("ONBOARD", 15);

//    QCC_SetLogLevels("PERMISSION_MGMT=15;AUTH_KEY_EXCHANGER=15;");
//    QCC_SetDebugLevel("PERMISSION_MGMT", 15);
//    QCC_SetDebugLevel("AUTH_KEY_EXCHANGER", 15);
//
    QCC_LogMsg(("Using %s", ajn::GetBuildInfo()));

    /* Parse options */
    OptParser opts(argc, argv);
    OptParser::ParseResultCode parseCode(opts.ParseResult());
    switch (parseCode) {
    case OptParser::PR_OK:
        break;

    case OptParser::PR_EXIT_NO_ERROR:
        return SERVICE_EXIT_OK;

    default:
        return SERVICE_OPTION_ERROR;
    }

    QCC_LogMsg(("Using Port %d", servicePort));

    if (!opts.GetConfigFile().empty()) {
        QCC_LogMsg(("Using Config at %s", opts.GetConfigFile().c_str()));
    }

    /* Install SIGINT handler so Ctrl + C deallocates memory properly */
    signal(SIGINT, SigIntHandler);

start:
    QCC_DbgTrace(("Starting Daemon"));

    /* Connect to the daemon */
    uint16_t retry = 0;

    do {
        msgBus = CommonSampleUtil::prepareBusAttachment(nullptr);

        if (msgBus == nullptr) {
            retry++;
            QCC_LogError(ER_NONE, ("BusAttachment init failed; retrying [%d,%d]", retry, BUS_ATTACHMENT_RETRY_MAX));
            std::this_thread::sleep_for(BUS_ATTACHMENT_RETRY_DELAY);
        }
    } while (msgBus == nullptr && retry < BUS_ATTACHMENT_RETRY_MAX && !s_interrupt);

    if (msgBus == nullptr) {
        QCC_LogError(ER_NONE, ("BusAttachment init failed after %d attempts; aborting.", BUS_ATTACHMENT_RETRY_MAX));
        cleanup();
        return 1;
    }

    BsSecurity security{*msgBus};
    security.SetPassword(opts.GetAuthPassword());
    if (!opts.GetAuthMechanism().empty()) {
        security.SetMechanism(opts.GetAuthMechanism());
    }
    security.Enable(KEYSTORE_PATH, true);

    {
        status = msgBus->GetPermissionConfigurator().SetClaimCapabilities(
            ajn::PermissionConfigurator::CAPABLE_ECDHE_SPEKE
            | ajn::PermissionConfigurator::CAPABLE_ECDHE_ECDSA
            | ajn::PermissionConfigurator::CAPABLE_ECDHE_PSK
            | ajn::PermissionConfigurator::CAPABLE_ECDHE_NULL
            );
        if (status != ER_OK) {
            QCC_LogError(ER_NONE, ("Failed to SetClaimCapabilities. (%s)", QCC_StatusText(status)));
        }

        PermissionConfigurator& pc = msgBus->GetPermissionConfigurator();
        PermissionConfigurator::ApplicationState appState;
        pc.GetApplicationState(appState);
        QCC_DbgHLPrintf(("Application is %s", PermissionConfigurator::ToString(appState)));

        if (PermissionConfigurator::ApplicationState::CLAIMABLE != appState &&
            PermissionConfigurator::ApplicationState::CLAIMED != appState) {
            Manifest manifest;
            SecurityUtil::GenerateManifest(manifest);

            std::vector<PermissionPolicy::Rule> rules = manifest->GetRules();
            pc.SetPermissionManifestTemplate(&rules[0], rules.size());

            pc.GetApplicationState(appState);
            QCC_DbgHLPrintf(("Application is %s", PermissionConfigurator::ToString(appState)));
        }
    }

    busListener = new CommonBusListener(msgBus, daemonDisconnectCB);
    busListener->setSessionPort(servicePort);

    aboutDataStore = new AboutDataStore(opts.GetFactoryConfigFile().c_str(), opts.GetConfigFile().c_str());
    aboutDataStore->SetOBCFG();
    aboutDataStore->Initialize(opts.GetDeviceId(), opts.GetAppId());

    aboutObj = new ajn::AboutObj(*msgBus, BusObject::ANNOUNCED);
    status = CommonSampleUtil::prepareAboutService(msgBus, static_cast<AboutData*>(aboutDataStore), aboutObj, busListener, servicePort);
    if (status != ER_OK) {
        QCC_LogError(ER_NONE, ("AboutService init failed; aborting."));
        cleanup();
        return 1;
    }

    AboutObjApi* aboutObjApi = AboutObjApi::getInstance();
    if (!aboutObjApi) {
        QCC_LogError(ER_NONE, ("AboutObjApi init failed; aborting."));
        cleanup();
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //aboutIconService
    uint8_t aboutIconContent[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x0A,
                                   0x00, 0x00, 0x00, 0x0A, 0x08, 0x02, 0x00, 0x00, 0x00, 0x02, 0x50, 0x58, 0xEA, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x00, 0xAF,
                                   0xC8, 0x37, 0x05, 0x8A, 0xE9, 0x00, 0x00, 0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6F, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x41, 0x64,
                                   0x6F, 0x62, 0x65, 0x20, 0x49, 0x6D, 0x61, 0x67, 0x65, 0x52, 0x65, 0x61, 0x64, 0x79, 0x71, 0xC9, 0x65, 0x3C, 0x00, 0x00, 0x00, 0x18, 0x49, 0x44,
                                   0x41, 0x54, 0x78, 0xDA, 0x62, 0xFC, 0x3F, 0x95, 0x9F, 0x01, 0x37, 0x60, 0x62, 0xC0, 0x0B, 0x46, 0xAA, 0x34, 0x40, 0x80, 0x01, 0x00, 0x06, 0x7C,
                                   0x01, 0xB7, 0xED, 0x4B, 0x53, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82 };

    qcc::String mimeType("image/png");
    icon = new ajn::AboutIcon();
    status = icon->SetContent(mimeType.c_str(), aboutIconContent, sizeof(aboutIconContent) / sizeof(*aboutIconContent));
    if (ER_OK != status) {
        QCC_LogError(ER_NONE, ("AboutIcon init failed."));
    }

    aboutIconObj = new ajn::AboutIconObj(*msgBus, *icon);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //OnboardingService

    ajn::MsgArg* arg;
    char* ScanFile, * StateFile, * ErrorFile, * ConfigureCmd, * ConnectCmd, * OffboardCmd, * ScanCmd;
    aboutDataStore->GetField("scan_file", arg);
    arg->Get("s", &ScanFile);
    aboutDataStore->GetField("state_file", arg);
    arg->Get("s", &StateFile);
    aboutDataStore->GetField("error_file", arg);
    arg->Get("s", &ErrorFile);
    aboutDataStore->GetField("connect_cmd", arg);
    arg->Get("s", &ConnectCmd);
    aboutDataStore->GetField("offboard_cmd", arg);
    arg->Get("s", &OffboardCmd);
    aboutDataStore->GetField("configure_cmd", arg);
    arg->Get("s", &ConfigureCmd);
    aboutDataStore->GetField("scan_cmd", arg);
    arg->Get("s", &ScanCmd);

    obController = new OnboardingControllerImpl(ScanFile,
                                                StateFile,
                                                ErrorFile,
                                                ConfigureCmd,
                                                ConnectCmd,
                                                OffboardCmd,
                                                ScanCmd,
                                                (OBConcurrency)0,
                                                *msgBus);
    onboardingService = new OnboardingService(*msgBus, *obController);

    status = onboardingService->Register();
    if (status != ER_OK) {
        QCC_LogError(ER_NONE, ("OnboardingService init failed; aborting."));
        cleanup();
        return 1;
    }

    status = msgBus->RegisterBusObject(*onboardingService);
    if (status != ER_OK) {
        QCC_LogError(ER_NONE, ("OnboardingService bus registration failed; aborting."));
        cleanup();
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //ConfigService

    configServiceListener = new ConfigServiceListenerImpl(*aboutDataStore, *msgBus, *busListener, *obController);
    configService = new ConfigService(*msgBus, *aboutDataStore, *configServiceListener);

    status = configService->Register();
    if (status != ER_OK) {
        QCC_LogError(ER_NONE, ("ConfigService init failed; aborting."));
        cleanup();
        return 1;
    }

    status = msgBus->RegisterBusObject(*configService);
    if (status != ER_OK) {
        QCC_LogError(ER_NONE, ("ConfigService bus registration failed; aborting."));
        cleanup();
        return 1;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const TransportMask SERVICE_TRANSPORT_TYPE = TRANSPORT_ANY;

    if (ER_OK == status) {
        status = AdvertiseName(SERVICE_TRANSPORT_TYPE);
    }

    if (ER_OK == status) {
        status = aboutObjApi->Announce();
    }

    /* Perform the service asynchronously until the user signals for an exit. */
    if (ER_OK == status) {
        WaitForSigInt();
    }

    cleanup();

    if (s_restart) {
        s_restart = false;
        goto start;
    }

    return 0;
} /* main() */
