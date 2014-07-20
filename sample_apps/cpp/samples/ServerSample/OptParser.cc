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

#include "OptParser.h"
#include <IniParser.h>
#include <alljoyn/about/AboutPropertyStoreImpl.h>
#include <alljoyn/services_common/GuidUtil.h>

static const char versionPreamble[] = "AC Server Sample 1.0";

using namespace ajn;
using namespace services;

OptParser::OptParser(int argc, char** argv) :
    argc(argc), argv(argv) {
    port = 900;
    concurrency = 0;
    GuidUtil::GetInstance()->GetDeviceIdString(&deviceId);
    GuidUtil::GetInstance()->GenerateGUID(&appGUID);
    defaultLanguage.assign("en");
    factoryConfigFile.assign("FactoryServerSample.conf");
    configFile.assign("ServerSample.conf");
    appName.assign("ServerSample");

    scanFile.assign("");
    stateFile.assign("");
    errorFile.assign("");
    configureCmd.assign("");
    connectCmd.assign("");
    offboardCmd.assign("");
}

qcc::String const& OptParser::GetAppId() const {
    return appGUID;
}

qcc::String const& OptParser::GetAppName() const {
    return appName;
}

qcc::String const& OptParser::GetFactoryConfigFile() const {
    return configFile;
}
qcc::String const& OptParser::GetConfigFile() const {
    return configFile;
}

qcc::String const& OptParser::GetDeviceId() const {
    return deviceId;
}

DeviceNamesType const& OptParser::GetDeviceNames() const {
    return deviceNames;
}

qcc::String const& OptParser::GetDefaultLanguage() const {
    return defaultLanguage;
}

int OptParser::GetPort() const {
    return port;
}

int OptParser::GetConcurrency() const {
    return concurrency;
}

qcc::String const& OptParser::GetScanFile() const {
    return scanFile;
}

qcc::String const& OptParser::GetStateFile() const {
    return stateFile;
}

qcc::String const& OptParser::GetErrorFile() const {
    return errorFile;
}

qcc::String const& OptParser::GetConfigureCmd() const {
    return configureCmd;
}

qcc::String const& OptParser::GetConnectCmd() const {
    return connectCmd;
}

qcc::String const& OptParser::GetOffboardCmd() const {
    return offboardCmd;
}

bool OptParser::FillDeviceNames() {
    deviceNames.clear();
    std::map<std::string, std::string> data;

    if (!IniParser::ParseFile(configFile.c_str(), data)) {
        std::cerr << "Could not parse configFile" << std::endl;
        return false;
    }

    typedef std::map<std::string, std::string>::iterator it_data;
    for (it_data iterator = data.begin(); iterator != data.end(); iterator++) {


        if (iterator->first.find(AboutPropertyStoreImpl::getPropertyStoreName(DEVICE_NAME).c_str()) == 0) {
            size_t lastDotLocation = iterator->first.find(".");
            if ((lastDotLocation ==  std::string::npos) || (lastDotLocation + 1 >= iterator->first.length())) {
                continue;
            }
            std::string key = iterator->first.substr(lastDotLocation + 1);
            deviceNames.insert(std::pair<qcc::String, qcc::String>(key.c_str(), iterator->second.c_str()));
        }
    }

    return true;
}

