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

#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <cstring>
#include <signal.h>

#include <alljoyn/AllJoynStd.h>
#include <alljoyn/config/ConfigClient.h>
#include <alljoyn/onboarding/OnboardingClient.h>

#include <AboutListenerImpl.h>
#include <AsyncSessionJoiner.h>
#include <SessionListenerImpl.h>
#include <OnboardingSignalListenerImpl.h>
#include <alljoyn/services_common/LogModulesNames.h>
#include <AJInitializer.h>
#include <SecurityUtil.h>
#include <CertificateUtil.h>

#include <alljoyn/AuthListener.h>
#include <alljoyn/AboutData.h>
#include <alljoyn/AboutListener.h>
#include <alljoyn/AboutObjectDescription.h>
#include <alljoyn/AboutProxy.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/AboutIconProxy.h>
#include <alljoyn/version.h>
#include <alljoyn/SecurityApplicationProxy.h>
#include <qcc/Log.h>
#include <qcc/GUID.h>
#include <qcc/KeyInfoECC.h>
#include <qcc/StringUtil.h>
#include <alljoyn/services_common/BsSecurity.h>
#include <qcc/Debug.h>
#include "OnboardingSignalListenerImpl.h"

#define QCC_MODULE "ONBOARD"

using namespace ajn;
using namespace services;

static volatile sig_atomic_t quit = false;
static ajn::BusAttachment* busAttachment = NULL;
static std::set<qcc::String> handledAnnouncements;

const qcc::String caName("Sample CA");

static qcc::Crypto_ECC* caKeyPair = NULL;
const qcc::String CA_CERT_FILENAME("caCert.pem");
const qcc::String CA_KEY_FILENAME("caKey.pem");
const qcc::String ADMIN_GUID_FILENAME("adminGuid");

static const char *const WIFI_PASSWORD_PROMPT = "Wifi password: ";
static const char *const CHANGE_NETWORK_PROMPT = "Change to the onboardee's wifi network and hit Enter\n";
static qcc::String authPassword("");
static qcc::String authMechanism{"ALLJOYN_ECDHE_ECDSA"};
static qcc::String wifiNetwork;
static std::string dummy;

static OBInfo oBInfo;

static void usage()
{
    std::cout << "OnboardingClient [-speke 000000] [-psk deadbeefcafebabeff] [-auth NULL|PSK...] [-wifi NETWORK] [-wifi_tkip|-wifi_ccmp]";
}


static void cmdLine(int argc, char** argv)
{
    qcc::String wifiPassword;
    OBAuthType wifiAuthType = WPA2_CCMP;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-psk") == 0) {
            if (i + 1 < argc) {
                authPassword = argv[++i];
                authMechanism = "ALLJOYN_ECDHE_ECDSA ALLJOYN_ECDHE_PSK";
            } else {
                usage();
            }
        } else
        if (strcmp(argv[i], "-speke") == 0) {
            if (i + 1 < argc) {
                authPassword = argv[++i];
                authMechanism = "ALLJOYN_ECDHE_ECDSA ALLJOYN_ECDHE_SPEKE";
            } else {
                usage();
            }
        } else
        if (strcmp(argv[i], "-auth") == 0) {
            if (i + 1 < argc) {
                authMechanism = argv[++i];

                if (authMechanism == "NULL") {
                    authMechanism = "ALLJOYN_ECDHE_ECDSA ALLJOYN_ECDHE_NULL";
                } else
                if (authMechanism == "PSK") {
                    authMechanism = "ALLJOYN_ECDHE_ECDSA ALLJOYN_ECDHE_PSK";
                } else
                if (authMechanism == "SPEKE") {
                    authMechanism = "ALLJOYN_ECDHE_ECDSA ALLJOYN_ECDHE_SPEKE";
                } else
                if (authMechanism == "ECDSA") {
                    authMechanism = "ALLJOYN_ECDHE_ECDSA";
                } else {
                    std::cerr << "Unknown auth mechanism: " << authMechanism.c_str() << "; aborting." << std::endl;
                    exit(1);
                }
            } else {
                usage();
            }
        } else
        if (strcmp(argv[i], "-wifi") == 0) {
            if (i + 1 < argc) {
                wifiNetwork = argv[++i];
            } else {
                usage();
            }
        } else
        if (strcmp(argv[i], "-wifi_tkip") == 0) {
            wifiAuthType = WPA2_TKIP;
        } else
        if (strcmp(argv[i], "-wifi_ccmp") == 0) {
            wifiAuthType = WPA2_CCMP;
        }
    }

    if (wifiNetwork.empty()) {
        std::cerr << "No WiFi network name was provided; aborting." << std::endl;
        usage();
        exit(1);
    }

    // Get the wifi password more securely
    std::cout << WIFI_PASSWORD_PROMPT;
    std::getline(std::cin, dummy);
    wifiPassword = qcc::Trim(dummy.c_str());

    // The password must be 2 digit ascii hex.
    std::ostringstream passwordstream;
    passwordstream << std::hex << std::setw(2) << std::setfill('0') << std::uppercase;

    const char* pptr = wifiPassword.c_str();

    while (*pptr) {
        passwordstream << int(*pptr++);
    }

    std::string s = passwordstream.str();

    oBInfo.SSID.assign(wifiNetwork.c_str());
    oBInfo.passcode.assign(s.c_str());
    oBInfo.authType = wifiAuthType;
}


