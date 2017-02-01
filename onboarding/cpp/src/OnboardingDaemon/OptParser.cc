/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
 *    Project (AJOSP) Contributors and others.
 *    
 *    SPDX-License-Identifier: Apache-2.0
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *    
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *    
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *    
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
******************************************************************************/

#include "OptParser.h"
#include <iostream>
#include <alljoyn/services_common/GuidUtil.h>
#include <alljoyn/AboutData.h>

#define QCC_MODULE "ONBOARD"

using namespace ajn;
using namespace ajn::services;

static int const APP_ID_LENGTH{32}; // FIX - this is a core constraint and should
                                     //  be defined by and obtained from core

static char const VERSION_MESSAGE[]{"AllJoyn Onboarding Service Version: 1"};

static char const FACTORY_FILE_OPT[]{"--factory-config-file"};
static char const APP_ID_OPT[]{"--appId"};
static char const VERSION_OPT[]{"--version"};
static char const CONFIG_FILE_OPT[]{"--config-file"};
static char const HELP_OPT_LONG[]{"--help"};
static char const HELP_OPT_SHORT[]{"-h"};
static char const AUTH_OPT[]{"--auth"};
static char const AUTH_VAL_NULL[]{"null"};
static char const AUTH_VAL_PSK[]{"psk"};
static char const AUTH_VAL_ECDSA[]{"ecdsa"};
static char const AUTH_VAL_SPEKE[]{"speke"};

static const char *const DEFAULT_FACTORY_CONFIG_FILE{"FactoryOnboardingService.conf"};
static const char *const DEFAULT_CONFIG_FILE{"OnboardingService.conf"};

static char const PASS_OPT[]{"--pass"};
char const OptParser::AUTH_MECHANISM_PSK[]{"ALLJOYN_ECDHE_ECDSA ALLJOYN_ECDHE_PSK"};
char const OptParser::AUTH_MECHANISM_NULL[]{"ALLJOYN_ECDHE_ECDSA ALLJOYN_ECDHE_NULL"};
char const OptParser::AUTH_MECHANISM_SPEKE[]{"ALLJOYN_ECDHE_ECDSA ALLJOYN_ECDHE_SPEKE"};
char const OptParser::AUTH_MECHANISM_ECDSA[]{"ALLJOYN_ECDHE_ECDSA"};

OptParser::OptParser(int argc, char** argv) :
    argc(argc), argv(argv)
{
    GuidUtil::GetInstance()->GetDeviceIdString(&deviceId);
    GuidUtil::GetInstance()->GenerateGUID(&appId);
    factoryConfigFile = DEFAULT_FACTORY_CONFIG_FILE;
    configFile = DEFAULT_CONFIG_FILE;
}

qcc::String const& OptParser::GetAppId() const
{
    return appId;
}

qcc::String const& OptParser::GetDeviceId() const
{
    return deviceId;
}

qcc::String const& OptParser::GetFactoryConfigFile() const
{
    return factoryConfigFile;
}

qcc::String const& OptParser::GetConfigFile() const
{
    return configFile;
}

qcc::String const& OptParser::GetAuthPassword() const
{
    return authPassword;
}

qcc::String const& OptParser::GetAuthMechanism() const
{
    return authMech;
}

void OptParser::PrintUsage()
{
    std::string cmd = argv[0];
    cmd = cmd.substr(cmd.find_last_of('/') + 1);

    std::cout << cmd.c_str()
              << " [" << FACTORY_FILE_OPT << "=FILE | " << CONFIG_FILE_OPT << "=FILE | " << APP_ID_OPT << " =APPID]\n\n"
              << "\t" << FACTORY_FILE_OPT << "=FILE\n"
              << "\t\tConfiguration file with factory settings.\n\n"
              << "\t" << CONFIG_FILE_OPT << "=FILE\n"
              << "\t\tActive configuration file that persists user's updates\n\n"
              << "\t" << APP_ID_OPT << "=ID\n"
              << "\t\tActive configuration file that persists user's updates\n\n"
              << "\t" << AUTH_OPT << "=auth\n"
              << "\t\tAuthentication mechanism: "
                  << AUTH_VAL_NULL << ", "
                  << AUTH_VAL_PSK << ", "
                  << AUTH_VAL_ECDSA << ", or "
                  << AUTH_VAL_SPEKE << ".\n\n"
              << "\t" << PASS_OPT << "=password\n"
              << "\t\tThe password to be used for authentication\n\n"
              << "\t" << VERSION_OPT << "\n"
              << "\t\tPrint the version and copyright string, and exit."
              << std::endl;
}

bool OptParser::IsAllHex(const char* data)
{
    for (size_t index = 0; index < strlen(data); ++index) {
        if (!isxdigit(data[index])) {
            return false;
        }

    }
    return true;
}

OptParser::ParseResultCode OptParser::ParseResult()
{
    ParseResultCode result = PR_OK;

    if (argc == 1) {

        return result;
    }

    const size_t APP_ID_OPT_LENGTH = sizeof(APP_ID_OPT);

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg.compare(VERSION_OPT) == 0) {
            std::cout << VERSION_MESSAGE << std::endl;
            result = PR_EXIT_NO_ERROR;
            break;
        } else if (arg.compare(0, APP_ID_OPT_LENGTH - 1, APP_ID_OPT) == 0) {
            appId = arg.substr(APP_ID_OPT_LENGTH);
            if ((appId.length() != APP_ID_LENGTH) || (!IsAllHex(appId.c_str()))) {
                result = PR_INVALID_APPID;
                QCC_LogError(ER_NONE, ("Invalid appId: %s", appId.c_str()));
                break;
            }
        } else if (arg.compare(0, sizeof(FACTORY_FILE_OPT) - 1, FACTORY_FILE_OPT) == 0) {
            factoryConfigFile = arg.substr(sizeof(FACTORY_FILE_OPT));
        } else if (arg.compare(0, sizeof(CONFIG_FILE_OPT) - 1, CONFIG_FILE_OPT) == 0) {
            configFile = arg.substr(sizeof(CONFIG_FILE_OPT));
        } else if (arg.compare(0, sizeof(AUTH_OPT) - 1, AUTH_OPT) == 0) {
            std::string authVal = arg.substr(sizeof(AUTH_OPT));
            if (authVal.compare(AUTH_VAL_NULL) == 0) {
                authMech = AUTH_MECHANISM_NULL;
            } else if (authVal.compare(AUTH_VAL_PSK) == 0) {
                authMech = AUTH_MECHANISM_PSK;
            } else if (authVal.compare(AUTH_VAL_ECDSA) == 0) {
                authMech = AUTH_MECHANISM_ECDSA;
            } else if (authVal.compare(AUTH_VAL_SPEKE) == 0) {
                authMech = AUTH_MECHANISM_SPEKE;
            } else {
                result = PR_UNKNOWN_AUTH;
                QCC_LogError(ER_NONE, ("Unknown authentication mechanism: %s", authVal.c_str()));
            }
        } else if (arg.compare(0, sizeof(PASS_OPT) - 1, PASS_OPT) == 0) {
            authPassword = arg.substr(sizeof(PASS_OPT));
        } else if ((arg.compare(HELP_OPT_LONG) == 0) || (arg.compare(HELP_OPT_SHORT) == 0)) {
            PrintUsage();
            result = PR_EXIT_NO_ERROR;
            break;
        } else {
            result = PR_INVALID_OPTION;
            QCC_LogError(ER_NONE, ("Invalid option: %s", arg.c_str()));
            break;
        }
    }
    return result;
}