bool OptParser::ParseExternalXML() {
    std::map<std::string, std::string> data;
    if (!IniParser::ParseFile(configFile.c_str(), data)) {
        std::cerr << "Could not parse configFile" << configFile.c_str() << std::endl;
        return false;
    }

    std::map<std::string, std::string>::iterator iter;

    if (!FillDeviceNames()) {
        return false;
    }

    iter = data.find(AboutPropertyStoreImpl::getPropertyStoreName(APP_ID).c_str());
    if (iter != data.end()) {
        appGUID = iter->second.c_str();
    }

    iter = data.find(AboutPropertyStoreImpl::getPropertyStoreName(APP_NAME).c_str());
    if (iter != data.end()) {
        appName = iter->second.c_str();
    }

    iter = data.find(AboutPropertyStoreImpl::getPropertyStoreName(DEFAULT_LANG).c_str());
    if (iter != data.end()) {
        defaultLanguage = iter->second.c_str();
    }

    iter = data.find("scan_file");
    if (iter != data.end()) {
        scanFile = iter->second.c_str();
    }

    iter = data.find("state_file");
    if (iter != data.end()) {
        stateFile = iter->second.c_str();
    }

    iter = data.find("error_file");
    if (iter != data.end()) {
        errorFile = iter->second.c_str();
    }

    iter = data.find("configure_cmd");
    if (iter != data.end()) {
        configureCmd = iter->second.c_str();
    }

    iter = data.find("connect_cmd");
    if (iter != data.end()) {
        connectCmd = iter->second.c_str();
    }

    iter = data.find("offboard_cmd");
    if (iter != data.end()) {
        offboardCmd = iter->second.c_str();
    }

    return true;
}

void OptParser::PrintUsage() {
    qcc::String cmd = argv[0];
    cmd = cmd.substr(cmd.find_last_of('/') + 1);

    fprintf(stderr, "%s [--port=PORT  | --config-file=FILE | --language=LANG |  --appId=APPID"
            "]\n"

            "    --port=\n"
            "        used to bind the service.\n\n"
            "    --factory-config-file=FILE\n"
            "        Configuration file with factory settings.\n\n"
            "    --config-file=FILE\n"
            "        Active configuration file that persists user's updates\n\n"
            "    --appId=\n"
            "        Use the specified it is HexString of 16 bytes (32 chars) \n\n"
            "    --language=\n"
            "       default language for PropertyStore\n\n"
            "    --version\n"
            "        Print the version and copyright string, and exit.\n", cmd.c_str());
}

bool OptParser::IsAllHex(const char* data) {

    for (size_t index = 0; index < strlen(data); ++index) {
        if (!isxdigit(data[index])) {
            return false;
        }

    }
    return true;

}

OptParser::ParseResultCode OptParser::ParseResult() {
    ParseResultCode result = PR_OK;

    if (argc == 1) {

        return result;
    }

    int indx;
    for (indx = 1; indx < argc; indx++) {
        qcc::String arg(argv[indx]);
        if (arg.compare("--version") == 0) {
            printf(versionPreamble, "1");
            result = PR_EXIT_NO_ERROR;
            break;
        } else if (arg.compare(0, sizeof("--port") - 1, "--port") == 0) {
            port = atoi(arg.substr(sizeof("--port")).c_str());
        } else if (arg.compare(0, sizeof("--appId") - 1, "--appId") == 0) {
            appGUID = arg.substr(sizeof("--appId"));
            if ((appGUID.length() != 32) || (!IsAllHex(appGUID.c_str()))) {
                result = PR_INVALID_APPID;
                fprintf(stderr, "Invalid appId: \"%s\"\n", argv[indx]);
                break;
            }
        } else if (arg.compare(0, sizeof("--factory-config-file") - 1, "--factory-config-file") == 0) {
            factoryConfigFile = arg.substr(sizeof("--factory-config-file"));
        } else if (arg.compare(0, sizeof("--config-file") - 1, "--config-file") == 0) {
            configFile = arg.substr(sizeof("--config-file"));
        } else if (arg.compare(0, sizeof("--language") - 1, "--language") == 0) {
            defaultLanguage = arg.substr(sizeof("--language"));
        } else if ((arg.compare("--help") == 0) || (arg.compare("-h") == 0)) {
            PrintUsage();
            result = PR_EXIT_NO_ERROR;
            break;
        } else {
            result = PR_INVALID_OPTION;
            fprintf(stderr, "Invalid option: \"%s\"\n", argv[indx]);
            break;
        }
    }
    return result;
}
