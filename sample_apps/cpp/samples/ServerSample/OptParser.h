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
    /**
     * ParseResultCode
     */
    enum ParseResultCode {
        PR_OK, PR_EXIT_NO_ERROR, PR_INVALID_OPTION, PR_MISSING_OPTION, PR_INVALID_APPID
    };

    /**
     * OptParser
     * @param argc
     * @param argv
     */
    OptParser(int argc, char** argv);

    /**
     * ParseResult
     * @return
     */
    ParseResultCode ParseResult();

    /**
     * GetFactoryConfigFile
     * @return
     */
    qcc::String const& GetFactoryConfigFile() const;

    /**
     * GetConfigFile
     * @return
     */
    qcc::String const& GetConfigFile() const;

    /**
     * GetAppId
     * @return
     */
    qcc::String const& GetAppId() const;

    /**
     * GetAppName
     * @return
     */
    qcc::String const& GetAppName() const;

    /**
     * GetDeviceId
     * @return
     */
    qcc::String const& GetDeviceId() const;

    /**
     * GetDeviceNames
     * @return
     */
    DeviceNamesType const& GetDeviceNames() const;

    /**
     * GetDefaultLanguage
     * @return
     */
    qcc::String const& GetDefaultLanguage() const;

    /**
     * GetScanFile
     * @return
     */
    qcc::String const& GetScanFile() const;

    /**
     * GetStateFile
     * @return
     */
    qcc::String const& GetStateFile() const;

    /**
     * GetErrorFile
     * @return
     */
    qcc::String const& GetErrorFile() const;

    /**
     * GetConfigureCmd
     * @return
     */
    qcc::String const& GetConfigureCmd() const;

    /**
     * GetConnectCmd
     * @return
     */
    qcc::String const& GetConnectCmd() const;

    /**
     * GetOffboardCmd
     * @return
     */
    qcc::String const& GetOffboardCmd() const;

    /**
     * GetConcurrency
     * @return
     */
    int GetConcurrency() const;

    /**
     * GetPort
     * @return
     */
    int GetPort() const;

    /**
     * ParseExternalXML
     * @return
     */
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
    qcc::String deviceName;
    qcc::String scanFile;
    qcc::String stateFile;
    qcc::String errorFile;
    qcc::String configureCmd;
    qcc::String connectCmd;
    qcc::String offboardCmd;
    int port;
    int concurrency;
    DeviceNamesType deviceNames;
};

#endif /* OPTPARSER_H_ */
