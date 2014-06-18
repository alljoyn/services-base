/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
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

#include <iostream>
#include <iomanip>
#include <signal.h>
#include <set>

#include <alljoyn/BusAttachment.h>

#include <alljoyn/about/AboutIconClient.h>
#include <alljoyn/about/AnnouncementRegistrar.h>
#include <alljoyn/config/ConfigClient.h>
#include <alljoyn/onboarding/OnboardingClient.h>

#include <SrpKeyXListener.h>
#include <AnnounceHandlerImpl.h>
#include <AsyncSessionJoiner.h>
#include <SessionListenerImpl.h>
#include <OnboardingSignalListenerImpl.h>
#include <alljoyn/services_common/LogModulesNames.h>

using namespace ajn;
using namespace services;

static volatile sig_atomic_t quit = 0;
static BusAttachment* busAttachment = NULL;
static std::set<qcc::String> handledAnnouncements;

static void SignalHandler(int sig) {
    switch (sig) {
    case SIGINT:
    case SIGTERM:
        quit = 1;
        break;
    }
}

void PrintAboutData(AboutClient::AboutData& aboutData)
{
    for (AboutClient::AboutData::iterator itx = aboutData.begin(); itx != aboutData.end(); ++itx) {
        qcc::String key = itx->first;
        ajn::MsgArg value = itx->second;
        if (value.typeId == ALLJOYN_STRING) {
            std::cout << "Key name=" << key.c_str() << " value=" << value.v_string.str << std::endl;
        } else if (value.typeId == ALLJOYN_ARRAY && value.Signature().compare("as") == 0) {
            std::cout << "Key name=" << key.c_str() << " values: ";
            const MsgArg* stringArray;
            size_t fieldListNumElements;
            value.Get("as", &fieldListNumElements, &stringArray);
            for (unsigned int i = 0; i < fieldListNumElements; i++) {
                char* tempString;
                stringArray[i].Get("s", &tempString);
                std::cout << tempString << " ";
            }
            std::cout << std::endl;
        } else if (value.typeId == ALLJOYN_BYTE_ARRAY) {
            std::cout << "Key name=" << key.c_str() << " value=" << std::hex << std::uppercase << std::setfill('0');
            uint8_t* AppIdBuffer;
            size_t numElements;
            value.Get("ay", &numElements, &AppIdBuffer);
            for (size_t i = 0; i < numElements; i++) {
                std::cout <<  std::setw(2) << (unsigned int)AppIdBuffer[i];
            }
            std::cout << std::nouppercase << std::dec << std::endl;
        }
    }
}

