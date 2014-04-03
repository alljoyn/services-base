
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

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <json/json.h>
#include <signal.h>

#include <alljoyn/about/AboutServiceApi.h>
#include <alljoyn/onboarding/OnboardingService.h>
#include <OnboardingControllerImpl.h>
#include <alljoyn/AllJoynStd.h>

#define CMD_SIZE 255

using namespace ajn;
using namespace services;

static int execute_system(const char*op);

typedef enum {
    CIPHER_NONE,
    CIPHER_TKIP,
    CIPHER_CCMP,
    CIPHER_BOTH
}GroupCiphers;


#define CASE(_auth) case _auth: return # _auth
static const char* AuthText(short authType)
{
    switch (authType) {
        CASE(WPA2_AUTO);
        CASE(WPA_AUTO);
        CASE(OPEN);
        CASE(WEP);
        CASE(WPA_TKIP);
        CASE(WPA_CCMP);
        CASE(WPA2_TKIP);
        CASE(WPA2_CCMP);
        CASE(WPS);

    default:
        // This function directly feeds the configure scripts - it should return a valid value
        // if all else fails, rather than ANY.
        return "OPEN";
    }
}

OnboardingControllerImpl::OnboardingControllerImpl(qcc::String scanFile,
                                                   qcc::String stateFile,
                                                   qcc::String errorFile,
                                                   qcc::String configureCmd,
                                                   qcc::String connectCmd,
                                                   qcc::String offboardCmd,
                                                   OBConcurrency concurrency,
                                                   BusAttachment& busAttachment) :
    m_state(0),
    m_ScanArray(NULL),
    m_BusAttachment(&busAttachment),
    m_scanFile(scanFile),
    m_stateFile(stateFile),
    m_errorFile(errorFile),
    m_configureCmd(configureCmd),
    m_connectCmd(connectCmd),
    m_offboardCmd(offboardCmd),
    m_concurrency(concurrency)
{
    // Ignore SIGCHLD so we do not have to wait on the child processes
    //signal(SIGCHLD, SIG_IGN);

    // Read state, error and scan info into memory
    GetState();
    GetLastError();
    ParseScanInfo();

    // if the m_concurrency values are out of range, set it to min
    if (m_concurrency < OBConcurrency::CONCURRENCY_MIN || m_concurrency > OBConcurrency::CONCURRENCY_MAX) {
        m_concurrency = OBConcurrency::CONCURRENCY_MIN;
    }

}

OnboardingControllerImpl::~OnboardingControllerImpl()
{
    // Scan results are stored in the scan array
    if (m_ScanArray) {
        delete [] m_ScanArray;
        m_ScanArray = NULL;
    }

    // Pointers to each element are stored in the map, so we need not iterate over values and delete
    m_ScanList.clear();
}

/*------------------------------------------------------------------------------
 * METHOD: ConfigureWiFi()
 * This method is called by the ConfigureWiFiHandler with the corresponding
 * input and output arguments. This method is empty, the developer should fill
 * it with the developer's implementation of the ConfigureWiFi method handler.
 *-----------------------------------------------------------------------------*/
void OnboardingControllerImpl::ConfigureWiFi(qcc::String SSID, qcc::String passphrase, short authType, short& status, qcc::String&  error, qcc::String& errorMessage) {
    std::cout << "entered ConfigureWiFi" << std::endl;

    // Set the return value based on presence of fast switching feature
    status = m_concurrency;

    if (ANY == authType) {
        std::map<qcc::String, OBScanInfo*>::iterator it = m_ScanList.find(SSID);
        if (it != m_ScanList.end() && it->second->authType != ANY) {
            // We have a scan record available with a known auth type.
            authType = it->second->authType;
        } else {
            // We have never heard of this SSID or couldnt figure out its auth type.
            // Gotta try them all :(
            if (passphrase.empty()) {
                // Try open encryption only
                authType = OPEN;
            } else {
                // Try WPA2, WPA, WEP if the passphrase has a chance of working
                // stop when we succeed
                for (authType = WPA2_AUTO; authType > OPEN; --authType) {
                    switch (authType) {
                    case WPA2_AUTO:
                    case WPA_AUTO:
                    case WPA_TKIP:
                    case WPA_CCMP:
                    case WPA2_TKIP:
                    case WPA2_CCMP:
                        {
                            if (!isValidWPAKey(passphrase)) {
                                continue;
                            }
                        }

                    case WEP:
                        {
                            if (!isValidWEPKey(passphrase)) {
                                continue;
                            }
                        }
                    }


                    if (!execute_configure(("'" + SSID + "'").c_str(), authType, ("'" + passphrase + "'").c_str())) {
                        return;
                    }
                }
                // Done trying applicable methods
                return;
            }
        }
    }

    execute_configure(("'" + SSID + "'").c_str(), authType, ("'" + passphrase + "'").c_str());
} /* ConfigureWiFi() */