static qcc::CertificateX509& caCert()
{
    static qcc::CertificateX509 cert;
    return cert;
}

static qcc::GUID128& adminGuid()
{
    static qcc::GUID128 guid;
    return guid;
}

static void CDECL_CALL SigIntHandler(int sig)
{
    QCC_UNUSED(sig);
    quit = true;
}

// Print out the fields found in the AboutData. Only fields with known signatures
// are printed out.  All others will be treated as an unknown field.
void printAboutData(AboutData& aboutData, const char* language)
{
    size_t count = aboutData.GetFields();

    const char** fields = new const char*[count];
    aboutData.GetFields(fields, count);

    for (size_t i = 0; i < count; ++i) {
        std::cout << "\tKey: " << fields[i];

        MsgArg* tmp;
        aboutData.GetField(fields[i], tmp, language);
        std::cout << "\t";
        if (tmp->Signature() == "s") {
            const char* tmp_s;
            tmp->Get("s", &tmp_s);
            std::cout << tmp_s;
        } else if (tmp->Signature() == "as") {
            size_t las;
            MsgArg* as_arg;
            tmp->Get("as", &las, &as_arg);
            for (size_t j = 0; j < las; ++j) {
                const char* tmp_s;
                as_arg[j].Get("s", &tmp_s);
                std::cout << tmp_s << " ";
            }
        } else if (tmp->Signature() == "ay") {
            size_t lay;
            uint8_t* pay;
            tmp->Get("ay", &lay, &pay);
            for (size_t j = 0; j < lay; ++j) {
                std::cout << std::hex << static_cast<int>(pay[j]) << " ";
            }
        } else {
            std::cout << "User Defined Value\tSignature: " << tmp->Signature().c_str();
        }
        std::cout << std::endl;
    }
    delete[] fields;
    std::cout << std::endl;
}

void printAllAboutData(AboutProxy& aboutProxy)
{
    MsgArg aArg;
    QStatus status = aboutProxy.GetAboutData(NULL, aArg);
    if (status == ER_OK) {
        std::cout << "*********************************************************************************" << std::endl;
        std::cout << "GetAboutData: (Default Language)" << std::endl;
        AboutData aboutData(aArg);
        printAboutData(aboutData, NULL);
        size_t lang_num;
        lang_num = aboutData.GetSupportedLanguages();
        // If the lang_num == 1 we only have a default language
        if (lang_num > 1) {
            const char** langs = new const char*[lang_num];
            aboutData.GetSupportedLanguages(langs, lang_num);
            char* defaultLanguage;
            aboutData.GetDefaultLanguage(&defaultLanguage);
            // print out the AboutData for every language but the
            // default it has already been printed.
            for (size_t i = 0; i < lang_num; ++i) {
                if (strcmp(defaultLanguage, langs[i]) != 0) {
                    status = aboutProxy.GetAboutData(langs[i], aArg);
                    if (ER_OK == status) {
                        aboutData.CreatefromMsgArg(aArg, langs[i]);
                        std::cout << "GetAboutData: (" << langs[i] << ")" << std::endl;
                        printAboutData(aboutData, langs[i]);
                    }
                }
            }
            delete[] langs;
        }
        std::cout << "*********************************************************************************" << std::endl;
    }
}

