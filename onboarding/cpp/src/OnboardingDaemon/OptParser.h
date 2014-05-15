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

#ifndef OPTPARSER_H_
#define OPTPARSER_H_

#include <alljoyn/about/AboutServiceApi.h>

typedef std::map<qcc::String, qcc::String> DeviceNamesType;

/**
 * Class to parse arguments
 */
class OptParser {
  public:
    enum ParseResultCode {
        PR_OK, PR_EXIT_NO_ERROR, PR_INVALID_OPTION, PR_MISSING_OPTION, PR_INVALID_APPID
    };

    OptParser(int argc, char** argv);

    ParseResultCode ParseResult();

    qcc::String const& GetFactoryConfigFile() const;

    qcc::String const& GetConfigFile() const;

    qcc::String const& GetAppId() const;

    qcc::String const& GetAppName() const;

    qcc::String const& GetDeviceId() const;

    DeviceNamesType GetDeviceNames() const;

    qcc::String const& GetDefaultLanguage() const;

    qcc::String const& GetScanFile() const;

    qcc::String const& GetStateFile() const;

    qcc::String const& GetErrorFile() const;

    qcc::String const& GetConfigureCmd() const;

    qcc::String const& GetConnectCmd() const;

    qcc::String const& GetOffboardCmd() const;

    qcc::String const& GetScanCmd() const;

    int GetConcurrency() const;

    int GetPort() const;

    bool ParseExternalXML();

  private:
    int argc;
    char** argv;

    bool IsAllHex(const char* data);

    void PrintUsage();

    bool FillDeviceNames();

    qcc::String factoryConfigFile;
    qcc::String configFile;
    qcc::String appGUID;
    qcc::String appName;
    qcc::String defaultLanguage;
    qcc::String deviceId;
    DeviceNamesType deviceNames;
    qcc::String scanFile;
    qcc::String stateFile;
    qcc::String errorFile;
    qcc::String configureCmd;
    qcc::String connectCmd;
    qcc::String offboardCmd;
    qcc::String m_scanCmd;
    int port;
    int concurrency;
};

#endif /* OPTPARSER_H_ */