/*------------------------------------------------------------------------------
 * METHOD: Connect()
 * This method is called by the ConnectHandler with the corresponding input and
 * output arguments. This method is empty, the developer should fill it with the
 * developer's implementation of the Connect method handler.
 *-----------------------------------------------------------------------------*/
void OnboardingControllerImpl::Connect() {
/* Fill in method handler implementation here. */
    std::cout << "entered Connect" << std::endl;
    CancelAdvertise();
    execute_system(m_connectCmd.c_str());
    AdvertiseAndAnnounce();
} /* Connect() */

OBAuthType TranslateToOBAuthType(int authNum, GroupCiphers theCiphers)
{
    switch (authNum) {
    case 0:
        return OPEN;

    case 1:
        switch (theCiphers) {
        case CIPHER_NONE:
        case CIPHER_BOTH:
            return WPA_AUTO;

        case CIPHER_TKIP:
            return WPA_TKIP;

        case CIPHER_CCMP:
            return WPA_CCMP;
        }
        break;

    case 2:
    case 3:
        switch (theCiphers) {
        case CIPHER_NONE:
        case CIPHER_BOTH:
            return WPA2_AUTO;

        case CIPHER_TKIP:
            return WPA2_TKIP;

        case CIPHER_CCMP:
            return WPA2_CCMP;
        }
        break;

    default:
        return ANY;
    }
    return ANY;
}


long GetMinElapsedFromLastScan(const char* filename)
{
    long minutes = -1;
    struct stat attrib;
    int result = stat(filename, &attrib);
    if (result == 0) {
        time_t t = time(0);   // get time now
        double seconds = difftime(t, attrib.st_mtime);
        minutes = seconds / 60;
    }
    return minutes;
}

void OnboardingControllerImpl::ParseScanInfo()
{
    // Scan records are already sorted by signal strength
    // Read and parse
    std::ifstream scanFile(m_scanFile.c_str());
    if (!scanFile.is_open()) {
        return;
    }
    std::string scanString((std::istreambuf_iterator<char>(scanFile)), std::istreambuf_iterator<char>());
    scanFile.close();
    json_object*jroot = json_tokener_parse(scanString.c_str());
    if (NULL == jroot) {
        return;
    }

    // Parse complete, now clear out the map and delete array
    m_ScanList.clear();

    if (m_ScanArray != NULL) {
        delete [] m_ScanArray;
        m_ScanArray = NULL;
    }

    // Rebuild scan result cache
    int length = json_object_array_length(jroot);
    m_ScanArray = new OBScanInfo[length];
    json_object*scanrec;
    int current = 0;
    for (int i = 0; i < length; ++i) {

        // Build an OBScanInfo from record
        scanrec = json_object_array_get_idx(jroot, i);
        json_object_object_foreach(scanrec, key, val) {
            if (!strcmp(key, "ssid")) {
                const char*ssid = json_object_get_string(val);
                m_ScanArray[current].SSID.append(ssid);
            } else if (!strcmp(key, "encryption")) {
                const char*gcipher;
                int wpa = -1;
                GroupCiphers GCiphers = CIPHER_NONE;
                json_object_object_foreach(val, ekey, eval) {
                    if (!strcmp(ekey, "wep")) {
                        if (json_object_get_boolean(eval)) {
                            m_ScanArray[current].authType = WEP;
                        }
                    } else if (!strcmp(ekey, "wpa")) {
                        wpa = json_object_get_int(eval);
                    } else if (!strcmp(ekey, "group_ciphers")) {
                        int length = json_object_array_length(eval);
                        if (length > 1) {
                            GCiphers = CIPHER_BOTH;
                        } else {
                            json_object* cipher_obj = json_object_array_get_idx(eval, 0);
                            gcipher = json_object_get_string(cipher_obj);
                            if (!gcipher) {
                                GCiphers = CIPHER_NONE;
                            } else if (!strcmp(gcipher, "TKIP")) {
                                GCiphers = CIPHER_TKIP;
                            } else if (!strcmp(gcipher, "CCMP")) {
                                GCiphers = CIPHER_CCMP;
                            }
                        }
                    }
                }

                // If wep element was true, leave it intact.
                if (m_ScanArray[current].authType != WEP) {
                    m_ScanArray[current].authType = TranslateToOBAuthType(wpa, GCiphers);
                }
            }
        }

        // If SSID was not found in the scan record, move on to next.
        if (m_ScanArray[current].SSID.empty()) {
            continue;
        }

        // add parsed record to array and map if SSID not duplicated
        std::map<qcc::String, OBScanInfo*>::iterator it = m_ScanList.find(m_ScanArray[current].SSID);
        if (it == m_ScanList.end()) {
            m_ScanList.insert(std::pair<qcc::String, OBScanInfo*>(m_ScanArray[current].SSID, &m_ScanArray[current]));
            ++current;
        }
    }
}