bool claim(const qcc::String& busName, const ajn::SessionId sessionId, const qcc::KeyInfoNISTP256& peerPubKeyInfo,
           const qcc::String& name, const qcc::String& caCN, const qcc::CertificateX509& caCert,
           const qcc::GUID128& adminGuid,
           const qcc::GUID128& identityGuid)
{
    QCC_DbgTrace(("%s", __FUNCTION__));

    QStatus status;

    QCC_DbgHLPrintf(("Claiming %s : %s", busName.c_str(), name.c_str()));
    QCC_DbgHLPrintf(("Admin GUID : %s", adminGuid.ToString().c_str()));
    QCC_DbgHLPrintf(("Ident GUID : %s", identityGuid.ToString().c_str()));

    Manifest manifest;
    SecurityUtil::GenerateManifest(manifest);

    qcc::IdentityCertificate identityCertificate;
    status = CertificateUtil::GenerateIdentityCertificate(*peerPubKeyInfo.GetPublicKey(), identityGuid, name, identityCertificate);

    if (ER_OK == status) {
        status = CertificateUtil::IssueCertificate(*caKeyPair, caCN, identityCertificate);
    }

    if (ER_OK == status) {
        QCC_ASSERT(identityCertificate.Verify(caKeyPair->GetDSAPublicKey()) == ER_OK && "invalid identity cert");
    }

    if (ER_OK == status) {
        status = CertificateUtil::SignManifest(caKeyPair->GetDSAPrivateKey(), identityCertificate, manifest);

    }

    if (ER_OK != status) {
        QCC_LogError(ER_NONE, ("WARNING: Manifest generation failed (%s)", QCC_StatusText(status)));
        return false;
    }

    std::vector<ajn::Manifest> manifests;
    manifests.push_back(manifest);

    std::vector<qcc::CertificateX509> certChain;
    certChain.push_back(identityCertificate); // NOTE: identity cert must be first in chain
    certChain.push_back(caCert);

    qcc::KeyInfoNISTP256 caPublicKeyInfo;
    {
        qcc::String aki;
        status = qcc::CertificateX509::GenerateAuthorityKeyId(caKeyPair->GetDSAPublicKey(), aki);
        if (ER_OK != status) {
            QCC_LogError(ER_NONE, ("WARNING: AuthorityKeyId generation failed (%s)", QCC_StatusText(status)));
            return false;
        }
        caPublicKeyInfo.SetKeyId((uint8_t*)aki.data(), aki.size());
        caPublicKeyInfo.SetPublicKey(caKeyPair->GetDSAPublicKey());
    }

    status = SecurityUtil::Claim(*busAttachment, busName, sessionId, caPublicKeyInfo, adminGuid, caPublicKeyInfo, certChain, manifests);
    if (ER_OK != status) {
        QCC_LogError(ER_NONE, ("WARNING: call to SecurityUtil::Claim() failed (%s)", QCC_StatusText(status)));
        return false;
    }

    QCC_DbgHLPrintf(("OnboardingClient has claimed %s", busName.c_str()));

    return true;
}

bool claimSelf(const qcc::String& caCN, const qcc::CertificateX509& caCert,
               const qcc::GUID128& adminGuid, const qcc::GUID128& identityGuid)
{
    QCC_DbgTrace(("%s", __FUNCTION__));

    QStatus status;
    const qcc::String busName = busAttachment->GetUniqueName().c_str();

    qcc::KeyInfoNISTP256 publicKeyInfo;
    status = busAttachment->GetPermissionConfigurator().GetSigningPublicKey(publicKeyInfo);
    if (ER_OK != status) {
        QCC_LogError(ER_NONE, ("WARNING: Failed to get signing public key (%s)", QCC_StatusText(status)));
        return false;
    }

    SessionId sessionId = SESSION_PORT_ANY;
    bool result = claim(busName, sessionId, publicKeyInfo, qcc::String("self"), caCN, caCert, adminGuid, identityGuid);
    if (!result) {
        QCC_LogError(ER_NONE, ("WARNING: Failed to get signing public key (%s)", QCC_StatusText(status)));
        return false;
    }

    // Add self to admin group
    {
        qcc::MembershipCertificate membershipCert;
        status = CertificateUtil::GenerateMembershipCertificate(*publicKeyInfo.GetPublicKey(), adminGuid, membershipCert);

        if (ER_OK == status) {
            status = CertificateUtil::IssueCertificate(*caKeyPair, caCN, membershipCert);
        }

        if (ER_OK == status) {
            status = SecurityUtil::InstallMembership(*busAttachment, busName, membershipCert);
        }

        if (ER_OK != status) {
            return false;
        }

        QCC_DbgHLPrintf(("OnboardingClient is member of admin group"));
    }

    return result;
}

