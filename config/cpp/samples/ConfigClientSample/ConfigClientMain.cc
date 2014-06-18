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

#include <stdio.h>
#include <signal.h>
#include <set>

#include <alljoyn/about/AnnouncementRegistrar.h>
#include <alljoyn/config/ConfigClient.h>

#include <SrpKeyXListener.h>
#include <AnnounceHandlerImpl.h>
#include <AsyncSessionJoiner.h>
#include <SessionListenerImpl.h>
#include <iostream>
#include <iomanip>
#include <alljoyn/services_common/LogModulesNames.h>

using namespace ajn;
using namespace services;

#define INITIAL_PASSCODE "000000"
#define NEW_PASSCODE "12345678"

static volatile sig_atomic_t quit;
static BusAttachment* busAttachment = 0;
static SrpKeyXListener* srpKeyXListener = 0;
static std::set<qcc::String> handledAnnouncements;

static void SignalHandler(int sig)
{
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
            //QStatus status =
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

    busAttachment->EnableConcurrentCallbacks();
    bool isIconInterface = false;
    AboutIconClient* iconClient = NULL;
    bool isConfigInterface = false;
    AboutClient* aboutClient = new AboutClient(*busAttachment);
    if (aboutClient) {
        std::cout << std::endl << busName.c_str() << " AboutClient ObjectDescriptions" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        AboutClient::ObjectDescriptions ObjectDescriptionsRefill;
        status = aboutClient->GetObjectDescriptions(busName.c_str(), ObjectDescriptionsRefill, id);

        if (status != ER_OK) {
            std::cout << "Call to getObjectDescriptions failed: " << QCC_StatusText(status) << std::endl;
        } else {
            for (AboutClient::ObjectDescriptions::const_iterator it = ObjectDescriptionsRefill.begin();
                 it != ObjectDescriptionsRefill.end(); ++it) {
                qcc::String key = it->first;
                std::vector<qcc::String> vector = it->second;
                std::cout << "key=" << key.c_str();
                for (std::vector<qcc::String>::const_iterator itv = vector.begin(); itv != vector.end();
                     ++itv) {
                    if (key.compare("/About/DeviceIcon") == 0 && itv->compare("org.alljoyn.Icon") == 0) {
                        isIconInterface = true;
                    }
                    if (key.compare("/Config") == 0 && itv->compare("org.alljoyn.Config") == 0) {
                        isConfigInterface = true;
                    }

                    std::cout << " value=" << itv->c_str();
                }
                std::cout << std::endl;
            }
        }

        std::cout << std::endl << busName.c_str() << " AboutClient AboutData Get Supported Languages" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        AboutClient::AboutData aboutDataRefill;

        std::vector<qcc::String> supportedLanguages;
        status = aboutClient->GetAboutData(busName.c_str(), NULL, aboutDataRefill);
        if (status != ER_OK) {
            std::cout << "Call to getAboutData failed: "  << QCC_StatusText(status) << std::endl;
        } else {
            AboutClient::AboutData::iterator search = aboutDataRefill.find("SupportedLanguages");
            if (search != aboutDataRefill.end()) {
                const MsgArg* stringArray;
                size_t fieldListNumElements;
                status = search->second.Get("as", &fieldListNumElements, &stringArray);
                for (unsigned int i = 0; i < fieldListNumElements; i++) {
                    char* tempString;
                    stringArray[i].Get("s", &tempString);
                    supportedLanguages.push_back(tempString);
                }
            }
        }

        for (std::vector<qcc::String>::iterator it = supportedLanguages.begin(); it != supportedLanguages.end();
             ++it) {
            std::cout << std::endl << busName.c_str() << " AboutClient AboutData using language=" << it->c_str() << std::endl;
            std::cout << "-----------------------------------" << std::endl;
            AboutClient::AboutData aboutDataRefill;
            status = aboutClient->GetAboutData(busName.c_str(), it->c_str(), aboutDataRefill);
            if (status != ER_OK) {
                std::cout << "Call to getAboutData failed: "  << QCC_StatusText(status) << std::endl;
            } else {
                PrintAboutData(aboutDataRefill);
            }
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
    } //if (aboutClient)

    if (isIconInterface) {
        iconClient = new AboutIconClient(*busAttachment);
        if (iconClient) {
            std::cout << std::endl << busName.c_str() << " AboutIcontClient GetUrl" << std::endl;
            std::cout << "-----------------------------------" << std::endl;

            size_t contentSize = 0;
            qcc::String url;
            status = iconClient->GetUrl(busName.c_str(), url, id);
            if (status != ER_OK) {
                std::cout << "Call to getUrl failed: " << QCC_StatusText(status) << std::endl;
            } else {
                std::cout << "url=" << url.c_str() << std::endl;
            }

            std::cout << std::endl << busName.c_str() << " AboutIcontClient GetContent" << std::endl;
            std::cout << "-----------------------------------" << std::endl;

            uint8_t*content = NULL;
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

            int ver;
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
        } //if (iconClient)
    } // if (isIconInterface)

    ConfigClient* configClient = NULL;
    if (isConfigInterface) {
        configClient = new ConfigClient(*busAttachment);
        if (configClient) {
            std::cout << std::endl << busName.c_str() << " ConfigClient GetVersion" << std::endl;
            std::cout << "-----------------------------------" << std::endl;
            int version;
            if ((status = configClient->GetVersion(busName.c_str(), version, id)) == ER_OK) {
                std::cout << "Success GetVersion. Version=" << version << std::endl;
            } else {
                std::cout << "Call to getVersion failed: " << QCC_StatusText(status) << std::endl;
            }

            ConfigClient::Configurations configurations;
            std::cout << std::endl << busName.c_str() << " ConfigClient GetConfigurations" << std::endl;
            std::cout << "-----------------------------------" << std::endl;

            if ((status = configClient->GetConfigurations(busName.c_str(), "en", configurations, id))
                == ER_OK) {

                for (ConfigClient::Configurations::iterator it = configurations.begin();
                     it != configurations.end(); ++it) {
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

            std::cout << std::endl << busName.c_str() << " ConfigClient Restart" << std::endl;
            std::cout << "-----------------------------------" << std::endl;

            if ((status = configClient->Restart(busName.c_str(), id)) == ER_OK) {
                std::cout << "Restart succeeded" << std::endl;
            } else {
                std::cout << "Call to Restart failed: " << QCC_StatusText(status) << std::endl;
            }

            std::cout << std::endl << busName.c_str() << " ConfigClient UpdateConfigurations" << std::endl;
            std::cout << "-----------------------------------" << std::endl;

            ConfigClient::Configurations updateConfigurations;
            updateConfigurations.insert(
                std::pair<qcc::String, ajn::MsgArg>("DeviceName",
                                                    MsgArg("s", "This is my new English name ! ! ! !")));

            if ((status = configClient->UpdateConfigurations(busName.c_str(), "en", updateConfigurations, id)) == ER_OK) {
                std::cout << "UpdateConfigurations succeeded" << std::endl;
            } else {
                std::cout << "Call to UpdateConfigurations failed: " << QCC_StatusText(status) << std::endl;
            }
            usleep(3000 * 1000);

            {
                ConfigClient::Configurations updateConfigurations;
                updateConfigurations.insert(
                    std::pair<qcc::String, ajn::MsgArg>("DefaultLanguage", MsgArg("s", "es")));
                if ((status = configClient->UpdateConfigurations(busName.c_str(), NULL, updateConfigurations,
                                                                 id)) == ER_OK) {
                    std::cout << "UpdateConfigurations succeeded" << std::endl;
                } else {
                    std::cout << "Call to UpdateConfigurations failed: " << QCC_StatusText(status) << std::endl;
                }
            }
            usleep(3000 * 1000);

            {
                std::cout << std::endl << busName.c_str() << " AboutClient AboutData After update 'DeviceName','DefaultLanguage'" << std::endl;
                std::cout << "-----------------------------------" << std::endl;

                AboutClient::AboutData aboutDataRefill;
                status = aboutClient->GetAboutData(busName.c_str(), "en", aboutDataRefill);
                if (status != ER_OK) {
                    std::cout << "Call to getAboutData failed: " << QCC_StatusText(status) << std::endl;
                } else {
                    PrintAboutData(aboutDataRefill);
                }
            }

            std::vector<qcc::String> configNames;
            configNames.push_back("DeviceName");

            std::cout << std::endl << busName.c_str() << " ConfigClient ResetConfigurations" << std::endl;
            std::cout << "-----------------------------------" << std::endl;

            if ((status = configClient->ResetConfigurations(busName.c_str(), "en", configNames, id)) == ER_OK) {
                std::cout << "ResetConfigurations succeeded" << std::endl;
            } else {
                std::cout << "Call to ResetConfigurations failed: " << QCC_StatusText(status) << std::endl;
            }

            usleep(3000 * 1000);

            {
                std::cout << std::endl << busName.c_str() << " AboutClient AboutData After ResetConfigurations 'DeviceName'" << std::endl;
                std::cout << "-----------------------------------" << std::endl;

                AboutClient::AboutData aboutDataRefill;
                status = aboutClient->GetAboutData(busName.c_str(), "en", aboutDataRefill);
                if (status != ER_OK) {
                    std::cout << "Call to getAboutData failed: " << QCC_StatusText(status) << std::endl;
                } else {
                    PrintAboutData(aboutDataRefill);
                }
            }

            std::cout << std::endl << busName.c_str() << " ConfigClient SetPasscode" << std::endl;
            std::cout << "-----------------------------------" << std::endl;
            if ((status = configClient->SetPasscode(busName.c_str(), "MyDeamonRealm", 8,
                                                    (const uint8_t*) NEW_PASSCODE, id)) == ER_OK) {
                std::cout << "SetPasscode succeeded" << std::endl;
                srpKeyXListener->setPassCode(NEW_PASSCODE);
                qcc::String guid;
                status = busAttachment->GetPeerGUID(busName.c_str(), guid);
                if (status == ER_OK) {

                    status = busAttachment->ClearKeys(guid);
                    std::cout << "busAttachment->ClearKey for " << guid.c_str() << ". Status: " << QCC_StatusText(status) << std::endl;
                }
            } else {
                std::cout << "Call to SetPasscode failed: " << QCC_StatusText(status) << std::endl;
            }

            {
                std::cout << std::endl << busName.c_str() << " AboutClient AboutData Before FactoryReset" << std::endl;
                std::cout << "-----------------------------------" << std::endl;

                AboutClient::AboutData aboutDataRefill;
                status = aboutClient->GetAboutData(busName.c_str(), "en", aboutDataRefill);
                if (status != ER_OK) {
                    std::cout << "Call to getAboutData failed: " << QCC_StatusText(status) << std::endl;
                } else {
                    PrintAboutData(aboutDataRefill);
                }
            }

            std::cout << std::endl << busName.c_str() << " ConfigClient FactoryReset" << std::endl;
            std::cout << "-----------------------------------" << std::endl;

            if ((status = configClient->FactoryReset(busName.c_str(), id)) == ER_OK) {
                std::cout << "FactoryReset succeeded" << std::endl;
                srpKeyXListener->setPassCode(INITIAL_PASSCODE);
                qcc::String guid;
                status = busAttachment->GetPeerGUID(busName.c_str(), guid);
                if (status == ER_OK) {
                    busAttachment->ClearKeys(guid);
                }

            } else {
                std::cout << "Call to FactoryReset failed: " << QCC_StatusText(status) << std::endl;
            }

            {
                std::cout << std::endl << busName.c_str() << " AboutClient AboutData After FactoryReset" << std::endl;
                std::cout << "-----------------------------------" << std::endl;

                AboutClient::AboutData aboutDataRefill;
                status = aboutClient->GetAboutData(busName.c_str(), "en", aboutDataRefill);
                if (status != ER_OK) {
                    std::cout << "Call to getAboutData failed: "  << QCC_StatusText(status) << std::endl;
                } else {
                    PrintAboutData(aboutDataRefill);
                }
            }
        } //if (configClient)
    } //if (isConfigInterface)

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

int main(int argc, char**argv, char**envArg)
{
    QStatus status = ER_OK;
    std::cout << "AllJoyn Library version: " << ajn::GetVersion() << std::endl;
    std::cout << "AllJoyn Library build info: " << ajn::GetBuildInfo() << std::endl;
    QCC_SetLogLevels("ALLJOYN_ABOUT_CLIENT=7");
    QCC_SetLogLevels("ALLJOYN_ABOUT_ICON_CLIENT=7");
    QCC_SetLogLevels("ALLJOYN_ABOUT_ANNOUNCE_HANDLER=7");
    QCC_SetDebugLevel(logModules::CONFIG_MODULE_LOG_NAME, logModules::ALL_LOG_LEVELS);

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

    busAttachment = new BusAttachment("ConfigClient", true);

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

    srpKeyXListener = new SrpKeyXListener();
    srpKeyXListener->setPassCode(INITIAL_PASSCODE);

    status = busAttachment->EnablePeerSecurity("ALLJOYN_SRP_KEYX ALLJOYN_PIN_KEYX ALLJOYN_ECDHE_PSK", srpKeyXListener,
                                               "/.alljoyn_keystore/central.ks", true);

    const char* interfaces[] = { "org.alljoyn.Config" };
    AnnounceHandlerImpl* announceHandler = new AnnounceHandlerImpl(announceHandlerCallback);
    AnnouncementRegistrar::RegisterAnnounceHandler(*busAttachment, *announceHandler, interfaces, 1);

    // Setup signals to wait for.
    sigfillset(&waitmask);
    sigdelset(&waitmask, SIGINT);
    sigdelset(&waitmask, SIGTERM);

    quit = 0;

    while (!quit) {
        // Wait for a signal.
        sigsuspend(&waitmask);
    }
    AnnouncementRegistrar::UnRegisterAnnounceHandler(*busAttachment, *announceHandler, interfaces, 1);

    delete srpKeyXListener;
    delete announceHandler;

    busAttachment->Stop();
    delete busAttachment;

    return 0;

} /* main() */