/*------------------------------------------------------------------------------
 * METHOD: GetScanInfo()
 * This method is called by the GetScanInfoHandler with the corresponding input
 * and output arguments. This method is empty, the developer should fill it with
 * the developer's implementation of the GetScanInfo method handler.
 *-----------------------------------------------------------------------------*/
void OnboardingControllerImpl::GetScanInfo(unsigned short& age, OBScanInfo*& scanList, size_t& scanListNumElements) {
    std::cout << "entered " << __FUNCTION__ << std::endl;
    ParseScanInfo();

    scanListNumElements = m_ScanList.size();
    scanList = m_ScanArray;

    // maxes out at around 45 days
    long minold = GetMinElapsedFromLastScan(m_scanFile.c_str());
    if (minold < 0xFFFF) {
        age = (short)minold;
    } else {
        age = 0xFFFF;
    }
} /* GetScanInfo() */

/*------------------------------------------------------------------------------
 * METHOD: Offboard()
 * This method is called by the OffboardHandler with the corresponding input and
 * output arguments. This method is empty, the developer should fill it with the
 * developer's implementation of the Offboard method handler.
 *-----------------------------------------------------------------------------*/
void OnboardingControllerImpl::Offboard() {
    std::cout << "entered " << __FUNCTION__ << std::endl;
    CancelAdvertise();
    execute_system(m_offboardCmd.c_str());
    AdvertiseAndAnnounce();

} /* Offboard() */

short OnboardingControllerImpl::GetState() {
    std::cout << "entered " << __FUNCTION__ << std::endl;
    std::ifstream stateFile(m_stateFile.c_str());
    if (stateFile.is_open()) {
        std::string line;
        getline(stateFile, line);
        std::istringstream iss(line);
        iss >> m_state;
        stateFile.close();
    }
    return m_state;
}

const OBLastError& OnboardingControllerImpl::GetLastError() {
    std::cout << "entered " << __FUNCTION__ << std::endl;
    std::ifstream errorFile(m_errorFile.c_str());
    if (errorFile.is_open()) {
        std::string line;
        getline(errorFile, line);
        std::istringstream iss(line);
        iss >> m_oBLastError.validationState;
        getline(errorFile, line);
        std::cout << line.c_str() << std::endl;
        m_oBLastError.message.assign(line.c_str());
        errorFile.close();
    }

    return m_oBLastError;
}

int OnboardingControllerImpl::execute_configure(const char* SSID, const int authType, const char* passphrase)
{
    std::cout << "entered " << __FUNCTION__ << std::endl;
#ifdef _OPEN_WRT_
    char cmd[CMD_SIZE] = { 0 };
    qcc::String authTypeString = qcc::String("'") + AuthText(authType) + qcc::String("'");
    snprintf(cmd, CMD_SIZE, m_configureCmd.c_str(), SSID, authTypeString.c_str(), passphrase);
    return execute_system(cmd);
#else
    return 0;
#endif
}

void OnboardingControllerImpl::CancelAdvertise()
{
    std::cout << "entered " << __FUNCTION__ << std::endl;
    m_BusAttachment->EnableConcurrentCallbacks();
    if (m_BusAttachment->IsConnected() && m_BusAttachment->GetUniqueName().size() > 0) {
        QStatus status = m_BusAttachment->CancelAdvertiseName(m_BusAttachment->GetUniqueName().c_str(), TRANSPORT_ANY);
        std::cout << "CancelAdvertiseName for " << m_BusAttachment->GetUniqueName().c_str() << " = " << QCC_StatusText(status) << std::endl;
    }
}

void OnboardingControllerImpl::AdvertiseAndAnnounce()
{
    std::cout << "entered " << __FUNCTION__ << std::endl;
    m_BusAttachment->EnableConcurrentCallbacks();
    if (m_BusAttachment->IsConnected() && m_BusAttachment->GetUniqueName().size() > 0) {
        QStatus status = m_BusAttachment->AdvertiseName(m_BusAttachment->GetUniqueName().c_str(), TRANSPORT_ANY);
        std::cout << "AdvertiseName for " << m_BusAttachment->GetUniqueName().c_str() << " = " << QCC_StatusText(status) << std::endl;
    }

    AboutServiceApi* aboutService = AboutServiceApi::getInstance();
    if (aboutService) {
        QStatus status = aboutService->Announce();
        std::cout << "Announce for " << m_BusAttachment->GetUniqueName().c_str() << " = " << QCC_StatusText(status) << std::endl;
    }
}

static int execute_system(const char*cmd)
{
    std::cout << "entered " << __FUNCTION__ << std::endl;
#ifdef _OPEN_WRT_
    if (!cmd) {
        return -1;
    }
    std::cout << "executing " << cmd << std::endl;
    int result = system(cmd);
    result = WEXITSTATUS(result);
    std::cout << "system result=" << result << std::endl;
    if (-1 == result) {
        std::cout << "Error executing system: " << strerror(errno) << std::endl;
    }
    return result;
#else
    return 0;
#endif
}