void sessionJoinedCallback(qcc::String const& busName, SessionId sessionId)
{
    QCC_DbgTrace(("%s", __FUNCTION__));

    QStatus status = ER_OK;
    if (busAttachment == NULL) {
        return;
    }

    busAttachment->EnableConcurrentCallbacks();

    bool wasClaimed = false;

    {
        SecurityApplicationProxy peerProxy(*busAttachment, busName.c_str(), sessionId);

        PermissionConfigurator::ApplicationState appState;
        peerProxy.GetApplicationState(appState);
        QCC_DbgHLPrintf(("Peer is %s", PermissionConfigurator::ToString(appState)));

        if (PermissionConfigurator::ApplicationState::CLAIMABLE == appState) {

            qcc::ECCPublicKey peerPubKey;
            status = peerProxy.GetEccPublicKey(peerPubKey);
            if (ER_OK != status) {
                QCC_LogError(ER_NONE, ("WARNING - GetEccPublicKey failed; (%s)", QCC_StatusText(status)));
                return;
            } else {
                QCC_DbgHLPrintf(("Peer public key: %s", peerPubKey.ToString().c_str()));
            }

            qcc::KeyInfoNISTP256 peerPubKeyInfo;
            peerPubKeyInfo.SetPublicKey(&peerPubKey);

            qcc::GUID128 identityGuid;
            bool result = claim(busName, sessionId, peerPubKeyInfo, qcc::String("other"), caName, caCert(), adminGuid(), identityGuid);
            if (!result) {
                QCC_LogError(ER_NONE, ("WARNING - Failed to claim ", busName.c_str()));
            } else {
                wasClaimed = true;
            }
        }

        status = busAttachment->SecureConnection(busName.c_str(), wasClaimed);
        if (ER_OK != status) {
            QCC_LogError(ER_NONE, ("WARNING - SecureConnection failed; (%s)", QCC_StatusText(status)));
        } else {
            QCC_DbgHLPrintf(("Opened SecureConnection with ", busName.c_str()));
        }
    }

    AboutProxy aboutProxy(*busAttachment, busName.c_str(), sessionId);

    bool isIconInterface = false;
    bool isConfigInterface = false;
    bool isOnboardingInterface = false;

    std::cout << std::endl << busName.c_str() << " AboutClient ObjectDescriptions" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    MsgArg objArg;
    status = aboutProxy.GetObjectDescription(objArg);
    if (ER_OK != status) {
        std::cout << "WARNING - GetObjectDescription " << busName << ". error: " << QCC_StatusText(status) << std::endl;
    } else {
        std::cout << "AboutProxy.GetObjectDescriptions:\n" << objArg.ToString().c_str() << "\n\n" << std::endl;
    }

    AboutObjectDescription objectDescription;
    objectDescription.CreateFromMsgArg(objArg);

    isIconInterface = objectDescription.HasInterface(org::alljoyn::Icon::ObjectPath, org::alljoyn::Icon::InterfaceName);

    if (isIconInterface) {
        std::cout << "The given interface '" << org::alljoyn::Icon::InterfaceName << "' is found in a given path '" << org::alljoyn::Icon::ObjectPath << "'" << std::endl;
    } else {
        std::cout << "WARNING - The given interface '" << org::alljoyn::Icon::InterfaceName << "' is not found in a given path '" << org::alljoyn::Icon::ObjectPath << "'" << std::endl;
    }

    isConfigInterface = false;
    isConfigInterface = objectDescription.HasInterface("/Config", "org.alljoyn.Config");
    if (isConfigInterface) {
        std::cout << "The given interface 'org.alljoyn.Config' is found in a given path '/Config'" << std::endl;
    } else {
        std::cout << "WARNING - The given interface 'org.alljoyn.Config' is not found in a given path '/Config'" << std::endl;
    }

    isOnboardingInterface = false;
    isOnboardingInterface = objectDescription.HasInterface("/Onboarding", "org.alljoyn.Onboarding");
    if (isOnboardingInterface) {
        std::cout << "The given interface 'org.alljoyn.Onboarding' is found in a given path '/Onboarding'" << std::endl;
    } else {
        std::cout << "WARNING - The given interface 'org.alljoyn.Onboarding' is not found in a given path '/Onboarding'" << std::endl;
    }
    printAllAboutData(aboutProxy);

    std::cout << "aboutProxy GetVersion " << std::endl;
    std::cout << "-----------------------" << std::endl;

    uint16_t version = 0;
    status = aboutProxy.GetVersion(version);
    if (status != ER_OK) {
        std::cout << "WARNING - Call to getVersion failed " << QCC_StatusText(status) << std::endl;
    } else {
        std::cout << "Version=" << version << std::endl;
    }

    if (isIconInterface) {
        AboutIconProxy aiProxy(*busAttachment, busName.c_str(), sessionId);
        AboutIcon aboutIcon;

        std::cout << std::endl << busName.c_str() << " AboutIconProxy GetIcon" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        status = aiProxy.GetIcon(aboutIcon);
        if (status != ER_OK) {
            std::cout << "WARNING - Call to GetIcon failed: " << QCC_StatusText(status) << std::endl;
            //goto
        }

        std::cout << "url=" << aboutIcon.url.c_str() << std::endl;
        std::cout << "Content size = " << aboutIcon.contentSize << std::endl;
        std::cout << "Content =\t";
        for (size_t i = 0; i < aboutIcon.contentSize; i++) {
            if (i % 8 == 0 && i > 0) {
                std::cout << "\n\t\t";
            }
            std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (unsigned int)aboutIcon.content[i]
                      << std::nouppercase << std::dec;

            //std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Mimetype =\t" << aboutIcon.mimetype.c_str() << std::endl;
        std::cout << std::endl << busName.c_str() << " AboutIcontClient GetVersion" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        uint16_t version;
        status = aiProxy.GetVersion(version);
        if (status != ER_OK) {
            std::cout << "WARNING - Call to getVersion failed: " << QCC_StatusText(status) << std::endl;
        } else {
            std::cout << "Version=" << version << std::endl;
        }
    }

    ConfigClient* configClient = NULL;
    if (isConfigInterface) {
        configClient = new ConfigClient(*busAttachment);

        std::cout << std::endl << busName.c_str() << " ConfigClient GetVersion" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        int version = 0;
        if ((status = configClient->GetVersion(busName.c_str(), version, sessionId)) == ER_OK) {
            std::cout << "Success GetVersion. Version=" << version << std::endl;
        } else {
            std::cout << "Call to getVersion failed: " << QCC_StatusText(status) << std::endl;
        }

        ConfigClient::Configurations configurations;
        std::cout << std::endl << busName.c_str() << " ConfigClient GetConfigurations" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        if ((status = configClient->GetConfigurations(busName.c_str(), "en", configurations, sessionId)) == ER_OK) {

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

        {
            ProxyBusObject* proxyBusObj = new ProxyBusObject(*busAttachment, busName.c_str(), "/Onboarding", sessionId);
            if (proxyBusObj->IntrospectRemoteObject() == ER_OK) {
                const InterfaceDescription* infdesc = proxyBusObj->GetInterface("org.alljoyn.Onboarding");
                if (infdesc) {
                    std::cout << "**** Onboarding interface:\n" << infdesc->Introspect() << std::endl;
                }
            } else {
                std::cout << "WARNING - Failed to InstrospectRemoteObject" << std::endl;
            }

            delete proxyBusObj;
            proxyBusObj = NULL;
        }

        onboardingClient = new OnboardingClient(*busAttachment, *signalListener);

        std::cout << std::endl << busName.c_str() << " OnboardingClient GetVersion" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        int version = 0;
        if ((status = onboardingClient->GetVersion(busName.c_str(), version, sessionId)) == ER_OK) {
            std::cout << "Version=" << version << std::endl;
        } else {
            std::cout << "Call to GetVersion failed " << QCC_StatusText(status) << std::endl;
        }


        std::cout << std::endl << busName.c_str() << " OnboardingClient GetState" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        short int state = 0;
        if ((status = onboardingClient->GetState(busName.c_str(), state, sessionId)) == ER_OK) {
            std::cout << "GetState=" << state << std::endl;
        } else {
            std::cout << "Call to GetState failed " << QCC_StatusText(status) << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " OnboardingClient GetLastError" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        OBLastError lastError = { 0, "" };
        if ((status = onboardingClient->GetLastError(busName.c_str(), lastError, sessionId)) == ER_OK) {
            std::cout << "OBLastError code=" << lastError.validationState << " message= " << lastError.message.c_str() << std::endl;
        } else {
            std::cout << "Call to GetLastError failed " << QCC_StatusText(status) << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " OnboardingClient GetScanInfo" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        unsigned short age = 0;
        OnboardingClient::ScanInfos scanInfos;
        if ((status = onboardingClient->GetScanInfo(busName.c_str(), age, scanInfos, sessionId)) == ER_OK) {
            for (OnboardingClient::ScanInfos::iterator it = scanInfos.begin(); it != scanInfos.end(); ++it) {
                std::cout << "Network  SSID=" << it->SSID.c_str() << " authType=" << it->authType << std::endl;
            }
        } else {
            std::cout << "Call to GetScanInfo failed " << QCC_StatusText(status) << std::endl;
        }

        std::cout << std::endl << busName.c_str() << " OnboardingClient ConfigureWiFi" << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        short resultStatus;

        if ((status = onboardingClient->ConfigureWiFi(busName.c_str(), oBInfo, resultStatus, sessionId)) == ER_OK) {
            std::cout << "Call to ConfigureWiFi succeeded " << std::endl;
        } else {
            std::cout << "Call to ConfigureWiFi failed " << QCC_StatusText(status) << std::endl;
        }

        if ((status = onboardingClient->ConnectTo(busName.c_str(), sessionId)) == ER_OK) {
            std::cout << "Call to ConnectTo succeeded " << std::endl;
        } else {
            std::cout << "Call to ConnectTo failed " << QCC_StatusText(status) << std::endl;
        }
#if 0
        // This resets the device.  Why would you do that?
        if ((status = onboardingClient->OffboardFrom(busName.c_str(), sessionId)) == ER_OK) {
            std::cout << "Call to OffboardFrom succeeded " << std::endl;
        } else {
            std::cout << "Call to OffboardFrom failed " << QCC_StatusText(status) << std::endl;
        }
#endif
        std::cout << "Change to " << wifiNetwork.c_str() << " and hit Enter\n";
        std::getline(std::cin, dummy);
    }

    status = busAttachment->LeaveSession(sessionId);
    std::cout << "Leaving session id = " << sessionId << " with " << busName.c_str() << " status: " << QCC_StatusText(status) << std::endl;

    if (wasClaimed) {
        status = SecurityUtil::Reset(*busAttachment, busName);
        if (ER_OK == status) {
            std::cout << "Reset " << busName << std::endl;
        }
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
    QCC_DbgTrace(("%s", __FUNCTION__));

    std::cout << "announceHandlerCallback " << busName.c_str() << " " << port << std::endl;
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
        std::cout << busName.c_str() << " has already been handled" << std::endl;
    }
}


bool LoadAdminGroupId(const qcc::String& filename, qcc::GUID128& guid)
{
    QCC_DbgTrace(("%s", __FUNCTION__));

    std::ifstream fs;
    fs.open(filename, std::fstream::binary);
    if (fs.is_open()) {
        uint8_t buf[qcc::GUID128::SIZE];
        fs.read((char*)buf, qcc::GUID128::SIZE);
        fs.close();
        guid.SetBytes(buf);
        return true;
    }
    return false;
}

bool SaveAdminGroupId(const qcc::String& filename, qcc::GUID128& guid)
{
    QCC_DbgTrace(("%s", __FUNCTION__));

    std::ofstream fs;
    fs.open(filename, std::fstream::binary | std::fstream::out | std::fstream::trunc);
    if (fs.is_open()) {
        fs.write((const char*)guid.GetBytes(), qcc::GUID128::SIZE);
        fs.close();
        return true;
    }
    return false;
}


int main(int argc, char** argv, char** envArg)
{
    QCC_UNUSED(envArg);

    cmdLine(argc, argv);

    std::cout << CHANGE_NETWORK_PROMPT;
    std::getline(std::cin, dummy);

    // Initialize AllJoyn
    AJInitializer ajInit;
    if (ajInit.Initialize() != ER_OK) {
        return 1;
    }

    QCC_SetLogLevels("ONBOARD=15;");
    QCC_SetDebugLevel("ONBOARD", 15);

    QCC_LogMsg(("Using %s", ajn::GetBuildInfo()));

    QStatus status = ER_OK;

    /* Install SIGINT handler so Ctrl + C deallocates memory properly */
    signal(SIGINT, SigIntHandler);

    busAttachment = new ajn::BusAttachment("OnboardingClient", true);

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

    BsSecurity security{*busAttachment};
    security.LoadFiles("security");

    if (!authPassword.empty()) {
        security.SetPassword(authPassword);
    }
    if (!authMechanism.empty()) {
        security.SetMechanism(authMechanism);
    }

    status = security.Enable(".alljoyn_keystore/client.ks", true);

    if (ER_OK == status) {
        std::cout << "EnablePeerSecurity called." << std::endl;
    } else {
        std::cout << "ERROR - EnablePeerSecurity call FAILED with status " << QCC_StatusText(status) << std::endl;
        return 1;
    }

    // load CA cert and private key or generate new
    caKeyPair = new qcc::Crypto_ECC;
    {
        qcc::ECCPrivateKey privateKey;

        if (CertificateUtil::LoadPrivateKey(CA_KEY_FILENAME, &privateKey) &&
            CertificateUtil::LoadCertificate(CA_CERT_FILENAME, caCert()) &&
            LoadAdminGroupId(ADMIN_GUID_FILENAME, adminGuid())) {
            caKeyPair->SetDSAPublicKey(caCert().GetSubjectPublicKey());
            caKeyPair->SetDSAPrivateKey(&privateKey);

            std::cout << "Loaded CA certificate " << caCert().ToString() << std::endl;
            std::cout << "Loaded admin guid: " << adminGuid().ToString() << std::endl;
        } else {
            caKeyPair->GenerateDSAKeyPair();

            status = CertificateUtil::GenerateCA(*caKeyPair, caName, caCert());

            if (ER_OK == status) {
                if (!(CertificateUtil::SaveCertificate(CA_CERT_FILENAME, caCert()) &&
                      CertificateUtil::SavePrivateKey(CA_KEY_FILENAME, caKeyPair->GetDSAPrivateKey()) &&
                      SaveAdminGroupId(ADMIN_GUID_FILENAME, adminGuid()))) {
                    status = ER_FAIL;
                }
            }

            if (ER_OK != status) {
                std::cout << "ERROR - Failed to generate and save the certificate: status " << QCC_StatusText(status) << std::endl;
                return 1;
            }
        }
    }

    {
        PermissionConfigurator::ApplicationState appState;
        busAttachment->GetPermissionConfigurator().GetApplicationState(appState);
        std::cout << "application state: " << PermissionConfigurator::ToString(appState) << std::endl;

        if (PermissionConfigurator::ApplicationState::NOT_CLAIMABLE == appState) {
            std::cout << "*** set manifest ***" << std::endl;

            Manifest manifest;
            SecurityUtil::GenerateManifest(manifest);
            std::cout << manifest->ToString() << std::endl;

            std::vector<PermissionPolicy::Rule> rules = manifest->GetRules();
            busAttachment->GetPermissionConfigurator().SetPermissionManifestTemplate(rules.data(), rules.size());

            busAttachment->GetPermissionConfigurator().GetApplicationState(appState);
            std::cout << "application state: " << PermissionConfigurator::ToString(appState) << std::endl;
        }

        if (PermissionConfigurator::ApplicationState::CLAIMABLE == appState) {
            std::cout << "*** claim self ***" << std::endl;
            qcc::GUID128 identityGuid;
            if (!claimSelf(caName, caCert(), adminGuid(), identityGuid)) {
                return 1;
            }

            busAttachment->GetPermissionConfigurator().GetApplicationState(appState);
            std::cout << "application state: " << PermissionConfigurator::ToString(appState) << std::endl;
        }
    }

    const char* interfaces[] = { "org.alljoyn.Onboarding" };
    AboutListenerImpl* announceHandler = new AboutListenerImpl(announceHandlerCallback);
    busAttachment->RegisterAboutListener(*announceHandler);

    status = busAttachment->WhoImplements(interfaces, sizeof(interfaces) / sizeof(interfaces[0]));
    if (ER_OK == status) {
        std::cout << "WhoImplements called." << std::endl;
    } else {
        std::cout << "ERROR - WhoImplements failed." << std::endl;
    }

    while (!quit) {
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100 * 1000);
#endif
    }

    busAttachment->CancelWhoImplements(interfaces, sizeof(interfaces) / sizeof(interfaces[0]));
    busAttachment->UnregisterAboutListener(*announceHandler);

    busAttachment->Stop();
    delete busAttachment;
    delete announceHandler;

    if (caKeyPair != NULL) {
        delete caKeyPair;
    }

    return 0;
} /* main() */