void sessionJoinedCallback(qcc::String const& busName, SessionId id)
{
    QStatus status = ER_OK;
    if (busAttachment == NULL) {
        return;
    }

    busAttachment->EnableConcurrentCallbacks();

    AboutClient* aboutClient = new AboutClient(*busAttachment);

    AboutIconClient* iconClient = NULL;
    bool isIconInterface = false;
    bool isConfigInterface = false;
    bool isOnboardingInterface = false;

    std::cout << std::endl << busName.c_str() << " AboutClient ObjectDescriptions" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    AboutClient::ObjectDescriptions ObjectDescriptionsRefill;
    status = aboutClient->GetObjectDescriptions(busName.c_str(), ObjectDescriptionsRefill, id);

    if (status != ER_OK) {
        std::cout << "Call to getObjectDescriptions failed: " << QCC_StatusText(status) << std::endl;
    } else {
        for (AboutClient::ObjectDescriptions::const_iterator it = ObjectDescriptionsRefill.begin(); it != ObjectDescriptionsRefill.end(); ++it) {
            qcc::String key = it->first;
            std::vector<qcc::String> vector = it->second;
            std::cout << "key=" << key.c_str();
            for (std::vector<qcc::String>::const_iterator itv = vector.begin(); itv != vector.end(); ++itv) {
                if (key.compare("/About/DeviceIcon") == 0 && itv->compare("org.alljoyn.Icon") == 0) {
                    isIconInterface = true;
                }
                if (key.compare("/Config") == 0 && itv->compare("org.alljoyn.Config") == 0) {
                    isConfigInterface = true;
                }
                if (key.compare("/Onboarding") == 0 && itv->compare("org.alljoyn.Onboarding") == 0) {
                    isOnboardingInterface = true;
                }
                std::cout << " value=" << itv->c_str();
            }
            std::cout << std::endl;
        }
    }

    std::cout << std::endl << busName.c_str() << " AboutClient AboutData" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    AboutClient::AboutData aboutData;
    status = aboutClient->GetAboutData(busName.c_str(), "en", aboutData);
    if (status != ER_OK) {
        std::cout << "Call to getAboutData failed: "  << QCC_StatusText(status) << std::endl;
    } else {
        PrintAboutData(aboutData);
    }

    std::cout << std::endl << busName.c_str() << " AboutClient GetVersion" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    int ver = 0;
    status = aboutClient->GetVersion(busName.c_str(), ver, id);
    if (status != ER_OK) {
        std::cout << "Call to getVersion failed " << QCC_StatusText(status) << std::endl;
    } else {
        std::cout << "Version=" << ver << std::endl;
    }

    if (isIconInterface) {
        iconClient = new AboutIconClient(*busAttachment);

        size_t contentSize;

        std::cout << std::endl << busName.c_str() << " AboutIcontClient GetUrl" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        qcc::String url;
        status = iconClient->GetUrl(busName.c_str(), url, id);
        if (status != ER_OK) {
            std::cout << "Call to getUrl failed: " << QCC_StatusText(status) << std::endl;
        } else {
            std::cout << "url=" << url.c_str() << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " AboutIcontClient GetContent" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        uint8_t* content = NULL;
        status = iconClient->GetContent(busName.c_str(), &content, contentSize, id);

        if (status != ER_OK) {
            std::cout << "Call to GetContent failed: " << QCC_StatusText(status) << std::endl;
        } else {
            std::cout << "Content size=" << contentSize << std::endl;
            std::cout << "Content :\t";
            for (size_t i = 0; i < contentSize; i++) {
                if (i % 8 == 0 && i > 0) {
                    std::cout << "\n\t\t";
                }
                std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (unsigned int)content[i]
                          << std::nouppercase << std::dec;
            }
            std::cout << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " AboutIcontClient GetVersion" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        status = iconClient->GetVersion(busName.c_str(), ver, id);
        if (status != ER_OK) {
            std::cout << "Call to getVersion failed: " << QCC_StatusText(status) << std::endl;
        } else {
            std::cout << "Version=" << ver << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " AboutIcontClient GetMimeType" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        qcc::String mimetype;

        status = iconClient->GetMimeType(busName.c_str(), mimetype, id);
        if (status != ER_OK) {
            std::cout << "Call to getMimetype failed: " << QCC_StatusText(status) << std::endl;
        } else {
            std::cout << "Mimetype=" << mimetype.c_str() << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " AboutIcontClient GetSize" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        status = iconClient->GetSize(busName.c_str(), contentSize, id);
        if (status != ER_OK) {
            std::cout << "Call to getSize failed: " << QCC_StatusText(status) << std::endl;
        } else {
            std::cout << "Size=" << contentSize << std::endl;
        }
    }

    ConfigClient* configClient = NULL;
    if (isConfigInterface) {
        configClient = new ConfigClient(*busAttachment);

        std::cout << std::endl << busName.c_str() << " ConfigClient GetVersion" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        int version = 0;
        if ((status = configClient->GetVersion(busName.c_str(), version, id)) == ER_OK) {
            std::cout << "Success GetVersion. Version=" << version << std::endl;
        } else {
            std::cout << "Call to getVersion failed: " << QCC_StatusText(status) << std::endl;
        }

        ConfigClient::Configurations configurations;
        std::cout << std::endl << busName.c_str() << " ConfigClient GetConfigurations" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        if ((status = configClient->GetConfigurations(busName.c_str(), "en", configurations, id)) == ER_OK) {

            for (ConfigClient::Configurations::iterator it = configurations.begin(); it != configurations.end(); ++it) {
                qcc::String key = it->first;
                ajn::MsgArg value = it->second;
                if (value.typeId == ALLJOYN_STRING) {
                    std::cout << "Key name=" << key.c_str() << " value=" << value.v_string.str << std::endl;
                } else if (value.typeId == ALLJOYN_ARRAY && value.Signature().compare("as") == 0) {
                    std::cout << "Key name=" << key.c_str() << " values: ";
                    const MsgArg* stringArray;
                    size_t fieldListNumElements;
                    status = value.Get("as", &fieldListNumElements, &stringArray);
                    for (unsigned int i = 0; i < fieldListNumElements; i++) {
                        char* tempString;
                        stringArray[i].Get("s", &tempString);
                        std::cout << tempString << " ";
                    }
                    std::cout << std::endl;
                }
            }
        } else {
            std::cout << std::endl << "Call to GetConfigurations failed: " << QCC_StatusText(status) << std::endl;
        }
    }

    OnboardingClient* onboardingClient = NULL;
    OnboardingSignalListenerImpl* signalListener = new OnboardingSignalListenerImpl();

    if (isOnboardingInterface) {
        onboardingClient = new    OnboardingClient(*busAttachment, *signalListener);

        std::cout << std::endl << busName.c_str() << " OnboardingClient GetVersion" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        int version = 0;
        if ((status = onboardingClient->GetVersion(busName.c_str(), version, id)) == ER_OK) {
            std::cout << "Version=" << version << std::endl;
        } else {
            std::cout << "Call to GetVersion failed " << QCC_StatusText(status) << std::endl;
        }


        std::cout << std::endl << busName.c_str() << " OnboardingClient GetState" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        short int state = 0;
        if ((status = onboardingClient->GetState(busName.c_str(), state, id)) == ER_OK) {
            std::cout << "GetState=" << state << std::endl;
        } else {
            std::cout << "Call to GetState failed " << QCC_StatusText(status) << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " OnboardingClient GetLastError" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        OBLastError lastError = { 0, "" };
        if ((status = onboardingClient->GetLastError(busName.c_str(), lastError, id)) == ER_OK) {
            std::cout << "OBLastError code=" << lastError.validationState << " message= " << lastError.message.c_str() << std::endl;
        } else {
            std::cout << "Call to GetLastError failed " << QCC_StatusText(status) << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " OnboardingClient GetScanInfo" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        unsigned short age = 0;
        OnboardingClient::ScanInfos scanInfos;
        if ((status = onboardingClient->GetScanInfo(busName.c_str(), age, scanInfos, id)) == ER_OK) {
            for (OnboardingClient::ScanInfos::iterator it = scanInfos.begin(); it != scanInfos.end(); ++it) {
                std::cout << "Network  SSID=" << it->SSID.c_str() << " authType=" << it->authType << std::endl;
            }
        } else {
            std::cout << "Call to GetScanInfo failed " << QCC_StatusText(status) << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " OnboardingClient ConfigureWiFi" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        OBInfo oBInfo;
        oBInfo.SSID.assign("MyWifi");
        oBInfo.passcode.assign("41424344454647484950");
        oBInfo.authType = WPA2_TKIP;

        short resultStatus;

        if ((status = onboardingClient->ConfigureWiFi(busName.c_str(), oBInfo, resultStatus, id)) == ER_OK) {
            std::cout << "Call to ConfigureWiFi succeeded " << std::endl;
        } else {
            std::cout << "Call to ConfigureWiFi failed " << QCC_StatusText(status) << std::endl;
        }

        if ((status = onboardingClient->ConnectTo(busName.c_str(), id)) == ER_OK) {
            std::cout << "Call to ConnectTo succeeded " << std::endl;
        } else {
            std::cout << "Call to ConnectTo failed " << QCC_StatusText(status) << std::endl;
        }

        if ((status = onboardingClient->OffboardFrom(busName.c_str(), id)) == ER_OK) {
            std::cout << "Call to OffboardFrom succeeded " << std::endl;
        } else {
            std::cout << "Call to OffboardFrom failed " << QCC_StatusText(status) << std::endl;
        }

    }

    status = busAttachment->LeaveSession(id);
    std::cout << "Leaving session id = " << id << " with " << busName.c_str() << " status: " << QCC_StatusText(status) << std::endl;
    if (aboutClient) {
        delete aboutClient;
        aboutClient = NULL;
    }

    if (iconClient) {
        delete iconClient;
        iconClient = NULL;
    }

    if (configClient) {
        delete configClient;
        configClient = NULL;
    }

    if (onboardingClient) {
        delete onboardingClient;
        onboardingClient = NULL;
    }

    if (signalListener) {
        delete signalListener;
        signalListener = NULL;
    }
}

void announceHandlerCallback(qcc::String const& busName, unsigned short port)
{
    std::set<qcc::String>::iterator searchIterator = handledAnnouncements.find(qcc::String(busName));
    if (searchIterator == handledAnnouncements.end()) {
        handledAnnouncements.insert(busName);

        SessionOpts opts(SessionOpts::TRAFFIC_MESSAGES, false, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);
        SessionListenerImpl* sessionListener = new SessionListenerImpl(busName);
        AsyncSessionJoiner* joincb = new AsyncSessionJoiner(busName.c_str(), sessionJoinedCallback);

        QStatus status = busAttachment->JoinSessionAsync(busName.c_str(), port, sessionListener, opts, joincb,
                                                         sessionListener);

        if (status != ER_OK) {
            std::cout << "Unable to JoinSession with " << busName.c_str() << std::endl;
        }
    } else {
        std::cout << busName.c_str()  << " has already been handled" << std::endl;
    }
}

int main(int argc, char**argv, char**envArg) {
    QStatus status = ER_OK;
    std::cout << "AllJoyn Library version: " << ajn::GetVersion() << std::endl;
    std::cout << "AllJoyn Library build info: " << ajn::GetBuildInfo() << std::endl;
    QCC_SetLogLevels("ALLJOYN_ABOUT_ANNOUNCE_HANDLER=7");
    QCC_SetLogLevels("ALLJOYN_ABOUT_CLIENT=7");
    QCC_SetLogLevels("ALLJOYN_ABOUT_ICON_CLIENT=7");
    QCC_SetDebugLevel(logModules::CONFIG_MODULE_LOG_NAME, logModules::ALL_LOG_LEVELS);
    QCC_SetDebugLevel(logModules::ONBOARDING_MODULE_LOG_NAME, logModules::ALL_LOG_LEVELS);

    struct sigaction act, oldact;
    sigset_t sigmask, waitmask;

    // Block all signals by default for all threads.
    sigfillset(&sigmask);
    sigdelset(&sigmask, SIGSEGV);
    pthread_sigmask(SIG_BLOCK, &sigmask, NULL);

    // Setup a handler for SIGINT and SIGTERM
    act.sa_handler = SignalHandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGINT, &act, &oldact);
    sigaction(SIGTERM, &act, &oldact);

    //set Daemon password only for bundled app
    #ifdef QCC_USING_BD
    PasswordManager::SetCredentials("ALLJOYN_PIN_KEYX", "000000");
    #endif

    busAttachment = new BusAttachment("OnboardingClient", true);

    status = busAttachment->Start();
    if (status == ER_OK) {
        std::cout << "BusAttachment started." << std::endl;
    } else {
        std::cout << "Unable to start BusAttachment. Status: " << QCC_StatusText(status) << std::endl;
        return 1;
    }

    status = busAttachment->Connect();
    if (ER_OK == status) {
        std::cout << "Daemon Connect succeeded." << std::endl;
    } else {
        std::cout << "Failed to connect daemon. Status: " << QCC_StatusText(status) << std::endl;
        return 1;
    }

    SrpKeyXListener* srpKeyXListener = new SrpKeyXListener();
    status = busAttachment->EnablePeerSecurity("ALLJOYN_SRP_KEYX ALLJOYN_PIN_KEYX ALLJOYN_ECDHE_PSK", srpKeyXListener, "/.alljoyn_keystore/central.ks", true);

    const char* interfaces[] = { "org.alljoyn.Onboarding" };
    AnnounceHandlerImpl* announceHandler = new AnnounceHandlerImpl(announceHandlerCallback);
    AnnouncementRegistrar::RegisterAnnounceHandler(*busAttachment, *announceHandler, interfaces, 1);

    // Setup signals to wait for.
    sigfillset(&waitmask);
    sigdelset(&waitmask, SIGINT);
    sigdelset(&waitmask, SIGTERM);

    while (!quit) {
        // Wait for a signal.
        sigsuspend(&waitmask);
    }

    AnnouncementRegistrar::UnRegisterAnnounceHandler(*busAttachment, *announceHandler, interfaces, 1);

    busAttachment->Stop();
    delete busAttachment;
    delete srpKeyXListener;
    delete announceHandler;

    return 0;
} /* main() */
