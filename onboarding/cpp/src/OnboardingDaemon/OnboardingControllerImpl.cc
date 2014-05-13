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
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <algorithm>

#include <alljoyn/onboarding/OnboardingService.h>
#include <OnboardingControllerImpl.h>
#include <alljoyn/AllJoynStd.h>
#include <alljoyn/onboarding/LogModule.h>

#define CMD_SIZE 255
#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1
#define SCAN_WIFI_MAX_TIME_IN_SEC 30


using namespace ajn;
using namespace services;

static int execute_system(const char*op);
static void TimerDone(int sig, siginfo_t*si, void*uc);
static void* ScanWifiThread(void* context);

typedef enum {
    CIPHER_NONE,
    CIPHER_TKIP,
    CIPHER_CCMP,
    CIPHER_BOTH
}GroupCiphers;

#ifdef _OPEN_WRT_
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
#endif

OnboardingControllerImpl::OnboardingControllerImpl(qcc::String scanFile,
                                                   qcc::String stateFile,
                                                   qcc::String errorFile,
                                                   qcc::String configureCmd,
                                                   qcc::String connectCmd,
                                                   qcc::String offboardCmd,
                                                   qcc::String scanCmd,
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
    m_scanCmd(scanCmd),
    m_concurrency(concurrency),
    m_scanWifiThreadIsRunning(false),
    m_scanWifiTimerId(0)
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
    QCC_DbgHLPrintf(("entered %s", __FUNCTION__));

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
                for (authType = WPS; authType >= WPA2_AUTO; --authType) {
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

void* OnboardingControllerImpl::OBS_Connect(void* obsArg)
{
    OnboardingControllerImpl* obController = (OnboardingControllerImpl*)obsArg;
    qcc::String connectCmd = obController->m_connectCmd;

    //stopping daemon before connect cmd and restarting it after
    execute_system("/etc/init.d/alljoyn stop");
    execute_system(connectCmd.c_str());
    execute_system("/etc/init.d/alljoyn start");

    return NULL;
}
/*------------------------------------------------------------------------------
 * METHOD: Connect()
 * This method is called by the ConnectHandler with the corresponding input and
 * output arguments. This method is empty, the developer should fill it with the
 * developer's implementation of the Connect method handler.
 *-----------------------------------------------------------------------------*/
void OnboardingControllerImpl::Connect() {
/* Fill in method handler implementation here. */
    QCC_DbgHLPrintf(("entered %s", __FUNCTION__));
    CancelAdvertise();
    pthread_t thread;
    pthread_create(&thread, NULL, OnboardingControllerImpl::OBS_Connect, this);
} /* Connect() */

OBAuthType TranslateToOBAuthType(int authNum, GroupCiphers theCiphers)
{
    switch (authNum) {

    case WPA_AUTO:
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

    case WPA2_AUTO:
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
    std::ifstream scanFile;

    scanFile.open(m_scanFile.c_str());
    if (scanFile.fail()) {
        return;
    }

    //get lines in file which is the number of ssid's scanned
    //when done, return to the beginning of the file
    int length = std::count(std::istreambuf_iterator<char>(scanFile), std::istreambuf_iterator<char>(), '\n');
    scanFile.clear();
    scanFile.seekg(0, std::ios::beg);

    // Clear out the map and delete array and initialize with the new length
    m_ScanList.clear();

    if (m_ScanArray != NULL) {
        delete [] m_ScanArray;
        m_ScanArray = NULL;
    }
    m_ScanArray = new OBScanInfo[length];

    // start parsing the scan file
    std::string line;
    int current = 0;

    while (std::getline(scanFile, line)) {
        std::stringstream currLine(line);

        char* ssid = strtok((char*)line.c_str(), "\t");
        char* auth = strtok(NULL, "\t");

        if (!ssid) {
            continue;
        }

        char* authType = strtok(auth, "-");
        char* firstCipher = strtok(NULL, "-");
        char* secondCipher = strtok(NULL, "-");

        if (!authType) {
            continue;
        }

        m_ScanArray[current].SSID = ssid;

        // get auth type
        if (!strcmp(authType, "WEP")) {
            m_ScanArray[current].authType = WEP;
        } else if (!strcmp(authType, "Open")) {
            m_ScanArray[current].authType = OPEN;
        } else if (!strcmp(authType, "WPA2")) {
            m_ScanArray[current].authType = WPA2_AUTO;
        } else if (!strcmp(authType, "WPA")) {
            m_ScanArray[current].authType = WPA_AUTO;
        }

        // still need to set the ciphers for WPA and WPA2
        if ((m_ScanArray[current].authType == WPA_AUTO) || (m_ScanArray[current].authType == WPA2_AUTO)) {
            // example:
            // One cipher CCMP would look like "WPA2-CCMP--PSK"
            // Two ciphers CCMP and TKIP "WPA2-CCMP-TKIP-PSK" we dont know the order of CCMP and TKIP

            GroupCiphers GCiphers = CIPHER_NONE;
            if (firstCipher == NULL) {
                GCiphers = CIPHER_NONE;
            } else if (!strcmp(firstCipher, "PSK")) {
                GCiphers = CIPHER_NONE;
            } else if (!strcmp(firstCipher, "CCMP")) {
                GCiphers = CIPHER_CCMP;
                if (secondCipher != NULL && !strcmp(secondCipher, "TKIP")) {
                    GCiphers = CIPHER_BOTH;
                }
            } else if (!strcmp(firstCipher, "TKIP")) {
                GCiphers = CIPHER_TKIP;
                if (secondCipher != NULL && !strcmp(secondCipher, "CCMP")) {
                    GCiphers = CIPHER_BOTH;
                }
            }

            m_ScanArray[current].authType = TranslateToOBAuthType(m_ScanArray[current].authType, GCiphers);
        }

        // add parsed record to array and map if SSID not duplicated
        std::map<qcc::String, OBScanInfo*>::iterator it = m_ScanList.find(m_ScanArray[current].SSID);
        if (it == m_ScanList.end()) {
            m_ScanList.insert(std::pair<qcc::String, OBScanInfo*>(m_ScanArray[current].SSID, &m_ScanArray[current]));
            ++current;
        }
    }

    scanFile.close();
}

void OnboardingControllerImpl::StartScanWifiTimer()
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = TimerDone;
    if (sigaction(SIGRTMAX, &sa, NULL) < 0) {
        return;
    }

    sigevent sigev;

    memset(&sigev, 0, sizeof(sigev));

    sigev.sigev_notify          = SIGEV_SIGNAL;
    sigev.sigev_signo           = SIGRTMAX;
    sigev.sigev_value.sival_ptr = &m_scanWifiTimerId;

    if (timer_create(CLOCK_REALTIME, &sigev, &m_scanWifiTimerId) == 0) {
        struct itimerspec itval, oitval;

        itval.it_value.tv_sec = SCAN_WIFI_MAX_TIME_IN_SEC;
        itval.it_value.tv_nsec = 0;
        itval.it_interval.tv_sec = 0;
        itval.it_interval.tv_nsec = 0;


        if (timer_settime(m_scanWifiTimerId, 0, &itval, &oitval) != 0) {
            return;
        }
    } else {
        QCC_DbgTrace(("error creating timer"));
    }
}

static void TimerDone(int sig, siginfo_t*si, void*context)
{
    OnboardingControllerImpl* thisClass = reinterpret_cast<OnboardingControllerImpl*>(context);
    thisClass->ScanWifiTimerDone();
}

void OnboardingControllerImpl::ScanWifiTimerDone()
{
    if (m_scanWifiThreadIsRunning) {
        int retval;
        pthread_cancel(m_scanWifiThread);
        pthread_join(m_scanWifiThread, (void**)&retval);
        m_scanWifiThreadIsRunning = false;
        QCC_DbgTrace(("ScanWifi timed out and is being canceled"));
    }
}

void OnboardingControllerImpl::StartScanWifi()
{
    m_scanWifiThreadIsRunning = true;
    execute_system(m_scanCmd.c_str());
    timer_delete(m_scanWifiTimerId);
    m_scanWifiThreadIsRunning = false;
}

/*
 * ScanWifiThread
 * A method called when m_scanWifiThread completes
 */
static void* ScanWifiThread(void* context)
{
    OnboardingControllerImpl* thisClass = reinterpret_cast<OnboardingControllerImpl*>(context);
    thisClass->StartScanWifi();
    return NULL;
}

/*------------------------------------------------------------------------------
 * METHOD: GetScanInfo()
 * This method is called by the GetScanInfoHandler with the corresponding input
 * and output arguments. This method is empty, the developer should fill it with
 * the developer's implementation of the GetScanInfo method handler.
 *-----------------------------------------------------------------------------*/
void OnboardingControllerImpl::GetScanInfo(unsigned short& age, OBScanInfo*& scanList, size_t& scanListNumElements)
{
    QCC_DbgHLPrintf(("entered %s", __FUNCTION__));
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

    // Spawn a thread to scan the wifi and update the wifi_scan_results
    if (!m_scanWifiThreadIsRunning) {
        StartScanWifiTimer();
        pthread_create(&m_scanWifiThread, NULL, ScanWifiThread, this);
    }

} /* GetScanInfo() */

void* OnboardingControllerImpl::OBS_Offboard(void* obsArg)
{
    OnboardingControllerImpl* obController = (OnboardingControllerImpl*)obsArg;
    qcc::String offboardCmd = obController->m_offboardCmd;

    //stopping daemon before offboard cmd and restarting it after
    execute_system("/etc/init.d/alljoyn stop");
    execute_system(offboardCmd.c_str());
    execute_system("/etc/init.d/alljoyn start");

    return NULL;
}

/*------------------------------------------------------------------------------
 * METHOD: Offboard()
 * This method is called by the OffboardHandler with the corresponding input and
 * output arguments. This method is empty, the developer should fill it with the
 * developer's implementation of the Offboard method handler.
 *-----------------------------------------------------------------------------*/
void OnboardingControllerImpl::Offboard()
{
    QCC_DbgHLPrintf(("entered %s", __FUNCTION__));
    CancelAdvertise();
    pthread_t thread;
    pthread_create(&thread, NULL, OnboardingControllerImpl::OBS_Offboard, this);
} /* Offboard() */

short OnboardingControllerImpl::GetState()
{
    QCC_DbgHLPrintf(("entered %s", __FUNCTION__));
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

const OBLastError& OnboardingControllerImpl::GetLastError()
{
    QCC_DbgHLPrintf(("entered %s", __FUNCTION__));
    std::ifstream errorFile(m_errorFile.c_str());
    if (errorFile.is_open()) {
        std::string line;
        getline(errorFile, line);
        std::istringstream iss(line);
        iss >> m_oBLastError.validationState;
        getline(errorFile, line);
        QCC_DbgHLPrintf(("%s", line.c_str()));
        m_oBLastError.message.assign(line.c_str());
        errorFile.close();
    }

    return m_oBLastError;
}

int OnboardingControllerImpl::execute_configure(const char* SSID, const int authType, const char* passphrase) {
    QCC_DbgHLPrintf(("entered %s", __FUNCTION__));
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
    QCC_DbgHLPrintf(("entered %s", __FUNCTION__));
    m_BusAttachment->EnableConcurrentCallbacks();
    if (m_BusAttachment->IsConnected() && m_BusAttachment->GetUniqueName().size() > 0) {
        QStatus status = m_BusAttachment->CancelAdvertiseName(m_BusAttachment->GetUniqueName().c_str(), TRANSPORT_ANY);
        QCC_DbgHLPrintf(("CancelAdvertiseName for %s = %s", m_BusAttachment->GetUniqueName().c_str(), QCC_StatusText(status)));
    }
}

static int execute_system(const char*cmd)
{
    QCC_DbgHLPrintf(("entered %s", __FUNCTION__));

#ifdef _OPEN_WRT_
    if (!cmd) {
        return -1;
    }
    QCC_DbgHLPrintf(("executing %s", cmd));
    int result = system(cmd);
    result = WEXITSTATUS(result);
    QCC_DbgHLPrintf(("system result=%d", result));
    if (-1 == result) {
        QCC_DbgHLPrintf(("Error executing system: %d", strerror(errno)));
    }
    return result;
#else
    return 0;
#endif
}

